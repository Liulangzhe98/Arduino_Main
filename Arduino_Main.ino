/*
   Author: Thijs van Herwijnen en Céline Vos
   Date Created: 22 Oktober 2016
   Last Modification: 24 Oktober 2016
   Version: 1.0
  
   Description:  This Arduino Script was written for a High Speed Photography setup using
   an Arduino Mega, an LCDKeypadShield v2.0, and a minimal number of components used for 
   Sound Triggers, IR Triggers, and switch position sensors.  The Keypad on the LCDKeypadShield
   is not used since this project was mounted in a project box and the buttons ont he keypad are
   not in a place where they are easily usable. So momentary push buttons were used for input on 
   the device.

   To change parameters while the device is running, flip the Arm/Update Switch to Update.  The first
   parameter will be displayed. You can scroll through the parameters using the Up and Down buttons
   and change the values by using the Left and Right Buttons.  When you have finished making updates,
   flip the Arm/Update Switch back to Arm. To save the new values to memory so they will be there the
   next time you turn the unit on, press the ENTER button to save the values while in update mode. 

   Libraries used:
   You will need the EXROM Extention Library for Arduino located at:
   http://code.google.com/p/arduino-exrom/downloads/list
    * Deze bibliotheek gaf een fout aan. Dit pas optelossen door de code in de bibliotheek te veranderen. 
   
   You will also need the LCDKeypad Library for the Arduino located at:
   http://www.dfrobot.com/image/data/DFR0009/LCDKeypad.zip
   The LiquidCrystal Library is also used and is a standard Library with the Arduino software.
 */

// Libraries 
#include <LiquidCrystal.h>
#include <LCDKeypad.h>
#include <EXROM.h>

LCDKeypad lcd;

//Set Arduno Pins
int LeftButton = A1;
int LeftValue = 0;
int RightButton = A2;
int RightValue = 0;
int DownButton = A3;
int DownValue = 0;
int UpButton = A4;
int UpValue = 0;
int EnterButton = A5;
int EnterValue = 0;
int LaunchButton = A6;
int LaunchButtonValue = 0;
int UpdateSwitch = A7;
int UpdateValue = 0;
int MultiFlashSwitch = A8;
int MultiFlashValue = 1;
int IRDetect = A9;
int IRValue = 0;
int PeizoDetect = A10;
int PeizoValue = 0;
int DetectorSwitch = A15;
int DetectorValue = 0;
int IVSwitch = A13;
int IVValue = 0;
int LightningSwitch = A14;
int LightningValue = 0;
int LightningDetect = A11;
int LightningDetectValue = 0;

//TODO: Moet ik nog aanpassen, omdat we een schakelaar te dicht bij de display hebben zitten en deze de oneven rij van digitale pins afdekt.
int ShutterTrigger = 23;
int FlashTrigger = 25;

int ValveATrigger = 38;
int ValveBTrigger = 40;
int ValveCTrigger = 41;
int ValveDTrigger = 44;

/*
 * Wij bedienen geen externe lampen dus is dit niet nodig.
 * 
  int LampASwitch = 27;
  int LampBSwitch = 37;
*/

/*
 * Wij hebben geen lampjes in het bedieningspaneel dus hebben we dit stuk niet nodig.
 * 
  int GreenLamp = 29;
  int YellowLamp = 31;
  int BlueLamp = 33;
  int RedLamp = 35;
*/



// Set Global Variables
int RunOnce = 0;
int DisplayDelay = 3000;
int memValue = 0;

