#include "DHT.h"

// Define the pin where the DHT11 data pin is connected
#define DHTPIN D2         // Connect DHT11 data pin to D4 (GPIO2)
#define DHTTYPE DHT11      // Define DHT sensor type (DHT11)

DHT dht(DHTPIN, DHTTYPE);  // Create a DHT object

void setup() {
  Serial.begin(115200);          // Start Serial Monitor
  Serial.println("DHT11 Sensor Test on NodeMCU (ESP-12E)");
  dht.begin();                   // Initialize DHT sensor
}

void loop() {
  delay(2000);                   // Wait 2 seconds between readings

  // Read humidity and temperature values
  float humidity = dht.readHumidity();
  float temperatureC = dht.readTemperature();      // Celsius
  float temperatureF = dht.readTemperature(true);  // Fahrenheit

  // Check if readings failed
  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("❌ Failed to read from DHT sensor!");
    return;
  }

  // Print the results to Serial Monitor
  Serial.print("💧 Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("🌡 Temperature: ");
  Serial.print(temperatureC);
  Serial.print(" °C  |  ");
  Serial.print(temperatureF);
  Serial.println(" °F");

  Serial.println("------------------------");
}
