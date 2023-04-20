#include <Arduino.h>

#define LED_LEFT 12
#define LED_RIGHT 13
#define TACTILE_PIN 27
#define POTENT_PIN 34
#define BUZZ_CHAN 0

static volatile int count{0};

void beep(int);

void IRAM_ATTR isr();

void setup()
{
  Serial.begin(115200);
  pinMode(TACTILE_PIN, INPUT_PULLUP);
  attachInterrupt(TACTILE_PIN, isr, FALLING);
}

void loop()
{
  static int value = 10;
  analogWrite(LED_LEFT, 255); // left LED on
  analogWrite(LED_RIGHT, 0);  // right LED off
  beep(value);
  delay(1000);

  value = map(analogRead(POTENT_PIN), 0, 4095, 0, 255); // analogRead outputs 0-4095, need to map it to 0-255
  analogWrite(LED_LEFT, 0);                             // left LED off
  analogWrite(LED_RIGHT, 255);                          // right LED on
  beep(value);
  delay(1000);
  value = map(analogRead(POTENT_PIN), 0, 4095, 0, 255);
}

void beep(int duty_cycle)
{
  ledcSetup(BUZZ_CHAN, 50, 8);      // set up PWM channel 0
  ledcAttachPin(14, BUZZ_CHAN);     // attach channel to pin 14
  ledcWrite(BUZZ_CHAN, duty_cycle); // output 50% duty cycle
  delay(250);                       // sound for 1 secound
  ledcWrite(BUZZ_CHAN, 0);          // output 0% duty cycle
  ledcDetachPin(14);                // release the channel
}

void IRAM_ATTR isr()
{
  long curr = millis();
  static long prev{0};
  if ((curr - prev) < 200)
  {
    return;
  }
  Serial.println("Prev value: ");
  Serial.println(count);

  // increment counts
  count++;
  prev = curr;
  Serial.println("Current value: ");
  Serial.println(count);
  Serial.println();
}