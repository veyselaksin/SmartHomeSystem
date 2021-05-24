
#include <FirebaseESP8266.h>
#include <ESP8266WiFi.h>
#include <XBee.h>
#include <SoftwareSerial.h>


//This example shows how to read, store and update database using get, set, push and update functions.


SoftwareSerial XBee(D2,D3);

char data[5];
char str[8];
int i=0;
float temp;
float humid;

float device_1 = 0.00;//defaulf value
char device_2 = 'n';//defaulf value

#define WIFI_SSID "Zyxel_16D1_2.4G"
#define WIFI_PASSWORD "4AM8EH8D7D"

#define FIREBASE_HOST "esp8266-d466d-default-rtdb.firebaseio.com"

/** The database secret is obsoleted, please use other authentication methods, 
 * see examples in the Authentications folder. 
*/
#define FIREBASE_AUTH "BdWSX4a0cRmMdF1Mb4x8nZlj5ogTbRrIsQ5tfsBz"

//Define FirebaseESP32 data object
FirebaseData fbdo;

FirebaseJson json;

void printResult(FirebaseData &data);

void setup() {
  // put your setup code here, to run once:
    Serial.begin(9600);
     XBee.begin(9600);

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

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(fbdo, "tiny");
}

void loop() {
 if (XBee.available() > 0) {
    str[i] = XBee.read();
    Serial.println(str[i]);
    if (i > 2) {
      data[i - 3] = str[i];
    }
    i++;
  }
  if(i==8){
    i=0;
     if (str[2] == '2') {
      device_2  = atof(data);
      Serial.print(device_2);
      Firebase.setDouble(fbdo, "ESP8266_Device/Door/Data", device_2); 
      
    }else if(str[2]=='1'){
        device_1  = atof(data);
        Serial.print(device_1);
        Firebase.setDouble(fbdo, "ESP8266_Device/Humidity/Data", device_1);
      }else{
        Serial.println(device_1);
        Serial.println(device_2);
        }
      
    }
    
}
