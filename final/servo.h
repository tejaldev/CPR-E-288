//Initialize Timer registers for Servo
void servo_init(void);

//Rotate the Servo based on pulse width
void servo_move(float degrees);

/**
 * rotate the servo to find right and left values
 */
void calibrate(void);

/**
 * rotates servo beyond the bounds of 0 and 180 to find left and right values in order to calibrate the servo motor
 */
void calibrate_move(float degrees);
