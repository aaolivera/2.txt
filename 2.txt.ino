#include <Adafruit_MPU6050.h>
#include <SD.h>
#define CS_PIN 10

short int  periodo = 250;
short int  periodoFlush = 5000;
unsigned long long int tiempoAhora = 0;
unsigned long long int tiempoAhora2 = 0;
Adafruit_MPU6050 mpu;
File textFile;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");
  setupAcelerometro();
  setupMemory();
}

void setupAcelerometro() {
  Serial.println("1");
  while (!mpu.begin()) {
    
  Serial.println("2");
    delay(1000);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
}

void setupMemory() {
  SD.begin(CS_PIN);
  textFile = SD.open("info.txt", FILE_WRITE);
}

void loop() {
  if (millis() > tiempoAhora + periodo) {    
    writeAceleracion();        
    tiempoAhora = millis();
  }

  if (millis() > tiempoAhora2 + periodoFlush) {
    textFile.flush();
    Serial.println("flush");
    tiempoAhora2 = millis();
  }
}

void writeAceleracion() {
  Serial.println("writeAceleracion");
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  writeNumber("{ t:", millis(), ", ");
  writeNumber("temp:", temp.temperature, ", ");
  writeNumber("a: { X:", a.acceleration.x, ", ");
       writeNumber("Y:", a.acceleration.y, ", ");
       writeNumber("Z:", a.acceleration.z, "}, ");
  writeNumber("g: { X:", g.gyro.x, ", ");
       writeNumber("Y:", g.gyro.y, ", ");
       writeNumber("Z:", g.gyro.z, "}}\r\n");
}

void writeNumber (const char* pre, float numero, const char* pos) {
  char result[8]; // Buffer big enough for 7-character float
  dtostrf(numero, 6, 2, result);
  textFile.write(pre);
  textFile.write(result);
  textFile.write(pos);
}
