#include <Servo.h>
#include <Ultrasonic.h>
#include <stdio.h>

Ultrasonic ultrasonic(8, 9);
Servo myservo;

const int pinLB = 5;
const int pinLF = 4;

const int pinRB = 7;
const int pinRF = 6;

const int LED = 10;

const int Switch = 2;
const int servoPositions[] = {90,60, 90, 120};
const int startingLevel = 1;
const int length = sizeof(servoPositions)/sizeof(servoPositions[0]);

volatile int level = startingLevel; // Set as volatile since it can be changed in ISR

void setup()
{
    Serial.begin(9600);
    delay(1000);
    Serial.println("Starting...");
    randomSeed(analogRead(A0));  // Seed the random function using an analog pin

    myservo.attach(3);
    myservo.write(90);

    pinMode(pinLB, OUTPUT);
    pinMode(pinLF, OUTPUT);
    pinMode(pinRB, OUTPUT);
    pinMode(pinRF, OUTPUT);

    pinMode(LED, OUTPUT);
    pinMode(Switch, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(Switch), switchPressed, FALLING);
}

void loop() {
    static int distance = 0;
    static int started = 0;

    if (level == 1) {
        distance = readUltrasonic();
        if ((distance > 40) && (started == 0)) {
            forward();
            started = 1;
        } else {
            // Stop and check if we need to rotate to avoid obstacles
            for (int i = 0; i < length; i++) {  // Start from index 0
                Serial.print("i: ");
                Serial.println(i);
                delay(20);
                turnServo(servoPositions[i], (i > 0) ? servoPositions[i-1] : 90, 15);

                distance = readUltrasonic();
                Serial.print("Distance: ");
                Serial.println(distance);

                if (distance < 40 && distance != 0) {
                    stop();  // Stop immediately if obstacle is detected
                    avoidObstacle();  // Avoid obstacle logic (explained below)
                    break;  // Break out of the servo movement loop once an obstacle is detected
                }
            }

            // After obstacle avoidance, continue moving forward if the path is clear
            distance = readUltrasonic();
            if (distance > 40) {
                forward();  // Resume forward motion after obstacle avoidance
                Serial.println("Path is clear, moving forward...");
            }
        }
    } else if (level == 2) {
        // Level 2 behavior with random movements
        static unsigned long lastDirectionChange = 0;
        static int randomTime = 0;

        if (millis() - lastDirectionChange > randomTime) {
            randomTime = random(1000, 3000); // Change direction every 1 to 3 seconds
            lastDirectionChange = millis();

            int randomDirection = random(0, 4); // 0 = forward, 1 = backward, 2 = turnLeft, 3 = turnRight
            switch (randomDirection) {
                case 0:
                    forward();
                    Serial.println("Moving forward");
                    break;
                case 1:
                    backward();
                    Serial.println("Moving backward");
                    break;
                case 2:
                    turnLeft();
                    Serial.println("Turning left");
                    break;
                case 3:
                    turnRight();
                    Serial.println("Turning right");
                    break;
            }
        }
    }
}

void test_motors()
{
    delay(5000);
    forward();
    delay(3000);
    backward();
    delay(3000);
    stop();
    delay(3000);
    turnRight();
    delay(3000);
    turnLeft();
    delay(3000);
    rotateRight();
    delay(3000);
    rotateLeft();
    delay(3000);
    stop();
}

void forward()
{
    digitalWrite(pinLB, HIGH);
    digitalWrite(pinLF, LOW);
    digitalWrite(pinRB, HIGH);
    digitalWrite(pinRF, LOW);
}

void backward()
{
    digitalWrite(pinLB, LOW);
    digitalWrite(pinLF, HIGH);
    digitalWrite(pinRB, LOW);
    digitalWrite(pinRF, HIGH);
}

void stop(){
    digitalWrite(pinLB, LOW);
    digitalWrite(pinLF, LOW);
    digitalWrite(pinRB, LOW);
    digitalWrite(pinRF, LOW);
}

void turnRight(){
    digitalWrite(pinLB, HIGH);
    digitalWrite(pinLF, LOW);
    digitalWrite(pinRB, LOW);
    digitalWrite(pinRF, LOW);
}

void turnLeft(){
    digitalWrite(pinLB, LOW);
    digitalWrite(pinLF, LOW);
    digitalWrite(pinRB, HIGH);
    digitalWrite(pinRF, LOW);
}

void rotateRight(){
    digitalWrite(pinLB, HIGH);
    digitalWrite(pinLF, LOW);
    digitalWrite(pinRB, LOW);
    digitalWrite(pinRF, HIGH);
}

void rotateLeft(){
    digitalWrite(pinLB, LOW);
    digitalWrite(pinLF, HIGH);
    digitalWrite(pinRB, HIGH);
    digitalWrite(pinRF, LOW);
}

void turnServo(int angle, int starting_angle, int delayTime)
{
    if (angle > starting_angle)
    {
        for (int i = starting_angle; i <= angle; i++)
        {
            myservo.write(i);
            delay(delayTime);
        }
    }
    else
    {
        for (int i = starting_angle; i >= angle; i--)
        {
            myservo.write(i);
            delay(delayTime);
        }
    }

    delay(500);
}

void switchPressed() {

    static long last_interrupt_time = 0;
    long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 200){
        Serial.println("Switch Pressed");
        digitalWrite(LED, HIGH);
        delay(1000);
        digitalWrite(LED, LOW);
        level = 2; // Change to level 2 after switch press
    }
    last_interrupt_time = interrupt_time;
}

int readUltrasonic(){
    int distance = ultrasonic.read();
    return distance;
}

void avoidObstacle() {
    // Stop and rotate to clear the obstacle
    do {
            rotateRight();  // Rotate right to try to avoid the obstacle
            delay(200);    // Delay to allow rotation
            stop();         // Stop after rotating
            delay(50);     // Stabilize after stopping
        } while (readUltrasonic() < 40);  // Continue rotating if an obstacle is still detected

        // After obstacle is avoided, resume forward movement
        delay(500);  // Small delay before resuming forward motion
    //forward();
}
