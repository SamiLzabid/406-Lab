/****************************************************
 * ESP32 UART Sender (Multi-Baud Parameter Grid)
 * DOIT ESP32 DEVKIT V1
 * 
 * Tests:
 *   Baud rates: 9600, 38400, 115200
 *   Message sizes: 10, 50, 100 bytes
 *   Intervals: 0 ms, 10 ms, 100 ms
 * 
 * TX -> GPIO17, RX -> GPIO16
 ****************************************************/

#define TXD1 19
#define RXD1 21

HardwareSerial mySerial(1);

// Test parameters
int baudRates[] = {9600, 38400, 115200};
int msgSizes[]  = {10, 50, 100};
int intervals[] = {0, 10, 100};

char padChar = 'A';
int testDuration = 10; // seconds per test window

void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ESP32 UART Sender (Multi-Baud Grid) ===");
  delay(1000);
}

void loop() {
  for (int b = 0; b < 3; b++) {
    int baud = baudRates[b];
    mySerial.begin(baud, SERIAL_8N1, RXD1, TXD1);
    Serial.print("\n--- Testing at Baud: ");
    Serial.println(baud);

    for (int s = 0; s < 3; s++) {
      int msgSize = msgSizes[s];
      String message = createMessage(msgSize);

      for (int i = 0; i < 3; i++) {
        int interval = intervals[i];

        Serial.print("Sending ");
        Serial.print(msgSize);
        Serial.print(" bytes every ");
        Serial.print(interval);
        Serial.print(" ms @ ");
        Serial.println(baud);

        unsigned long startTime = millis();
        unsigned long msgCount = 0, totalBytes = 0;

        while ((millis() - startTime) < testDuration * 1000UL) {
          mySerial.println(message);
          msgCount++;
          totalBytes += msgSize;
          delay(interval);
        }

        // End of one parameter test
        mySerial.println("TEST_END");

        float throughput = totalBytes / (float)testDuration;
        float msgRate = msgCount / (float)testDuration;

        Serial.print("Result -> Baud: "); Serial.print(baud);
        Serial.print(" | Size: "); Serial.print(msgSize);
        Serial.print(" | Interval: "); Serial.print(interval);
        Serial.print(" | Throughput: "); Serial.print(throughput, 2);
        Serial.print(" B/s | Msg/s: "); Serial.println(msgRate, 2);
        Serial.println("------------------------------------");
        delay(2000);
      }
    }
  }

  Serial.println("\n✅ All parameter combinations complete.");
  while (true); // stop repeating
}

String createMessage(int size) {
  String msg = "";
  for (int i = 0; i < size; i++) msg += padChar;
  return msg;
}
