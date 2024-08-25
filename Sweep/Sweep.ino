#include <Servo.h>
#include <Ultrasonic.h>

Ultrasonic ultrasonic(8, 9);
int distance;
Servo myservo;

int pinLB = 5;
int pinLF = 4;

int pinRB = 7;
int pinRF = 6;

int LED = 10;

int Switch = 2;




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

    myservo.write(0);
    distance = ultrasonic.read();

    Serial.print("Distance in CM: ");
    Serial.println(distance);


    delay(2000);

    myservo.write(90);

    distance = ultrasonic.read();

    Serial.print("Distance in CM: ");
    Serial.println(distance);


    delay(2000);
    myservo.write(180);
    distance = ultrasonic.read();

    Serial.print("Distance in CM: ");
    Serial.println(distance);

    delay(2000);
    myservo.write(90);
    distance = ultrasonic.read();

    Serial.print("Distance in CM: ");
    Serial.println(distance);

    delay(10000);

    //forward(10000);
}


void forward(int time_in_millisec)
{
    digitalWrite(pinLB, HIGH);
    digitalWrite(pinLF, LOW);
    digitalWrite(pinRB, HIGH);
    digitalWrite(pinRF, LOW);
    delay(time_in_millisec);
    digitalWrite(pinLB, LOW);
    digitalWrite(pinLF, LOW);
    digitalWrite(pinRB, LOW);
    digitalWrite(pinRF, LOW);
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
