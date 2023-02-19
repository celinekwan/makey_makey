const int PHOTORESISTOR_PIN = A0;
const double ANALOG_OUTPUT_CONVERSION = 0.0048828125;
const double RESISTOR = 10000;
const int VIN = 5;

double brightStart;
boolean brightTrack;
double dimStart;
boolean dimTrack;

double luminosityLevel(int analogValue) {
  // Convert analog value to voltage
  double Vout = double(analogValue) * ANALOG_OUTPUT_CONVERSION;
  
  // Convert voltage output to luminosity (unit lux)
  int luminosity = 500 /(RESISTOR / 1000 * ((VIN-Vout)/Vout));
   
  return luminosity;
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  int analogValue = analogRead(PHOTORESISTOR_PIN);
  double luminosity = luminosityLevel(analogValue);
  Serial.println(luminosity);

  if (luminosity > 1000) {
    if (!brightTrack) {
      brightTrack = !brightTrack;
      brightStart = millis();
    } else if ( millis() - brightStart > 5000) {
       Serial.println("b");
    } else {
      Serial.println("n");
    }
  } else if (luminosity < 250) {
    if (!dimTrack) {
      dimTrack = !dimTrack;
      dimStart = millis();
    } else if (millis() - dimStart > 5000) {
       Serial.println("d");
    } else {
      Serial.println("n");
    }
  } else {
    Serial.println("n");
  }
  
  delay(250);
}