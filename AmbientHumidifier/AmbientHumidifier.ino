#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include <XBee.h>


#define DHTPIN 5
#define DHTTYPE DHT11 
#define relay 4

SoftwareSerial XBee(2,3);

DHT dht(DHTPIN, DHTTYPE);

float Humid;
float Temp;

float datas[2];

void setup() {
  
  Serial.begin(9600);
  XBee.begin(9600);
  pinMode(relay,OUTPUT);
  dht.begin();

}

void loop() {
  Humid = dht.readHumidity();
  Temp = dht.readTemperature();
  
  Serial.print("Temp: ");
  Serial.println(Temp);
  Serial.print("Humid: ");
  Serial.println(Humid);
 

  XBee.write("d:1");
  XBee.print(Humid);
  
  if(Humid<80){
        digitalWrite(relay,LOW);
    }
    else{
        digitalWrite(relay,HIGH);
    }

  delay(2000);

    
  
  
}
