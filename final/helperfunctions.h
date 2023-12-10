    /*
     * functions to aid in calculations/printing data
     */

    #ifndef HELPERFUNCTIONS_H_
    #define HELPERFUNCTIONS_H_

    #include <inc/tm4c123gh6pm.h>
    #include "missioncommands.h"

    #define M_PI 3.14159265358979323846


    /**
    *   Convert polar coordinates to Cartesian, cap distances to 100 cm
    */
    void cartFromPolar(int degree, float ir, float sonar, float *x, float *y);


    /**
    *   Sets the proper data for each object detected to be outputted to Putty later.
    */
    void setObjectData(struct object *object_data, int objectCount, float angWidth, float actualDist, int objectLocation);



    /**
    *   Write cartesian values from arrays capped at a range of 100 cm to Putty for use of Radial plot in Putty.
    */
    void arrayOutput(float *x, float *y);



    /**
    *   Print out a 2 dimensional array in Putty, based off of a radial plot for object detection.
    */
    void gridPrintPutty(float *x, float *y);




    /**
    *   Output info. from struct with object data to Putty.
    */
    void objectDataOutput(struct object *object_data, int objectCount);




    #endif
