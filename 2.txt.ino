#include <Adafruit_MPU6050.h>
#include <Adafruit_BMP085.h>
#include <SD.h>
#define CS_PIN 10

short int  periodo = 100;
unsigned long long int tiempoAhora = 0;
Adafruit_MPU6050 mpu;
Adafruit_BMP085 bmp;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  setupAcelerometro();
  setupAltimetro();
  setupMemory();
}

void setupAcelerometro() {
  while (!mpu.begin()) delay(100);
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
}

void setupAltimetro() {
  while (!bmp.begin()) delay(100);
}

void setupMemory() {
  if (!SD.begin(CS_PIN)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");
}

void loop() {
  if (millis() > tiempoAhora + periodo) {
    String dataString = "{t:" + String(millis());
    readAceleracion(dataString);
    readAltimetro(dataString);
    dataString += "}";
    Serial.println(dataString);

    File dataFile = SD.open("info.txt", FILE_WRITE);
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }

    tiempoAhora = millis();
  }
}

void readAceleracion(String& dataString) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  dataString += ",\"temp\":" + String(temp.temperature);
  dataString += ",\"a\":{\"X\":" + String(a.acceleration.x);
  dataString += ",\"Y\":" + String(a.acceleration.y);
  dataString += ",\"Z\":" + String(a.acceleration.z) + "}";
  dataString += ",\"g\":{\"X\":" + String(g.gyro.x);
  dataString += ",\"Y\":" + String(g.gyro.y);
  dataString += ",\"Z\":" + String(g.gyro.z) + "}";
}

void readAltimetro(String& dataString) {
  dataString += ",\"p\":{\"P\":" + String(bmp.readPressure());
  dataString += ",\"A\":" + String(bmp.readAltitude()) + "}";
}
