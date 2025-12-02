#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>

Adafruit_SSD1306 display(128, 64, &Wire, -1);
RoboEyes<Adafruit_SSD1306> eyes(display);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  eyes.begin(128, 64, 30);
  
  // Test new features
  eyes.setEyebrows(ON);
  eyes.setPupils(ON);
}

void loop() {
  eyes.eyebrowsRaised();
  eyes.anim_hearts();
  eyes.update();
}