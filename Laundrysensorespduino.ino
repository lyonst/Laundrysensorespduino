#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>

const int washerPin = 0;
const int dryerPin = 3; 
int washerVal = 0;
int dryerVal = 0;
boolean washerAlarm = false;
boolean dryerAlarm = false;

unsigned long lastWasherTime = 0;
unsigned long lastDryerTime = 0;

int alarmTime = 1800000;

const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets

unsigned long nextTimeUpdate = 0;

WiFiUDP Udp;
WiFiServer server(80);

void checkForWebRequest();
boolean checkValue(const int pin);
void checkWasher();
void checkDryer();

void sendWasherStatus();
void sendDryerStatus();
void sendTime();

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(250);

  pinMode(0, INPUT);
  pinMode(3, INPUT);

  SetupWiFi();

  Udp.begin(8888);

  updateClock();

  server.begin();
}

void loop() {
  if ((long)(millis() - nextTimeUpdate) >= 0)
  {
    updateClock();
  }
  
  checkWasher();
  checkDryer();
  checkForWebRequest();  

  delay(2000);
}

void updateClock()
{
  time_t currentTime;

  int i = 0;

  while ((currentTime = getNtpTime()) == 0 && i < 5)
  {
    i++;
  }

  if (currentTime != 0)
  {
    setTime(currentTime);
  }

  nextTimeUpdate = millis() + 86400000;
}

void SetupWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin("ssid", "password");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Not connected");
    delay(500);
  }

  Serial.print("Connected.\t");
  Serial.println(WiFi.localIP());
}
