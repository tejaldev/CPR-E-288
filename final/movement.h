/*
 * movement.h
 *
 *      Authors: Carter Hauschildt, Ethan Griesmann
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"

void move_forward(oi_t *sensor_data, int centimeters);

void move_backwards(oi_t *sensor_data, int centimeters);


void turn_right(oi_t *sensor_data, int degrees);

void turn_left(oi_t *sensor_data, int degrees);

int checkBumper(oi_t *sensor_data);

int checkCliff(oi_t *sensor_data);

#endif /* MOVEMENT_H_ */
