    /*
     * mission commands file
     * contains functions that take human input through putty in order to drive/scan surroundings
     */



    //Struct for detected object data.
    struct object{
        float width;
        float distance;
        int count;
        int location;
    };


    /**
     * scan an area with the infrared sensor to detect the center of objects and their distances from the cybot
     */
    void scan();



    /**
     * takes wasd inputs to move the cybot. will also play a song with another key's input
     */
    void move_cybot();





    /*
       * "harvest" a tree within a thirty degree cone in front of the robot. if nothing is close enough, let the user know
       */
      void harvest();
