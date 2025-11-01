#define RXD1 21
#define TXD1 19

HardwareSerial mySerial(1);

// Duration of test window (same as sender)
int testDuration = 10; // seconds

// Counters
unsigned long totalBytes = 0;
unsigned long messageCount = 0;
unsigned long startTime = 0;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);

  Serial.println("=== ESP32 UART Receiver with Metrics ===");
  Serial.println("Waiting for incoming messages...");
  delay(2000);

  // Start timer
  startTime = millis();
}

void loop() {
  // Receive incoming data
  if (mySerial.available()) {
    String received = mySerial.readStringUntil('\n'); // read one line
    received.trim();

    if (received.length() > 0) {
      messageCount++;
      totalBytes += received.length();
      Serial.print("Received [");
      Serial.print(messageCount);
      Serial.print("] (");
      Serial.print(received.length());
      Serial.println(" bytes)");
    }
  }

  // Check if test window is over
  unsigned long elapsed = millis() - startTime;
  if (elapsed >= (testDuration * 1000)) {
    float T = elapsed / 1000.0;

    float throughput = totalBytes / T;         // Bytes per second
    float msgRate = messageCount / T;          // Messages per second
    float errorRate = 0.0;                     // Needs sender info to calculate

    Serial.println("\n--- Test Window Complete ---");
    Serial.print("Duration (s): "); Serial.println(T, 2);
    Serial.print("Messages Received: "); Serial.println(messageCount);
    Serial.print("Total Bytes: "); Serial.println(totalBytes);
    Serial.print("Throughput (B/s): "); Serial.println(throughput, 2);
    Serial.print("Message Rate (msg/s): "); Serial.println(msgRate, 2);
    Serial.println("Error Rate (%): Requires sender sent count");
    Serial.println("-----------------------------\n");

    // Reset for next test window
    totalBytes = 0;
    messageCount = 0;
    startTime = millis();
  }
}