// Create arrays to hold Variable Descriptions and Values
const char* DescArray[] = {"Flash Delay", "Drop Delay", "Drop Size", "Initial Delay", "Shutter Delay", "MutiFlash", "MultiFlash Delay", "# of Drops", "Valve A Active", "Valve B Active", "Valve C Active", "Valve D Active", "Valve Delay AB", "Valve Delay BC", "Valve Delay CD","PreFlash Trigger","Sound Threshold","Current Sound","Lightning Threshold","Current Light","IV Start Delay","IV Shutter Speed","IV Interval Secs","IV Intrval MSecs","IV Repetitions"};
float ValueArray[] = {245.0,20.0,85.0,5.0,0.0,1.0,100.0,2.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,1.0,200.0,0.0,200.0,0.0,0.0,0.0,1.0,0.0,0.0};
const char* TypeArray[] = {"Millisecs","Millisecs","Millisecs","Second(s)","Second(s)","Pulses","Millisecs","Drops","0=Off","0=Off","0=Off","0=Off","Millisecs","Millisecs","Millisecs","0=Off","Dn= >Sens","Level","Dn= >Sens","Level","Seconds","Seconds","Seconds","Millisecs","0=Endless"};
float array2[sizeof(ValueArray)];
char* tval;


/* Hier kan je eigen characters maken.
 * Een handige site hiervoor is : 
 * https://omerk.github.io/lcdchargen/
 */
byte darrow[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100,
};

byte uarrow[8] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
};

byte rarrow[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
};

byte larrow[8] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
};

byte waterdrop[8] = {
  B00000,
  B00100,
  B01100,
  B11110,
  B11111,
  B11111,
  B11111,
  B01110
};

byte raree[8] = {
  B00010,
  B00100,
  B01110,
  B10001,
  B11111,
  B10000,
  B01110,
  B00000
};




void setup()
{
  /*
   * Wij hebben geen lampjes in het bedieningspaneel dus hebben we dit stuk niet nodig.
   * 
  
  // Set Pin Modes for Indicator Lights and turn them on
  pinMode(RedLamp, OUTPUT);
  digitalWrite(RedLamp, HIGH);
  pinMode(BlueLamp, OUTPUT);
  digitalWrite(BlueLamp, HIGH);
  pinMode(YellowLamp, OUTPUT);
  digitalWrite(YellowLamp, HIGH);
  pinMode(GreenLamp, OUTPUT);
  digitalWrite(GreenLamp, HIGH);
  delay(1000);*/

  // Send Intro message to Display
  /* 
   *  Intro van Micheal Ross
   *  PlayIntro();
  */  

  PlayIntroCenT();


  // Set the rest of the Pin Modes for the Arduino
  pinMode(IRDetect, INPUT); 
  pinMode(FlashTrigger, OUTPUT);
  digitalWrite(FlashTrigger, LOW);
  pinMode(ShutterTrigger, OUTPUT);
  digitalWrite(ShutterTrigger, LOW);
/*
 * Wij bedienen geen externe lampen dus is dit niet nodig.
 * 
  pinMode(LampASwitch, OUTPUT);
  pinMode(LampBSwitch, OUTPUT);
  digitalWrite(LampASwitch, HIGH);
  digitalWrite(LampBSwitch, HIGH);
*/
  
  pinMode(ValveATrigger, OUTPUT);
  pinMode(ValveBTrigger, OUTPUT);
  pinMode(ValveCTrigger, OUTPUT);
  pinMode(ValveDTrigger, OUTPUT);
  digitalWrite(ValveATrigger, LOW);
  digitalWrite(ValveBTrigger, LOW);
  digitalWrite(ValveCTrigger, LOW);
  digitalWrite(ValveDTrigger, LOW);

/*
 * Wij hebben geen lampjes in het bedieningspaneel dus hebben we dit stuk niet nodig.
  IndicatorLightTest();
*/

/* .createChar zorgt ervoor dat je later je jezelf gemaakte characters kunt hergebruiken.
 * https://www.sparkfun.com/datasheets/LCD/HD44780.pdf pagina 17 is de tabel voor het lcd'tje wat wij gebruiken
 * Nummers die vrij zijn: 0 - 32 & 126 - 160 
 */
  lcd.createChar(0, darrow);
  lcd.createChar(1, uarrow);
  lcd.createChar(2, larrow);
  lcd.createChar(3, rarrow);
  lcd.createChar(4, waterdrop);
  lcd.createChar(5, raree);
  
  delay(5000);
  
  
  // Read the Paramters from saved memory and update the default
  // Array values with those retrieved from memory
  EXROM.read(0, array2, sizeof(array2));
  if(array2[0] > 0)
  {
    for (int i = 0; i < 24; i++)
    {
      ValueArray[i] = array2[i];
    }  
  }

  //Serial.begin(9600);
}


