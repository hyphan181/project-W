/*
 * FluxGarage RoboEyes for OLED Displays V 1.2.1
 * Draws smoothly animated robot eyes on OLED displays, based on the Adafruit GFX 
 * library's graphics primitives, such as rounded rectangles and triangles.
 *   
 * Copyright (C) 2024-2025 Dennis Hoelscher
 * www.fluxgarage.com
 * www.youtube.com/@FluxGarage
 *
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */


#ifndef _FLUXGARAGE_ROBOEYES_H
#define _FLUXGARAGE_ROBOEYES_H


// Display colors
uint8_t BGCOLOR = 0; // background and overlays
uint8_t MAINCOLOR = 1; // drawings

// For mood type switch
#define DEFAULT 0
#define TIRED 1
#define ANGRY 2
#define HAPPY 3

// For turning things on or off
#define ON 1
#define OFF 0

// For switch "predefined positions"
#define N 1 // north, top center
#define NE 2 // north-east, top right
#define E 3 // east, middle right
#define SE 4 // south-east, bottom right
#define S 5 // south, bottom center
#define SW 6 // south-west, bottom left
#define W 7 // west, middle left
#define NW 8 // north-west, top left 
// for middle center set "DEFAULT"


// Constructor: takes a reference to the active Adafruit display object (e.g., Adafruit_SSD1327)
// Eg: roboEyes<Adafruit_SSD1327> = eyes(display);
template<typename AdafruitDisplay>
class RoboEyes
{
private:

// Yes, everything is currently still accessible. Be responsible and don't mess things up :)

public:

// Reference to Adafruit display object
AdafruitDisplay *display;

// For general setup - screen size and max. frame rate
int screenWidth = 128; // OLED display width, in pixels
int screenHeight = 64; // OLED display height, in pixels
int frameInterval = 20; // default value for 50 frames per second (1000/50 = 20 milliseconds)
unsigned long fpsTimer = 0; // for timing the frames per second

// For controlling mood types and expressions
bool tired = 0;
bool angry = 0;
bool happy = 0;
bool curious = 0; // if true, draw the outer eye larger when looking left or right
bool cyclops = 0; // if true, draw only one eye
bool eyeL_open = 0; // left eye opened or closed?
bool eyeR_open = 0; // right eye opened or closed?


//*********************************************************************************************
//  Eyes Geometry
//*********************************************************************************************

// EYE LEFT - size and border radius
int eyeLwidthDefault = 36;
int eyeLheightDefault = 36;
int eyeLwidthCurrent = eyeLwidthDefault;
int eyeLheightCurrent = 1; // start with closed eye, otherwise set to eyeLheightDefault
int eyeLwidthNext = eyeLwidthDefault;
int eyeLheightNext = eyeLheightDefault;
int eyeLheightOffset = 0;
// Border Radius
byte eyeLborderRadiusDefault = 18;
byte eyeLborderRadiusCurrent = eyeLborderRadiusDefault;
byte eyeLborderRadiusNext = eyeLborderRadiusDefault;

// EYE RIGHT - size and border radius
int eyeRwidthDefault = eyeLwidthDefault;
int eyeRheightDefault = eyeLheightDefault;
int eyeRwidthCurrent = eyeRwidthDefault;
int eyeRheightCurrent = 1; // start with closed eye, otherwise set to eyeRheightDefault
int eyeRwidthNext = eyeRwidthDefault;
int eyeRheightNext = eyeRheightDefault;
int eyeRheightOffset = 0;
// Border Radius
byte eyeRborderRadiusDefault = 18;
byte eyeRborderRadiusCurrent = eyeRborderRadiusDefault;
byte eyeRborderRadiusNext = eyeRborderRadiusDefault;

// EYE LEFT - Coordinates
int eyeLxDefault = ((screenWidth)-(eyeLwidthDefault+spaceBetweenDefault+eyeRwidthDefault))/2;
int eyeLyDefault = ((screenHeight-eyeLheightDefault)/2);
int eyeLx = eyeLxDefault;
int eyeLy = eyeLyDefault;
int eyeLxNext = eyeLx;
int eyeLyNext = eyeLy;

// EYE RIGHT - Coordinates
int eyeRxDefault = eyeLx+eyeLwidthCurrent+spaceBetweenDefault;
int eyeRyDefault = eyeLy;
int eyeRx = eyeRxDefault;
int eyeRy = eyeRyDefault;
int eyeRxNext = eyeRx;
int eyeRyNext = eyeRy;

// BOTH EYES 
// Eyelid top size
byte eyelidsHeightMax = eyeLheightDefault/2; // top eyelids max height
byte eyelidsTiredHeight = 0;
byte eyelidsTiredHeightNext = eyelidsTiredHeight;
byte eyelidsAngryHeight = 0;
byte eyelidsAngryHeightNext = eyelidsAngryHeight;
// Bottom happy eyelids offset
byte eyelidsHappyBottomOffsetMax = (eyeLheightDefault/2)+3;
byte eyelidsHappyBottomOffset = 0;
byte eyelidsHappyBottomOffsetNext = 0;
// Space between eyes
int spaceBetweenDefault = 10;
int spaceBetweenCurrent = spaceBetweenDefault;
int spaceBetweenNext = 10;


//*********************************************************************************************
//  Macro Animations
//*********************************************************************************************

// Animation - horizontal flicker/shiver
bool hFlicker = 0;
bool hFlickerAlternate = 0;
byte hFlickerAmplitude = 2;

// Animation - vertical flicker/shiver
bool vFlicker = 0;
bool vFlickerAlternate = 0;
byte vFlickerAmplitude = 10;

// Animation - auto blinking
bool autoblinker = 0; // activate auto blink animation
int blinkInterval = 1; // basic interval between each blink in full seconds
int blinkIntervalVariation = 4; // interval variaton range in full seconds, random number inside of given range will be add to the basic blinkInterval, set to 0 for no variation
unsigned long blinktimer = 0; // for organising eyeblink timing

// Animation - idle mode: eyes looking in random directions
bool idle = 0;
int idleInterval = 1; // basic interval between each eye repositioning in full seconds
int idleIntervalVariation = 3; // interval variaton range in full seconds, random number inside of given range will be add to the basic idleInterval, set to 0 for no variation
unsigned long idleAnimationTimer = 0; // for organising eyeblink timing

// Animation - eyes confused: eyes shaking left and right
bool confused = 0;
unsigned long confusedAnimationTimer = 0;
int confusedAnimationDuration = 500;
bool confusedToggle = 1;

// Animation - eyes laughing: eyes shaking up and down
bool laugh = 0;
unsigned long laughAnimationTimer = 0;
int laughAnimationDuration = 500;
bool laughToggle = 1;

// Animation - sweat on the forehead
bool sweat = 0;
byte sweatBorderradius = 3;

// Sweat drop 1
int sweat1XPosInitial = 2;
int sweat1XPos;
float sweat1YPos = 2;
int sweat1YPosMax;
float sweat1Height = 2;
float sweat1Width = 1;

// Sweat drop 2
int sweat2XPosInitial = 2;
int sweat2XPos;
float sweat2YPos = 2;
int sweat2YPosMax;
float sweat2Height = 2;
float sweat2Width = 1;

// Sweat drop 3
int sweat3XPosInitial = 2;
int sweat3XPos;
float sweat3YPos = 2;
int sweat3YPosMax;
float sweat3Height = 2;
float sweat3Width = 1;
//*********************************************************************************************
//  EXTENDED FEATURES - NEW ANIMATIONS
//*********************************************************************************************

// Animation - eyebrows above eyes
bool eyebrows = 0;
int eyebrowLx = 0;
int eyebrowLy = 0;
int eyebrowRx = 0;
int eyebrowRy = 0;
int eyebrowWidth = 32;
int eyebrowHeight = 4;
int eyebrowOffset = 8;
int eyebrowLangle = 0;
int eyebrowRangle = 0;
int eyebrowLangleNext = 0;
int eyebrowRangleNext = 0;

// Animation - tears falling from eyes
bool tears = 0;
byte tearBorderRadius = 3;
float tear1XPos = 0;
float tear1YPos = 0;
int tear1YPosMax = 0;
float tear1Height = 4;
float tear1Width = 2;
bool tear1Active = 0;
float tear2XPos = 0;
float tear2YPos = 0;
int tear2YPosMax = 0;
float tear2Height = 4;
float tear2Width = 2;
bool tear2Active = 0;

// Animation - hearts floating up
bool hearts = 0;
unsigned long heartsTimer = 0;
int heartsDuration = 4000;
float heart1X = 0;
float heart1Y = 0;
byte heart1Size = 6;
float heart2X = 0;
float heart2Y = 0;
byte heart2Size = 5;
float heart3X = 0;
float heart3Y = 0;
byte heart3Size = 7;

// Animation - ZZZ symbols
bool sleepy = 0;
float zzz1X = 0;
float zzz1Y = 0;
byte zzz1Size = 4;
float zzz2X = 0;
float zzz2Y = 0;
byte zzz2Size = 6;
float zzz3X = 0;
float zzz3Y = 0;
byte zzz3Size = 8;

// Animation - pupils inside eyes
bool pupils = 0;
int pupilLx = 0;
int pupilLy = 0;
int pupilRx = 0;
int pupilRy = 0;
int pupilSize = 10;
int pupilOffsetX = 0;
int pupilOffsetY = 0;
int pupilOffsetXNext = 0;
int pupilOffsetYNext = 0;

// Animation - eye shimmer
bool shimmer = 0;
bool shimmerToggle = 0;
unsigned long shimmerTimer = 0;
int shimmerInterval = 400;

// Animation - surprise
bool surprise = 0;
unsigned long surpriseTimer = 0;
int surpriseDuration = 600;
bool surpriseToggle = 1;
int surpriseScale = 0;

// Animation - dizzy stars
bool dizzy = 0;
float dizzyAngle = 0;
unsigned long dizzyTimer = 0;
int dizzyDuration = 4000;

// Animation - angry vein
bool angryVein = 0;
bool angryVeinPulse = 0;
unsigned long angryVeinTimer = 0;

// Animation - eye roll
bool eyeRolling = 0;
unsigned long eyeRollTimer = 0;
int eyeRollDuration = 1500;
float eyeRollAngle = 0;
bool eyeRollToggle = 1;

//*********************************************************************************************
//  GENERAL METHODS
//*********************************************************************************************

RoboEyes(AdafruitDisplay &disp) : display(&disp) {};

// Startup RoboEyes with defined screen-width, screen-height and max. frames per second
void begin(int width, int height, byte frameRate) {
	screenWidth = width; // OLED display width, in pixels
	screenHeight = height; // OLED display height, in pixels
  display->clearDisplay(); // clear the display buffer
  display->display(); // show empty screen
  eyeLheightCurrent = 1; // start with closed eyes
  eyeRheightCurrent = 1; // start with closed eyes
  setFramerate(frameRate); // calculate frame interval based on defined frameRate
}

void update(){
  // Limit drawing updates to defined max framerate
  if(millis()-fpsTimer >= frameInterval){
    drawEyes();
    fpsTimer = millis();
  }
}


//*********************************************************************************************
//  SETTERS METHODS
//*********************************************************************************************

// Calculate frame interval based on defined frameRate
void setFramerate(byte fps){
  frameInterval = 1000/fps;
}

// Set color values
void setDisplayColors(uint8_t background, uint8_t main) {
  BGCOLOR = background; // background and overlays, choose 0 for monochrome displays and 0x00 for grayscale displays such as SSD1322
  MAINCOLOR = main; // drawings, choose 1 for monochrome displays and 0x0F for grayscale displays such as SSD1322 (0x0F = maximum brightness)
}

void setWidth(byte leftEye, byte rightEye) {
	eyeLwidthNext = leftEye;
	eyeRwidthNext = rightEye;
  eyeLwidthDefault = leftEye;
  eyeRwidthDefault = rightEye;
}

void setHeight(byte leftEye, byte rightEye) {
	eyeLheightNext = leftEye;
	eyeRheightNext = rightEye;
  eyeLheightDefault = leftEye;
  eyeRheightDefault = rightEye;
}

// Set border radius for left and right eye
void setBorderradius(byte leftEye, byte rightEye) {
	eyeLborderRadiusNext = leftEye;
	eyeRborderRadiusNext = rightEye;
  eyeLborderRadiusDefault = leftEye;
  eyeRborderRadiusDefault = rightEye;
}

// Set space between the eyes, can also be negative
void setSpacebetween(int space) {
  spaceBetweenNext = space;
  spaceBetweenDefault = space;
}

// Set mood expression
void setMood(unsigned char mood)
  {
    switch (mood)
    {
    case TIRED:
      tired=1; 
      angry=0; 
      happy=0;
      break;
    case ANGRY:
      tired=0; 
      angry=1; 
      happy=0;
      break;
    case HAPPY:
      tired=0; 
      angry=0; 
      happy=1;
      break;
    default:
      tired=0; 
      angry=0; 
      happy=0;
      break;
    }
  }

// Set predefined position
void setPosition(unsigned char position)
  {
    switch (position)
    {
    case N:
      // North, top center
      eyeLxNext = getScreenConstraint_X()/2;
      eyeLyNext = 0;
      break;
    case NE:
      // North-east, top right
      eyeLxNext = getScreenConstraint_X();
      eyeLyNext = 0;
      break;
    case E:
      // East, middle right
      eyeLxNext = getScreenConstraint_X();
      eyeLyNext = getScreenConstraint_Y()/2;
      break;
    case SE:
      // South-east, bottom right
      eyeLxNext = getScreenConstraint_X();
      eyeLyNext = getScreenConstraint_Y();
      break;
    case S:
      // South, bottom center
      eyeLxNext = getScreenConstraint_X()/2;
      eyeLyNext = getScreenConstraint_Y();
      break;
    case SW:
      // South-west, bottom left
      eyeLxNext = 0;
      eyeLyNext = getScreenConstraint_Y();
      break;
    case W:
      // West, middle left
      eyeLxNext = 0;
      eyeLyNext = getScreenConstraint_Y()/2;
      break;
    case NW:
      // North-west, top left
      eyeLxNext = 0;
      eyeLyNext = 0;
      break;
    default:
      // Middle center
      eyeLxNext = getScreenConstraint_X()/2;
      eyeLyNext = getScreenConstraint_Y()/2;
      break;
    }
  }

// Set automated eye blinking, minimal blink interval in full seconds and blink interval variation range in full seconds
void setAutoblinker(bool active, int interval, int variation){
  autoblinker = active;
  blinkInterval = interval;
  blinkIntervalVariation = variation;
}
void setAutoblinker(bool active){
  autoblinker = active;
}

// Set idle mode - automated eye repositioning, minimal time interval in full seconds and time interval variation range in full seconds
void setIdleMode(bool active, int interval, int variation){
  idle = active;
  idleInterval = interval;
  idleIntervalVariation = variation;
}
void setIdleMode(bool active) {
  idle = active;
}

// Set curious mode - the respectively outer eye gets larger when looking left or right
void setCuriosity(bool curiousBit) {
  curious = curiousBit;
}

// Set cyclops mode - show only one eye 
void setCyclops(bool cyclopsBit) {
  cyclops = cyclopsBit;
}

// Set horizontal flickering (displacing eyes left/right)
void setHFlicker (bool flickerBit, byte Amplitude) {
  hFlicker = flickerBit; // turn flicker on or off
  hFlickerAmplitude = Amplitude; // define amplitude of flickering in pixels
}
void setHFlicker (bool flickerBit) {
  hFlicker = flickerBit; // turn flicker on or off
}

// Set vertical flickering (displacing eyes up/down)
void setVFlicker (bool flickerBit, byte Amplitude) {
  vFlicker = flickerBit; // turn flicker on or off
  vFlickerAmplitude = Amplitude; // define amplitude of flickering in pixels
}
void setVFlicker (bool flickerBit) {
  vFlicker = flickerBit; // turn flicker on or off
}

void setSweat (bool sweatBit) {
  sweat = sweatBit; // turn sweat on or off
}
void setEyebrows(bool eyebrowBit) {
  eyebrows = eyebrowBit;
}

void setEyebrowExpression(int leftAngle, int rightAngle) {
  eyebrowLangleNext = constrain(leftAngle, -10, 10);
  eyebrowRangleNext = constrain(rightAngle, -10, 10);
}

void eyebrowsRaised() {
  eyebrowLangleNext = 5;
  eyebrowRangleNext = 5;
}

void eyebrowsAngry() {
  eyebrowLangleNext = -5;
  eyebrowRangleNext = -5;
}

void eyebrowsSkeptical() {
  eyebrowLangleNext = 0;
  eyebrowRangleNext = 5;
}

void eyebrowsSad() {
  eyebrowLangleNext = 3;
  eyebrowRangleNext = 3;
}

void setTears(bool tearBit) {
  tears = tearBit;
  if(tears){
    tear1Active = 0;
    tear2Active = 0;
  }
}

void setPupils(bool pupilBit) {
  pupils = pupilBit;
}

void setPupilPosition(int offsetX, int offsetY) {
  pupilOffsetXNext = constrain(offsetX, -5, 5);
  pupilOffsetYNext = constrain(offsetY, -5, 5);
}

void setPupilSize(int size) {
  pupilSize = constrain(size, 4, 12);
}

void setShimmer(bool shimmerBit) {
  shimmer = shimmerBit;
}

void setSleepy(bool sleepyBit) {
  sleepy = sleepyBit;
}

void setDizzy(bool dizzyBit) {
  dizzy = dizzyBit;
  if(dizzy){
    dizzyTimer = millis();
  }
}

void setAngryVein(bool veinBit) {
  angryVein = veinBit;
}

//*********************************************************************************************
//  GETTERS METHODS
//*********************************************************************************************

// Returns the max x position for left eye
int getScreenConstraint_X(){
  return screenWidth-eyeLwidthCurrent-spaceBetweenCurrent-eyeRwidthCurrent;
} 

// Returns the max y position for left eye
int getScreenConstraint_Y(){
 return screenHeight-eyeLheightDefault; // using default height here, because height will vary when blinking and in curious mode
}


//*********************************************************************************************
//  BASIC ANIMATION METHODS
//*********************************************************************************************

// BLINKING FOR BOTH EYES AT ONCE
// Close both eyes
void close() {
	eyeLheightNext = 1; // closing left eye
  eyeRheightNext = 1; // closing right eye
  eyeL_open = 0; // left eye not opened (=closed)
	eyeR_open = 0; // right eye not opened (=closed)
}

// Open both eyes
void open() {
  eyeL_open = 1; // left eye opened - if true, drawEyes() will take care of opening eyes again
	eyeR_open = 1; // right eye opened
}

// Trigger eyeblink animation
void blink() {
  close();
  open();
}

// BLINKING FOR SINGLE EYES, CONTROL EACH EYE SEPARATELY
// Close eye(s)
void close(bool left, bool right) {
  if(left){
    eyeLheightNext = 1; // blinking left eye
    eyeL_open = 0; // left eye not opened (=closed)
  }
  if(right){
      eyeRheightNext = 1; // blinking right eye
      eyeR_open = 0; // right eye not opened (=closed)
  }
}

// Open eye(s)
void open(bool left, bool right) {
  if(left){
    eyeL_open = 1; // left eye opened - if true, drawEyes() will take care of opening eyes again
  }
  if(right){
    eyeR_open = 1; // right eye opened
  }
}

// Trigger eyeblink(s) animation
void blink(bool left, bool right) {
  close(left, right);
  open(left, right);
}


//*********************************************************************************************
//  MACRO ANIMATION METHODS
//*********************************************************************************************

// Play confused animation - one shot animation of eyes shaking left and right
void anim_confused() {
	confused = 1;
}

// Play laugh animation - one shot animation of eyes shaking up and down
void anim_laugh() {
  laugh = 1;
}
void anim_surprise() {
  surprise = 1;
}

void anim_hearts() {
  hearts = 1;
  heartsTimer = millis();
  heart1X = eyeLx + eyeLwidthDefault/2;
  heart1Y = screenHeight;
  heart2X = (eyeLx + eyeRx + eyeRwidthDefault)/2;
  heart2Y = screenHeight;
  heart3X = eyeRx + eyeRwidthDefault/2;
  heart3Y = screenHeight;
}

void anim_eyeRoll() {
  eyeRolling = 1;
}
//*********************************************************************************************
//  PRE-CALCULATIONS AND ACTUAL DRAWINGS
//*********************************************************************************************

void drawEyes(){

  //// PRE-CALCULATIONS - EYE SIZES AND VALUES FOR ANIMATION TWEENINGS ////

  // Vertical size offset for larger eyes when looking left or right (curious gaze)
  if(curious){
    if(eyeLxNext<=10){eyeLheightOffset=8;}
    else if (eyeLxNext>=(getScreenConstraint_X()-10) && cyclops){eyeLheightOffset=8;}
    else{eyeLheightOffset=0;} // left eye
    if(eyeRxNext>=screenWidth-eyeRwidthCurrent-10){eyeRheightOffset=8;}
    else{eyeRheightOffset=0;} // right eye
  } else {
    eyeLheightOffset=0; // reset height offset for left eye
    eyeRheightOffset=0; // reset height offset for right eye
  }

  // Left eye height
  eyeLheightCurrent = (eyeLheightCurrent + eyeLheightNext + eyeLheightOffset)/2;
  eyeLy+= ((eyeLheightDefault-eyeLheightCurrent)/2); // vertical centering of eye when closing
  eyeLy-= eyeLheightOffset/2;
  // Right eye height
  eyeRheightCurrent = (eyeRheightCurrent + eyeRheightNext + eyeRheightOffset)/2;
  eyeRy+= (eyeRheightDefault-eyeRheightCurrent)/2; // vertical centering of eye when closing
  eyeRy-= eyeRheightOffset/2;


  // Open eyes again after closing them
	if(eyeL_open){
  	if(eyeLheightCurrent <= 1 + eyeLheightOffset){eyeLheightNext = eyeLheightDefault;} 
  }
  if(eyeR_open){
  	if(eyeRheightCurrent <= 1 + eyeRheightOffset){eyeRheightNext = eyeRheightDefault;} 
  }

  // Left eye width
  eyeLwidthCurrent = (eyeLwidthCurrent + eyeLwidthNext)/2;
  // Right eye width
  eyeRwidthCurrent = (eyeRwidthCurrent + eyeRwidthNext)/2;


  // Space between eyes
  spaceBetweenCurrent = (spaceBetweenCurrent + spaceBetweenNext)/2;

  // Left eye coordinates
  eyeLx = (eyeLx + eyeLxNext)/2;
  eyeLy = (eyeLy + eyeLyNext)/2;
  // Right eye coordinates
  eyeRxNext = eyeLxNext+eyeLwidthCurrent+spaceBetweenCurrent; // right eye's x position depends on left eyes position + the space between
  eyeRyNext = eyeLyNext; // right eye's y position should be the same as for the left eye
  eyeRx = (eyeRx + eyeRxNext)/2;
  eyeRy = (eyeRy + eyeRyNext)/2;

  // Left eye border radius
  eyeLborderRadiusCurrent = (eyeLborderRadiusCurrent + eyeLborderRadiusNext)/2;
  // Right eye border radius
  eyeRborderRadiusCurrent = (eyeRborderRadiusCurrent + eyeRborderRadiusNext)/2;
  

  //// APPLYING MACRO ANIMATIONS ////

	if(autoblinker){
		if(millis() >= blinktimer){
		blink();
		blinktimer = millis()+(blinkInterval*1000)+(random(blinkIntervalVariation)*1000); // calculate next time for blinking
		}
	}

  // Laughing - eyes shaking up and down for the duration defined by laughAnimationDuration (default = 500ms)
  if(laugh){
    if(laughToggle){
      setVFlicker(1, 5);
      laughAnimationTimer = millis();
      laughToggle = 0;
    } else if(millis() >= laughAnimationTimer+laughAnimationDuration){
      setVFlicker(0, 0);
      laughToggle = 1;
      laugh=0; 
    }
  }

  // Confused - eyes shaking left and right for the duration defined by confusedAnimationDuration (default = 500ms)
  if(confused){
    if(confusedToggle){
      setHFlicker(1, 20);
      confusedAnimationTimer = millis();
      confusedToggle = 0;
    } else if(millis() >= confusedAnimationTimer+confusedAnimationDuration){
      setHFlicker(0, 0);
      confusedToggle = 1;
      confused=0; 
    }
  }

  // Idle - eyes moving to random positions on screen
  if(idle){
    if(millis() >= idleAnimationTimer){
      eyeLxNext = random(getScreenConstraint_X());
      eyeLyNext = random(getScreenConstraint_Y());
      idleAnimationTimer = millis()+(idleInterval*1000)+(random(idleIntervalVariation)*1000); // calculate next time for eyes repositioning
    }
  }

  // Adding offsets for horizontal flickering/shivering
  if(hFlicker){
    if(hFlickerAlternate) {
      eyeLx += hFlickerAmplitude;
      eyeRx += hFlickerAmplitude;
    } else {
      eyeLx -= hFlickerAmplitude;
      eyeRx -= hFlickerAmplitude;
    }
    hFlickerAlternate = !hFlickerAlternate;
  }

  // Adding offsets for horizontal flickering/shivering
  if(vFlicker){
    if(vFlickerAlternate) {
      eyeLy += vFlickerAmplitude;
      eyeRy += vFlickerAmplitude;
    } else {
      eyeLy -= vFlickerAmplitude;
      eyeRy -= vFlickerAmplitude;
    }
    vFlickerAlternate = !vFlickerAlternate;
  }

  // Cyclops mode, set second eye's size and space between to 0
  if(cyclops){
    eyeRwidthCurrent = 0;
    eyeRheightCurrent = 0;
    spaceBetweenCurrent = 0;
  }

  //// ACTUAL DRAWINGS ////

  display->clearDisplay(); // start with a blank screen

  // Draw basic eye rectangles
  display->fillRoundRect(eyeLx, eyeLy, eyeLwidthCurrent, eyeLheightCurrent, eyeLborderRadiusCurrent, MAINCOLOR); // left eye
  if (!cyclops){
    display->fillRoundRect(eyeRx, eyeRy, eyeRwidthCurrent, eyeRheightCurrent, eyeRborderRadiusCurrent, MAINCOLOR); // right eye
  }

  // Prepare mood type transitions
  if (tired){eyelidsTiredHeightNext = eyeLheightCurrent/2; eyelidsAngryHeightNext = 0;} else{eyelidsTiredHeightNext = 0;}
  if (angry){eyelidsAngryHeightNext = eyeLheightCurrent/2; eyelidsTiredHeightNext = 0;} else{eyelidsAngryHeightNext = 0;}
  if (happy){eyelidsHappyBottomOffsetNext = eyeLheightCurrent/2;} else{eyelidsHappyBottomOffsetNext = 0;}

  // Draw tired top eyelids 
    eyelidsTiredHeight = (eyelidsTiredHeight + eyelidsTiredHeightNext)/2;
    if (!cyclops){
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx, eyeLy+eyelidsTiredHeight-1, BGCOLOR); // left eye 
      display->fillTriangle(eyeRx, eyeRy-1, eyeRx+eyeRwidthCurrent, eyeRy-1, eyeRx+eyeRwidthCurrent, eyeRy+eyelidsTiredHeight-1, BGCOLOR); // right eye
    } else {
      // Cyclops tired eyelids
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx, eyeLy+eyelidsTiredHeight-1, BGCOLOR); // left eyelid half
      display->fillTriangle(eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy+eyelidsTiredHeight-1, BGCOLOR); // right eyelid half
    }

  // Draw angry top eyelids 
    eyelidsAngryHeight = (eyelidsAngryHeight + eyelidsAngryHeightNext)/2;
    if (!cyclops){ 
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy+eyelidsAngryHeight-1, BGCOLOR); // left eye
      display->fillTriangle(eyeRx, eyeRy-1, eyeRx+eyeRwidthCurrent, eyeRy-1, eyeRx, eyeRy+eyelidsAngryHeight-1, BGCOLOR); // right eye
    } else {
      // Cyclops angry eyelids
      display->fillTriangle(eyeLx, eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy+eyelidsAngryHeight-1, BGCOLOR); // left eyelid half
      display->fillTriangle(eyeLx+(eyeLwidthCurrent/2), eyeLy-1, eyeLx+eyeLwidthCurrent, eyeLy-1, eyeLx+(eyeLwidthCurrent/2), eyeLy+eyelidsAngryHeight-1, BGCOLOR); // right eyelid half
    }

  // Draw happy bottom eyelids
    eyelidsHappyBottomOffset = (eyelidsHappyBottomOffset + eyelidsHappyBottomOffsetNext)/2;
    display->fillRoundRect(eyeLx-1, (eyeLy+eyeLheightCurrent)-eyelidsHappyBottomOffset+1, eyeLwidthCurrent+2, eyeLheightDefault, eyeLborderRadiusCurrent, BGCOLOR); // left eye
    if (!cyclops){ 
      display->fillRoundRect(eyeRx-1, (eyeRy+eyeRheightCurrent)-eyelidsHappyBottomOffset+1, eyeRwidthCurrent+2, eyeRheightDefault, eyeRborderRadiusCurrent, BGCOLOR); // right eye
    }

  // Add sweat drops
    if (sweat){
      // Sweat drop 1 -> left corner
      if(sweat1YPos <= sweat1YPosMax){sweat1YPos+=0.5;} // vertical movement from initial to max
      else {sweat1XPosInitial = random(30); sweat1YPos = 2; sweat1YPosMax = (random(10)+10); sweat1Width = 1; sweat1Height = 2;} // if max vertical position is reached: reset all values for next drop
      if(sweat1YPos <= sweat1YPosMax/2){sweat1Width+=0.5; sweat1Height+=0.5;} // shape grows in first half of animation ...
      else {sweat1Width-=0.1; sweat1Height-=0.5;} // ... and shrinks in second half of animation
      sweat1XPos = sweat1XPosInitial-(sweat1Width/2); // keep the growing shape centered to initial x position
      display->fillRoundRect(sweat1XPos, sweat1YPos, sweat1Width, sweat1Height, sweatBorderradius, MAINCOLOR); // draw sweat drop


      // Sweat drop 2 -> center area
      if(sweat2YPos <= sweat2YPosMax){sweat2YPos+=0.5;} // vertical movement from initial to max
      else {sweat2XPosInitial = random((screenWidth-60))+30; sweat2YPos = 2; sweat2YPosMax = (random(10)+10); sweat2Width = 1; sweat2Height = 2;} // if max vertical position is reached: reset all values for next drop
      if(sweat2YPos <= sweat2YPosMax/2){sweat2Width+=0.5; sweat2Height+=0.5;} // shape grows in first half of animation ...
      else {sweat2Width-=0.1; sweat2Height-=0.5;} // ... and shrinks in second half of animation
      sweat2XPos = sweat2XPosInitial-(sweat2Width/2); // keep the growing shape centered to initial x position
      display->fillRoundRect(sweat2XPos, sweat2YPos, sweat2Width, sweat2Height, sweatBorderradius, MAINCOLOR); // draw sweat drop


      // Sweat drop 3 -> right corner
      if(sweat3YPos <= sweat3YPosMax){sweat3YPos+=0.5;} // vertical movement from initial to max
      else {sweat3XPosInitial = (screenWidth-30)+(random(30)); sweat3YPos = 2; sweat3YPosMax = (random(10)+10); sweat3Width = 1; sweat3Height = 2;} // if max vertical position is reached: reset all values for next drop
      if(sweat3YPos <= sweat3YPosMax/2){sweat3Width+=0.5; sweat3Height+=0.5;} // shape grows in first half of animation ...
      else {sweat3Width-=0.1; sweat3Height-=0.5;} // ... and shrinks in second half of animation
      sweat3XPos = sweat3XPosInitial-(sweat3Width/2); // keep the growing shape centered to initial x position
      display->fillRoundRect(sweat3XPos, sweat3YPos, sweat3Width, sweat3Height, sweatBorderradius, MAINCOLOR); // draw sweat drop
    }
