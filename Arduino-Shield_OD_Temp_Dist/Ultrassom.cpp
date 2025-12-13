#include "include/Ultrassom.h"

Ultrassom::Ultrassom(int trigPin, int echoPin) {
    trig = trigPin;
    echo = echoPin;

    pinMode(trig, OUTPUT);
    pinMode(echo, INPUT);
}

float Ultrassom::lerDistancia() {
    long duration;

    digitalWrite(trig, LOW);
    delayMicroseconds(2);

    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH);

    float distance = (duration * 0.0343) / 2;

    return distance;
}