void loop()
{
  // Read the Switch Values to see what mode we are in
  ReadSwitches();
  lcd.clear();
  if(UpdateValue > 500)
  {
    if((LightningValue > 500) || (IVValue > 500))
    {
      if(LightningValue > 500)
      {
        // We are in Lightning Mode
        lcd.print("Lightning Mode");
//        digitalWrite(RedLamp, HIGH);
//        digitalWrite(BlueLamp, HIGH);
      }
      if(IVValue > 500)
      {
        // We are in Intervalometer Mode
        lcd.print("Intervalometer");
//        digitalWrite(RedLamp, HIGH);
//        digitalWrite(BlueLamp, HIGH);
      }
    }
    else
    {
      if(DetectorValue > 500)
      { 
        // We are in Infrared Detector Mode
        lcd.print("Infrared Mode...");
//        digitalWrite(RedLamp, HIGH);
//        digitalWrite(BlueLamp, LOW);
        ValueArray[0] = array2[0];
        ValueArray[3] = array2[3];
      }
      else
      {
        // We are in Sound Detection Mode
        lcd.print("Sound Mode...");
//        digitalWrite(RedLamp, LOW);
//        digitalWrite(BlueLamp, HIGH);
        ValueArray[0] = 0;
        ValueArray[3] = 0;
      }
    }
    lcd.setCursor(0,1);
    lcd.print("READY TO FIRE!");
    // Turn the Green Lamp "Ready" indicator on
//    digitalWrite(YellowLamp, LOW);
//    digitalWrite(GreenLamp, HIGH);
    delay(50);
  }
}

//TODO: Volgens mij klopt dit niet. Dus nog even testen.
void FireOneValve()
{
  for(int i=0; i < ValueArray[7]; i++)
  {
    digitalWrite(ValveATrigger, HIGH);
    digitalWrite(ValveBTrigger, HIGH);
    delay(ValueArray[2]);
    digitalWrite(ValveATrigger, LOW);
    digitalWrite(ValveBTrigger, LOW);
    if(i < (ValueArray[7] - 1))
    {
      delay(ValueArray[1]);
    }
  }
}

void FireTwoValves()
{
  for(int i=0; i < ValueArray[7]; i++)
  {
    digitalWrite(ValveATrigger, HIGH);
    digitalWrite(ValveBTrigger, HIGH);
    delay(ValueArray[2]);
    digitalWrite(ValveATrigger, LOW);
    digitalWrite(ValveBTrigger, LOW);
    if(i < (ValueArray[7] - 1))
    {
      delay(ValueArray[1]);
    }
  }
}

void FireThreeValves()
{
  for(int i=0; i < ValueArray[7]; i++)
  {
    digitalWrite(ValveATrigger, HIGH);
    digitalWrite(ValveBTrigger, HIGH);
    digitalWrite(ValveCTrigger, HIGH);
    delay(ValueArray[2]);
    digitalWrite(ValveATrigger, LOW);
    digitalWrite(ValveBTrigger, LOW);
    digitalWrite(ValveCTrigger, LOW);
    if(i < (ValueArray[7] - 1))
    {
      delay(ValueArray[1]);
    }
  }
}

void FireFourValves()
{
  for(int i=0; i < ValueArray[7]; i++)
  {
    digitalWrite(ValveATrigger, HIGH);
    digitalWrite(ValveBTrigger, HIGH);
    digitalWrite(ValveCTrigger, HIGH);
    digitalWrite(ValveDTrigger, HIGH);
    delay(ValueArray[2]);
    digitalWrite(ValveATrigger, LOW);
    digitalWrite(ValveBTrigger, LOW);
    digitalWrite(ValveCTrigger, LOW);
    digitalWrite(ValveDTrigger, LOW);
    if(i < (ValueArray[7] - 1))
    {
      delay(ValueArray[1]);
    }
  }
}



