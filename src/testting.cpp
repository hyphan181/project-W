#if 0 // disabled - duplicate test file kept for reference
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// For I2C connection (ESP32 default: SDA=21, SCL=22)
Adafruit_SH1106 display(21, 22);  // SDA, SCL pins for ESP32

#include "FluxGarage_RoboEyes_Extended.h"
RoboEyes<Adafruit_SH1106> roboEyes(display); // create RoboEyes instance

const int BUTTON = 4;
bool animationActive = false;
unsigned long previousMillis = 0;
int animationStep = 0;

// Forward declaration for helper defined later
void resetEyes();

void setup() {
  Serial.begin(9600);
  pinMode(BUTTON, INPUT_PULLUP);
  
  delay(250); // wait for the OLED to power up
  display.begin(SH1106_SWITCHCAPVCC, i2c_Address); // Initialize display
  display.clearDisplay();
  
  // Startup robo eyes with smooth framerate
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 60); // 60fps for ultra-smooth animations

  // Default settings - eyes closed initially
  roboEyes.setAutoblinker(OFF, 3, 2);
  roboEyes.setIdleMode(OFF, 2, 2);
  
  // Natural eye proportions - perfectly round
  roboEyes.setWidth(32, 32);
  roboEyes.setHeight(32, 32);
  roboEyes.setBorderradius(16, 16); // Perfect circles
  roboEyes.setSpacebetween(8);

  // Initialize features
  roboEyes.setMood(DEFAULT);
  roboEyes.setPosition(DEFAULT);
  roboEyes.setCuriosity(OFF);
  roboEyes.setEyebrows(OFF);
  roboEyes.setPupils(OFF);
  roboEyes.setTears(OFF);
  roboEyes.setShimmer(OFF);
  roboEyes.setSleepy(OFF);
  roboEyes.setDizzy(OFF);
  roboEyes.setAngryVein(OFF);
  roboEyes.setHFlicker(OFF, 2);
  roboEyes.setVFlicker(OFF, 2);
  
  display.clearDisplay();
  display.display();
  
  Serial.println("🤖 RoboEyes Natural Life - Press button to begin");
} // end of setup


void loop() {}
#endif // end disabled