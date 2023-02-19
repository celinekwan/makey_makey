const int PHOTORESISTOR_PIN = A0;
const double ANALOG_OUTPUT_CONVERSION = 0.0048828125;
const double RESISTOR = 10000;
const int VIN = 5;

const int buzzPin = 5;
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor

double brightStart;
boolean brightTrack;
double dimStart;
boolean dimTrack;
boolean closeTrack;
boolean buzzed = false;

void setup() {
  Serial.begin(9600);
  pinMode(PHOTORESISTOR_PIN, INPUT);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  digitalWrite(buzzPin, LOW);
}

void loop() {
  // Measure luminosity
  int analogValue = analogRead(PHOTORESISTOR_PIN);
  double luminosity = luminosityLevel(analogValue);

  // Measure distance
  long duration, inches, cm;
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  cm = microsecondsToCentimeters(duration);

  // Too bright for too long
  if (luminosity > 1000) {
    if (!brightTrack) {
      brightTrack = !brightTrack;
      brightStart = millis();
    } else if ( millis() - brightStart > 5000) {
      if (!buzzed) {
        buzzed = !buzzed;
        buzz();
       }
       Serial.println("b");
    } else {
      Serial.println("n");
    }

  // Too dim for too long
  } else if (luminosity < 250) {
    if (!dimTrack) {
      dimTrack = !dimTrack;
      dimStart = millis();
    } else if (millis() - dimStart > 5000) {
       if (!buzzed) {
        buzzed = !buzzed;
        buzz();
       }
       Serial.println("d");
    } else {
      Serial.println("n");
    }

  // Too close
  } else if (cm < 20) {
    if (!closeTrack) {
      closeTrack = !closeTrack;
      if (!buzzed) {
        buzzed = !buzzed;
        buzz();
      }
    }
    Serial.println("c");
    
  } else {
    Serial.println("n");
  }
  
  delay(250);
}

double luminosityLevel(int analogValue) {
  // Convert analog value to voltage
  double Vout = double(analogValue) * ANALOG_OUTPUT_CONVERSION;
  
  // Convert voltage output to luminosity (unit lux)
  int luminosity = 500 /(RESISTOR / 1000 * ((VIN-Vout)/Vout));
   
  return luminosity;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void buzz() {
  digitalWrite(buzzPin, HIGH);
  delay(10);
  digitalWrite(buzzPin, LOW);
}