void IRSequence()
{
  // Turn the Green Lamp off and Yellow Lamp on to show that the process is running
//  digitalWrite(YellowLamp, HIGH);
//  digitalWrite(GreenLamp, LOW);
  lcd.clear();
  lcd.print("Running...");
  lcd.setCursor(0,1);
  lcd.print("Stand By...");
  delay(DisplayDelay);

  // Send a preflash trigger to make sure the Flash units are not sleeping
  if(ValueArray[15] == 1)
  {
    digitalWrite(FlashTrigger, HIGH);
    digitalWrite(FlashTrigger, LOW);
  }
/*
  // Turn off the Lamps
  digitalWrite(LampASwitch, LOW);
  digitalWrite(LampBSwitch, LOW);
*/
  delay(ValueArray[3] * 1000);


  // Open the Shutter
  digitalWrite(ShutterTrigger, HIGH);
  delay(1000);
  
  valveChooser();

  // Wait for the last drop from valve A to pass through the IR detector
  IRValue = analogRead(IRDetect); 
  while(IRValue >= 50)
  {  
    IRValue = analogRead(IRDetect);
  }

  // Wait before firing the flash
  delay(ValueArray[0]);

  // Check to see if we need to fire Multiple Flash pulses
  if(ValueArray[5] > 1)
  {
    for(int i = 1; i <= ValueArray[5]; i++)
    {
      // Fire the Flash
      digitalWrite(FlashTrigger, HIGH);
      delay(1);
      // Reset the FlashTrigger
      digitalWrite(FlashTrigger, LOW);
      delay((ValueArray[6]) - 1);
    }
  }
  else
  {
    // Just fire the flash once
    // Fire the Flash
    digitalWrite(FlashTrigger, HIGH);
    delay(1);
    // Reset the FlashTrigger
    digitalWrite(FlashTrigger, LOW);
  }

  // Wait for the designated Shutter release delay
  delay(ValueArray[4] * 1000);

  // Close the Shutter
  digitalWrite(ShutterTrigger, LOW);
  delay(500);

  /*
    // Turn the Lamps back on 
    digitalWrite(LampASwitch, HIGH);
    digitalWrite(LampBSwitch, HIGH);
  */
}


void PeizoSequence()
{
//  digitalWrite(BlueLamp, HIGH);
//  digitalWrite(YellowLamp, LOW);
//  digitalWrite(GreenLamp, LOW);
  PeizoValue = 1;
  
  lcd.clear();
  lcd.print("Running...");
  lcd.setCursor(0,1);
  lcd.print("Stand By...");
  delay(DisplayDelay);

  // Send a preflash trigger to make sure the Flash units are not sleeping
  if(ValueArray[15] == 1)
  {
    digitalWrite(FlashTrigger, HIGH);
    digitalWrite(FlashTrigger, LOW);
  }

//  // Turn off the Lights
//  digitalWrite(LampASwitch, LOW);
//  digitalWrite(LampBSwitch, LOW);
  delay(ValueArray[3] * 1000);
    
  // Open the Shutter
  digitalWrite(ShutterTrigger, HIGH);
  delay(1000);
  
  // Wait for the Trigger Sound
  lcd.setCursor(0,1);
  lcd.print("Waiting for Sound...");
  lcd.setCursor(0,1);
  while(PeizoValue < ValueArray[16])
  {  
    PeizoValue = analogRead(PeizoDetect);
  }
  
  if (PeizoValue > ValueArray[16]) {  
    // Wait before firing the flash
    delay(ValueArray[0]);

    // Check to see if we need to fire Multiple Flash pulses
    if(ValueArray[5] > 1)
    {
      for(int i = 1; i <= ValueArray[5]; i++)
      {
        // Fire the Flash
        digitalWrite(FlashTrigger, HIGH);
        delay(1);
        // Reset the FlashTrigger
        digitalWrite(FlashTrigger, LOW);
        delay(ValueArray[6]);
      }
    }
    else
    {
      // Just fire the flash once
      // Fire the Flash
      digitalWrite(FlashTrigger, HIGH);
      delay(1);
      // Reset the FlashTrigger
      digitalWrite(FlashTrigger, LOW);
    }

    valveChooser();

    // Close the Shutter
    digitalWrite(ShutterTrigger, LOW);
    delay(500);

//    // Turn the Lamps back on 
//    digitalWrite(LampASwitch, HIGH);
//    digitalWrite(LampBSwitch, HIGH);
  }
  delay(2000);
}

