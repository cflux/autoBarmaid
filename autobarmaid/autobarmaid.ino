#include <AceButton.h>
using namespace ace_button;
#include <AdjustableButtonConfig.h>
#include <ButtonConfig.h>

#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif

U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);
const int btn1Pin = 22;
const int btn2Pin = 23;
AceButton button1(btn1Pin);
AceButton button2(btn2Pin);

int btn1State = 0;
int btn2State = 0;
int btnBothState = 0;

ButtonConfig btnCfg;
void handleBtnEvent(AceButton*, uint8_t, uint8_t);
void prime();
void allOFF();
void pour();
void pump(int, int);

int lcount = 0; // start at zero
const int maxCnt = 25;
//int r = 0;
//char* buf = "0000";
//const int MAX_MENU = 0; // should be below service menu so we can't get there with single press
const int SERVICE_MENU = 1;
const int ROOT_MENU = 0;
int mnu = ROOT_MENU;

const int pumpApin = 0;
const int pumpBpin = 1;
const int pumpCpin = 2;
const int pumpDpin = 3;
const int pumpEpin = 4;
const int pumpFpin = 5;
const int pumpGpin = 6;
const int pumpHpin = 7;

const int primeTime = 1500; // 1.5 sec

const int hlfOZTime = 600; // .6 sec to pump an oz

void setup(void)
{
  pinMode(btn1Pin,INPUT_PULLUP);
  pinMode(btn2Pin,INPUT_PULLUP);

  pinMode(pumpApin, OUTPUT); 
  pinMode(pumpBpin, OUTPUT);
  pinMode(pumpCpin, OUTPUT);
  pinMode(pumpDpin, OUTPUT);
  pinMode(pumpEpin, OUTPUT);
  pinMode(pumpFpin, OUTPUT);
  pinMode(pumpGpin, OUTPUT);
  pinMode(pumpHpin, OUTPUT);
  allOFF();
  
  btnCfg.setEventHandler(handleBtnEvent);
  //btnCfg.setFeature(ButtonConfig::kFeatureClick);
  button1.setButtonConfig(&btnCfg);
  button1.init(btn1Pin, HIGH, 0);

  button2.setButtonConfig(&btnCfg);
  button2.init(btn2Pin, HIGH, 1);
  
  u8x8.begin();
  u8x8.setPowerSave(0);
  
}

void allOFF() {
  digitalWrite(pumpApin, HIGH); // turn off pump
  digitalWrite(pumpBpin, HIGH); // turn off pump
  digitalWrite(pumpCpin, HIGH); // turn off pump
  digitalWrite(pumpDpin, HIGH); // turn off pump
  digitalWrite(pumpEpin, HIGH); // turn off pump
  digitalWrite(pumpFpin, HIGH); // turn off pump
  digitalWrite(pumpGpin, HIGH); // turn off pump
  digitalWrite(pumpHpin, HIGH); // turn off pump
}

void pump(int pin, int ptime)
{
  digitalWrite(pin, LOW); // turn on pump
  delay(ptime); //1 
  digitalWrite(pin, HIGH); // turn on pump
}

void prime() {
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
  u8x8.clear();
  u8x8.draw2x2String(1,2,"Priming");
  u8x8.draw2x2String(2,5,"Pump A");
  u8x8.refreshDisplay();
  // run pumps here:
  pump(pumpApin,primeTime);
  
  u8x8.draw2x2String(2,5,"Pump B");
  u8x8.refreshDisplay();
  pump(pumpBpin,primeTime);

  u8x8.draw2x2String(2,5,"Pump C");
  u8x8.refreshDisplay();
  pump(pumpCpin,primeTime);

  u8x8.draw2x2String(2,5,"Pump D");
  u8x8.refreshDisplay();
  pump(pumpDpin,primeTime);

  u8x8.draw2x2String(2,5,"Pump E");
  u8x8.refreshDisplay();
  pump(pumpEpin,primeTime);

  u8x8.draw2x2String(2,5,"Pump F");
  u8x8.refreshDisplay();
  pump(pumpFpin,primeTime);

  u8x8.draw2x2String(2,5,"Pump G");
  u8x8.refreshDisplay();
  pump(pumpGpin,primeTime);
  
  u8x8.draw2x2String(2,5,"Pump H");
  u8x8.refreshDisplay();
  pump(pumpHpin,primeTime);
  
  u8x8.clear();
  u8x8.draw2x2String(2,2,"Priming");
  u8x8.draw2x2String(3,5,"Done!");
  u8x8.refreshDisplay();
  delay(2000); // 2 sec delay
}
int numDrinks = 12;
/*
 * Pump A  | Coffee Liqueur
 * Pump B  | Vodka
 * Pump C  | Rum
 * Pump D  | Whiskey
 * Pump E  | Cold Brew
 * Pump F  | Lime Juice
 * Pump G  | Tequila
 * Pump H  | Cointreau
 */

