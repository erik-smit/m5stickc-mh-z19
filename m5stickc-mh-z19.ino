#include <M5StickC.h>
#include "MHZ.h"

// pin for pwm reading
#define CO2_IN G36

// pin for uart reading
#define MH_Z19_RX G26
#define MH_Z19_TX G0

MHZ co2(Serial2, CO2_IN, MHZ19C);

void setup() {
  Serial.begin(115200);
  Serial.println("MHZ 19C");

  M5.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.fillScreen(BLACK);
  
  Serial2.begin(9600, SERIAL_8N1, 26, 0);
 
  pinMode(CO2_IN, INPUT);
  delay(100);

  // enable debug to get addition information
  co2.setDebug(true);

  if (co2.isPreHeating()) {
    M5.Lcd.print("Preheating");
    Serial.print("Preheating");
    
    while (co2.isPreHeating()) {
      Serial.print(".");
      M5.Lcd.print(".");
      delay(5000);
    }
    Serial.println();
  }

  M5.Lcd.fillScreen(BLACK);  
}

void loop() {
  M5.update(); // need to call update()
  M5.Lcd.setCursor(0, 2, 1);
    
  Serial.print("\n----- Time from start: ");
  Serial.print(millis() / 1000);
  Serial.println(" s");

  int ppm_uart = co2.readCO2UART();
  Serial.print("PPMuart: ");

  if (ppm_uart > 0) {
    Serial.print(ppm_uart);
  } else {
    Serial.print("n/a");
  }

  M5.Lcd.printf("CO2: %.4d ppm\r\n", ppm_uart);
  
//  int ppm_pwm = co2.readCO2PWM();
//  Serial.print(", PPMpwm: ");
//  Serial.print(ppm_pwm);
//  M5.Lcd.printf("PPMpwm : %.4d ppm\r\n", ppm_pwm);

  int temperature = co2.getLastTemperature();
  Serial.print(", Temperature: ");

  if (temperature > 0) {
    Serial.println(temperature);
  } else {
    Serial.println("n/a");
  }

  Serial.println("\n------------------------------");
  delay(5000);
}
