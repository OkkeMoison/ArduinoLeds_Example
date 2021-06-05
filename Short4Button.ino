// 30 LED strip with 4 buttons

#include <FastLED.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "Keypad.h"

//LED strip setup, change according to type
#define NUM_LEDS    30  // number leds on strip 
#define LED_TYPE    WS2811 //led type
#define COLOR_ORDER GRB //rgb order

//Arduino settings and variables - do not change
#define LED_PIN     7   // output pin arduino
CRGB leds[NUM_LEDS]; //array of leds, CRGB = object that represents a color in RGB color space.
unsigned long previousMS = 0;
int currentMode = 0;  
const long INTERVAL_MS_RANDOMIZER = 60000; //interval for randomizer check
int randomCounter;
int randomnr;
unsigned long previousMSRandomizer;

//Keypad settings
const byte Rows= 1; //number of rows on the keypad i.e. 4
const byte Cols= 4; //number of columns on the keypad i,e, 3

//Keypad mapping
char keymap[Rows][Cols]=
{
  {'2', '3', '0', '1'}
};

//Keypad connection to arduino
byte rPins[Rows]= {A4}; //Rows 0 to 3
byte cPins[Cols]= {A3,A2,A1,A0}; //Columns 0 to 2

//Initialize keypad
Keypad kpd= Keypad(makeKeymap(keymap), rPins, cPins, Rows, Cols);

void setup() {
  delay( 1000 ); //power-up safety delay
  Serial.begin(9600); //Setup serial connection (for output)
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip ); //Setup leds
  randomSeed(analogRead(0)); 
  //Set all lights to off
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
  randomCounter = 0;
  randomnr = 0;
  currentMode = 0;
  FastLED.show();
}

void loop() {
   char keypressed = kpd.getKey();
   if (keypressed != NO_KEY)
   { 
     currentMode = (int)(keypressed - '0');
   }

  switch (currentMode) {
    case 0: 
      rainbowWave(1500);
      break;
    case 1: 
      randomChill();
      break;
    case 2: 
      randomParty();
      break;
    case 3: 
      normalLight();
      break;
    default:
      //Set all lights to off
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB(0, 0, 0);
      }
      break;
  }
}

//randomchill settings
int randomMode = 0;
int randomChillModes = 4; //nr of modes

void randomChill() 
{
  unsigned long currentMS = millis();
  int prevMode = randomMode; 
  // Only continue at certain time intervals to control the pace of the mode selection
  if (currentMS - previousMSRandomizer >= INTERVAL_MS_RANDOMIZER){ 
    previousMSRandomizer += INTERVAL_MS_RANDOMIZER;
    randomnr = random(10);
    if (randomnr <= randomCounter)
    {
      while (randomMode == prevMode)
        randomMode = random(randomChillModes);
      randomCounter = 0;
    }
    else 
      randomCounter++;
  }

  int flowSpeed = 65;
  int flowDelay = 20;
  switch (randomMode) {
    case 0: 
      flowBounce(flowSpeed, flowDelay);
      break;
    case 1: 
      rainbowWave(1000);
      break;
    case 2: 
      flow(flowSpeed, flowDelay);
      break;
    case 3: 
      flowMiddle(flowSpeed, flowDelay);
      break;
    default:
      //Set all lights to off
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB(0, 0, 0);
      }
      break;
  }
}

//random party settings
int randomModeParty = 0;
int randomPartyModes = 5; //nr of modes
void randomParty()
{
  unsigned long currentMS = millis();
  int prevMode = randomModeParty;
  // Only continue at certain time intervals to control the pace of the mode selection
  if (currentMS - previousMSRandomizer >= INTERVAL_MS_RANDOMIZER){ 
    previousMSRandomizer += INTERVAL_MS_RANDOMIZER;
    randomnr = random(10);
    if (randomnr <= randomCounter)
    {
      while (randomModeParty == prevMode)
        randomModeParty = random(randomPartyModes);
      randomCounter = 0;
    }
    else 
      randomCounter++;
  }

  int flowSpeed = 30;
  int flowDelay = 0;
  switch (randomModeParty) {
    case 0: 
      partyMode(500);
      break;
    case 1: 
      rainbowWave(500);
      break;
    case 2: 
      flow(flowSpeed, flowDelay);
      break;
    case 3: 
      flowBounce(flowSpeed, flowDelay);
      break;
    case 4:
      flowMiddle(flowSpeed, flowDelay);
      break;
    default:
      //Set all lights to off
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB(0, 0, 0);
      }
      break;
  }  
}

//Settings and Modes

