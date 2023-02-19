const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
const int photoresistorPin = A0; // Photoresistor Pin 
const int buzzPin = 5;

// globals for lux conversion
const double ANALOG_OUTPUT_CONVERSION = 0.0048828125;
const double RESISTOR = 10000;
const int VIN = 5;

const int AVG_COUNT = 100;
unsigned long timeLastBrightnessBuzz;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // Starting Serial Terminal
  pinMode(photoresistorPin, INPUT);// Set pResistor - A0 pin as an input (optional)
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  digitalWrite(buzzPin, LOW);
  timeLastBrightnessBuzz = millis();

  //print out column headers
  Serial.print("Distance_sensor(cm)");
  Serial.print(",");
  Serial.print("Photoresistor(10-bit value)");
  Serial.print(",");
  Serial.println("Photoresistor(lux)");
}

//globals
int distance, brightness; //store data from both sensors
double luxBrightness; // store lux value 
int freq = 1000; //data collection frequency ~x milliseconds

double luxBrightnessArr[AVG_COUNT] = {0};
int luxBrightnessIdx = 0;
int luxBrightnessCount = 0;
double luxBrightnessAvg;

void loop() {
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

  // Check values and buzz
  updateBrightnessAvg();
  buzzBrightness();
  // buzzDistance();

  //Display Data to Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(", Brightness: ");
  Serial.print(brightness);
  Serial.print(", lux Brightness: ");
  Serial.print(luxBrightness);
  Serial.println("");
  Serial.print("luxBrightnessCount: ");
  Serial.print(luxBrightnessCount);
  Serial.print(", luxBrightnessIdx: ");
  Serial.print(luxBrightnessIdx);
  Serial.print(", lux Average: ");
  Serial.println(luxBrightnessAvg);

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

void updateBrightnessAvg() {
  if (luxBrightnessCount < AVG_COUNT) {
    luxBrightnessCount++;
  }

  if (luxBrightnessIdx == AVG_COUNT) {
    luxBrightnessIdx = 0;
  } else {
    luxBrightnessIdx++;
  }

  luxBrightnessArr[luxBrightnessIdx] = luxBrightness;
  double sum = 0;
  for (int i = 0; i < luxBrightnessCount; i++ ) {
    sum += luxBrightnessArr[i];
  }
  luxBrightnessAvg = sum / luxBrightnessCount;
}

void buzz() {
  digitalWrite(buzzPin, HIGH);
  delay(10);
  digitalWrite(buzzPin, LOW);
}

void buzzBrightness() {
  if (luxBrightnessAvg > 400 && (timeLastBrightnessBuzz - millis() > 10000)) { // Too bright for too long, buzz
    buzz();
    delay(100);
    buzz();
  }
  timeLastBrightnessBuzz = millis();
}

// void buzzDistance() {

// }