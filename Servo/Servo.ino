#include <Servo.h>
Servo myservo;

int pinLB = 18;
int pinLF = 17;

int pinRB = 16;
int pinRF = 15;


void setup()
{
    myservo.attach(3);
    pinMode(pinLB, OUTPUT);
    pinMode(pinLF, OUTPUT);
    pinMode(pinRB, OUTPUT);
    pinMode(pinRF, OUTPUT);



}

void loop()
{
    turnServo(0, 90);
    turnServo(180, 0);
    turnServo(90, 180);

    delay(4000);
}

void turnServo(int angle, int starting_angle)
{
    if (angle > starting_angle)
    {
        for (int i = starting_angle; i <= angle; i++)
        {
            myservo.write(i);
            delay(15);
        }
    }
    else
    {
        for (int i = starting_angle; i >= angle; i--)
        {
            myservo.write(i);
            delay(15);
        }
    }

    delay(2000);
}


void forward(int time_in_millisec)
{
    digitalWrite(pinLB, HIGH);
    digitalWrite(pinLF, LOW);
    digitalWrite(pinRB, HIGH);
    digitalWrite(pinRF, LOW);

    delay(time_in_millisec);
}
