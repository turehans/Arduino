#include <Servo.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic(8, 9);
Servo myservo;

const int pinLB = 5;
const int pinLF = 4;

const int pinRB = 7;
const int pinRF = 6;

const int LED = 10;

const int Switch = 2;




void setup()
{
    Serial.begin(9600);

    myservo.attach(3);

    pinMode(pinLB, OUTPUT);
    pinMode(pinLF, OUTPUT);
    pinMode(pinRB, OUTPUT);
    pinMode(pinRF, OUTPUT);

    pinMode(LED, OUTPUT);

    pinMode(Switch, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Switch), switchPressed, FALLING);
}

void loop()
{
    static int distance = 0;
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
    }
    last_interrupt_time = interrupt_time;
}

int readUltrasonic(){
    int distance = ultrasonic.read();
    return distance;
}
