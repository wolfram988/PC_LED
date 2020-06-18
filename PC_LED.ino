#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    30
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define MAX_MODES_VALUE 8
CRGB leds[NUM_LEDS];
CRGB currentColor;
CRGB current2Color;
CRGB current3Color;
CRGB current4Color;

#define UPDATES_PER_SECOND 60

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
int BRIGHTNESS = 64;
int CurrentModeIndex = 0;
int mixing = 7;


void setup() {
currentBlending = LINEARBLEND;
currentColor = CRGB::Black;
current2Color = CRGB::Black;
current3Color = CRGB::Black;
current4Color = CRGB::Black;
currentPalette = RainbowStripeColors_p;
delay( 3000 ); // небольшая задержка для того, чтобы цепь «устаканилась» после включения питания
Serial.begin(9600);
Serial1.begin(9600);
FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
FastLED.setBrightness( BRIGHTNESS );
ChangeColorMode("Off");


}

void loop() {
    static uint8_t startIndex = 0;
    startIndex = startIndex + 1; // скорость движения
    
    FillLEDsFromPaletteColors( startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
    if (Serial.available() != 0) {  
    String b = Serial.readString();
    currentColor = strToCRGB(getPart(b,',',4));
    current2Color = strToCRGB(getPart(b,',',5));
    current3Color = strToCRGB(getPart(b,',',6));
    current4Color = strToCRGB(getPart(b,',',7));
    ChangeColorMode(getPart(b,',',0));
    BRIGHTNESS = map(getPart(b,',',1).toInt(),0,100,0,64);
    mixing = getPart(b,',',2).toInt();
    ChangeBlending(getPart(b,',',3));
    FastLED.setBrightness( BRIGHTNESS );
}
}
void ChangeColorMode(String CMode){
  
  if(CMode.equals("Off"))           {fill_solid( currentPalette, 16, CRGB::Black);}
  if(CMode.equals("Lines"))         {SetupStrippedPallete();}
  if(CMode.equals("Solid"))         {fill_solid( currentPalette, 16, currentColor);}
  if(CMode.equals("Rainbow"))       {currentPalette = RainbowColors_p;}
  if(CMode.equals("RainbowStripes")){currentPalette = RainbowStripeColors_p;}
  if(CMode.equals("Ocean"))         {currentPalette = OceanColors_p;}
  if(CMode.equals("Cloud"))         {currentPalette = CloudColors_p;}
  if(CMode.equals("Lava"))          {currentPalette = LavaColors_p;}
  if(CMode.equals("Forest"))        {currentPalette = ForestColors_p;}
  if(CMode.equals("Party"))         {currentPalette = PartyColors_p;}
  if(CMode.equals("CustomPalette")) {SetupBlackAndWhiteStripedPalette();}
  
  }
void ChangeBlending(String blending){
  if(blending.equals("N")){currentBlending = NOBLEND;}
  if(blending.equals("L")){currentBlending = LINEARBLEND;}
 }


void ChangeCurrentBlending(){
  
   if(currentBlending == NOBLEND) {
    currentBlending = LINEARBLEND;
    }
   else {
    currentBlending = NOBLEND;
    }
   
    }

void FillLEDsFromPaletteColors( uint8_t colorIndex)
    {
    uint8_t brightness = 255;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += mixing;}
    }

   void SetupBlackAndWhiteStripedPalette()
{
    // сначала делаем все фрагменты черными...
    fill_solid( currentPalette, 16, CRGB::Black);
    // ...а потом делаем каждый четвертый фрагмент белым:
    currentPalette[0] = currentColor;
    currentPalette[4] = current2Color;
    currentPalette[8] = current3Color;
    currentPalette[12] = current4Color;
    
}
 void SetupStrippedPallete(){
  // сначала делаем все фрагменты черными...
    fill_solid( currentPalette, 16, CRGB::Black);
    // ...а потом делаем каждый четвертый фрагмент белым:
    currentPalette[0] = currentColor;
    currentPalette[4] = currentColor;
    currentPalette[8] = currentColor;
    currentPalette[12] = currentColor;}
    
CRGB strToCRGB(String incoming) //incoming looks like this -> Q:0x00FF00
{
  long color = strtol(incoming.c_str(), NULL, 16);
  CRGB color_ = color;
  return color_;
}
String getPart(String text, char del, int index)
{
  int found = 0;
  int sInd[] = {0, -1};
  int mInd = text.length()-1;

  for(int i=0; i<=mInd && found<=index; i++){
    if(text.charAt(i)==del || i==mInd){
        found++;
        sInd[0] = sInd[1]+1;
        sInd[1] = (i == mInd) ? i+1 : i;
    }
  }

  return found>index ? text.substring(sInd[0], sInd[1]) : "";
}
