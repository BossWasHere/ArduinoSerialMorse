#include <Arduino.h>

#include "morse.h"

#define LED_PIN LED_BUILTIN
#define DELAY_DELIM '\\'
#define MIN_DOT_DELAY 20
#define ENABLE_FEEDBACK

unsigned int dotDelay = 200;

void flashSequence(const char *sequence);
void flashDotOrDash(char dotOrDash);

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);

  Serial.println("Serial Morse started");
}

void loop()
{
  char ch;
  char buf[8];

  int validChar = 1;

  if (Serial.available())
  {
    ch = Serial.read();
    if (ch == DELAY_DELIM)
    {
      memset(buf, 0, sizeof(buf));
      Serial.readBytesUntil(DELAY_DELIM, buf, sizeof(buf) - 1);
      dotDelay = strtoul(buf, nullptr, 10);

      if (dotDelay < MIN_DOT_DELAY)
      {
        dotDelay = MIN_DOT_DELAY;
      }
      Serial.print("Dot delay set to: ");
      Serial.print(dotDelay);
      Serial.println("ms");
      return;
    }
    else if (ch >= 'a' && ch <= 'z')
    {
      flashSequence(letters[ch - 'a']);
    }
    else if (ch >= 'A' && ch <= 'Z')
    {
      flashSequence(letters[ch - 'A']);
    }
    else if (ch >= '0' && ch <= '9')
    {
      flashSequence(numbers[ch - '0']);
    }
    else if (ch == ' ')
    {
      delay(dotDelay * 4);
    }
    else
    {
      validChar = 0;
    }

    #ifdef ENABLE_FEEDBACK
    if (validChar)
    {
      Serial.write(ch);
    }
    else
    {
      Serial.write('-');
    }
    #endif
  }
}

void flashSequence(const char *sequence)
{
  int i = 0;
  while (sequence[i] != '\0')
  {
    flashDotOrDash(sequence[i]);
    i++;
  }
  delay(dotDelay * 3);
}

void flashDotOrDash(char dotOrDash)
{
  digitalWrite(LED_PIN, HIGH);
  if (dotOrDash == '.')
  {
    delay(dotDelay);
  }
  else // must be a -
  {
    delay(dotDelay * 3);
  }
  digitalWrite(LED_PIN, LOW);
  delay(dotDelay);
}