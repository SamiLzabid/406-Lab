/*********
  Rui Santos & Sara Santos - Random Nerd Tutorials
  Complete instructions at https://RandomNerdTutorials.com/esp32-uart-communication-serial-arduino/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*********/

// Define TX and RX pins for UART (change if needed)
#define RXD1 21  // UART receive pin
#define TXD1 19  // UART transmit pin (not used for receiving)

HardwareSerial mySerial(1);

// Match sender parameters
int baudRates[] = {9600, 38400, 115200};
int testDuration = 10; // seconds per test
unsigned long totalBytes = 0, msgCount = 0;
unsigned long startTime = 0;
bool testRunning = false;

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ESP32 UART Receiver (Multi-Baud Grid) ===");
  Serial.println("Baud | Size | Interval | Throughput (B/s) | Msg/s | Error (%)");
  Serial.println("--------------------------------------------------------------");
}

void loop() {
  for (int b = 0; b < 3; b++) {
    int baud = baudRates[b];
    mySerial.begin(baud, SERIAL_8N1, RXD1, TXD1);
    Serial.print("\n--- Listening at Baud: ");
    Serial.println(baud);

    totalBytes = 0;
    msgCount = 0;
    testRunning = false;
    startTime = millis();

    unsigned long lastActivity = millis();
    while (millis() - lastActivity < (testDuration + 2) * 1000UL) { // small buffer
      if (mySerial.available()) {
        String received = mySerial.readStringUntil('\n');
        received.trim();

        if (received == "TEST_END") {
          computeMetrics(baud);
          totalBytes = msgCount = 0;
          testRunning = false;
          delay(1000);
          lastActivity = millis();
          continue;
        }

        if (received.length() > 0) {
          msgCount++;
          totalBytes += received.length();
          lastActivity = millis();
          testRunning = true;
        }
      }
    }
  }

  Serial.println("\n✅ All baud tests complete.");
  while (true);
}

void computeMetrics(int baud) {
  float T = testDuration;
  float throughput = totalBytes / T;
  float msgRate = msgCount / T;
  float errorRate = 0.0; // Placeholder unless sender feedback known
  int sizeEstimate = msgCount > 0 ? totalBytes / msgCount : 0;

  Serial.print(baud);
  Serial.print(" | ");
  Serial.print(sizeEstimate);
  Serial.print(" | ");
  Serial.print("N/A");  // interval unknown here
  Serial.print(" | ");
  Serial.print(throughput, 2);
  Serial.print(" | ");
  Serial.print(msgRate, 2);
  Serial.print(" | ");
  Serial.println(errorRate, 2);
}