void IVSequence()
{
//  digitalWrite(RedLamp, HIGH);
//  digitalWrite(BlueLamp, HIGH);
//  digitalWrite(YellowLamp, LOW);
//  digitalWrite(GreenLamp, LOW);
  
  lcd.clear();
  lcd.print("Intervalometer");
  lcd.setCursor(0,1);
  lcd.print("Running...");
  delay(DisplayDelay);

  delay(ValueArray[3] * 1000);

  LaunchButtonValue = 0;
  if(ValueArray[24] < 1)
  {
    //Single shot
    while(LaunchButtonValue < 500)
    {
      // Open the Shutter
      digitalWrite(ShutterTrigger, HIGH);
//      digitalWrite(GreenLamp, HIGH);

      if(ValueArray[21] > 0)
      {
        delay(ValueArray[21] * 1000);
      }
      digitalWrite(ShutterTrigger, LOW);
//      digitalWrite(GreenLamp, LOW);
      delay((ValueArray[22] * 1000) + (ValueArray[23]));
      LaunchButtonValue = analogRead(LaunchButton); 
    }
    delay(500);
  }
  else
  {
    for(int j = 0; j < ValueArray[24]; j++)
    {
      // Open the Shutter
      digitalWrite(ShutterTrigger, HIGH);
//      digitalWrite(GreenLamp, HIGH);

      if(ValueArray[21] > 0)
      {
        delay(ValueArray[21] * 1000);
      }
      digitalWrite(ShutterTrigger, LOW);
//      digitalWrite(GreenLamp, LOW);
      delay((ValueArray[22] * 1000) + (ValueArray[23]));
    }
  }
}

void LightningSequence()
{
//  digitalWrite(RedLamp, HIGH);
//  digitalWrite(BlueLamp, HIGH);
//  digitalWrite(YellowLamp, HIGH);
//  digitalWrite(GreenLamp, LOW);
  LightningDetectValue = 1;
  
  lcd.clear();
  lcd.print("Running...");
  lcd.setCursor(0,1);
  lcd.print("Lightning Mode");
  delay(DisplayDelay);

  // Wait for the Lightning Trigger 
  lcd.setCursor(0,1);
  lcd.print("Wait-Lightning");
  while(LightningDetectValue < ValueArray[18])
  {  
    LightningDetectValue = analogRead(LightningDetect);
  }
    
  // Release the Shutter
  digitalWrite(ShutterTrigger, HIGH);
  digitalWrite(ShutterTrigger, LOW);
  delay(50);
  delay(2000);
}



void FireSequence()
{
  //See if we are in Intervalometer or Lightning Mode
  if((LightningValue > 500) || (IVValue > 500))
  {
    if(LightningValue > 500)
    {
      LightningSequence();
    }
    if(IVValue > 500)
    {
      IVSequence();
    }
  }
  else
  {
    //See if we are in IR Trigger or Piezo Trigger Mode
    //DetectorValue = analogRead(DetectorSwitch);
    if(DetectorValue > 300)
    {
      // We are in IR Trigger Mode
      IRSequence(); 
    }
    else
    {
      // We are in Peizo Trigger Mode
      PeizoSequence();
    }
  }
}


