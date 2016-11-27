
boolean checkValue(const int pin)
{
  int runningCnt = 0;

  for (int i = 0; i < 25; i++)
  {
    if (digitalRead(pin) == HIGH)
    {
      runningCnt++;
    }
  }

  return runningCnt > 12;
}

void checkWasher()
{
  boolean current = checkValue(washerPin);
  if (current != washerAlarm)
  {
    if (current)
    {
      washerAlarm = true;
      lastWasherTime = now();
    }
    else
    {
      if (now() - lastWasherTime >= 900)
      {
        washerAlarm = false;
        lastWasherTime = now();
      }
    }
  }
}

void checkDryer()
{
  boolean current = checkValue(dryerPin);
  if (current != dryerAlarm)
  {
    lastDryerTime = now();
    dryerAlarm = current;
  } 
}
