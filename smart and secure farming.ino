#include <common.h>
#include <Firebase.h>
#include <FirebaseESP8266.h>
#include <FirebaseFS.h>
#include <Utils.h>



#include <ESP8266Firebase.h>
//FirebaseESP8266.h must be included before ESP8266WiFi.h

//#include <FirebaseESP8266.h>  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <DHT.h> //install DHT Library and Adafruit Unified Sensor Library


#define FIREBASE_HOST "smart-iot-farm-7caf7-default-rtdb.firebaseio.com" 
#define WIFI_SSID "Aditi" // Change the name of your WIFI
#define WIFI_PASSWORD "Aditiaditi890" // Change the password of your WIFI
#define FIREBASE_AUTH ""  
 
#define DHTPIN D3     // Connect Data pin of DHT to D2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
//Define FirebaseESP8266 data object
 FirebaseData firebaseData;
 FirebaseJson json;

int Gas_Buzzer = D4;        // used for ESP82
int Gas_analog = A0;        // used for ESP82
int Motorpin= D5;           // Dry Indicator at Digital pin D5
int sense_Pin = D0;         // sensor input at soil moisture pin d0 
int value = 0;
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd (0x27,16,2);    //(Address,16 columns,2 rows)
 




void setup() {
  Serial.begin(9600);
  dht.begin();       //dht begin
  lcd.init();           //initiliaze lcd
  lcd.backlight();
  Serial.println("Humidity|Temperature|Gas|Soil Moisture /n/n");
  pinMode(Gas_Buzzer, OUTPUT);  
  pinMode(Motorpin, OUTPUT);    
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin (FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void loop() 
{
    float gassensorAnalog = analogRead(Gas_analog);
    float h = dht.readHumidity();
    float t = dht.readTemperature();   
    float value = digitalRead(sense_Pin);       
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    Serial.print("Gas Sensor: ");
    Serial.print(gassensorAnalog);
    Serial.print("\t");  
  if (gassensorAnalog >400) {
    Serial.println("Gas");
       lcd.setCursor(8,1);
       lcd.print("G=  Gas");
       digitalWrite (Gas_Buzzer, HIGH) ; //send tone
       delay(1000);
       digitalWrite (Gas_Buzzer, LOW) ;  //no tone
  }
  else {
       Serial.println("No Gas");
       lcd.setCursor(8,1);
       lcd.print("G=NoGas"); 
  }
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
  if(value==0)
  {
            digitalWrite(Motorpin, LOW);
            Serial.print("wet");
            lcd.setCursor(0,1);
            lcd.print("S=Wet");
        }
  else
       {
           digitalWrite(Motorpin,HIGH);
           Serial.print("dry");
           lcd.setCursor(0,1);
           lcd.print("S=Dry");
          
       }
       delay(1000);
       digitalWrite(Motorpin,LOW);     
       lcd.setCursor(0,0);
       lcd.print("T=");
       lcd.print(t);
       lcd.setCursor(8,0);
       lcd.print("H=");
       lcd.print(h);  
       delay(1000);
       /////////////////////////////////////////////////////
     {
        sensorUpdate();
        delay(100);
     }    
}
void sensorUpdate()
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);


   float gassensorAnalog = analogRead(Gas_analog);
  
    float value= digitalRead(sense_Pin);  

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("C  ,"));
  Serial.print(f);
  Serial.println(F("F  "));

  if (Firebase.setFloat(firebaseData,"/temperature", t))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (Firebase.setFloat(firebaseData,"/humidity", h))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  } 

  if (Firebase.setFloat(firebaseData,"/Gas",gassensorAnalog))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }



   if (Firebase.setFloat(firebaseData,"/Soil", value))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }


}