void ReadSwitches()
{
  //Read the Go Switch first to see if we need to go through the Fire sequence
  LaunchButtonValue = analogRead(LaunchButton);
  if(LaunchButtonValue > 500  && memValue == 0)
  {
    // Run the Fire Sequence
    FireSequence();
    memValue = 1;
  }
  if(LaunchButtonValue <= 500)
  {
    memValue = 0;
  }

  //Next we read the Lightning Switch to see if we need to be in Lightning Mode
  LightningValue = analogRead(LightningSwitch);

  //Next we read the Intervalometer Switch to see if we need to be in Intervalometer Mode
  IVValue = analogRead(IVSwitch);

  //Next we read the Sound/IR Mode Switch to see which mode we need to be in
  DetectorValue = analogRead(DetectorSwitch);

  //Next we read the Update/Arm Switch to see if we need to go into Update Mode
  UpdateValue = analogRead(UpdateSwitch);

  if(UpdateValue < 500)
  {
    // Go into Update mode to allow updating of each user setting parameter
    lcd.clear();
    lcd.print("Update Mode...");
    lcd.setCursor(0,1);
    lcd.print("Stand by...");
    delay(1000);
    lcd.clear();
    lcd.write((byte)0);
    lcd.write((byte)1);
    lcd.setCursor(3,0);
    lcd.print(" Select Param");
    lcd.setCursor(0,1);
    lcd.write((byte)2);
    lcd.write((byte)3);
    lcd.setCursor(3,1);
    lcd.print(" Change Value");
    delay(3000);

    UpdateMode();
//    digitalWrite(BlueLamp, LOW);

  }
}


void UpdateDisplay()
{
  lcd.clear();
  lcd.print("Current Values...");
  delay(1000);
  for(int k = 0; k < 25; k++)
  {
    lcd.clear();
    lcd.print(DescArray[k]);
    lcd.setCursor(0,1);
    lcd.print(ValueArray[k]);
    lcd.setCursor(7,1);
    lcd.print(TypeArray[k]);
    delay(1000);
  }
}
  

