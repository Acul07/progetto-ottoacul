#include "FastIMU.h"
#include <Wire.h>

#define IMU_ADDRESS 0x68    //Change to the address of the IMU
#define PERFORM_CALIBRATION //Comment to disable startup calibration
BMI160 IMU;               //Change to the name of any supported IMU!               //Change to the name of any supported IMU! 

// Currently supported IMUS: MPU9255 MPU9250 MPU6886 MPU6500 MPU6050 ICM20689 ICM20690 BMI055 BMX055 BMI160 LSM6DS3 LSM6DSL QMI8658

calData calib = { 0 };  //Calibration data
AccelData accelData;    //Sensor data
GyroData gyroData;
MagData magData;


unsigned long tempoora = 0;
unsigned long tempoinizio = 0;

const int sens_rotazione = A0;   // sensore analogico (potenziometro)
const int sens_infrarossi = 9;   // PWM output

#define INT_SENS_ROT 4          // sensore digitale rotazione
#define INT_SENS_INFRA 3         // sensore infrarossi
#define INT_SENS_GRAV 5         // sensore di gravita

int Stato_sens_rot;
int Stato_sens_infra;
int Stato_sens_grav;

int sensorValue = 0;
int outputValue = 0;

const int buttonPin = 4;         // PIN CORRETTO (non più 2)

void setup() {
  pinMode(INT_SENS_ROT, INPUT_PULLUP);
  pinMode(INT_SENS_INFRA, INPUT_PULLUP);
  pinMode(INT_SENS_GRAV, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

   Wire.begin();
  Wire.setClock(400000); //400khz clock
  Serial.begin(115200);
  while (!Serial) {
    ;
}


int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0) {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }
  
#ifdef PERFORM_CALIBRATION
  Serial.println("FastIMU calibration & data example");
  if (IMU.hasMagnetometer()) {
    delay(1000);
    Serial.println("Move IMU in figure 8 pattern until done.");
    delay(3000);
    IMU.calibrateMag(&calib);
    Serial.println("Magnetic calibration done!");
  }
  else {
    delay(5000);
  }

  delay(5000);
  Serial.println("Keep IMU level.");
  delay(5000);
  IMU.calibrateAccelGyro(&calib);
  Serial.println("Calibration done!");
  Serial.println("Accel biases X/Y/Z: ");
  Serial.print(calib.accelBias[0]);
  Serial.print(", ");
  Serial.print(calib.accelBias[1]);
  Serial.print(", ");
  Serial.println(calib.accelBias[2]);
  Serial.println("Gyro biases X/Y/Z: ");
  Serial.print(calib.gyroBias[0]);
  Serial.print(", ");
  Serial.print(calib.gyroBias[1]);
  Serial.print(", ");
  Serial.println(calib.gyroBias[2]);
  if (IMU.hasMagnetometer()) {
    Serial.println("Mag biases X/Y/Z: ");
    Serial.print(calib.magBias[0]);
    Serial.print(", ");
    Serial.print(calib.magBias[1]);
    Serial.print(", ");
    Serial.println(calib.magBias[2]);
    Serial.println("Mag Scale X/Y/Z: ");
    Serial.print(calib.magScale[0]);
    Serial.print(", ");
    Serial.print(calib.magScale[1]);
    Serial.print(", ");
    Serial.println(calib.magScale[2]);
  }
  delay(5000);
  IMU.init(calib, IMU_ADDRESS);
#endif

  //err = IMU.setGyroRange(500);      //USE THESE TO SET THE RANGE, IF AN INVALID RANGE IS SET IT WILL RETURN -1
  //err = IMU.setAccelRange(2);       //THESE TWO SET THE GYRO RANGE TO ±500 DPS AND THE ACCELEROMETER RANGE TO ±2g
  
  if (err != 0) {
    Serial.print("Error Setting range: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }
  
}



void loop() {
  tempoora = millis();

  // ===== ESEGUI IL CODICE OGNI 3 SECONDI =====
  if (tempoora - tempoinizio >= 3000) {
    tempoinizio = tempoora; // reset timer

    // Legge i sensori ogni 3 secondi
    Stato_sens_rot   = digitalRead(INT_SENS_ROT);
    Stato_sens_infra = digitalRead(INT_SENS_INFRA);
    Stato_sens_grav = digitalRead(INT_SENS_GRAV);

    // --- Sensore rotazione ---
    if (Stato_sens_rot == LOW) { // LOW perché INPUT_PULLUP
      sensorValue = analogRead(sens_rotazione);
      outputValue = map(sensorValue, 0, 1023, 0, 255);
      analogWrite(sens_infrarossi, outputValue);

      Serial.print("sensorRot = ");
      Serial.println(sensorValue);
      Serial.print("output = ");
      Serial.println(outputValue);
    } else {
      Serial.println("nessun sensore di rotazione");
    }

    // --- Sensore infrarossi ---
    if (Stato_sens_infra == LOW) { // LOW perché INPUT_PULLUP
      int buttonState = digitalRead(buttonPin);
      Serial.print("sensorInfra = ");
      Serial.println(buttonState);
    } else {
      Serial.println("nessun sensore di vibrazione");
    }




    //--- Sensore di gravita ---
    if (Stato_sens_grav == LOW) {
      IMU.update();
  IMU.getAccel(&accelData);
  Serial.print(accelData.accelX);
  Serial.print("\t");
  Serial.print(accelData.accelY);
  Serial.print("\t");
  Serial.print(accelData.accelZ);
  Serial.print("\t");
  IMU.getGyro(&gyroData);
  Serial.print(gyroData.gyroX);
  Serial.print("\t");
  Serial.print(gyroData.gyroY);
  Serial.print("\t");
  Serial.print(gyroData.gyroZ);
  if (IMU.hasMagnetometer()) {
    IMU.getMag(&magData);
    Serial.print("\t");
    Serial.print(magData.magX);
    Serial.print("\t");
    Serial.print(magData.magY);
    Serial.print("\t");
    Serial.print(magData.magZ);
  }
  if (IMU.hasTemperature()) {
	  Serial.print("\t");
	  Serial.println(IMU.getTemp());
  }
  else {
    Serial.println();
  }
  delay(50);
    }else{
      Serial.println("nessun sensore di gravita");
    }

    Serial.println("-----------------------"); // separatore
  }
}