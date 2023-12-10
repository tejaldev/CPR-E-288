    /*
     *commands to aid in certain aspects of our project
     */
    #include "ADC.h"
    #include "button.h"
    #include "lcd.h"
    #include "movement.h"
    #include "open_interface.h"
    #include "ping.h"
    #include "servo.h"
    #include "timer.h"
    #include "uart.h"
    #include <stdio.h>
    #include "helperfunctions.h"

    /**
    *   Sweep with both the Ping and IR sensors over 180 degrees,
    *   take distance measurements from both sensors every 2 degrees, transmitting data to PuTTy
    */
    void scan(){
        int degree=0, beginDegree = 0, angSize = 0;
        int objectLocation = 0;     //location of object in degrees
        int objectCount = 0;        //total count of object
        float rawDist = 0.0;        //used to average out distance of object
        float actualDist = 0.0;     //actual distance in centimeters from object
        float angWidth = 0.0;       //actual angular width/linear size of object
        int objectIter = 0;         //used to divide raw distance for average distance of an object

        /**
         *  OBJECT STATES (7):
         *  BEGIN = 0: no objects have been detected. (iterate servo)
         *  FIRSTDETECT = 1: first data point that could be an object. *start values* (Prev. state == BEGIN || NOTOBJECT || NONE)
         *  STILLDETECT = 2: second data point that could be an object. *iterate values* (Prev. state == FIRSTDETECT)
         *  OBJECT = 3: third data point, so definitely an object since angular size is >= 4 degrees. *iterate values and count object* (Prev. state == STILLDETECT)
         *  NOTOBJECT = 4: Cant be an object because we didn't point three data points. *reset values* (Prev. state == FIRSTDETECT || STILLDETECT)
         *  STILLOBJECT = 5: fourth or more data point. Already know and counted it as an object *iterate values* (Prev. state == OBJECT || STILLOBJECT)
         *  NONE = 6: Object was detected, but not seen anymore. *do math and save values in object_data* (Prev. state == OBJECT || STILL OBJECT)
         */
        int objectState = 0;

        //Arrays to store Cartesian distance to be outputed to Putty.
        float xCartesian[92] = {};
        float yCartesian[92] = {};

        //Data for each object detected to send over Putty (assumption that we max detect 6 objects)
        struct object object_data[6];

        for(degree=0; degree<=180; degree+=2){
            servo_move(degree); //move 2 degrees
            timer_waitMillis(10);
            float irDist = ADC_getDistance();
            float sonarDist = ping_read();

            //lcd_printf("Object State: %d\nIR (cm): %.1f\nSonar (cm): %.1f\nObject Count: %d",objectState, irDist, sonarDist, objectCount);

            //FIRST DETECT STATE
            if((sonarDist < 100.0) && (irDist < 100.0) && ((objectState == 0) || (objectState == 4) || (objectState == 6))){
                //start values
                objectState = 1;
                beginDegree = degree;
                rawDist += sonarDist;
                objectIter++;
            }
            //STILL DETECT STATE
            else if((irDist < 100.0) && (sonarDist < 100.0) && (objectState == 1)){
                //object still detected (not first time seeing it)
                objectState = 2;
                rawDist += sonarDist;
                objectIter++;
            }
            //OBJECT STATE
            else if((sonarDist < 100.0) && (irDist < 100.0) && (objectState == 2)){
                objectState = 3;
                rawDist += sonarDist;
                objectIter++;
                objectCount++;
            }
            //NOT OBJECT STATE
            else if(((sonarDist > 100.0) || (irDist > 100.0)) && ((objectState == 1) || (objectState == 2))){
                //Reset values
                if(objectCount < 1){
                    objectState = 0;
                }
                else{
                    objectState = 4;
                }
                rawDist = 0.0;
                objectIter = 0;
            }
            //STILL OBJECT STATE
            else if((sonarDist < 100.0) && (irDist < 100.0) && (objectState == 3 || (objectState == 5))){
                //iterate values
                objectState = 5;
                rawDist += sonarDist;
                objectIter++;
            }
            //NONE STATE
            else if(((sonarDist > 100.0) || (irDist > 100.0)) && (objectState == 3 || (objectState == 5))){
                objectState = 6;
                objectLocation = ((degree-2) + beginDegree) / 2;    //location of object in degrees (endingDegree - beginningDegree)/2
                angSize = (degree-2) - beginDegree;                 //angular size of object
                actualDist = (rawDist/((float) objectIter));        //actual or averaged sensor distance away from object
                rawDist = 0.0;                                      //reset raw distance to be used for new object
                objectIter = 0;                                     //reset iterator to be used for new object

                //calculate width of object, convert degrees to radians (degree*pi/180) for tanf() function
                angWidth = 2 * actualDist * tanf((angSize*M_PI)/(2.0*180));

                //Set detected object data
                if(angWidth > 4){
                    setObjectData(object_data, objectCount, angWidth, actualDist, objectLocation);
                }
                else{
                    objectCount--;
                }

            }

            //Polar to Cartesian calculations
            cartFromPolar(degree, irDist, sonarDist, xCartesian, yCartesian);
        }
        //send Arrays of x and y cartesian coordinates to Putty
        //arrayOutput(xCartesian, yCartesian);
        //timer_waitMillis(2000);
        gridPrintPutty(xCartesian, yCartesian);
        timer_waitMillis(3000);
        //send struct of object_data to Putty
        objectDataOutput(object_data, objectCount);
        servo_move(0);
        timer_waitMillis(1000);
    }


    /**
     * takes wasd inputs to move the cybot. will also play a song with another key's input
     */
    void move_cybot(){

        oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);
        char input;
        input = uart_receive();

        if(input == 'q')
        {
            char moveForward[] = "Moving Forward a short distance\r\n";
            uart_sendStr(moveForward);
            move_forward(sensor_data, 5); //move forward
        }

        if (input == 'w')
            {
                char moveForward[] = "Moving Forward\r\n";
                uart_sendStr(moveForward);
                move_forward(sensor_data, 10); //move forward

            }
            //backwards
            else if (input == 's')
            {
                char moveBackwards[] = "Moving Backwards\r\n";
                uart_sendStr(moveBackwards) ;
                move_backwards(sensor_data, -100); //backwards
            }

            //rotates clockwise
            else if (input == 'd')
            {
                char turnRight[] = "Rotating Clockwise 5 degrees\r\n";
                uart_sendStr(turnRight);
                turn_right(sensor_data, 5) ;

            }

            //rotates counterclockwise
            else if (input == 'a')
            {
                char turnLeft[] = "Rotating CountClockwise 5 degrees\r\n";
                uart_sendStr(turnLeft) ;
                turn_left(sensor_data, 5) ;
            }

            //scan surroundings
            else if (input == 'c')
            {
                char scanning[] = "Scanning, please wait \r\n";
                uart_sendStr(scanning);
                scan() ;
            }

            else if (input == 'h'){
                char harvesting[] = "Attempting to harvest, please wait \r\n";
                uart_sendStr(harvesting);
                harvest();
            }

            //play song
            /*else if (input == 'j')
            {
                char song[] = "Playing Song\r\n";
                uart_sendStr(song) ;

                unsigned char notes[] = {};
                unsigned char lengths[] = {};
                oi_loadSong();

                oi_play_song(0);

            }*/

                oi_free(sensor_data);
    }


    /*
     * "harvest" a tree within a thirty degree cone in front of the robot. if nothing is close enough, let the user know
     */
    void harvest(){

        int i;
        int j = 0;
        float distances[16];

        //check a small cone of space in front of the cybot for objects in "harvesting range"
        for(i=75;i<106;i+=2){
            servo_move(i);
            distances[j] = ping_read();
            j++;
        }

        //if values within a cone of range in front of the robot are low enough, "harvest" the object
        for(i=0;i<16;i++){
            if(distances[i] < 15){

                char cutMessage[] = "Tree was cut. \r\n";
                uart_sendStr(cutMessage);
                break;
            }
            else if (i == 15){
                char cutMessage[] = "tree not in range. Please scan again and approach to within 10 cm. \r\n";
                uart_sendStr(cutMessage);
            }

        }


    }

