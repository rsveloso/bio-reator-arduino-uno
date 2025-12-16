#ifndef ULTRASSOM_H
#define ULTRASSOM_H

#include <Arduino.h>

class Ultrassom {
    public:
        Ultrassom(int trigPin, int echoPin);
        float lerDistancia();

    private:
        int trig;
        int echo;
};

#endif
