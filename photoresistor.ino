const int PHOTORESISTOR_PIN = A0;
const double ANALOG_OUTPUT_CONVERSION = 0.0048828125;
const double RESISTOR = 10000;
const int VIN = 5;

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
  
  Serial.print("Light Intensity: ");
  Serial.print(luminosity);
  Serial.println(" Lux");

  delay(500);
}