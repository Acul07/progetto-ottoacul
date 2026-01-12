unsigned long tempoora = 0;
unsigned long tempoinizio = 0;

const int sens_rotazione = A0;   // sensore analogico (potenziometro)
const int sens_infrarossi = 9;   // PWM output

#define INT_SENS_ROT 2           // sensore digitale rotazione
#define INT_SENS_INFRA 3         // sensore infrarossi

int Stato_sens_rot;
int Stato_sens_infra;

int sensorValue = 0;
int outputValue = 0;

const int buttonPin = 4;         // PIN CORRETTO (non più 2)

void setup() {
  pinMode(INT_SENS_ROT, INPUT_PULLUP);
  pinMode(INT_SENS_INFRA, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  Serial.begin(115200);
}

void loop() {
  tempoora = millis();

  // ===== ESEGUI IL CODICE OGNI 3 SECONDI =====
  if (tempoora - tempoinizio >= 3000) {
    tempoinizio = tempoora; // reset timer

    // Legge i sensori ogni 3 secondi
    Stato_sens_rot   = digitalRead(INT_SENS_ROT);
    Stato_sens_infra = digitalRead(INT_SENS_INFRA);

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

    Serial.println("-----------------------"); // separatore
  }
}
