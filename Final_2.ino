#define BLYNK_TEMPLATE_ID "TMPL6qkLAr6Yu"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "7lqAycrQvDyXylRIWYd9X-HIhllSpzkA"

#include <TimeLib.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <DHT.h>

BlynkTimer timer;

#define DHTPIN 22  
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

unsigned long lastReadTime = 0;

LiquidCrystal lcd(12, 14, 27, 26, 25, 33); 

unsigned long timeout = 1000;  // กำหนดเวลา timeout เป็น 1000ms หรือ 1 วินาที
unsigned long startMillis;

long rtc_sec;
unsigned char day_of_week;

// ข้อมูล WiFi
char ssid[] = "Foki 2.4G";
char pass[] = "foki1822";

Servo myServo1, myServo2, myServo3, myServo4, myServo5;

String timeFormatted1 = "0";
String timeFormatted2 = "0";
String timeFormatted3 = "0";
String timeFormatted4 = "0";
String timeFormatted5 = "0";

BLYNK_WRITE(V1) {
  int selectedTime = param.asInt(); 

  int hours = selectedTime / 3600;
  int minutes = (selectedTime % 3600) / 60;

  timeFormatted1 = String(hours) + ":" + String(minutes);
  // อ่านเวลาปัจจุบัน
  String currentTime = String(hour()) + ":" + String(minute());
  long currentTimeInSeconds = hour() * 3600 + minute() * 60;

  Serial.println("Selected time: " + timeFormatted1);
  Serial.println("Current time: " + currentTime);
  Serial.println(currentTimeInSeconds);
}

BLYNK_WRITE(V2) {
  int selectedTime = param.asInt(); 

  int hours = selectedTime / 3600;
  int minutes = (selectedTime % 3600) / 60;

  timeFormatted2 = String(hours) + ":" + String(minutes);
  // อ่านเวลาปัจจุบัน
  String currentTime = String(hour()) + ":" + String(minute());
  long currentTimeInSeconds = hour() * 3600 + minute() * 60;

  Serial.println("Selected time: " + timeFormatted2);
  Serial.println("Current time: " + currentTime);
  Serial.println(currentTimeInSeconds);
}

BLYNK_WRITE(V3) {
  int selectedTime = param.asInt();

  int hours = selectedTime / 3600;
  int minutes = (selectedTime % 3600) / 60;

  timeFormatted3 = String(hours) + ":" + String(minutes);
  // อ่านเวลาปัจจุบัน
  String currentTime = String(hour()) + ":" + String(minute());
  long currentTimeInSeconds = hour() * 3600 + minute() * 60;

  Serial.println("Selected time: " + timeFormatted3);
  Serial.println("Current time: " + currentTime);
  Serial.println(currentTimeInSeconds);
}

BLYNK_WRITE(V4) {
  int selectedTime = param.asInt(); 

  int hours = selectedTime / 3600;
  int minutes = (selectedTime % 3600) / 60;

  timeFormatted4 = String(hours) + ":" + String(minutes);
  // อ่านเวลาปัจจุบัน
  String currentTime = String(hour()) + ":" + String(minute());
  long currentTimeInSeconds = hour() * 3600 + minute() * 60;

  Serial.println("Selected time: " + timeFormatted4);
  Serial.println("Current time: " + currentTime);
  Serial.println(currentTimeInSeconds);
}

BLYNK_WRITE(V5) {
  int selectedTime = param.asInt();

  int hours = selectedTime / 3600;
  int minutes = (selectedTime % 3600) / 60;

  timeFormatted5 = String(hours) + ":" + String(minutes);

  String currentTime = String(hour()) + ":" + String(minute());
  long currentTimeInSeconds = hour() * 3600 + minute() * 60;

  Serial.println("Selected time: " + timeFormatted5);
  Serial.println("Current time: " + currentTime);
  Serial.println(currentTimeInSeconds);
}

BLYNK_WRITE(InternalPinRTC) {
  const unsigned long DEFAULT_TIME = 1357041600;  // Jan 1 2013
  unsigned long blynkTime = param.asLong();

  if (blynkTime >= DEFAULT_TIME) {
    setTime(blynkTime);

    day_of_week = weekday();

    if (day_of_week == 1)
      day_of_week = 7;
    else
      day_of_week -= 1;

    rtc_sec = (hour() * 60 * 60) + (minute() * 60) + second();

    Serial.println(blynkTime);
    Serial.println(String("RTC Server: ") + hour() + ":" + minute());
  }
}