// === NEW FEATURES DRAWING CODE - Add before display->display() ===

  // Surprise animation
  if(surprise){
    if(surpriseToggle){
      surpriseScale = 8;
      eyeLwidthNext += surpriseScale;
      eyeLheightNext += surpriseScale;
      eyeRwidthNext += surpriseScale;
      eyeRheightNext += surpriseScale;
      surpriseTimer = millis();
      surpriseToggle = 0;
    } else if(millis() >= surpriseTimer + surpriseDuration){
      eyeLwidthNext -= surpriseScale;
      eyeLheightNext -= surpriseScale;
      eyeRwidthNext -= surpriseScale;
      eyeRheightNext -= surpriseScale;
      surpriseScale = 0;
      surpriseToggle = 1;
      surprise = 0;
    }
  }

  // Eye roll animation
  if(eyeRolling){
    if(eyeRollToggle){
      eyeRollAngle = 0;
      eyeRollTimer = millis();
      eyeRollToggle = 0;
    } else if(millis() <= eyeRollTimer + eyeRollDuration){
      eyeRollAngle += 0.3;
      pupilOffsetXNext = sin(eyeRollAngle) * 5;
      pupilOffsetYNext = cos(eyeRollAngle) * 5;
    } else {
      pupilOffsetXNext = 0;
      pupilOffsetYNext = 0;
      eyeRollToggle = 1;
      eyeRolling = 0;
    }
  }

  // Draw pupils
  if(pupils){
    pupilOffsetX = (pupilOffsetX + pupilOffsetXNext)/2;
    pupilOffsetY = (pupilOffsetY + pupilOffsetYNext)/2;
    pupilLx = eyeLx + (eyeLwidthCurrent/2) + pupilOffsetX;
    pupilLy = eyeLy + (eyeLheightCurrent/2) + pupilOffsetY;
    pupilRx = eyeRx + (eyeRwidthCurrent/2) + pupilOffsetX;
    pupilRy = eyeRy + (eyeRheightCurrent/2) + pupilOffsetY;
    display->fillCircle(pupilLx, pupilLy, pupilSize/2, BGCOLOR);
    if(!cyclops){
      display->fillCircle(pupilRx, pupilRy, pupilSize/2, BGCOLOR);
    }
  }

  // Draw eyebrows
  if(eyebrows){
    eyebrowLangle = (eyebrowLangle + eyebrowLangleNext)/2;
    eyebrowRangle = (eyebrowRangle + eyebrowRangleNext)/2;
    eyebrowLx = eyeLx + (eyeLwidthCurrent - eyebrowWidth)/2;
    eyebrowLy = eyeLy - eyebrowOffset;
    eyebrowRx = eyeRx + (eyeRwidthCurrent - eyebrowWidth)/2;
    eyebrowRy = eyeRy - eyebrowOffset;
    
    for(int i = 0; i < eyebrowHeight; i++){
      int angleOffset = map(i, 0, eyebrowHeight, 0, eyebrowLangle);
      display->drawLine(eyebrowLx, eyebrowLy + i + abs(eyebrowLangle) - angleOffset, 
                       eyebrowLx + eyebrowWidth, eyebrowLy + i - angleOffset, MAINCOLOR);
    }
    
    if(!cyclops){
      for(int i = 0; i < eyebrowHeight; i++){
        int angleOffset = map(i, 0, eyebrowHeight, 0, eyebrowRangle);
        display->drawLine(eyebrowRx, eyebrowRy + i - angleOffset, 
                         eyebrowRx + eyebrowWidth, eyebrowRy + i + abs(eyebrowRangle) - angleOffset, MAINCOLOR);
      }
    }
  }

  // Draw tears
  if(tears){
    if(!tear1Active && random(100) < 5){
      tear1XPos = eyeLx + (eyeLwidthCurrent/2);
      tear1YPos = eyeLy + eyeLheightCurrent;
      tear1YPosMax = tear1YPos + random(15, 30);
      tear1Active = 1;
      tear1Width = 2;
      tear1Height = 3;
    }
    if(tear1Active){
      if(tear1YPos < tear1YPosMax){
        tear1YPos += 0.5;
        if(tear1YPos < tear1YPosMax/2){
          tear1Width += 0.1;
          tear1Height += 0.2;
        } else {
          tear1Width -= 0.05;
          tear1Height -= 0.1;
        }
        display->fillRoundRect(tear1XPos, tear1YPos, tear1Width, tear1Height, tearBorderRadius, MAINCOLOR);
      } else {
        tear1Active = 0;
      }
    }
    
    if(!cyclops){
      if(!tear2Active && random(100) < 5){
        tear2XPos = eyeRx + (eyeRwidthCurrent/2);
        tear2YPos = eyeRy + eyeRheightCurrent;
        tear2YPosMax = tear2YPos + random(15, 30);
        tear2Active = 1;
        tear2Width = 2;
        tear2Height = 3;
      }
      if(tear2Active){
        if(tear2YPos < tear2YPosMax){
          tear2YPos += 0.5;
          if(tear2YPos < tear2YPosMax/2){
            tear2Width += 0.1;
            tear2Height += 0.2;
          } else {
            tear2Width -= 0.05;
            tear2Height -= 0.1;
          }
          display->fillRoundRect(tear2XPos, tear2YPos, tear2Width, tear2Height, tearBorderRadius, MAINCOLOR);
        } else {
          tear2Active = 0;
        }
      }
    }
  }

  // Draw floating hearts
  if(hearts){
    if(millis() - heartsTimer < heartsDuration){
      heart1Y -= 0.5;
      heart2Y -= 0.4;
      heart3Y -= 0.6;
      
      if(heart1Y > 0){
        display->fillCircle(heart1X - heart1Size/2, heart1Y, heart1Size/2, MAINCOLOR);
        display->fillCircle(heart1X + heart1Size/2, heart1Y, heart1Size/2, MAINCOLOR);
        display->fillTriangle(heart1X - heart1Size, heart1Y, 
                             heart1X + heart1Size, heart1Y,
                             heart1X, heart1Y + heart1Size, MAINCOLOR);
      }
      
      if(heart2Y > 0){
        display->fillCircle(heart2X - heart2Size/2, heart2Y, heart2Size/2, MAINCOLOR);
        display->fillCircle(heart2X + heart2Size/2, heart2Y, heart2Size/2, MAINCOLOR);
        display->fillTriangle(heart2X - heart2Size, heart2Y, 
                             heart2X + heart2Size, heart2Y,
                             heart2X, heart2Y + heart2Size, MAINCOLOR);
      }
      
      if(heart3Y > 0){
        display->fillCircle(heart3X - heart3Size/2, heart3Y, heart3Size/2, MAINCOLOR);
        display->fillCircle(heart3X + heart3Size/2, heart3Y, heart3Size/2, MAINCOLOR);
        display->fillTriangle(heart3X - heart3Size, heart3Y, 
                             heart3X + heart3Size, heart3Y,
                             heart3X, heart3Y + heart3Size, MAINCOLOR);
      }
    } else {
      hearts = 0;
    }
  }

  // Draw sleepy ZZZ
  if(sleepy){
    zzz1Y -= 0.3;
    zzz2Y -= 0.25;
    zzz3Y -= 0.2;
    
    if(zzz1Y < -10){
      zzz1X = eyeRx + eyeRwidthCurrent + 5;
      zzz1Y = eyeRy;
    }
    if(zzz2Y < -10){
      zzz2X = eyeRx + eyeRwidthCurrent + 12;
      zzz2Y = eyeRy + 8;
    }
    if(zzz3Y < -10){
      zzz3X = eyeRx + eyeRwidthCurrent + 18;
      zzz3Y = eyeRy + 15;
    }
    
    if(zzz1X == 0){
      zzz1X = eyeRx + eyeRwidthCurrent + 5;
      zzz1Y = eyeRy;
      zzz2X = eyeRx + eyeRwidthCurrent + 12;
      zzz2Y = eyeRy + 8;
      zzz3X = eyeRx + eyeRwidthCurrent + 18;
      zzz3Y = eyeRy + 15;
    }
    
    display->drawLine(zzz1X, zzz1Y, zzz1X + zzz1Size, zzz1Y, MAINCOLOR);
    display->drawLine(zzz1X + zzz1Size, zzz1Y, zzz1X, zzz1Y + zzz1Size, MAINCOLOR);
    display->drawLine(zzz1X, zzz1Y + zzz1Size, zzz1X + zzz1Size, zzz1Y + zzz1Size, MAINCOLOR);
    
    display->drawLine(zzz2X, zzz2Y, zzz2X + zzz2Size, zzz2Y, MAINCOLOR);
    display->drawLine(zzz2X + zzz2Size, zzz2Y, zzz2X, zzz2Y + zzz2Size, MAINCOLOR);
    display->drawLine(zzz2X, zzz2Y + zzz2Size, zzz2X + zzz2Size, zzz2Y + zzz2Size, MAINCOLOR);
    
    display->drawLine(zzz3X, zzz3Y, zzz3X + zzz3Size, zzz3Y, MAINCOLOR);
    display->drawLine(zzz3X + zzz3Size, zzz3Y, zzz3X, zzz3Y + zzz3Size, MAINCOLOR);
    display->drawLine(zzz3X, zzz3Y + zzz3Size, zzz3X + zzz3Size, zzz3Y + zzz3Size, MAINCOLOR);
  }

  // Draw shimmer
  if(shimmer){
    if(millis() - shimmerTimer > shimmerInterval){
      shimmerToggle = !shimmerToggle;
      shimmerTimer = millis();
    }
    if(shimmerToggle){
      display->drawPixel(eyeLx + 3, eyeLy + 3, MAINCOLOR);
      display->drawPixel(eyeLx + 4, eyeLy + 3, MAINCOLOR);
      display->drawPixel(eyeLx + 3, eyeLy + 4, MAINCOLOR);
      if(!cyclops){
        display->drawPixel(eyeRx + eyeRwidthCurrent - 4, eyeRy + 3, MAINCOLOR);
        display->drawPixel(eyeRx + eyeRwidthCurrent - 5, eyeRy + 3, MAINCOLOR);
        display->drawPixel(eyeRx + eyeRwidthCurrent - 4, eyeRy + 4, MAINCOLOR);
      }
    }
  }

  // Draw dizzy stars
  if(dizzy){
    if(millis() - dizzyTimer < dizzyDuration){
      dizzyAngle += 0.1;
      for(int i = 0; i < 4; i++){
        float angle = dizzyAngle + (i * PI/2);
        int starX = screenWidth/2 + cos(angle) * 35;
        int starY = screenHeight/2 + sin(angle) * 25;
        display->drawPixel(starX, starY, MAINCOLOR);
        display->drawPixel(starX+1, starY, MAINCOLOR);
        display->drawPixel(starX-1, starY, MAINCOLOR);
        display->drawPixel(starX, starY+1, MAINCOLOR);
        display->drawPixel(starX, starY-1, MAINCOLOR);
      }
    } else {
      dizzy = 0;
    }
  }

  // Draw angry vein
  if(angryVein){
    if(millis() - angryVeinTimer > 200){
      angryVeinPulse = !angryVeinPulse;
      angryVeinTimer = millis();
    }
    int veinX = screenWidth/2;
    int veinY = 8;
    if(angryVeinPulse){
      display->drawLine(veinX-3, veinY, veinX-1, veinY-3, MAINCOLOR);
      display->drawLine(veinX-1, veinY-3, veinX+1, veinY, MAINCOLOR);
      display->drawLine(veinX+1, veinY, veinX+3, veinY-3, MAINCOLOR);
    }
  }
  display->display();
   // show drawings on display

} // end of drawEyes method


}; // end of class roboEyes


#endif