char* drinks[]={"White Russian","Black Russian","White Widow","Morning Joe","Rum and Coke","Margarita","Kamikaze","Acapulco", "Brave Bull", "Huntsman", "7 and 7", "Whiskey & Coke"};
char* postStep[]={  "1oz Cream",             "",           "",  "1oz Cream",        "Coke","0.5oz Syrup",      "","1tsp Sugar", "Lime Slice", "0.5oz Syrup", "7up",          "Coke"};
int pumpATime[]={             2,              2,            2,            4,             0,          0,         0,         0,            3,          0,         0,                0}; // number of hlfOZTime to run pumpA for each drink
int pumpBTime[]={             3,              3,            0,            2,             0,          0,         2,         0,            0,          3,         0,                0};
int pumpCTime[]={             0,              0,            2,            0,             4,          0,         0,         3,            0,          1,         0,                0};
int pumpDTime[]={             0,              0,            0,            0,             0,          0,         0,         0,            0,          0,         3,                4};
int pumpETime[]={             2,              2,            0,            8,             0,          0,         0,         0,            0,          0,         0,                0};
int pumpFTime[]={             0,              0,            0,            0,             1,          2,         2,         1,            0,          2,         0,                0};
int pumpGTime[]={             0,              0,            2,            0,             0,          4,         0,         0,            3,          0,         0,                0};
int pumpHTime[]={             0,              0,            2,            0,             0,          2,         2,         1,            0,          0,         0,                0};          
int first = 0;
const int maxPerPage = 5; // max index per page (ie 5 per page)
int curSelected = 0;

void pour()
{
   u8x8.clear();
   u8x8.draw2x2String(0,0,"Pouring:");
   u8x8.drawString(0,4,drinks[curSelected]);
   u8x8.refreshDisplay();
   if (pumpATime[curSelected] > 0) pump(pumpApin, hlfOZTime * pumpATime[curSelected]);
   if (pumpBTime[curSelected] > 0) pump(pumpBpin, hlfOZTime * pumpBTime[curSelected]);
   if (pumpCTime[curSelected] > 0) pump(pumpCpin, hlfOZTime * pumpCTime[curSelected]);
   if (pumpDTime[curSelected] > 0) pump(pumpDpin, hlfOZTime * pumpDTime[curSelected]);
   if (pumpETime[curSelected] > 0) pump(pumpEpin, hlfOZTime * pumpETime[curSelected]);
   if (pumpFTime[curSelected] > 0) pump(pumpFpin, hlfOZTime * pumpFTime[curSelected]);
   if (pumpGTime[curSelected] > 0) pump(pumpGpin, hlfOZTime * pumpGTime[curSelected]);
   if (pumpHTime[curSelected] > 0) pump(pumpHpin, hlfOZTime * pumpHTime[curSelected]);
   
   if (postStep[curSelected] != "")
   {
     u8x8.clear();
     u8x8.draw2x2String(4,0,"Add:");
     u8x8.drawString(0,4,postStep[curSelected]);
     u8x8.refreshDisplay();
   }
   delay(5000); // make sure the dripping is done
   u8x8.clear();
   u8x8.draw2x2String(2,2,"Enjoy!");
   u8x8.refreshDisplay();

   delay(5000);
}
void loop(void)
{
  lcount--;
  
   // handel menu button
  if (btnBothState == 2)
  {
    btnBothState = 0;
    btn1State = 0;
    btn2State = 0;
    mnu = SERVICE_MENU;
    u8x8.clear();
    u8x8.refreshDisplay();
  }
  // exit service menu
  if (mnu == SERVICE_MENU && btn1State == 1) { 
    mnu = ROOT_MENU; // go to next menu
    btn1State = 0;
    u8x8.clear();
    u8x8.refreshDisplay();
  }
  if (btn1State == 1 && mnu == ROOT_MENU)
  {
    // move selection:
    curSelected++;
    if (curSelected >= numDrinks) curSelected =0;    
    btn1State = 0;
  }
  if (btn2State == 1 && mnu == ROOT_MENU)
  {
    // pour requested:
    pour();
    btn2State = 0;
    u8x8.clear();
    u8x8.refreshDisplay();
  }
  int ofirst = first;
  first = (curSelected / maxPerPage) * maxPerPage; // select the correct page
  if (first != ofirst) {
    // page change:
    u8x8.clear();
    u8x8.refreshDisplay();
  }
  if (lcount < 1) {
    lcount = maxCnt;  
    switch (mnu) {
      case ROOT_MENU:
        //u8x8.setFont( u8x8_font_5x7_f);
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        for (int i = first; i < numDrinks; i++)
        {
          if ((i - first) >= maxPerPage) 
          {
            break; // only draw max 
          }
          u8x8.drawString(1,(i-first),drinks[i]);
          if (i == curSelected) u8x8.drawString(0,(i - first),"*");
          else u8x8.drawString(0,(i - first)," ");
        }
        // mark selected
        
        u8x8.drawString(0,7,"DOWN      SELECT");
        u8x8.refreshDisplay();
        break;
        
      case SERVICE_MENU:
        // prime pump?
        u8x8.setFont(u8x8_font_amstrad_cpc_extended_f);
        u8x8.drawString(0,2,"Push Yellow");
        u8x8.drawString(0,5,"To Prime");
        u8x8.refreshDisplay();
        if (btn2State == 1){
          prime();
          btn2State = 0;
          mnu = 0;
          u8x8.clear();
          u8x8.refreshDisplay();
        }
        break;
    }
  }
   // read the state of the switch into a local variable:
  button1.check();
  button2.check();
  delay(10);
}

void handleBtnEvent(AceButton* button, uint8_t eventType, uint8_t /* buttonState */) {
  uint8_t pin = button->getPin();
  switch (eventType) {
    /*case AceButton::kEventClicked:
      if (pin == btn1Pin)
        btn1State = 1;
      else
        btn2State = 1;
      break;*/
    case AceButton::kEventPressed:
      btnBothState += 1;
      break;
    case AceButton::kEventReleased:
      btnBothState = 0; // reset 
      //if (btnBothState != 1) { 
        if (pin == btn1Pin)
          btn1State = 1;
        else
          btn2State = 1;
      //}
      if (btn1State == 1 && btn2State == 1) {
        btnBothState = 1;
        btn1State = 0;
        btn2State = 0;
      }
      break;
  }
}
