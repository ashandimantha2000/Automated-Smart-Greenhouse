//Including Libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Defining variables
#define LDRpin A0     //LDR Pin
#define CO2_Sensor A2
                 
int LDRValue = 0;       //LDR
int Thungston_Bulb = 11;   //Thungston Bulb
int CO2_Value;           //CO2 Sensor

//Define screen sizes
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup()
{
  Serial.begin(9600);                 
  pinMode(Thungston_Bulb, OUTPUT);
  pinMode(CO2_Sensor, INPUT);

  //OLED Screen allocation
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  
}

void loop()
{
  //LDR Values assignment
  LDRValue = analogRead(LDRpin);

  //CO2 Values assignment
  CO2_Value = analogRead(CO2_Sensor); 

  //Output to serial monitor     
  //Serial.print("Light Intensity: ");  
  //Serial.print(LDRValue);   
  String LDR_value_str = (String) LDRValue;       

  //Serial.print("CO2 Value: "); 
  //Serial.print(CO2_Value, DEC); 
  String CO2_value_str = (String) CO2_Value; 
  delay(5000);  

  Serial.print("&ldr="+LDR_value_str+"&co2="+CO2_value_str);

  //OLED Output
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.print("Light Intensity:");
  display.println(LDRValue);

  display.print("CO2 Value:");
  display.println(CO2_Value);
  display.display(); 

  //Operations
  //LDT -> LED
  if(LDRValue>500){
    digitalWrite(Thungston_Bulb, HIGH);
  }
  else{
    digitalWrite(Thungston_Bulb, LOW);
  }


}

