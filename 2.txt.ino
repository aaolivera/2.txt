#include <Adafruit_MPU6050.h>
#include <SD.h>
#define CS_PIN 10

short int  periodo = 100;
unsigned long long int tiempoAhora = 0;
Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  setupAcelerometro();
  setupMemory();
}

void setupAcelerometro() {
  while (!mpu.begin()) delay(100);
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
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
    String dataString = "";
    readAceleracion(dataString);
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
  dataString += "{t:" + String(millis());
  dataString += ",temp:" + String(temp.temperature);
  dataString += ",a:{ X:" + String(a.acceleration.x);
  dataString += ",Y:" + String(a.acceleration.y);
  dataString += ",Z:" + String(a.acceleration.z);
  dataString += "},g:{ X:" + String(g.gyro.x);
  dataString += ",Y:" + String(g.gyro.y);
  dataString += ",Z:" + String(g.gyro.z) + "}}";
}
