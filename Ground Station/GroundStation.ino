
#include <LiquidCrystal.h>
#include <Keypad.h> ;

void show_waiting_message() ;
void Show_Ping_message() ;
void Show_telemtry_Value() ;
void Request_Command_From_USer() ; 


#define Send_Command  10
#define GET_Telemetry 11
#define PING_DONE     12

    int Telemtry_Buffer[4];  
String Command_From_COM_STR ;
int Command_From_COM = 0 ; 
char Request_flag     = 0  ; 


const byte ROWS = 4; // rows
const byte COLS = 4; // columns
//define the symbols on the buttons of the keypads
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {A2, A3, A4, A5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 9, 10, 11}; //connect to the column pinouts of the keypad

String inputString;
long inputInt;

String stri  ; 

const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

 int Command[5] = {0,} ;   




void setup() {

  // set up the LCD's number of columns and rows:
   inputString.reserve(10);
  lcd.begin(16, 2); 
  Serial.begin(9600) ; 
  // Print a message to the LCD.
  lcd.print("hello, world!");
  delay(2000) ;
  lcd.clear() ;  
  lcd.print("IM Cube (*_*)!");
  delay(2000) ; 
  lcd.clear() ; 
  lcd.print("Enter command");
  delay(2000) ; 
  lcd.clear() ; 
  

 Request_Command_From_USer() ;

delay(2000) ; 
  lcd.clear() ;
  lcd.print("Wating for ping ") ; 
  delay(2000) ; 
 
  
}

void loop() {


if(Serial.available()>0) 
{
  Command_From_COM_STR = Serial.readString()  ; 
  Command_From_COM = Command_From_COM_STR.toInt() ; 
  Serial.println(Command_From_COM,DEC)  ;
}


if (Command_From_COM == PING_DONE) 
{
  Show_Ping_message () ; 
  lcd.clear() ; 
  Command_From_COM = 0 ; 
}

 if (Request_flag == 1)
  {
    Request_Command_From_USer();
    Request_flag = 0; 
}

  if (Command_From_COM == Send_Command)
  { 
    Serial.println("Sending Command") ; 
    for (int i = 0; i < 5; i++)
    {
      Serial.println(Command[i]);
      //Serial.write(Command[i]);
      
    }
    Command_From_COM = 0 ; 
  }

if (Command_From_COM == GET_Telemetry)
{
  for (int i = 0; i < 4; i++)
  { 
  Telemtry_Buffer[i] = (Serial.readString()).toInt() ;
    delay(50);

    
    /*  uncomment for real HW 
    Telemtry_Buffer[i] = Serial.read();
    delay(50);
    */
  }

  Show_telemtry_Value() ;  
  Request_flag =  1 ; 
  Command_From_COM = 0 ;  
 
}



}



void show_waiting_message () 
{
  String stri = "Waiting";
  for (int i = 0; i = 6; i++)
  {
    stri += ".";
    lcd.print(stri);
    delay(500) ; 
  }
}



void Show_Ping_message ()
{
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("NOW im In space");
  delay(5000) ;
  lcd.clear();

}


void Show_telemtry_Value()
{
   
  /*print the temprture value in the screen */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temprture state ");
  lcd.setCursor(0, 1);
  lcd.print("Value : ");
  lcd.setCursor(8, 1);
  lcd.print(Telemtry_Buffer[0]); 
  delay(2000) ;

  /*print the Voltage value in the screen */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Volatege state ");
  lcd.setCursor(0, 1);
  lcd.print("Value : ");
  lcd.setCursor(8, 1);
  lcd.print(Telemtry_Buffer[1]);
  delay(2000);


/* LDR MAX value */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LDR MAX  ");
  lcd.setCursor(0, 1);
  lcd.print("Value : ");
  lcd.setCursor(8, 1);
  lcd.print(Telemtry_Buffer[2]);
  delay(2000);

  /*print the Faluarr value in the screen */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Failure  state ");
  lcd.setCursor(0, 1);
  if (Telemtry_Buffer[3] == 1 ) 
  {
    lcd.print(" Have Failure!! ");
  }
  else 
  {
    lcd.print(" Have NO Failure!! ");
  }
  delay(2000);
}




int GET_PRESSED_VALUE() 
{
char key ; 

   while(key != '#') 
   {
  lcd.setCursor(0, 1);
   lcd.print("Value : ");
   lcd.setCursor(8, 1);

   key = keypad.getKey();

  if (key) {

    if (key >= '0' && key <= '9') {     // only act on numeric keys
      inputString += key;               // append new character to input string
    } else if (key == '#') {
      if (inputString.length() > 0) {
        inputInt = inputString.toInt(); // YOU GOT AN INTEGER NUMBER
        inputString = "";               // clear input
        // DO YOUR WORK HERE
        lcd.print(inputInt);
        delay(1000) ; 

      }
    } else if (key == '*') {
      inputString = "";  
      inputInt = 0 ; 
      lcd.clear() ; // clear input
       lcd.setCursor(0, 1);
   lcd.print("Value : ");
   lcd.setCursor(8, 1); 
    }
  }
   }
   return inputInt ; 
}



void Request_Command_From_USer()  
{   
  lcd.clear() ; 
   lcd.setCursor(0, 0); 
   lcd.print("Run Auto mode "); 
   Command[0] = GET_PRESSED_VALUE()  ; 
   lcd.clear() ; 
   lcd.setCursor(0, 0);
   lcd.print(" PAYLOAD Delay");
   Command[1] = GET_PRESSED_VALUE()  ;
   lcd.clear() ; 
   lcd.setCursor(0, 0); 
   lcd.print("PAYLOAD Duration"); 
   Command[2] = GET_PRESSED_VALUE()  ;
   lcd.clear() ; 
   lcd.setCursor(0, 0); 
   lcd.print("Run Test Mode "); 
   Command[3] = GET_PRESSED_VALUE()  ;
   lcd.clear() ; 
   lcd.setCursor(0, 0); 
   lcd.print("Mode ID "); 
   Command[4] = GET_PRESSED_VALUE()  ;


   lcd.setCursor(0, 0); 
   lcd.clear() ; 
   lcd.print("wait Conection ");  
  lcd.setCursor(0, 1);  
   lcd.print("   To BE in Line  "); 

      lcd.clear() ; 

   
  
   




   
}
