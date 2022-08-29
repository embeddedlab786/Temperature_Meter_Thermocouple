#include <LiquidCrystal.h> //Libraries
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //Arduino pins to lcd

#define SO  A0
#define CS  A1
#define SCK A2

#define G_led 8 
#define R_led 9 
#define buzzer 13

float temp_c, temp_f;
  
void setup(){

pinMode(R_led,OUTPUT); // declare Red LED as output
pinMode(G_led,OUTPUT); // declare Green LED as output
pinMode(buzzer,OUTPUT); // declare Buzzer as output 

lcd.begin(16, 2); // Configura lcd numero columnas y filas
lcd.clear();
lcd.setCursor (0,0);
lcd.print("   Welcome To   ");
lcd.setCursor (0,1);
lcd.print("  Thermocouple  "); 
delay(2000);
lcd.clear();
}
 
void loop(){
lcd.setCursor(0,0);
lcd.print("  Temperature   ");
temp_c = Thermocouple_read();
if(isnan(temp_c)){
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Connect");
lcd.setCursor(0,1);
lcd.print("Thermocouple");
delay(1000);
lcd.clear();
loop();
}
temp_f = (temp_c * 9.0)/ 5.0 + 32.0; //convert Celcius to Fahrenheit

lcd.setCursor(0,1);
lcd.print(temp_c,1);
lcd.write(0xdf); // to display °
lcd.print("C  ");

lcd.setCursor(9,1);
lcd.print(temp_f,1); 
lcd.write(0xdf); // to display °
lcd.print("F  ");

    
if(temp_c>150){ 
digitalWrite(buzzer, HIGH);
digitalWrite(G_led, LOW); // Turn LED off.   
digitalWrite(R_led, HIGH);  // Turn LED on.
delay(300);
}else{  
digitalWrite(G_led, HIGH); // Turn LED on.  
digitalWrite(R_led, LOW);  // Turn LED off.
}
  
digitalWrite(buzzer, LOW);
delay(500); 
}

double Thermocouple_read() {
uint16_t v_out;

pinMode(CS, OUTPUT);
pinMode(SO, INPUT);
pinMode(SCK, OUTPUT);
 
digitalWrite(CS, LOW);
delay(1); 
v_out = shiftIn(SO, SCK, MSBFIRST);
v_out <<= 8;
v_out |= shiftIn(SO, SCK, MSBFIRST);
 
digitalWrite(CS, HIGH);
if (v_out & 0x4) { 
//Thermocouple is disconnected
return NAN; 
}
 
// The lower three bits (0,1,2) are discarded status bits
v_out >>= 3;
// The remaining bits are the number of 0.25 degree (C) counts
return v_out*0.2529;
}
