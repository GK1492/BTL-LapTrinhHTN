#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

#define ENA 4  // maps to GPIO23
#define ENB 5  // maps to GPIO24
#define IN1 21 // maps to GPIO5
#define IN2 22 // maps to GPIO6
#define IN3 23 // maps to GPIO13
#define IN4 24 // maps to GPIO19

#define LEFT_SENSOR_PIN 1 // maps to GPIO18
#define CENTER_SENSOR_PIN 2 // maps to GPIO27
#define RIGHT_SENSOR_PIN 3 // maps to GPIO22

// Initialize GPIO pins
void setup() {
    wiringPiSetup();
    pinMode(LEFT_SENSOR_PIN, INPUT);
    pinMode(CENTER_SENSOR_PIN, INPUT);
    pinMode(RIGHT_SENSOR_PIN, INPUT);
    pinMode(ENA, PWM_OUTPUT);
    pinMode(ENB, PWM_OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(1024);
    pwmSetClock(32);
}

void setMotorSpeed(int speed) {
    pwmWrite(ENA, speed);
    pwmWrite(ENB, speed);
}

void controlRobot(char direction) {
    int speed = 512; // 50% speed

    setMotorSpeed(speed);

    switch (direction) {
        case 'S': // Move straight
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            break;
        case 'L': // Turn left
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);
            break;
        case 'R': // Turn right
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);
            break;
        case 'X':
            // Stop
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, LOW);
            setMotorSpeed(0);
            break;
    }
}

char readSensors() {
    int left = digitalRead(LEFT_SENSOR_PIN);
    int center = digitalRead(CENTER_SENSOR_PIN);
    int right = digitalRead(RIGHT_SENSOR_PIN);

    if ((left == HIGH && center == HIGH && right == HIGH) || 
       (left == LOW && center == LOW && right == LOW )) {
        return 'X'; 
    }

    if (center == HIGH && right == LOW && left == LOW) {
        return 'S'; // Line detected in the center
    } else if (left == LOW && right == HIGH) {
        return 'R'; // Line detected on the right
    } else if (right == LOW && left == HIGH) {
        return 'L'; // Line detected on the left
    } else {
        return 'X'; 
    }
}

int main() {
    setup();
    while (1) {
        char direction = readSensors();
        controlRobot(direction);
        delay(50);
    }

    return 0;
}

