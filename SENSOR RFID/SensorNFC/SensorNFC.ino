
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 5
#define RST_PIN 2 //2
#define LED_G 4 //define green LED pin
#define LED_R 15 //define red LED
#define BUZZER 22 //buzzer pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 500
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  //noTone(BUZZER);
  Serial.println("Inserte la tarjeta...");
  Serial.println();

}
void loop() 
{
  // Esperando tarjeta
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Leer tarjeta
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Imprimir código NFC
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "04 DB 28 8A 95 61 84") //change here the UID of the card/cards that you want to give access
  {
    Serial.println("Acceso permitido");
    Serial.println();
    delay(500);
    digitalWrite(LED_G, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(LED_G, LOW);
  }
 
 else   {
    Serial.println(" Acceso denegado");
    digitalWrite(LED_R, HIGH);
    digitalWrite(BUZZER, HIGH);
    //tone(BUZZER, 300);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    digitalWrite(BUZZER, LOW);
    //noTone(BUZZER);
  }
}
