#include "stdint.h"

// utility defines
#define _2_SQRT3 1.15470053838f
#define _SQRT3 1.73205080757f
#define _1_SQRT3 0.57735026919f
#define _SQRT3_2 0.86602540378f
#define _SQRT2 1.41421356237f
#define _120_D2R 2.09439510239f
#define _PI 3.14159265359f
#define _PI_2 1.57079632679f
#define _PI_3 1.0471975512f
#define _2PI 6.28318530718f
#define _3PI_2 4.71238898038f
#define _PI_6 0.52359877559f
#define _RPM_TO_RADS 0.10471975512f

#define _PWM_RESOLUTION 12 // 12bit
#define _PWM_RANGE 4095.0f // 2^12 -1 = 4095
#define _PWM_FREQUENCY 25000 // 25khz
#define _PWM_FREQUENCY_MAX 50000 // 50khz

#define _constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

float target; //!< current target value - depends of the controller
float feed_forward_velocity = 0.0f; //!< current feed forward velocity
float shaft_angle;//!< current motor angle
float electrical_angle;//!< current electrical angle
float shaft_velocity;//!< current motor velocity 
float current_sp;//!< target current ( q current )
float shaft_velocity_sp;//!< current target velocity
float shaft_angle_sp;//!< current target angle

float voltage_bemf; //!< estimated backemf voltage (if provided KV constant)
float	Ualpha, Ubeta; //!< Phase voltages U alpha and U beta used for inverse Park and Clarke transform


// motor configuration parameters
float voltage_sensor_align;//!< sensor and motor align voltage parameter
float velocity_index_search;//!< target velocity for index search 

// motor physical parameters
float	phase_resistance; //!< motor phase resistance
int pole_pairs;//!< motor pole pairs number
float KV_rating; //!< motor KV rating
float	phase_inductance; //!< motor phase inductance

// limiting variables
float voltage_power_supply; //!< power supply voltage
float voltage_limit; //!< Voltage limiting variable - global limit
float current_limit; //!< Current limiting variable - global limit
float velocity_limit; //!< Velocity limiting variable - global limit

// motor status vairables
int8_t enabled = 0;//!< enabled or disabled motor flag


int8_t modulation_centered = 1;//!< flag (1) centered modulation around driver limit /2  or  (0) pulled to 0


float Ua, Ub, Uc; 

float dc_a; //!< currently set duty cycle on phaseA
float dc_b; //!< currently set duty cycle on phaseB
float dc_c; //!< currently set duty cycle on phaseC