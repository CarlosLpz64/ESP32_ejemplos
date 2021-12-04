//si usas esp32
#include <HTTPClient.h>
#include <WiFi.h>

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 5
#define RST_PIN 2 //2
#define LED_G 4 //define green LED pin
#define LED_R 15 //define red LED
#define BUZZER 22 //buzzer pin
#define ACCESS_DELAY 2000
#define DENIED_DELAY 200
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

const char* ssid = "Galaxy A20F6F8";
const char* password =  "jepg3576";
String serverName = "http://192.168.13.205/comprobar/prueba.php";
String msg = "";


void setup() {
  //CONECTAR A WIFI
  delay(10);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.print(".");
  }
  Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());
  
  //CONFIGURAR SENSOR RFID
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.println("Inserte la tarjeta...");
  Serial.println();

}

void loop() {
  //BUSCAR TARJETA
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
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  msg = content.substring(1);
  if (content.substring(1) == "4DB288A956184") //change here the UID of the card/cards that you want to give access
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
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    digitalWrite(BUZZER, LOW);
        delay(DENIED_DELAY);
        digitalWrite(LED_R, HIGH);
    digitalWrite(BUZZER, HIGH);
            delay(DENIED_DELAY);
             digitalWrite(LED_R, LOW);
    digitalWrite(BUZZER, LOW);


    //noTone(BUZZER);
  }
  EnviarDatos();
}

void EnviarDatos () {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      Serial.println("Se mandó: " + msg);
      String serverPath = serverName + "?var1=" + msg;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        Serial.println(serverPath);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    //delay(5000);
}
