    /**
     *
     * movement.c
     * @author Carter Hauschildt
     * @author Ethan Griesmann
     *
     */
    #include <string.h>
    #include "Timer.h"
    #include "lcd.h"
    #include "open_interface.h"
    #include "ADC.h"
    #include "servo.h"
    #include "ping.h"
    #include "uart.h"
    #include "movement.h"

    #define ANGLE_MOD 15

    int checkBumper(oi_t *sensor_data)
    {
        /**
         * Bumper States:
         * bumper = 0, no bumps
         * bumper = 1, right bumper
         * bumper = 2, front right bumper
         * bumper = 3, center right bumper
         * bumper = 4, center left bumper
         * bumper = 5, front left bumper
         * bumper = 6, left bumper
         */
        int bumper = 0;
        oi_update(sensor_data);
        if (sensor_data->bumpLeft)
        {
            bumper = 6;
        }
//        else if (sensor_data->bumperFrontLeft)
//        {
//            bumper = 5;
//        }
//        else if (sensor_data->bumperCenterLeft)
//        {
//            bumper = 4;
//        }
//        else if (sensor_data->bumperCenterRight)
//        {
//            bumper = 3;
//        }
//        else if (sensor_data->bumperFrontRight)
//        {
//            bumper = 2;
//        }
        else if (sensor_data->bumpRight)
        {
            bumper = 1;
        }
        else
        {
            bumper = 0;
        }

        return bumper;
    }



    int checkCliff(oi_t *sensor_data)
    {
        oi_update(sensor_data);

        /**
         *  0 = no cliff
         *  1 = cliffleft
         *  2 = cliffright
         *  3 = cliffront left
         *  4 = clifffront right
         */
        int cliff = 0;

        if(sensor_data-> cliffLeft)
        {
            cliff = 1;
        }
        else if(sensor_data -> cliffRight)
        {
            cliff = 2;
        }
        else if(sensor_data -> cliffFrontLeft)
        {
            cliff = 3;
        }
        else if (sensor_data -> cliffFrontRight)
        {
            cliff = 4;
        }

        else
        {
            cliff = 0;
        }

        return cliff;
    }


    void move_forward(oi_t *sensor_data, int centimeters)
    {
        int sum = 0 ;
        while (sum < centimeters)
        {
            int leftSignal = sensor_data -> cliffLeftSignal;
            int rightSignal = sensor_data -> cliffRightSignal;
            int frontLeftSignal = sensor_data -> cliffFrontLeftSignal;
            int frontRightSignal = sensor_data -> cliffFrontRightSignal;
            oi_setWheels(75,75) ; //move forward
            oi_update(sensor_data);
            sum += sensor_data->distance;

            //boundary detection
            if(leftSignal > 2500)
            {
                char* warning = "White Line on Left Side! \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break;
            }
            else if(rightSignal > 2600)
            {
                char* warning = "White Line on RIGHT side! \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break ;
            }
            else if(frontLeftSignal > 2600)
            {
                char* warning = "White Line on Front Left side! \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break ;
            }
            else if(frontRightSignal > 2600)
            {
                char* warning = "White Line on Front Right side! \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break ;
            }

            //Bumper detection
            if (checkBumper(sensor_data) == 1)
            {
                char* warning = "Hit right bumper! \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break ;
            }
//            else if (checkBumper(sensor_data) == 2)
//            {
//                char* warning = "Hit front right bumper! \n\r";
//                uart_sendStr(warning);
//                oi_setWheels(0,0);
//                break ;
//            }
//            else if (checkBumper(sensor_data) == 3)
//            {
//                char* warning = "Hit center right bumper! \n\r";
//                uart_sendStr(warning);
//                oi_setWheels(0,0);
//                break ;
//            }
//            else if (checkBumper(sensor_data) == 4)
//            {
//                char* warning = "Hit center left bumper! \n\r";
//                uart_sendStr(warning);
//                oi_setWheels(0,0);
//                break ;
//            }
//            else if (checkBumper(sensor_data) == 5)
//            {
//                char* warning = "Hit front left bumper! \n\r";
//                uart_sendStr(warning);
//                oi_setWheels(0,0);
//                break;
//            }
            else if (checkBumper(sensor_data) == 6)
            {
                char* warning = "Hit left bumper! \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break;
            }

            //Cliff (black tape) detection check
            if (checkCliff(sensor_data) == 1)
            {
                char* warning = "WARNING CLIFF ON LEFT SIDE! \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break;
            }
            else if (checkCliff(sensor_data) ==2)
            {
                char* warning = "WARNING CLIFF ON RIGHT SIDE! \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break;
            }
            else if (checkCliff(sensor_data) ==3)
            {
                char* warning = "WARNING CLIFF IN FRONT ON LEFT SIDE! \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break;
            }
            else if (checkCliff(sensor_data) == 4)
            {
                char* warning = "WARNING CLIFF IN FRONT ON RIGHT SIDE \n\r";
                uart_sendStr(warning);
                oi_setWheels(0,0);
                break;
            }
            timer_waitMillis(100);
        }
        oi_setWheels(0,0); //stop wheels
        oi_free(sensor_data);
    }


    void move_backwards(oi_t *sensor_data, int centimeters)
    {
        oi_setWheels(-100, -100); //move backward

       int sum = 0 ;

       while (sum > centimeters)
       {
           oi_update(sensor_data);

           sum += sensor_data->distance;
           timer_waitMillis(1);

       }

       oi_setWheels(0,0); //stop wheels
       oi_free(sensor_data);

    }



    void turn_right(oi_t *sensor_data, int degrees){
        oi_setWheels(-100, 100);
        timer_waitMillis(degrees*22);
        oi_setWheels(0, 0); //stop wheels
        oi_free(sensor_data) ;
    }



    void turn_left(oi_t *sensor_data, int degrees){
        oi_setWheels(100, -100);
        timer_waitMillis(degrees*22);
        oi_setWheels(0, 0); //stop wheels
        oi_free(sensor_data) ;
    }

