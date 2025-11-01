

#define RXD1 21
#define TXD1 19

HardwareSerial mySerial(1);

unsigned long totalBytes = 0;
unsigned long msgCount = 0;
unsigned long startTime = 0;
bool testRunning = false;

int testDuration = 10;  // seconds, same as sender
int currentSize = 0;
int currentInterval = 0;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RXD1, TXD1);

  Serial.println("\n=== ESP32 UART Receiver ===");
  Serial.println("Baud | Size | Interval | Throughput (B/s) | Msg/s | Error (%)");
  Serial.println("--------------------------------------------------------------");
}

void loop() {
  if (mySerial.available()) {
    String received = mySerial.readStringUntil('\n');
    received.trim();

    if (received == "TEST_END") {
      computeMetrics();
      resetCounters();
      return;
    }

    if (received.length() > 0) {
      if (!testRunning) {
        startTime = millis();
        testRunning = true;
      }
      msgCount++;
      totalBytes += received.length();
    }
  }

  if (testRunning && (millis() - startTime >= testDuration * 1000)) {
    computeMetrics();
    resetCounters();
  }
}

void computeMetrics() {
  float T = (millis() - startTime) / 1000.0;
  float throughput = totalBytes / T;
  float msgRate = msgCount / T;
  float errorRate = 0; // Only measurable if sender feedback known

  Serial.print("9600 | ");
  Serial.print(receivedMsgSize());
  Serial.print(" | ");
  Serial.print(receivedMsgInterval());
  Serial.print(" | ");
  Serial.print(throughput, 2);
  Serial.print(" | ");
  Serial.print(msgRate, 2);
  Serial.print(" | ");
  Serial.println(errorRate, 2);
}

void resetCounters() {
  totalBytes = 0;
  msgCount = 0;
  startTime = 0;
  testRunning = false;
}

int receivedMsgSize() {
  // Approximate last message size seen
  return totalBytes > 0 && msgCount > 0 ? totalBytes / msgCount : 0;
}

int receivedMsgInterval() {
  // Dummy placeholder, as interval isn't transmitted
  return 0;
}