void UpdateMode() 
{
//  digitalWrite(RedLamp, LOW);
//  digitalWrite(BlueLamp, LOW);
//  digitalWrite(GreenLamp, LOW);
//  digitalWrite(YellowLamp, HIGH);
  int x = 0;
  // Invoke a while loop and stay in it until the Update/Fire switch has been moved to Fire
  while(UpdateValue < 500)
  {
    // Display Variable values
    lcd.clear();
    lcd.print(DescArray[x]);
    lcd.setCursor(0,1);
    if((x == 17) || (x == 19))
    {
      if( x == 17 )
      {
        PeizoValue = analogRead(PeizoDetect); 
        lcd.print(PeizoValue);
        delay(100);
      }
      if( x == 19 )
      {
        LightningDetectValue = analogRead(LightningDetect); 
        lcd.print(LightningDetectValue);
        delay(100);
      }
    }
    else
    {
      lcd.print(ValueArray[x]); 
    }
    lcd.setCursor(7,1);
    lcd.print(TypeArray[x]);
    delay(100);
    
    //Read the EnterButton to see if is pressed to save the current values to the EEROM
    EnterValue = analogRead(EnterButton);
    if(EnterValue > 500)
    {
//      digitalWrite(RedLamp, HIGH);
//      digitalWrite(BlueLamp, HIGH);
//      digitalWrite(YellowLamp, HIGH);
//      digitalWrite(GreenLamp, LOW);
//  
      lcd.clear();
      lcd.print("Saving Values");
      lcd.setCursor(0,1);
      lcd.print("To Memory...");
      delay(DisplayDelay);
      EXROM.write(0, ValueArray, sizeof(ValueArray));
      lcd.clear();
      lcd.print("Values Saved!");
      delay(DisplayDelay);
    }

    //Read the UpSwitch to see if is pressed to move up to the next Variable
    UpValue = analogRead(UpButton);
    if(UpValue > 500)
    {
      if(x <= 0)
      {
        x = 24;
      }
      else
      {
        x = x - 1;
      }
    }

    //Read the DownSwitch to see if is pressed to move up to the next Variable
    DownValue = analogRead(DownButton);
    if(DownValue > 500)
    {
      if(x >= 24)
      {
        x = 0;
      }
      else
      {
        x = x + 1;
      }
    }
    
    RightValue = analogRead(RightButton);
    if(RightValue > 500)
    {
      switch (x)
      {
        case 0:
          ValueArray[x] += .1;
          break;
        case 1:
          ValueArray[x] += .1;
          break;
        case 2:
          ValueArray[x] += .1;
          break;
        case 3:
          ValueArray[x] += .1;
          break;
        case 4:
          ValueArray[x] += .1;
          break;
        case 5:
          ValueArray[x] += 1.0;
          break;
        case 6:
          ValueArray[x] += .1;
          break;
        case 7:
          ValueArray[x] += 1.0;
          break;
        case 8:
          ValueArray[x] += 1.0;
          break;
        case 9:
          ValueArray[x] += 1.0;
          break;
        case 10:
          ValueArray[x] += 1.0;
          break;
        case 11:
          ValueArray[x] += 1.0;
          break;
        case 12:
          ValueArray[x] += .1;
          break;
        case 13:
          ValueArray[x] += .1;
          break;
        case 14:
          ValueArray[x] += .1;
          break;
        case 15:
          ValueArray[x] += 1.0;
          break;
        case 16:
          ValueArray[x] += 1.0;
          break;
        case 17:
          ValueArray[x] += 1.0;
          break;
        case 18:
          ValueArray[x] += 1.0;
          break;
        case 19:
          ValueArray[x] += 1.0;
          break;
        case 20:
          ValueArray[x] += .1;
          break;
        case 21:
          ValueArray[x] += 1.0;
          break;
        case 22:
          ValueArray[x] += 1.0;
          break;
        case 23:
          ValueArray[x] += 1.0;
          break;
        case 24:
          ValueArray[x] += 1.0;
          break;
      }
      if( ValueArray[x] >= 1000 )
      {
        ValueArray[x] = 999.0;
      }
    }
    
    LeftValue = analogRead(LeftButton);
    if(LeftValue > 500)
    {
      switch (x)
      {
        case 0:
          ValueArray[x] -= .1;
          break;
        case 1:
          ValueArray[x] -= .1;
          break;
        case 2:
          ValueArray[x] -= .1;
          break;
        case 3:
          ValueArray[x] -= .1;
          break;
        case 4:
          ValueArray[x] -= .1;
          break;
        case 5:
          ValueArray[x] -= 1.0;
          break;
        case 6:
          ValueArray[x] -= .1;
          break;
        case 7:
          ValueArray[x] -= 1.0;
          break;
        case 8:
          ValueArray[x] -= 1.0;
          break;
        case 9:
          ValueArray[x] -= 1.0;
          break;
        case 10:
          ValueArray[x] -= 1.0;
          break;
        case 11:
          ValueArray[x] -= 1.0;
          break;
        case 12:
          ValueArray[x] -= .1;
          break;
        case 13:
          ValueArray[x] -= .1;
          break;
        case 14:
          ValueArray[x] -= .1;
          break;
        case 15:
          ValueArray[x] -= 1.0;
          break;
        case 16:
          ValueArray[x] -= 1.0;
          break;
        case 17:
          ValueArray[x] -= 1.0;
          break;
        case 18:
          ValueArray[x] -= 1.0;
          break;
        case 19:
          ValueArray[x] -= 1.0;
          break;
        case 20:
          ValueArray[x] -= .1;
          break;
        case 21:
          ValueArray[x] -= 1.0;
          break;
        case 22:
          ValueArray[x] -= 1.0;
          break;
        case 23:
          ValueArray[x] -= 1.0;
          break;
        case 24:
          ValueArray[x] -= 1.0;
          break;
      }
      if( ValueArray[x] <= 0 )
      {
        ValueArray[x] = 0.0;
      }
    }
    delay(50);        
    
    //Read the position of the Update/Arm switch to see if we are done
    UpdateValue = analogRead(UpdateSwitch);  
  }
//  digitalWrite(GreenLamp, HIGH);
//  digitalWrite(YellowLamp, LOW);
}



void PlayIntro()
{
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("   PRECISION    ");
  lcd.setCursor(0,1);
  lcd.print("HIGH SPEED PHOTO");
  delay(2000);
  lcd.clear();
  lcd.print("   PRECISION    ");
  lcd.setCursor(0,1);
  lcd.print("    TRIGGER");
  delay(2000);
  lcd.clear();
  lcd.print("Custom Designed");
  lcd.setCursor(0,1);
  lcd.print("by Michael Ross");
  delay(2000);
  lcd.clear();
  lcd.print("Michael Ross   ");
  lcd.setCursor(0,1);
  lcd.print("MRossPhoto.com ");
  delay(2000);
  lcd.clear();
  lcd.print("    Version    ");
  lcd.setCursor(0,1);
  lcd.print("      2.3      ");
  delay(2000);
  lcd.clear();
  lcd.print("Initializing...");
  lcd.setCursor(0,1);
  lcd.print("Stand By... ");
  delay(2000);

}

