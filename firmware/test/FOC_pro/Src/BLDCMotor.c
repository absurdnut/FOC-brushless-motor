#include "BLDCMotor.h"
#include "tim.h"


void setPhaseVoltage(float Uq, float Ud, float angle_el) {

  float center;
  int sector;
  float _ca,_sa;


  // Sinusoidal PWM modulation
  // Inverse Park + Clarke transformation
  _sincos(angle_el, &_sa, &_ca);//将angle_el转换为sin和cos，明显是在实现逆Park变换

  // Inverse park transform
  Ualpha =  _ca * Ud - _sa * Uq;  // -sin(angle) * Uq;
  Ubeta =  _sa * Ud + _ca * Uq;    //  cos(angle) * Uq;

  // Inverse Clarke transform
  Ua = Ualpha;
  Ub = -0.5f * Ualpha + _SQRT3_2 * Ubeta;  //注释还写错了，明显是逆Clarke变换，直角转三相
  Uc = -0.5f * Ualpha - _SQRT3_2 * Ubeta;  //此时输出的三相波形是正负均有的效果，后续具体使用时需要增加偏移量

  center = voltage_limit/2;

    // Midpoint Clamp
  float Umin = min(Ua, min(Ub, Uc));
  float Umax = max(Ua, max(Ub, Uc));
  center -= (Umax+Umin) / 2;


  if (!modulation_centered) {
    float Umin = min(Ua, min(Ub, Uc));
    Ua -= Umin;
    Ub -= Umin;
    Uc -= Umin;
  }else{
    Ua += center;
    Ub += center;
    Uc += center;
  }


  // set the voltages in driver
  setPwm(Ua, Ub, Uc);
}

void setPwm(float Ua, float Ub, float Uc) {

  // limit the voltage in driver
  Ua = _constrain(Ua, 0.0f, voltage_limit);
  Ub = _constrain(Ub, 0.0f, voltage_limit);
  Uc = _constrain(Uc, 0.0f, voltage_limit);
  // calculate duty cycle
  // limited in [0,1]
  dc_a = _constrain(Ua / voltage_power_supply, 0.0f , 1.0f ); //pwm值归一化设计，因为pwm函数的接口是统一的，不可能为了你不同的电压值设计不同的数值范围
  dc_b = _constrain(Ub / voltage_power_supply, 0.0f , 1.0f );
  dc_c = _constrain(Uc / voltage_power_supply, 0.0f , 1.0f );

  // hardware specific writing
  // hardware specific function - depending on driver and mcu

  _writeDutyCycle3PWM(dc_a, dc_b, dc_c);

}

void _writeDutyCycle3PWM(float dc_a,  float dc_b, float dc_c){
  // transform duty cycle from [0,1] to [0,4095]
  // _setPwm(((STM32DriverParams*)params)->timers[0], ((STM32DriverParams*)params)->channels[0], _PWM_RANGE*dc_a, _PWM_RESOLUTION);
  // _setPwm(((STM32DriverParams*)params)->timers[1], ((STM32DriverParams*)params)->channels[1], _PWM_RANGE*dc_b, _PWM_RESOLUTION);
  // _setPwm(((STM32DriverParams*)params)->timers[2], ((STM32DriverParams*)params)->channels[2], _PWM_RANGE*dc_c, _PWM_RESOLUTION);
  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, (uint32_t)(dc_a * 10000.0f));
  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, (uint32_t)(dc_b * 10000.0f));
  __HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, (uint32_t)(dc_c * 10000.0f));
  
}

//setPWM();我先实现一个函数，可以在配置好的引脚输出占空比，这样就可以直接调用这个函数，入参就选择占空比，引脚名,引脚组别。

__attribute__((weak)) float _sin(float a){
  // 16bit integer array for sine lookup. interpolation is used for better precision
  // 16 bit precision on sine value, 8 bit fractional value for interpolation, 6bit LUT size
  // resulting precision compared to stdlib sine is 0.00006480 (RMS difference in range -PI,PI for 3217 steps)
  static uint16_t sine_array[65] = {0,804,1608,2411,3212,4011,4808,5602,6393,7180,7962,8740,9512,10279,11039,11793,12540,13279,14010,14733,15447,16151,16846,17531,18205,18868,19520,20160,20788,21403,22006,22595,23170,23732,24279,24812,25330,25833,26320,26791,27246,27684,28106,28511,28899,29269,29622,29957,30274,30572,30853,31114,31357,31581,31786,31972,32138,32286,32413,32522,32610,32679,32729,32758,32768};
  int32_t t1, t2;
  unsigned int i = (unsigned int)(a * (64*4*256.0f/_2PI));
  int frac = i & 0xff;
  i = (i >> 8) & 0xff;
  if (i < 64) {
    t1 = (int32_t)sine_array[i]; t2 = (int32_t)sine_array[i+1];
  }
  else if(i < 128) {
    t1 = (int32_t)sine_array[128 - i]; t2 = (int32_t)sine_array[127 - i];
  }
  else if(i < 192) {
    t1 = -(int32_t)sine_array[-128 + i]; t2 = -(int32_t)sine_array[-127 + i];
  }
  else {
    t1 = -(int32_t)sine_array[256 - i]; t2 = -(int32_t)sine_array[255 - i];
  }
  return (1.0f/32768.0f) * (t1 + (((t2 - t1) * frac) >> 8));
}

// function approximating cosine calculation by using fixed size array
// ~55us (float array)
// ~56us (int array)
// precision +-0.005
// it has to receive an angle in between 0 and 2PI
__attribute__((weak)) float _cos(float a){
  float a_sin = a + _PI_2;
  a_sin = a_sin > _2PI ? a_sin - _2PI : a_sin;
  return _sin(a_sin);
}


__attribute__((weak)) void _sincos(float a, float* s, float* c){
  *s = _sin(a);
  *c = _cos(a);
}