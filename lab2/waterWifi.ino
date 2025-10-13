/*
  Water Level Reading using NodeMCU 1.0 (ESP-12E Module)
  Reads the raw analog value from a water level sensor connected to A0.
*/

const int sensorPin = A0;  // Water level sensor signal pin connected to A0

void setup() {
  // Initialize serial communication at 9600 bps
  Serial.begin(9600);
  Serial.println("Water Level Sensor Reading - NodeMCU (ESP-12E)");
}

void loop() {
  // Read the analog value from the sensor (range 0–1023)
  int sensorValue = analogRead(sensorPin);

  // Print the value to the Serial Monitor
  Serial.print("Raw Sensor Value: ");
  Serial.println(sensorValue);

  // Optional: convert the value to percentage (0–100%)
  float waterLevelPercent = (sensorValue / 1023.0) * 100.0;
  Serial.print("Water Level: ");
  Serial.print(waterLevelPercent, 1);
  Serial.println(" %");

  // Wait 500 milliseconds before the next reading
  delay(500);
}
