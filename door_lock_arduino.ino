#include <SPI.h>
#include <LiquidCrystal.h>
char buff [255];
volatile byte indx;
volatile boolean process;

//// Define LCD pins
//const int rs = 5, en = 2, d4 = 7, d5 = 6, d6 = 4, d7 = 3;
//LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

 
void setup (void) {
   Serial.begin (115200);

//  // Initialize LCD
//  lcd.begin(16, 2); // 16 columns, 2 rows
//  lcd.print("Waiting for SPI...");
   
   pinMode(MISO, OUTPUT); // have to send on master in so it set as output
   SPCR |= _BV(SPE); // turn on SPI in slave mode
   indx = 0; // buffer empty
   process = false;
   SPI.attachInterrupt(); // turn on interrupt
}
 
ISR (SPI_STC_vect) // SPI interrupt routine 
{ 
   byte c = SPDR; // read byte from SPI Data Register
   
   if (indx < sizeof(buff)) {
      buff[indx++] = c; // save data in the next index in the array buff
      if (c == '\n') { 
        digitalWrite(7, HIGH);
        buff[indx - 1] = 0; // replace newline ('\n') with end of string (0)
        
        process = true;
      }
   }   
}
 
void loop (void) {
//    int timeOfDay = 0; // Flag to track if it's day, 11 for night 12 for day
//    boolean isDay = false;
//    char msg = 'a'; //for communication
//    int lastDay = 0; 
   if (process) {
      
      process = false; //reset the process
      digitalWrite(7,LOW);
      Serial.println(buff); //print the array on serial monitor

      //char msg = buff[0];
      //timeOfDay = (int)buff[1]; //Or convert to an int

      
//      if(msg == 'T'){// Check for "day" or "night"
//        if (timeOfDay == 49) {//49 is day, 48 is night
//        isDay = true;          // Allow password entry
//
//        lcd.setCursor(0, 0);
//        lcd.print("It is day time,");
//        lcd.setCursor(0, 1);
//        lcd.print("Unlocked");
//
//        lastDay++;
//        
//        } else /*if (buff == "night")*/ {
//        isDay = false;         // Disable password entry
//        lastDay = 0; //reset hold
//
//        lcd.setCursor(0, 0);
//        lcd.print("It is night time,");
//        lcd.setCursor(0, 1);
//        lcd.print("Locked"); 
//
//        //no other action at night time
//        }
//      //Serial.println(isDay);
//        
//       }
//
//       if(lastDay > 1){
//          if(msg == 'U'){
//            lcd.clear();
//            lcd.setCursor(0, 0);
//            lcd.print("Please Enter PWD");
//          }else if(msg == 'W'){
//            lcd.clear();
//            lcd.setCursor(0, 0);
//            lcd.print("Wrong PassWord");
//          }else if (msg == 'R'){
//            char temp = buff[1];
//            lcd.setCursor(0, 1);
//            lcd.print(temp);
//          }else if(msg == 'E'){
//            lcd.clear();
//            lcd.setCursor(0, 0);
//            lcd.print("Session Ended");
//          }else{
//            lcd.clear();
//            lcd.setCursor(0, 0);
//            lcd.print("Please Enter PWD");
//          }
//       }


      indx= 0; //reset button to zero
   }

}
