#include <SPI.h> //SPI kütüphanesini ekliyoruz.
#include <SoftwareSerial.h>
#include <XBee.h>
#include <MFRC522.h> //RFID modülünü kullanmak için gerekli kütüphaneyi ekliyoruz.


int RST_PIN = 9; //RC522 modülü reset pinini tanımlıyoruz.
int SS_PIN = 10; //RC522 modülü chip select pinini tanımlıyoruz.
bool hareket = false; //hareket adlı bir değişken oluşturup değerini false olarak ayarlıyoruz.
bool aktif = false; //aktif adlı bir değişken oluşturup değerini false olarak ayarlıyoruz.
float bekleme1 = 10; //1. bekleme süresini belirlediğimiz bekleme1 değişkenini oluşturup değerini 10 olarak ayarlıyoruz.
int bekleme2 = 10; //2. bekleme süresini belirlediğimiz bekleme2 değişkenini oluşturup değerini 10 olarak ayarlıyoruz.
float sayac = bekleme2; //sayac değişkenini oluşturup değerini bekleme2'nin değerine eşitliyoruz.
bool isOpened=false;

SoftwareSerial XBee(2,3);

MFRC522 rfid(SS_PIN, RST_PIN); //RC522 modülü ayarlarını yapıyoruz.
byte ID[4] = {
  229,
  3,
  207,
  35
}; //Yetkili kart ID'sini tanımlıyoruz.

void setup() {

  Serial.begin(9600); //Seri haberleşmeyi başlatıyoruz.
  XBee.begin(9600);
  SPI.begin(); //SPI iletişimini başlatıyoruz.
  rfid.PCD_Init(); //RC522 modülünü başlatıyoruz.
  pinMode(8, INPUT); //PIR hareket sensörünü bağlamış olduğumuz 8 numaralı pini giriş pini olarak ayarlıyoruz.
  pinMode(7, OUTPUT); //Buzzerı bağlamış olduğumuz 7 numaralı pini çıkış pini olarak ayarlıyoruz.
  pinMode(6, INPUT); //Butonu bağlamış olduğumuz 6 numaralı pini giriş pini olarak ayarlıyoruz.
}

void loop() {
  if (digitalRead(6) == HIGH) { //Butona basıldığında
    digitalWrite(7, HIGH);
    delay(100);
    digitalWrite(7, LOW); //Buzzerı 100 ms liyeniğine öttürüyoruz.
    delay(bekleme1 * 1000); //Alarmı aktifleştirmeden önce bekleme1 süresince bekliyoruz.
    aktif = true; // Alarmı aktifleştiriyoruz.
  }
  if (digitalRead(8) == HIGH && aktif == true) { //Hareket algılanırsa ve alarm aktifse
    digitalWrite(7, HIGH);
    delay(100);
    digitalWrite(7, LOW); //Buzzerı 100 ms liyeniğine öttürüyoruz.
    hareket = true; //hareket değişkenini True olarak değiştiriyoruz.
    while (hareket) { //hareket değişkeni True olduğu sürece
      if (sayac <= 0) {
        digitalWrite(7, HIGH); //Süre biterse buzzerın ötmeisni sağlıyoruz.
      } else { //Süre bitmediyse
        delay(100); // 0.1 saniye bekliyoruz. 
        sayac -= 0.1; //sayacı 0.1 azaltıyoruz.
      }
      Serial.println(sayac);
      if (rfid.PICC_IsNewCardPresent()) { //Okuyucuya bir kart yaklaştrılmışsa

        if (rfid.PICC_ReadCardSerial()) { //Kartın ID'si okunuyorsa

          if (rfid.uid.uidByte[0] == ID[0] &&
            rfid.uid.uidByte[1] == ID[1] &&
            rfid.uid.uidByte[2] == ID[2] &&
            rfid.uid.uidByte[3] == ID[3]) { //Kartın ID'sini yetkili ID ile karşılaştırıyoruz.
            hareket = false;
            aktif = false;
            digitalWrite(7, HIGH);
            delay(100);
            digitalWrite(7, LOW); //hareket ve aktif değişkenlerini false olarak ayarlıyoruz ve buzzerı 100 ms liyeniğine çalıştırıyoruz.
            Serial.println(rfid.uid.uidByte[0]);
            isOpened=true;
            break;
          }
        }else{
          isOpened=false;
          break;
          
          }
      }
      rfid.PICC_HaltA(); //Aynı kartın tekrar tekrar okunmasını engellemek için bu komutu yazıyoruz.
    }
   
    
    digitalWrite(7, LOW); //hareket ve aktif değişkeni artık True olmayınca buzzerı kapatıyoruz.
    sayac = bekleme2; //sayacı tekrardan baştaki süreye getiriyoruz.
  }

  XBee.write("d:2");
  switch(isOpened){
    case true:
    XBee.write('o');
    break;
    case false:
    XBee.write('c');
    break;
    }

    delay(5000);
  
}
