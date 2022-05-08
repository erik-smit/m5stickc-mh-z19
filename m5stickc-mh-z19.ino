#include <M5StickC.h>
#include "MHZ.h"

#define DEBUG

#ifdef DEBUG
#define D_SerialBegin(...) Serial.begin(__VA_ARGS__)
#define D_print(...)       Serial.print(__VA_ARGS__)
#define D_write(...)       Serial.print(__VA_ARGS__)
#define D_println(...)     Serial.println(__VA_ARGS__)
#define D_printf(...)      Serial.printf(__VA_ARGS__)
#else
#define D_SerialBegin(...)
#define D_print(...)
#define D_write(...)
#define D_println(...)
#define D_printf(...)
#endif

// pin for pwm reading
#define CO2_IN G36

// pin for uart reading
#define MH_Z19_RX G26
#define MH_Z19_TX G0

MHZ co2(Serial2, CO2_IN, MHZ19C);

void setup() {
  D_SerialBegin(115200);
  D_println("MHZ 19C");

  M5.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.fillScreen(BLACK);
  
  Serial2.begin(9600, SERIAL_8N1, 26, 0);
 
  pinMode(CO2_IN, INPUT);
  delay(100);

  // enable debug to get addition information
#ifdef DEBUG
  co2.setDebug(true);
#endif

  if (co2.isPreHeating()) {
    M5.Lcd.print("Preheating");
    D_print("Preheating");
    
    while (co2.isPreHeating()) {
      D_print(".");
      M5.Lcd.print(".");
      delay(5000);
    }
    D_println();
  }

  M5.Lcd.fillScreen(BLACK);  
}

void loop() {
  int ppm_uart = 0;
  int temperature = 0;
  
  M5.update(); // need to call update()
  M5.Lcd.setCursor(0, 2, 1);
    
  D_printf("----- Time from start: %ds\n", millis() / 1000);

  ppm_uart = co2.readCO2UART();
#ifdef DEBUG
  temperature = co2.getLastTemperature();
#endif
  
  M5.Lcd.printf("CO2: %.4d ppm\r\n", ppm_uart);
  D_printf("PPMuart: %d, Temp: %d \n", ppm_uart, temperature);
  D_println("------------------------------");

  delay(10000);
}
