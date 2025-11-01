/****************************************************
 * ESP32 UART Sender for Metrics Table Output
 * DOIT ESP32 DEVKIT V1
 *
 * Tests combinations of:
 *   - Baud: 9600
 *   - Message sizes: 10, 50, 100 bytes
 *   - Intervals: 0, 10, 100 ms
 * 
 * Outputs in table-friendly format
 ****************************************************/

#define TXD1 19
#define RXD1 21

HardwareSerial mySerial(1);

// Configurations
int msgSizes[] = {10, 50, 100};
int intervals[] = {0, 10, 100};
int testDuration = 10; // seconds per test
char padChar = 'A';

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);

  Serial.println("\n=== ESP32 UART Sender ===");
  Serial.println("Baud | Size | Interval | Throughput (B/s) | Msg/s | Error (%)");
  Serial.println("--------------------------------------------------------------");
}

void loop() {
  for (int s = 0; s < 3; s++) {
    for (int i = 0; i < 3; i++) {

      int msgSize = msgSizes[s];
      int interval = intervals[i];
      String msg = createMessage(msgSize);

      unsigned long startTime = millis();
      unsigned long sentBytes = 0;
      unsigned long sentCount = 0;

      while (millis() - startTime < (testDuration * 1000)) {
        mySerial.println(msg);
        sentBytes += msgSize;
        sentCount++;
        delay(interval);
      }

      // Notify test end (so receiver computes)
      mySerial.println("TEST_END");
      delay(1000);

      // Print summary locally
      Serial.print("9600 | ");
      Serial.print(msgSize);
      Serial.print(" | ");
      Serial.print(interval);
      Serial.print(" | ");
      Serial.print(sentBytes / (float)testDuration, 2);
      Serial.print(" | ");
      Serial.print(sentCount / (float)testDuration, 2);
      Serial.print(" | ");
      Serial.println("—");

      delay(2000);
    }
  }

  Serial.println("✅ All tests done.");
  while (true);
}

// Helper to make padded message
String createMessage(int size) {
  String msg = "";
  for (int i = 0; i < size; i++) msg += padChar;
  return msg;
}
