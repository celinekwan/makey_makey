//int sensor1 = A0;
//int sensor2 = A1;

const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
const int photoresistorPin = A0; // Photoresistor Pin 

// globals for lux conversion
const double ANALOG_OUTPUT_CONVERSION = 0.0048828125;
const double RESISTOR = 10000;
const int VIN = 5;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Starting Serial Terminal
  pinMode(photoresistorPin, INPUT);// Set pResistor - A0 pin as an input (optional)
  //pinMode(sensor1, INPUT);
  //pinMode(sensor2, INPUT);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

//globals
int distance, brightness; //store data from both sensors
double luxBrightness; // store lux value 
int freq = 1000; //data collection frequency ~x milliseconds

//global - store the header names in CSV file
String dataLabel1 = "Distance_sensor(cm)";
String dataLabel2 = "Photoresistor(10-bit value)";
String dataLabel3 = "Photoresistor(lux)";
bool label = true;

void loop() {
  //print out column headers
  while(label){ //runs once
    Serial.print(dataLabel1);
    Serial.print(",");
    Serial.print(dataLabel2);
    Serial.print(",");
    Serial.println(dataLabel3);
    label=false; // print label once at the start 
  }

  // put your main code here, to run repeatedly:

  // distance sensor code
  long duration, inches, cm;
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration); // data

  // photoresistor code
  int analogValue = analogRead(photoresistorPin); // 0V-VCC -> 0-1023
  double luminosity = luminosityLevel(analogValue);

  // data from sensors
  distance = cm;
  brightness = analogValue;
  luxBrightness = luminosity;

  //distance = analogRead(sensor1);
  //brightness = analogRead(sensor2);

  //Display Data to Serial Monitor
  Serial.print(distance);
  Serial.print(",");
  Serial.print(brightness);
  Serial.print(",");
  Serial.println(luxBrightness);

  delay(freq);

}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

// convert to lux
double luminosityLevel(int analogValue) {
  // Convert analog value to voltage
  double Vout = double(analogValue) * ANALOG_OUTPUT_CONVERSION;
  
  // Convert voltage output to luminosity (unit lux)
  int luminosity = 500 /(RESISTOR / 1000 * ((VIN-Vout)/Vout));
   
  return luminosity;
}