BLYNK_CONNECTED() {
  Blynk.sendInternal("rtc", "sync");
}


void checkTime() {
  // Show time every second
  Serial.println(String("Time: ") + hour() + ":" + minute());
  Blynk.sendInternal("rtc", "sync");
}

void checkTimeAndTriggerEmergency() {
  String currentTime = String(hour()) + ":" + String(minute());
  Serial.println(currentTime);

  if (currentTime == timeFormatted1) {
    handleEmergency1();
    timeFormatted1 = "0";
  }
  if (timeFormatted2 == currentTime) {
    handleEmergency2();
    timeFormatted2 = "0";
  }
  if (timeFormatted3 == currentTime) {
    handleEmergency3();
    timeFormatted3 = "0";
  }
  if (timeFormatted4 == currentTime) {
    handleEmergency4();
    timeFormatted4 = "0";
  }
  if (timeFormatted5 == currentTime) {
    handleEmergency5();
    timeFormatted5 = "0";
  }
}

void setup() {
  Serial.begin(115200);

  startMillis = millis();

  //dht.begin();
  // เชื่อมต่อกับ WiFi
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi!");
  Serial.println(WiFi.localIP()); 

 
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, checkTimeAndTriggerEmergency); 


  myServo1.attach(13);  
  myServo2.attach(14);
  myServo3.attach(15);
  myServo4.attach(16);
  myServo5.attach(17);


  myServo1.write(45);
  myServo2.write(45);
  myServo3.write(45);
  myServo4.write(45);
  myServo5.write(45);
}


void loop() {
  Blynk.run(); 
  timer.run(); 
  readDHTSensor();
  if (Serial.available() > 0) 
  {
    String Received = Serial.readString(); 
    startMillis = millis();
    if (int(Received[0]) == 1) 
    {
      String currentTime = String(hour()) + ":" + String(minute());
      while ("8:30" != currentTime) {
        handleEmergency1();
        handleEmergency3();
        currentTime = String(hour()) + ":" + String(minute());
      }
    }
    if (int(Received[0]) == 2)
    {
      String currentTime = String(hour()) + ":" + String(minute());
      while ("12:30" != currentTime) {
        handleEmergency2();
        currentTime = String(hour()) + ":" + String(minute());
      }
    }
    if (int(Received[0]) == 3)
    {
      String currentTime = String(hour()) + ":" + String(minute());
      while ("18:30" != currentTime) {
        handleEmergency4();
        handleEmergency5();
        currentTime = String(hour()) + ":" + String(minute());
      }
    }
    if (int(Received[0]) == 4) {
      handleEmergency1();
    }
    if (int(Received[0]) == 5) {
      handleEmergency2();
    }
    if (int(Received[0]) == 6) {
      handleEmergency3();
    }
    if (int(Received[0]) == 7) {
      handleEmergency4();
    }
    if (int(Received[0]) == 8) {
      handleEmergency5();
    }
  }
  if (millis() - startMillis > timeout) {
    Serial.println("No data received for a while");
    startMillis = millis();  // รีเซ็ตเวลาเพื่อเริ่มจับใหม่
  }
}

// ฟังก์ชัน Emergency
void handleEmergency1() {
  triggerServo(myServo1, "EMERGENCY 1");
}
void handleEmergency2() {
  triggerServo(myServo2, "EMERGENCY 2");
}
void handleEmergency3() {
  triggerServo(myServo3, "EMERGENCY 3");
}
void handleEmergency4() {
  triggerServo(myServo4, "EMERGENCY 4");
}
void handleEmergency5() {
  triggerServo(myServo5, "EMERGENCY 5");
}

void triggerServo(Servo &servo, const char *message) {
  lcd.clear();
  lcd.print(message);
  unsigned long startTime = millis();
  
  while (millis() - startTime < 400) {
    servo.write(20);
    delay(1); // small delay to avoid blocking the loop completely
  }
  
  startTime = millis();
  while (millis() - startTime < 400) {
    servo.write(45);
    delay(1); // small delay to avoid blocking the loop completely
  }
  
  lcd.setCursor(0, 1);
  lcd.print("Success!");
}


void readDHTSensor() {
  unsigned long currentTime = millis();
  if (currentTime - lastReadTime > 2000) {
    lastReadTime = currentTime;
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("%, Temp: ");
    Serial.print(t);
    Serial.println("°C");
  }
}