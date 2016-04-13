#include <Adafruit_NeoPixel.h>

#define PIN 9
#define WAIT_BTN 7
#define RCV_BTN  6
#define ACK_BTN  5
#define ERR_BTN  4
#define DENY_BTN 3
#define CDWN_BTN 2
 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(16, PIN);
//Colors and Stuff 
uint8_t  offset    = 0; // Position of spinny eyes
uint32_t bluebrt   = 0x008AC3; 
uint32_t bluemid   = 0x004561;
uint32_t bluedim   = 0x002230;
uint32_t bluedark  = 0x001017;
uint32_t bluenight = 0x000508;
uint32_t fullgreen = 0x00FF00;


  uint8_t waitval = 0;
  uint8_t rcvval  = 0;
  uint8_t ackval  = 0;
  uint8_t errval  = 0;
  uint8_t denyval = 0;
  uint8_t cdwnval = 0;
  
  
  uint8_t btnData = 0;
  uint8_t btnDataPrev = 0;
  uint8_t buttons = 0;


void setup() {
  //InputPinAssigns
  pinMode(WAIT_BTN, INPUT_PULLUP);
  pinMode(RCV_BTN, INPUT_PULLUP);
  pinMode(ACK_BTN, INPUT_PULLUP);
  pinMode(ERR_BTN, INPUT_PULLUP);
  pinMode(DENY_BTN, INPUT_PULLUP);
  pinMode(CDWN_BTN, INPUT_PULLUP);
  //pinMode(WAIT_BTN, INPUT_PULLUP);
  
  Serial.begin(9600);
  pixels.begin();
  pixels.show();
  //pixels.setBrightness(100); // 1/3 brightness
}
 
void loop() {
//Check all Button States
  if(1){
	  readButtonStates();
 }
 else {
 // all pixels off
 }
 switch (buttons) {
	 
	case(1):
	//	pixels.setBrightness(100); // 1/3 brightness
		waitLoop(65);
		break;
		
	case(2):
	//pixels.setBrightness(255); // Set back to full brightness
		waitLoop(25);
		break;
		
    case(4):
		ackAnim();
		break;
	
	case(8):
		errAnim();
		break;
		
}
 
   }


void waitLoop(int speed) {
    uint8_t  i;
  
    for(i=0; i<16; i++){
      uint32_t c = 0;
      uint32_t ctwo = 0;
      if(((offset + i) & 15) < 1) {
        c = bluebrt; // 4 pixels on...
      }
      else if(((offset + i - 1 ) & 15) < 1) {
        c = bluemid;
        }  
      else if(((offset + i - 2 ) & 15) < 1) {
        c = bluedim;
        }
      else if(((offset + i - 3 ) & 15) < 1) {
        c = bluedark;
        }  
      else if(((offset + i - 4 ) & 15) < 1) {
        c = bluenight;
        }
          
        
      pixels.setPixelColor(i, c);

    }   
    pixels.show();
    offset++;
    delay(speed);
  
}

void recvLoop(){}

void ackAnim () {
	
	for (uint8_t i = 0; i < 255; i++){
		for (uint8_t n = 0; n <16; n++){
		pixels.setPixelColor(n,0,i,0);
		}
		pixels.show();
		delay(4);
	}	
	delay (10);
	for (uint8_t i = 255; i > 0; i--){
		for (uint8_t n = 0; n <16; n++){
		pixels.setPixelColor(n,0,i,0);
		}
		pixels.show();
		delay(4);
	}
	buttons = 1;				// Okay, this and the next lines are hacky.
	btnDataPrev = 254;			// Set Variables back so I can trigger same state again
	delay(10);
}
void errAnim () {
	for (int x = 0; x < 4; x++){
		for (uint8_t i = 0; i < 255; i++){
			for (uint8_t n = 0; n <16; n++){
			pixels.setPixelColor(n,i,0,0);
			}
			pixels.show();
		}	
		delay (5);
		for (uint8_t i = 255; i > 0; i--){
			for (uint8_t n = 0; n <16; n++){
			pixels.setPixelColor(n,i,0,0);
			}
			pixels.show();
		}
		delay(5);
	}
	buttons = 1;				// Okay, this and the next lines are hacky.
	btnDataPrev = 254;			// Set Variables back so I can trigger same state again
	delay(10);
	}










/***********************************************************
*                   readButtonStates                       *
***********************************************************/
void readButtonStates(){

  btnData <<= 1;
  btnData += (1);          // Button 7
  btnData <<= 1;
  btnData += (1);          // Button 6
  btnData <<= 1;
  btnData += (digitalRead(CDWN_BTN));          
  btnData <<= 1;      
  btnData += (digitalRead(DENY_BTN));        
  btnData <<= 1;
  btnData += (digitalRead(ERR_BTN));  
  btnData <<= 1;  
  btnData += (digitalRead(ACK_BTN));  
  btnData <<= 1;
  btnData += (digitalRead(RCV_BTN));  
  btnData <<= 1;    
  btnData += (digitalRead(WAIT_BTN));  
 
	if (btnData != btnDataPrev)  {          // only write out to the variable if there is a change//
		if(btnData < 255){
			buttons = ~btnData;       		// 	Inverts the Pulled up values to be low and writes to 
			Serial.println("Data changed");     // buttons variable
			Serial.println(buttons,BIN);
			btnDataPrev = btnData;
			}
	}
btnData = 255;
	}
