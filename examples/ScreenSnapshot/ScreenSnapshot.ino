#include <Arduino.h>
#include <Sentry.h>
#include <Wire.h>

typedef Sentry2 Sentry;

// #define SENTRY_I2C
#define SENTRY_UART
Sentry sentry;

#define SNAPSHOT_TO_SD_CARD false
#define SNAPSHOT_TO_UART false
#define SNAPSHOT_TO_USB true
#define SNAPSHOT_TO_WIFI false
#define SNAPSHOT_FROM_SCREEN false

int serial_putc(char c, struct __file*) {
  Serial.write(c);
  return c;
}

void setup() {
  sentry_err_t err = SENTRY_OK;

  Serial.begin(9600);
  fdevopen(&serial_putc, 0);

  printf("Waiting for sentry initialize...\n");
#ifdef SENTRY_I2C
  Wire.begin();
  while (SENTRY_OK != sentry.begin(&Wire)) {
    yield();
  }
#endif  // SENTRY_I2C
#ifdef SENTRY_UART
  Serial3.begin(9600);
  while (SENTRY_OK != sentry.begin(&Serial3)) {
    yield();
  }
#endif  // SENTRY_UART
  printf("Sentry begin Success.\n");
  printf("Sentry image_shape = %hux%hu\n", sentry.cols(), sentry.rows());
  err = sentry.Snapshot(SNAPSHOT_TO_SD_CARD, SNAPSHOT_TO_UART, SNAPSHOT_TO_USB,
                        SNAPSHOT_TO_WIFI, SNAPSHOT_FROM_SCREEN);
  printf("Snapshot %s!\n", err ? "Error" : "Success");
}

void loop() { yield(); }
