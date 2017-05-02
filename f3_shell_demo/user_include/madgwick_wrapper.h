#ifndef MADGWICK_AHRS_WRAPPER_H_
#define MADGWICK_AHRS_WRAPPER_H_	1

#include <math.h>

#include "MadgwickAHRS.h"
#include "imu.h"

void init_madgwick(ACC_SCALE a, GYRO_SCALE g, MAG_SCALE m, 
					float dt_sec, float beta_value);
void quaternionToEuler(float* euler_angles, float* quaternion_input);
void update_madgwick(void);
void madgwick_get_attitude(float* attitude);

#endif