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
  roboEyes.setPupils(false);
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


void loop() {
  unsigned long currentMillis = millis();
  
  // Button handling
  static bool lastButtonState = HIGH;
  int buttonState = digitalRead(BUTTON);
  
  // Detect button press (with debounce)
  if (buttonState == LOW && lastButtonState == HIGH) {
    delay(50); // Debounce
    if (digitalRead(BUTTON) == LOW) {
      if (!animationActive) {
        Serial.println("\n🎬 Starting natural life cycle...\n");
        animationActive = true;
        animationStep = 0;
        previousMillis = currentMillis;
      } else {
        Serial.println("\n⏸️  Pausing...\n");
        animationActive = false;
        resetEyes();
      }
    }
  }
  lastButtonState = buttonState;
  
  // Always update display when active
  if (animationActive) {
    roboEyes.update();
  } else {
    display.clearDisplay();
    display.display();
  }
  
  // Natural life emotion sequence
  if (animationActive) {
    
    switch(animationStep) {
      
      // ========== 1. DEEP SLEEP ==========
      case 0:
        if (currentMillis - previousMillis >= 0) {
          Serial.println("💤 Deep sleep...");
          roboEyes.close();
          roboEyes.setMood(DEFAULT);
          roboEyes.setSleepy(ON);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 2. FIRST STIRRING ==========
      case 1:
        if (currentMillis - previousMillis >= 3000) {
          Serial.println("😴 Starting to wake up...");
          roboEyes.setSleepy(OFF);
          roboEyes.setMood(TIRED);
          roboEyes.setEyebrows(OFF);
          roboEyes.eyebrowsRaised();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 3. EYES FLUTTER OPEN ==========
      case 2:
        if (currentMillis - previousMillis >= 1500) {
          Serial.println("👁️  Eyes fluttering...");
          // Natural awakening - one eye at a time
          roboEyes.open(1, 0); // Left eye first
          delay(300);
          roboEyes.close(1, 0);
          delay(200);
          roboEyes.open(1, 0);
          delay(400);
          roboEyes.open(0, 1); // Then right eye
          roboEyes.setPupils(false);
          roboEyes.setPupilSize(6); // Small sleepy pupils
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 4. SLOW BLINK & ADJUST ==========
      case 3:
        if (currentMillis - previousMillis >= 2000) {
          Serial.println("😌 Adjusting to light...");
          roboEyes.setAutoblinker(ON, 5, 1); // Very slow sleepy blinks
          roboEyes.setPupilSize(8);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 5. BECOMING ALERT ==========
      case 4:
        if (currentMillis - previousMillis >= 3000) {
          Serial.println("😊 Becoming alert...");
          roboEyes.setMood(DEFAULT);
          roboEyes.setAutoblinker(ON, 3, 2); // Normal blinks
          roboEyes.setPupilSize(9);
          roboEyes.eyebrowsRaised();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 6. LOOK AROUND CURIOUS ==========
      case 5:
        if (currentMillis - previousMillis >= 2500) {
          Serial.println("🤔 Looking around...");
          roboEyes.setCuriosity(ON);
          roboEyes.setPupilPosition(-2, 0);
          delay(800);
          roboEyes.setPosition(W);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      case 6:
        if (currentMillis - previousMillis >= 1500) {
          roboEyes.setPupilPosition(2, 0);
          roboEyes.setPosition(E);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      case 7:
        if (currentMillis - previousMillis >= 1500) {
          roboEyes.setPupilPosition(0, -2);
          roboEyes.setPosition(N);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 7. NOTICE SOMETHING ==========
      case 8:
        if (currentMillis - previousMillis >= 1800) {
          Serial.println("👀 Oh! What's that?");
          roboEyes.setPosition(NE);
          roboEyes.setPupilPosition(3, -1);
          roboEyes.eyebrowsRaised();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 8. SURPRISE! ==========
      case 9:
        if (currentMillis - previousMillis >= 1200) {
          Serial.println("😲 SURPRISE!");
          roboEyes.setCuriosity(OFF);
          roboEyes.setPosition(DEFAULT);
          roboEyes.anim_surprise();
          roboEyes.setPupilSize(11); // Dilated from surprise
          roboEyes.setPupilPosition(0, 0);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 9. EXCITEMENT BUILDS ==========
      case 10:
        if (currentMillis - previousMillis >= 1500) {
          Serial.println("✨ Getting excited!");
          roboEyes.setPupilSize(9);
          roboEyes.setShimmer(ON);
          roboEyes.eyebrowsRaised();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 10. PURE JOY ==========
      case 11:
        if (currentMillis - previousMillis >= 2000) {
          Serial.println("😍 So happy!");
          roboEyes.setMood(HAPPY);
          roboEyes.anim_hearts();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 11. LAUGHTER ==========
      case 12:
        if (currentMillis - previousMillis >= 3500) {
          Serial.println("😂 Laughing!");
          roboEyes.anim_laugh();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 12. CATCHING BREATH ==========
      case 13:
        if (currentMillis - previousMillis >= 2000) {
          Serial.println("😅 Catching breath...");
          roboEyes.setShimmer(OFF);
          roboEyes.setMood(DEFAULT);
          roboEyes.blink();
          delay(400);
          roboEyes.blink();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 13. CONTEMPLATIVE ==========
      case 14:
        if (currentMillis - previousMillis >= 2500) {
          Serial.println("🤨 Hmm... thinking...");
          roboEyes.setPupilPosition(0, -2);
          roboEyes.setPosition(N);
          roboEyes.eyebrowsRaised();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 14. SKEPTICAL GLANCE ==========
      case 15:
        if (currentMillis - previousMillis >= 2500) {
          Serial.println("🤔 Skeptical...");
          roboEyes.setPosition(E);
          roboEyes.eyebrowsSkeptical();
          roboEyes.setPupilPosition(4, 0);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 15. CONFUSION STARTS ==========
      case 16:
        if (currentMillis - previousMillis >= 2200) {
          Serial.println("😕 Wait... what?");
          roboEyes.setPosition(DEFAULT);
          roboEyes.setPupilPosition(0, 0);
          roboEyes.setEyebrowExpression(2, -2);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 16. VERY CONFUSED ==========
      case 17:
        if (currentMillis - previousMillis >= 1500) {
          Serial.println("😵 So confused!");
          roboEyes.anim_confused();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 17. DIZZY ==========
      case 18:
        if (currentMillis - previousMillis >= 2000) {
          Serial.println("😵‍💫 Dizzy!");
          roboEyes.setDizzy(ON);
          roboEyes.anim_eyeRoll();
          roboEyes.setEyebrowExpression(0, 0);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 18. SHAKE IT OFF ==========
      case 19:
        if (currentMillis - previousMillis >= 4000) {
          Serial.println("🤯 Shaking head...");
          roboEyes.setDizzy(OFF);
          roboEyes.setHFlicker(ON, 5);
          delay(600);
          roboEyes.setHFlicker(OFF);
          roboEyes.blink();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 19. BUILDING FRUSTRATION ==========
      case 20:
        if (currentMillis - previousMillis >= 1800) {
          Serial.println("😠 Getting frustrated...");
          roboEyes.eyebrowsAngry();
          roboEyes.setPupilSize(7);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 20. ANGER RISING ==========
      case 21:
        if (currentMillis - previousMillis >= 2000) {
          Serial.println("😡 Getting angry!");
          roboEyes.setMood(ANGRY);
          roboEyes.setAngryVein(ON);
          roboEyes.setPupilSize(5); // Focused anger
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 21. RAGE ==========
      case 22:
        if (currentMillis - previousMillis >= 2500) {
          Serial.println("🤬 Very angry!");
          roboEyes.setCuriosity(ON);
          roboEyes.setPosition(W);
          delay(600);
          roboEyes.setPosition(E);
          delay(600);
          roboEyes.setPosition(W);
          delay(600);
          roboEyes.setPosition(DEFAULT);
          roboEyes.setHFlicker(ON, 3);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 22. CALMING DOWN ==========
      case 23:
        if (currentMillis - previousMillis >= 3000) {
          Serial.println("😤 Deep breath...");
          roboEyes.setHFlicker(OFF);
          roboEyes.setCuriosity(OFF);
          roboEyes.setAngryVein(OFF);
          roboEyes.blink();
          delay(600);
          roboEyes.blink();
          delay(600);
          roboEyes.blink();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 23. EXHAUSTED ==========
      case 24:
        if (currentMillis - previousMillis >= 2000) {
          Serial.println("😔 Feeling drained...");
          roboEyes.setMood(DEFAULT);
          roboEyes.eyebrowsSad();
          roboEyes.setPupilPosition(0, 2); // Looking down
          roboEyes.setPupilSize(8);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 24. SADNESS ==========
      case 25:
        if (currentMillis - previousMillis >= 2500) {
          Serial.println("😢 Feeling sad...");
          roboEyes.setTears(ON);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 25. CRYING ==========
      case 26:
        if (currentMillis - previousMillis >= 3500) {
          Serial.println("😭 Crying...");
          roboEyes.setVFlicker(ON, 4); // Sobbing motion
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 26. WIPING TEARS ==========
      case 27:
        if (currentMillis - previousMillis >= 3000) {
          Serial.println("🥺 Wiping tears...");
          roboEyes.setVFlicker(OFF);
          roboEyes.setTears(OFF);
          roboEyes.blink();
          delay(500);
          roboEyes.blink();
          delay(500);
          roboEyes.blink();
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 27. RECOVERY ==========
      case 28:
        if (currentMillis - previousMillis >= 2500) {
          Serial.println("😌 Feeling a bit better...");
          roboEyes.setEyebrowExpression(0, 0);
          roboEyes.setPupilPosition(0, 0);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 28. TIREDNESS SETS IN ==========
      case 29:
        if (currentMillis - previousMillis >= 2000) {
          Serial.println("😪 Getting tired...");
          roboEyes.setMood(TIRED);
          roboEyes.setAutoblinker(ON, 6, 1); // Slow tired blinks
          roboEyes.setPupilSize(6);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 29. VERY SLEEPY ==========
      case 30:
        if (currentMillis - previousMillis >= 3000) {
          Serial.println("😴 Can barely keep eyes open...");
          roboEyes.setSleepy(ON);
          roboEyes.setPupilSize(5);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 30. FIGHTING SLEEP ==========
      case 31:
        if (currentMillis - previousMillis >= 2500) {
          Serial.println("💤 Fighting sleep...");
          roboEyes.close(1, 0); // Left eye drooping
          delay(900);
          roboEyes.open(1, 0); // Try to stay awake
          delay(600);
          roboEyes.close(0, 1); // Right eye drooping
          delay(900);
          roboEyes.open(0, 1);
          delay(600);
          roboEyes.close(1, 1); // Both closing
          delay(1200);
          roboEyes.open(1, 1); // Jolt awake
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 31. CAN'T FIGHT IT ==========
      case 32:
        if (currentMillis - previousMillis >= 2000) {
          Serial.println("😴 Can't... stay... awake...");
          roboEyes.close(1, 0);
          delay(1000);
          roboEyes.close(0, 1);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 32. ASLEEP ==========
      case 33:
        if (currentMillis - previousMillis >= 3000) {
          Serial.println("💤 Asleep...");
          roboEyes.setMood(DEFAULT);
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== 33. DEEP SLEEP AGAIN ==========
      case 34:
        if (currentMillis - previousMillis >= 4000) {
          Serial.println("😴 Deep sleep...\n");
          previousMillis = currentMillis;
          animationStep++;
        }
        break;
        
      // ========== RESTART CYCLE ==========
      case 35:
        if (currentMillis - previousMillis >= 2000) {
          Serial.println("🔄 Life cycle complete! Restarting...\n");
          roboEyes.setSleepy(OFF);
          roboEyes.setAutoblinker(ON, 3, 2);
          animationStep = 0;
          previousMillis = currentMillis;
        }
        break;
    }
  }
}

// Helper function to reset all eyes features
void resetEyes() {
  roboEyes.setAutoblinker(OFF);
  roboEyes.setIdleMode(OFF);
  roboEyes.setHFlicker(OFF);
  roboEyes.setVFlicker(OFF);
  roboEyes.setMood(DEFAULT);
  roboEyes.setPosition(DEFAULT);
  roboEyes.setEyebrows(OFF);
  roboEyes.setPupils(false);
  roboEyes.setShimmer(OFF);
  roboEyes.setTears(OFF);
  roboEyes.setSleepy(OFF);
  roboEyes.setDizzy(OFF);
  roboEyes.setAngryVein(OFF);
  roboEyes.setCuriosity(OFF);
  roboEyes.setPupilPosition(0, 0);
  roboEyes.close();
}


/*
 * ============================================
 * 🤖 NATURAL ROBO LIFE CYCLE (36 Steps)
 * ============================================
 * 
 * 💤 AWAKENING (Steps 0-4)
 *   Deep sleep → Stirring → Eyes flutter → 
 *   Adjust to light → Becoming alert
 * 
 * 👁️ CURIOSITY & EXPLORATION (Steps 5-8)
 *   Looking around → Notice something → Surprise!
 * 
 * 😊 JOY & HAPPINESS (Steps 9-13)
 *   Excitement → Hearts → Laughter → Catching breath
 * 
 * 🤔 CONTEMPLATION (Steps 14-15)
 *   Thinking → Skeptical look
 * 
 * 😵 CONFUSION & DISORIENTATION (Steps 16-19)
 *   Confused → Very confused → Dizzy → Shake off
 * 
 * 😠 FRUSTRATION TO ANGER (Steps 20-23)
 *   Frustrated → Angry → Rage → Calming down
 * 
 * 😢 SADNESS & RECOVERY (Steps 24-28)
 *   Exhausted → Sad → Crying → Wiping tears → Recovery
 * 
 * 😴 TIREDNESS TO SLEEP (Steps 29-34)
 *   Getting tired → Very sleepy → Fighting sleep → 
 *   Can't stay awake → Asleep → Deep sleep
 * 
 * 🔄 Then the cycle repeats...
 * 
 * Total cycle: ~120 seconds (2 minutes)
 * Each emotion flows naturally into the next
 * Press button to start/pause anytime
 * ============================================
 */