#include <Arduino.h>

#define RED_PIN     32
#define YELLOW_PIN  33
#define GREEN_PIN   25
#define BUZZER_PIN  26
#define TOUCH_PIN   T0

#define RED_STATE         0
#define RED_YELLOW_STATE  1
#define YELLOW_STATE      2
#define GREEN_STATE       3

#define RED_MILLIS        10000
#define RED_YELLOW_MILLIS  2000
#define YELLOW_MILLIS      2000
#define GREEN_HOLD_MILLIS  5000

#define GREEN_BUZZ_ON_MS    500
#define GREEN_BUZZ_OFF_MS  1500
#define RED_BUZZ_ON_MS      250
#define RED_BUZZ_OFF_MS     250

#define TOUCH_THRESHOLD  30

int tl_state;
unsigned long tl_timer;
bool button_active = false;
bool buzzer_on = false;
unsigned long buzzer_timer = 0;

bool touchButtonPressed()
{
  return touchRead(TOUCH_PIN) < TOUCH_THRESHOLD;
}

void setup()
{
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  tl_state = RED_STATE;
  tl_timer = millis() + RED_MILLIS;
}

void loop()
{
  unsigned long now = millis();

  // First press during green latches the request and starts the 5 s hold.
  if (tl_state == GREEN_STATE && !button_active && touchButtonPressed())
  {
    button_active = true;
    tl_timer = now + GREEN_HOLD_MILLIS;
  }

  switch (tl_state)
  {
    case RED_STATE:
      digitalWrite(RED_PIN, HIGH);
      if (now >= tl_timer)
      {
        tl_state = RED_YELLOW_STATE;
        tl_timer = now + RED_YELLOW_MILLIS;
      }
      break;

    case RED_YELLOW_STATE:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(YELLOW_PIN, HIGH);
      if (now >= tl_timer)
      {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(YELLOW_PIN, LOW);
        tl_state = GREEN_STATE;
      }
      break;

    case YELLOW_STATE:
      digitalWrite(YELLOW_PIN, HIGH);
      if (now >= tl_timer)
      {
        digitalWrite(YELLOW_PIN, LOW);
        tl_state = RED_STATE;
        tl_timer = now + RED_MILLIS;
      }
      break;

    case GREEN_STATE:
      digitalWrite(GREEN_PIN, HIGH);
      if (button_active && now >= tl_timer)
      {
        digitalWrite(GREEN_PIN, LOW);
        button_active = false;
        tl_state = YELLOW_STATE;
        tl_timer = now + YELLOW_MILLIS;
      }
      break;
  }

  // Buzzer: green 500/1500 ms, red 250/250 ms, silent in transitions.
  if (tl_state == GREEN_STATE || tl_state == RED_STATE)
  {
    unsigned long on_ms  = (tl_state == GREEN_STATE) ? GREEN_BUZZ_ON_MS  : RED_BUZZ_ON_MS;
    unsigned long off_ms = (tl_state == GREEN_STATE) ? GREEN_BUZZ_OFF_MS : RED_BUZZ_OFF_MS;

    if (now >= buzzer_timer)
    {
      buzzer_on = !buzzer_on;
      digitalWrite(BUZZER_PIN, buzzer_on ? HIGH : LOW);
      buzzer_timer = now + (buzzer_on ? on_ms : off_ms);
    }
  }
  else
  {
    buzzer_on = false;
    digitalWrite(BUZZER_PIN, LOW);
    buzzer_timer = now;
  }
}