void PlayIntroCenT()
{  
  String namen = "C";
  namen.concat((char)5);
  namen.concat("line en Thijs");
  
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print(namen);
  delay(2000);
}

void valveChooser()
{
  if(ValueArray[8] > 0.0) 
  {
    if(ValueArray[9] > 0.0)
    {  
      if(ValueArray[10] > 0.0)
      { 
        if(ValueArray[11] > 0.0)
        {
          FireFourValves();
        }
        else
        {
          FireThreeValves();
        }
      }
      else
      {
        FireTwoValves();
      }
    }
    else
    {
      FireOneValve();
    }
  }
}


/*
void IndicatorLightTest()
{
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  digitalWrite(RedLamp, HIGH);
  delay(50);
  digitalWrite(BlueLamp, HIGH);
  delay(20);
  digitalWrite(YellowLamp, LOW);
  delay(60);
  digitalWrite(GreenLamp, LOW);
  delay(30);
  digitalWrite(RedLamp, LOW);
  delay(10);
  digitalWrite(BlueLamp, LOW);
  delay(10);
  digitalWrite(YellowLamp, HIGH);
  delay(40);
  digitalWrite(GreenLamp, HIGH);
  delay(10);
  digitalWrite(RedLamp, LOW);
  delay(60);
  digitalWrite(BlueLamp, HIGH);
  delay(20);
  digitalWrite(YellowLamp, LOW);
  delay(10);
  digitalWrite(GreenLamp, HIGH);
  digitalWrite(RedLamp, HIGH);
  delay(40);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  delay(50);
  digitalWrite(GreenLamp, LOW);
  delay(50);
  digitalWrite(RedLamp, HIGH);
  delay(50);
  digitalWrite(BlueLamp, HIGH);
  delay(10);
  digitalWrite(YellowLamp, LOW);
  delay(20);
  digitalWrite(GreenLamp, LOW);
  delay(10);
  digitalWrite(RedLamp, LOW);
  delay(50);
  digitalWrite(BlueLamp, LOW);
  delay(50);
  digitalWrite(YellowLamp, HIGH);
  delay(50);
  digitalWrite(GreenLamp, HIGH);
  delay(10);
  digitalWrite(RedLamp, LOW);
  delay(15);
  digitalWrite(BlueLamp, HIGH);
  delay(10);
  digitalWrite(YellowLamp, LOW);
  delay(50);
  digitalWrite(GreenLamp, HIGH);
  digitalWrite(RedLamp, HIGH);
  delay(50);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  delay(50);
  digitalWrite(GreenLamp, LOW);
  delay(30);
  digitalWrite(RedLamp, HIGH);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(20);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(50);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(40);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, HIGH);
  delay(60);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(20); 
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(40);
  digitalWrite(RedLamp, HIGH);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(60);  
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(30);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(50);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, HIGH);
  delay(10);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(30); 
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(60);
  digitalWrite(RedLamp, HIGH);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(50);  
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(10);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(50);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, HIGH);
  delay(30);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(20); 
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(60);
  digitalWrite(RedLamp, HIGH);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(40);  
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(10);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(20);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, HIGH);
  delay(40);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(30); 
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(60);
  digitalWrite(RedLamp, HIGH);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(40);  
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(30);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(50);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, HIGH);
  delay(20);
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, HIGH);
  digitalWrite(GreenLamp, LOW);
  delay(10); 
  digitalWrite(RedLamp, LOW);
  digitalWrite(BlueLamp, HIGH);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(60);
  digitalWrite(RedLamp, HIGH);
  digitalWrite(BlueLamp, LOW);
  digitalWrite(YellowLamp, LOW);
  digitalWrite(GreenLamp, LOW);
  delay(40);  
}
*/

