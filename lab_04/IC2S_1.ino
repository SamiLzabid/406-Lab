#include <Wire.h>

#define I2C_DEV_ADDR 0x55  // must match master

volatile uint8_t last_checksum = 0;
volatile uint32_t rx_count = 0;
volatile uint32_t valid_count = 0;
volatile uint32_t err_count = 0;

// XOR checksum helper
static inline uint8_t checksum(const uint8_t *p, uint8_t n) {
  uint8_t s = 0;
  for (uint8_t i = 0; i < n; i++) s ^= p[i];
  return s;
}

// Called when master sends data
void onReceive(int len) {
  if (len < 4) return;
  uint8_t buf[128];
  uint8_t n = 0;

  while (Wire.available() && n < sizeof(buf)) buf[n++] = Wire.read();
  if (n < 4) return;

  uint16_t seq = ((uint16_t)buf[0] << 8) | buf[1];
  uint8_t size = buf[2];

  if (size + 4 > n) {
    err_count++;
    Serial.printf("Seq=%u truncated (%uB)\n", seq, n);
    return;
  }

  uint8_t chk_recv = buf[3 + size];
  uint8_t chk_calc = checksum(&buf[3], size);

  last_checksum = chk_recv;
  rx_count++;

  if (chk_recv == chk_calc) {
    valid_count++;
  } else {
    err_count++;
    Serial.printf("ERR seq=%u chk_recv=%02X chk_calc=%02X\n", seq, chk_recv, chk_calc);
  }
}

// Called when master requests data (ACK)
void onRequest() {
  Wire.write(last_checksum);
}

void setup() {
  Serial.begin(115200);
  Wire.begin((uint8_t)I2C_DEV_ADDR);  // SDA=21, SCL=22 default
  Wire.onReceive(onReceive);
  Wire.onRequest(onRequest);
  Serial.println("I2C Slave ready for stress test");
}

void loop() {
  static uint32_t t0 = 0;
  if (millis() - t0 > 2000) {
    t0 = millis();
    Serial.printf("Stats: rx=%lu valid=%lu err=%lu\n",
                  (unsigned long)rx_count,
                  (unsigned long)valid_count,
                  (unsigned long)err_count);
  }
}
