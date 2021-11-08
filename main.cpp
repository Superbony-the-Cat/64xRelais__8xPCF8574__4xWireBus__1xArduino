#include <Arduino.h>

#include <Wire.h>    // I2C-Bibliothek einbinden
#define  I2CADRESSE 32 //    "B0100 + 0000"  = oct 40  =  dez. 32
//#define  I2CADRESSE 56 //  "B0111 + 0000"  = oct 70  =  dez  56 a typ
//https://old.circuitmess.com/wp-content/uploads/2017/10/screenshot.99.jpg

//https://youtu.be/-CXSEWlMYFs

//   +  /  5V    Arduino Uno
//   -  /  GND       "
//   SDA / A4        "
//   SCL / A5        "

byte txData = 0;
byte relaisstatus_all = 255;    // hex FF   , bit  1111 1111 = all relais off


byte weg = 0;   // the waytempo switch ( 0 (slow)  or  1 (to fast))
int zeit = 599;   // first time slow (599)

byte canl = 7;  // (0) 8 relais  , (1) 16 relais  ....  (7) 64 relais(max)

////////////////////////////////////////////////////////////////////////////////////////////////////////
void deviceWrite1(byte txData, int ADRESSE) /// relais output PCF8574 /a , Dat + Adress
		{
	Wire.beginTransmission(ADRESSE);
	Wire.write(txData);
	Wire.endTransmission();
}
void setup() {
	Wire.begin();
}
void loop() {
                                                                // channel  0 - 7  p = 0 /7  
	    for (int p = 0; p <= canl ; p++) {
		int I2CADRESSE1 = I2CADRESSE + p;
                byte bit_relais = 1;
		 for (int i = 7; i >= 0; i--) {
		  	relaisstatus_all = bit_relais << i;
			deviceWrite1(~relaisstatus_all, I2CADRESSE1);
			delay(zeit);
		  }
                 
		  deviceWrite1(~0, I2CADRESSE1);                  // new  channel (1-7)

			if ((weg == 0) && (zeit > 10))            // to fast
				     {
					zeit = zeit - zeit / 10;
                                     } else {
					zeit = zeit + zeit / 10;
					weg = 1;
				     }

			if ((weg == 1) && (zeit < 800))           // to slow  (max800)       
				     {
					zeit = zeit + zeit / 10;
				      } else {
					zeit = zeit - zeit / 10;
					weg = 0;
				      }
              } // out for
} //out loop
