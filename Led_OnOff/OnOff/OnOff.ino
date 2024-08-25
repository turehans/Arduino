
#include <Arduino.h>

int LED_PORT1 = 4;
int SWITCH_PORT1 = 6;

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    pinMode(LED_PORT1, OUTPUT);
}

void loop()
{
    digitalWrite(LED_PORT1, HIGH);
    delay(200);
    digitalWrite(LED_PORT1, LOW);
    delay(200);
}
