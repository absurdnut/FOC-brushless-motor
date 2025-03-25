#include "BLDCDriver3PWM.h"

BLDCDriver3PWM::BLDCDriver3PWM(TIM_HandleTypeDef* handle, float Ua, float Ub, float Uc){
  // Pin initialization
  // pwmA = phA;
  // pwmB = phB;
  // pwmC = phC;

  // // enable_pin pin
  // enableA_pin = en1;
  // enableB_pin = en2;
  // enableC_pin = en3;
  // 执行初始化的过程录入abc对应的channel, 对应的TIM of handler.
  htimx = handle;

  // limit the voltage in driver
  Ua = _constrain(Ua, 0.0f, voltage_limit);
  Ub = _constrain(Ub, 0.0f, voltage_limit);
  Uc = _constrain(Uc, 0.0f, voltage_limit);
  // calculate duty cycle
  // limited in [0,1]
  dc_a = _constrain(Ua / voltage_power_supply, 0.0f , 1.0f );
  dc_b = _constrain(Ub / voltage_power_supply, 0.0f , 1.0f );
  dc_c = _constrain(Uc / voltage_power_supply, 0.0f , 1.0f );

  // hardware specific writing
  // hardware specific function - depending on driver and mcu
  unsigned int cache_ARR = htimx->Instance->ARR;
  __HAL_TIM_SetCompare(htimx, TIM_CHANNEL_1, dc_a*cache_ARR);
  __HAL_TIM_SetCompare(htimx, TIM_CHANNEL_2, dc_b*cache_ARR);
  __HAL_TIM_SetCompare(htimx, TIM_CHANNEL_3, dc_c*cache_ARR);


  // default power-supply value
  voltage_power_supply = DEF_POWER_SUPPLY;
  voltage_limit = NOT_SET;
  pwm_frequency = NOT_SET;

}

// enable motor driver
void  BLDCDriver3PWM::enable(){
    // enable_pin the driver - if enable_pin pin available
    // if ( _isset(enableA_pin) ) digitalWrite(enableA_pin, enable_active_high);
    // if ( _isset(enableB_pin) ) digitalWrite(enableB_pin, enable_active_high);
    // if ( _isset(enableC_pin) ) digitalWrite(enableC_pin, enable_active_high);
    // set zero to PWM
    setPwm(0,0,0);
}

// disable motor driver
void BLDCDriver3PWM::disable()
{
  // set zero to PWM
  setPwm(0, 0, 0);
  // disable the driver - if enable_pin pin available
  // if ( _isset(enableA_pin) ) digitalWrite(enableA_pin, !enable_active_high);
  // if ( _isset(enableB_pin) ) digitalWrite(enableB_pin, !enable_active_high);
  // if ( _isset(enableC_pin) ) digitalWrite(enableC_pin, !enable_active_high);

}

// init hardware pins
int BLDCDriver3PWM::init() {
  // PWM pins
  // pinMode(pwmA, OUTPUT);
  // pinMode(pwmB, OUTPUT);
  // pinMode(pwmC, OUTPUT);
  // if( _isset(enableA_pin)) pinMode(enableA_pin, OUTPUT);
  // if( _isset(enableB_pin)) pinMode(enableB_pin, OUTPUT);
  // if( _isset(enableC_pin)) pinMode(enableC_pin, OUTPUT);
  //启动三个通道的PWM
  HAL_TIM_PWM_Start(htimx, TIM_CHANNEL_1);
  HAL_TIMEx_PWMN_Start(htimx, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(htimx, TIM_CHANNEL_2);
  HAL_TIMEx_PWMN_Start(htimx, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(htimx, TIM_CHANNEL_3);
  HAL_TIMEx_PWMN_Start(htimx, TIM_CHANNEL_3);
  // sanity check for the voltage limit configuration
  if(!_isset(voltage_limit) || voltage_limit > voltage_power_supply) voltage_limit =  voltage_power_supply;

  // Set the pwm frequency to the pins
  // hardware specific function - depending on driver and mcu
  // params = _configure3PWM(pwm_frequency, pwmA, pwmB, pwmC);
  initialized = (params!=SIMPLEFOC_DRIVER_INIT_FAILED);
  return params!=SIMPLEFOC_DRIVER_INIT_FAILED;
}



// Set voltage to the pwm pin
void BLDCDriver3PWM::setPhaseState(PhaseState sa, PhaseState sb, PhaseState sc) {
  // disable if needed
  if( _isset(enableA_pin) &&  _isset(enableB_pin)  && _isset(enableC_pin) ){
    // digitalWrite(enableA_pin, sa == PhaseState::PHASE_ON ? enable_active_high:!enable_active_high);
    // digitalWrite(enableB_pin, sb == PhaseState::PHASE_ON ? enable_active_high:!enable_active_high);
    // digitalWrite(enableC_pin, sc == PhaseState::PHASE_ON ? enable_active_high:!enable_active_high);
  }
}

// Set voltage to the pwm pin
void BLDCDriver3PWM::setPwm(float Ua, float Ub, float Uc) {

  // limit the voltage in driver
  Ua = _constrain(Ua, 0.0f, voltage_limit);
  Ub = _constrain(Ub, 0.0f, voltage_limit);
  Uc = _constrain(Uc, 0.0f, voltage_limit);
  // calculate duty cycle
  // limited in [0,1]
  dc_a = _constrain(Ua / voltage_power_supply, 0.0f , 1.0f );
  dc_b = _constrain(Ub / voltage_power_supply, 0.0f , 1.0f );
  dc_c = _constrain(Uc / voltage_power_supply, 0.0f , 1.0f );

  // hardware specific writing
  // hardware specific function - depending on driver and mcu
  unsigned int cache_ARR = htimx->Instance->ARR;
  __HAL_TIM_SetCompare(htimx, TIM_CHANNEL_1, dc_a*cache_ARR);
  __HAL_TIM_SetCompare(htimx, TIM_CHANNEL_2, dc_b*cache_ARR);
  __HAL_TIM_SetCompare(htimx, TIM_CHANNEL_3, dc_c*cache_ARR);

  // _writeDutyCycle3PWM(dc_a, dc_b, dc_c, params);
}
