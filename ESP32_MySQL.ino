//including libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Adafruit_GFX.h>  //Libraries for OLED Display
#include <Adafruit_SSD1306.h>
#include <OneWire.h>
#include <DallasTemperature.h>

//Wifi Credentials declaration
const char ssid[] = "My Dialog 4G";
const char password[] = "aiME7gY9";

//Hostname and php declaration
String HOST_NAME = "http://192.168.8.102"; // REPLACE WITH YOUR PC's IP ADDRESS
String PHP_FILE_NAME   = "/insert_temperature.php";  //REPLACE WITH YOUR PHP FILE NAME
// String tempQuery = "?temperature=31.0";


//defining variables
#define DHTPIN 15         // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22    // DHT 22
#define DHTPIN_OUT 27     // Digital pin connected to the DHT sensor (External)
#define WaterTemp_PIN  5    //Water Temperature
#define RXp2 16           //RX
#define TXp2 17           //TX
int LED_WIFI = 4;          //Wifi LED
int Thungston_LED = 14; //Thungston bulb relay
float temp;                       //temperature (in)
float humidity;                   //humidity (in)
float temp_out;                   //temperature (out)
float humidity_out;               //humidity (out)
int CO2_sensorValue;    //Assigning initial value to CO2
float WaterTemp; // temperature in Celsius
String Data_Uno; //   String to store data from Uno

//Define screen variables
#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

//assiging to DHT
DHT dht(DHTPIN, DHTTYPE);
DHT dht_out(DHTPIN_OUT, DHTTYPE);

////assiging to Water Temp
OneWire oneWire(WaterTemp_PIN);
DallasTemperature DS18B20(&oneWire);

//Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  //Defining the I/O (Pins/Serial)
  Serial.begin(115200); 
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);  //Recieve data from Uno
  pinMode(LED_WIFI, OUTPUT);
  pinMode(Thungston_LED, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  dht.begin();
  dht_out.begin();
  DS18B20.begin();
  delay(2000);

  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
}

//Custom function to send data from esp32
void sendData(String tempQuery){
    HTTPClient http;
  String server = HOST_NAME + PHP_FILE_NAME + tempQuery;
  http.begin(server); 
 // Serial.print(http.GET());
  int httpCode = http.GET();

  if(httpCode > 0) {
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Data successfuly sent");
      digitalWrite(LED_WIFI, HIGH);
    } else {
      Serial.printf("HTTP GET... code: %d\n", httpCode);
    }
  } else {
    Serial.printf("HTTP GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    digitalWrite(LED_WIFI, LOW); 
  }

  http.end();
  
}

void loop() {

  //Serial data read from Uno
  Data_Uno = Serial2.readString();
  Serial.println(Data_Uno);

  //DHT22 (In)
  temp = dht.readTemperature();
  humidity = dht.readHumidity();

  //DHT22 (Out)
  temp_out = dht_out.readTemperature();
  humidity_out = dht_out.readHumidity();

 
  if (isnan(humidity) || isnan(temp))  // Check if any reads failed and exit early (to try again).
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    //delay(3000);
    return;
  }


  //DS18B20
  DS18B20.requestTemperatures();       // send the command to get temperatures
  WaterTemp = DS18B20.getTempCByIndex(0); 

  //Print in Serial Monitor
  //DHT22 (In) Serial Print
  Serial.print("Temperature (In) : ");
  Serial.print(temp);
  Serial.println("°C");
  Serial.print("Humidity (In) : ");
  Serial.print(humidity);
  Serial.println("%");

  //DHT22 (In) Serial Print
  Serial.print("Temperature (Out) : ");
  Serial.print(temp_out);
  Serial.println("°C");
  Serial.print("Humidity (Out) : ");
  Serial.print(humidity_out);
  Serial.println("%");

  //Water Temperature
  Serial.print("Water Temperature: ");
  Serial.print(WaterTemp);    // print the temperature in °C
  Serial.println("°C");
  delay(2000);

  //Typecasting
  String data = (String) temp;
  String data2 = (String) humidity;
  String data3 = (String) WaterTemp;
  String data4 = (String) temp_out;
  String data5 = (String) humidity_out;
  String tempQuery = "?temperature="+data+"&humidity="+data2+"&WaterTemp="+data3+"&data="+Data_Uno+"&temp_out="+data4+"&humidity_out="+data5;

  // Sending Data
  sendData(tempQuery);
  delay(2000);
  
  //OLED allocation
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }


  //Output to OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 5);

  display.println("Group AZ\n");

  display.print("Temp (In) : ");
  display.print(temp);
  display.println("C");

  display.print("Humidi (In) : ");
  display.print(humidity);
  display.println("%");

  display.print("Temp (Out) : ");
  display.print(temp_out);
  display.println("C");

  display.print("Humidi (Out) : ");
  display.print(humidity_out);
  display.println("%");

  display.print("Water Temp.: ");
  display.print(WaterTemp);
  display.println("C");

  display.display();
  delay(1000);

  //Operations
  if (temp >= 30) {
    Serial.println("Turning on thungston bulb");
    digitalWrite(Thungston_LED, HIGH); // turn the LED on
    delay(500);             // wait for 500 milliseconds
    digitalWrite(Thungston_LED, LOW);  // turn the LED off
    delay(500);
  }
}