/****************************************************
 * ESP32 UART Sender with Metrics (Parameter Grid)
 * DOIT ESP32 DEVKIT V1
 *
 * Runs combinations of:
 *   Message size: 10, 50, 100 bytes
 *   Interval: 0 ms, 10 ms, 100 ms
 *
 * Calculates:
 *   - Throughput (B/s)
 *   - Message rate (msg/s)
 *   - Error rate (simulated)
 ****************************************************/

#define TXD1 19
#define RXD1 21

HardwareSerial mySerial(1);

// Configurable parameters
int msgSizes[] = {10, 50, 100};
int intervals[] = {0, 10, 100};
int testDuration = 10;   // seconds for each test window

// ASCII payload
char padChar = 'A';

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);
  
  Serial.println("=== ESP32 UART Sender with Metrics ===");
}

void loop() {
  // Loop through parameter grid
  for (int s = 0; s < 3; s++) {
    for (int i = 0; i < 3; i++) {
      
      int msgSize = msgSizes[s];
      int interval = intervals[i];
      String message = createMessage(msgSize);
      
      // Reset counters
      unsigned long startTime = millis();
      unsigned long totalBytesSent = 0;
      unsigned long msgCount = 0;

      Serial.println("----------------------------------");
      Serial.print("Test: ");
      Serial.print(msgSize);
      Serial.print(" bytes | Interval: ");
      Serial.print(interval);
      Serial.println(" ms");
      Serial.print("Running for ");
      Serial.print(testDuration);
      Serial.println(" seconds...");

      // Run test window
      while ((millis() - startTime) < (testDuration * 1000)) {
        mySerial.println(message);
        totalBytesSent += msgSize;
        msgCount++;
        delay(interval);
      }

      // Simulated "valid received" = assume some loss (for demo)
      // In real test, receiver sends actual counts back
      int validReceived = msgCount - random(0, 3); // simulate small error
      if (validReceived < 0) validReceived = 0;

      // Metrics
      float T = testDuration;
      float throughput = totalBytesSent / T;
      float msgRate = msgCount / T;
      float errorRate = 100.0 * (msgCount - validReceived) / msgCount;

      // Print results
      Serial.println("\n--- Test Results ---");
      Serial.print("Messages Sent: "); Serial.println(msgCount);
      Serial.print("Bytes Sent: "); Serial.println(totalBytesSent);
      Serial.print("Throughput (B/s): "); Serial.println(throughput, 2);
      Serial.print("Message Rate (msg/s): "); Serial.println(msgRate, 2);
      Serial.print("Error Rate (%): "); Serial.println(errorRate, 2);
      Serial.println("----------------------\n");

      delay(3000);
    }
  }

  Serial.println("✅ All parameter combinations complete.");
  delay(10000);
}

// Create fixed-length ASCII message
String createMessage(int size) {
  String msg = "";
  for (int i = 0; i < size; i++) msg += padChar;
  return msg;
}