// RAINBOW SETTINGS //
const int LEDSPERCOLORRAINBOW = NUM_LEDS/7; // the amount of leds per rainbow color
const int LEDSNOTUSEDRAINBOW = NUM_LEDS%7;  // the amount of leds that are left 
int rainbowIndex = 0;
int startCounterRainbow = 0;
bool extraColorRainbow = false;
CRGB rainbow[7] = { 
{120, 0, 211},  //violet
{40, 0, 170},   //indigo
{0, 0, 255},    //blue
{0, 255, 0},    //green
{255, 255, 0},  //yellow
{200, 90, 0},  //orange
{255, 0, 0},    //red
};

// RAINBOW METHOD //
// Loops over all lights with a rainbow pattern
// Standard INTERVAL_MS_RAINBOW = 1000
void rainbowWave(const int INTERVAL_MS_RAINBOW)
{
  unsigned long currentMS = millis();
  int currentLed;
  int currentRainbowColor;
  int rainbowCounter;
  int rainbowLeftoverCounter;
  // Only continue at certain time intervals to control the pace of the color transition
  if (currentMS - previousMS >= INTERVAL_MS_RAINBOW){ 
    previousMS += INTERVAL_MS_RAINBOW;
    currentRainbowColor = 0;
    rainbowCounter = 0;
    rainbowLeftoverCounter = 0;
    if (startCounterRainbow == NUM_LEDS) // Prevent overflow
      startCounterRainbow = 0;
    currentLed = startCounterRainbow;
    for (int i = 0; i < NUM_LEDS; i++) {
      if (currentLed == NUM_LEDS) // Loop back to first LED
        currentLed = 0;
      if (rainbowCounter >= LEDSPERCOLORRAINBOW){ // Go to next color, unless we need to fill in one more
        if(rainbowLeftoverCounter < LEDSNOTUSEDRAINBOW && !extraColorRainbow) {
          extraColorRainbow = true;
          rainbowLeftoverCounter++;
        }
        else {
          currentRainbowColor++;
          rainbowCounter = 0;
          extraColorRainbow = false;
        }
      }
      leds[currentLed] = rainbow[currentRainbowColor];
      rainbowCounter++;
      currentLed++;
    }
    FastLED.show();
    startCounterRainbow++;
  }
}

// PARTY SETTINGS //
CRGB partyColor;
CRGB prevPartyColor;
CRGB party[5] = { 
{139, 0, 255},  //violet
{0, 0, 255},    //blue
{0, 255, 0},    //green
{255, 127, 0},  //orange
{255, 0, 0},    //red
};
int adjecentLeds = NUM_LEDS/10;

// PARTY METHOD //
// Standard PARTY_INTERVAL_MS = 750
void partyMode(const int PARTY_INTERVAL_MS)
{
  unsigned long currentMS = millis();
  if (currentMS - previousMS >= PARTY_INTERVAL_MS){ 
    // Only continue at certain time intervals to control the pace of the color transition
    previousMS += PARTY_INTERVAL_MS;
    for (int i = 0; i < NUM_LEDS; i+=adjecentLeds) {
      while (prevPartyColor == partyColor)
        partyColor = party[random(0, sizeof(party)/sizeof(party[0]) + 1)];
      for (int j = i; j < i+adjecentLeds; j++) {
        if(j < NUM_LEDS) {
           leds[j] = partyColor;
           prevPartyColor = partyColor;
        }
      }
    }
    FastLED.show();
  }
}

// FLOW SETTINGS //
int currentColorFlow = 0; 
int currentLedFlow = 0;
int delayedFlow = 0;
int counterFlow;

CRGB flowColors[5] = { 
{139, 0, 255},  //violet
{0, 255, 0},    //green
{0, 0, 255},    //blue
{255, 255, 0},  //yellow
{255, 0, 0},    //red
};


// FLOW SETTINGS //
// Base INTERVAL_MS_FLOW = 50 -> Speed of flow
// Base DELAYFLOW = 15 -> Pause between colors (actually used as DELAYFLOW * INTERVAL_MS_FLOW)
void flow(const int INTERVAL_MS_FLOW, const int DELAYFLOW)
{
  unsigned long currentMS = millis();
  if (currentMS - previousMS >= INTERVAL_MS_FLOW){ 
    // Only continue at certain time intervals to control the pace of the color transition
    previousMS += INTERVAL_MS_FLOW;
    if (currentLedFlow <= NUM_LEDS - 1)
    {
      leds[currentLedFlow] = flowColors[currentColorFlow];
      currentLedFlow++;
    }
    else if (delayedFlow >= DELAYFLOW)
    {
      currentLedFlow = 0;
      delayedFlow = 0;
      if (currentColorFlow < sizeof(flowColors)/sizeof(flowColors[0]) - 1)
        currentColorFlow++;
      else
        currentColorFlow = 0; 
    }
    else
    {
      delayedFlow++;
    }
    FastLED.show();
  }
}

// FLOWBOUNCE SETTINGS //
const int halfLedsNrBounce = NUM_LEDS/2;
int currentColorFlowBounce = 0; 
int currentLedFlowBounceRight = halfLedsNrBounce;
int currentLedFlowBounceLeft = halfLedsNrBounce-1;
int delayedFlowBounce = 0;
int counterFlowBounce;
bool rightToLeftBounce = true;

CRGB flowBounceColors[8] = { 
{255, 0, 0},    //red
{102, 0, 204},  
{0, 255, 0},    //green
{0, 255, 255},  
{255, 255, 0}, 
{0, 102, 0},     
{102, 0, 51},    
{0, 0, 255},    //blue
};


// FLOWMIDDLE METHOD //
// Base INTERVAL_MS_FLOWBOUNCE = 50 -> Speed of flow
// Base int DELAYFLOWBOUNCE = 0 -> Pause between colors (actually used as DELAWYFLOW * INTERVAL_MS_FLOW)
void flowBounce(const int INTERVAL_MS_FLOWBOUNCE, const int DELAYFLOWBOUNCE)
{
  unsigned long currentMS = millis();
  if (currentMS - previousMS >= INTERVAL_MS_FLOWBOUNCE){ 
    // Only continue at certain time intervals to control the pace of the color transition
    previousMS += INTERVAL_MS_FLOWBOUNCE;
    if (rightToLeftBounce && currentLedFlowBounceRight <= NUM_LEDS - 1)
    {
      leds[currentLedFlowBounceRight] = flowBounceColors[currentColorFlowBounce];
      leds[currentLedFlowBounceLeft] = flowBounceColors[currentColorFlowBounce];
      currentLedFlowBounceRight++;
      currentLedFlowBounceLeft--;
    }    
    else if (!rightToLeftBounce && currentLedFlowBounceLeft <= halfLedsNrBounce - 1)
    {
      currentLedFlowBounceRight--;
      currentLedFlowBounceLeft++;
      leds[currentLedFlowBounceRight] = flowBounceColors[currentColorFlowBounce];
      leds[currentLedFlowBounceLeft] = flowBounceColors[currentColorFlowBounce];
    }
    else if (delayedFlowBounce >= DELAYFLOWBOUNCE)
    {
      int prevColor = currentColorFlowBounce;
      delayedFlowBounce = 0;
      rightToLeftBounce = !rightToLeftBounce; 
      while (prevColor == currentColorFlowBounce)
        currentColorFlowBounce = random(sizeof(flowBounceColors)/sizeof(flowBounceColors[0]) - 1);
    }
    else
    {
      delayedFlowBounce++;
    }
    FastLED.show();
  }
}

// FLOWMIDDLE SETTINGS //
const int halfLedsNr = NUM_LEDS/2;
int currentColorFlowMiddle = 0; 
int currentLedFlowMiddleRight = halfLedsNr;
int currentLedFlowMiddleLeft = halfLedsNr-1;
int delayedFlowMiddle = 0;
int counterFlowMiddle;

CRGB flowMiddleColors[8] = { 
{255, 0, 0},    //red
{102, 0, 204},  
{0, 255, 0},    //green
{0, 255, 255},  
{255, 255, 0}, 
{0, 102, 0},     
{102, 0, 51},    
{0, 0, 255},    //blue
};


// FLOWMIDDLE METHOD //
// Base INTERVAL_MS_FLOWMIDDLE = 50 -> Speed of flow
// Base DELAYFLOWMIDDLE = 15 -> Pause between colors (actually used as DELAYFLOW * INTERVAL_MS_FLOW)
void flowMiddle(const int INTERVAL_MS_FLOWMIDDLE, const int DELAYFLOWMIDDLE)
{
  unsigned long currentMS = millis();
  if (currentMS - previousMS >= INTERVAL_MS_FLOWMIDDLE){ 
    // Only continue at certain time intervals to control the pace of the color transition
    previousMS += INTERVAL_MS_FLOWMIDDLE;
    if (currentLedFlowMiddleRight <= NUM_LEDS - 1)
    {
      leds[currentLedFlowMiddleRight] = flowMiddleColors[currentColorFlowMiddle];
      leds[currentLedFlowMiddleLeft] = flowMiddleColors[currentColorFlowMiddle];
      currentLedFlowMiddleRight++;
      currentLedFlowMiddleLeft--;
    }
    else if (delayedFlowMiddle >= DELAYFLOWMIDDLE)
    {
      currentLedFlowMiddleRight = halfLedsNr;
      currentLedFlowMiddleLeft = halfLedsNr-1;
      delayedFlowMiddle = 0;
      if (currentColorFlowMiddle < sizeof(flowMiddleColors)/sizeof(flowMiddleColors[0]) - 1)
        currentColorFlowMiddle++;
      else
        currentColorFlowMiddle = 0; 
    }
    else
    {
      delayedFlowMiddle++;
    }
    FastLED.show();
  }
}

void normalLight() 
{
  unsigned long currentMS = millis();
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(189, 183, 107);
  }
  FastLED.show();
}
