#include <WiFi.h> 
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/********************Parametros de conexion Wifi*******************************/

#define WLAN_SSID       "Totalplay-5BA2"  // Ingresa el nombre de tu red         
#define WLAN_PASS       "5BA2A07B7xkB8p2w"  // Ingresa la contraseña de tu red 
/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883 // Use 8883 para SSL
#define AIO_USERNAME "CarlosLpz" // Reemplace con su nombre de usuario
#define AIO_KEY "aio_hYoR31zPLFG6x8Sv1xZ5cHl4DeNl" // Reemplace con su Clave de autenticación
/************ Estado global (¡no necesita cambiar esto!) *********************/

// Crea una clase WiFiClient para conectarse al servidor MQTT
WiFiClient client;
// o ... use WiFiFlientSecure para SSL
//WiFiClientSecure client;

// Configuración cliente MQTT pasando, cliente WiFi y servidor MQTT y los detalles de inicio de sesión.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/***************** Feeds que se definieron en Adafruit IO *******************/

// Configuracion para publicar los feeds
Adafruit_MQTT_Publish codigosnfc = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME"/feeds/codigosnfc"); 

/****************************************************************************/

void MQTT_connect();

void setup()
{
  Serial.begin(115200);
  delay(10);

/*************************Conexión a la red wifi**************************/

Serial.println();
Serial.print("Connecting to ");
Serial.println(WLAN_SSID);

WiFi.begin(WLAN_SSID, WLAN_PASS);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println();
Serial.println("WiFi conectado");
Serial.println("IP address: "); 
Serial.println(WiFi.localIP());
}
/****************************************************************************/

void loop()
{

MQTT_connect();

String info = "ejemplo";
char Buf[50];
info.toCharArray(Buf, 50);
//float tem = 5.1; //Se lee la temperatura 

Serial.print("Enviando info");

if (! codigosnfc.publish(Buf)) {
Serial.println("No se envió");
} else {
Serial.println("Se envió");
}

 delay(2000);
}

void MQTT_connect() {
int8_t ret;
// Detener si ya está conectado
if (mqtt.connected()) {
return;
}

Serial.print("Conectando a MQTT... ");

uint8_t retries = 3;

while ((ret = mqtt.connect()) != 0) { // connect devolverá 0 para conectado
Serial.println(mqtt.connectErrorString(ret));
Serial.println("Re-intentando la conexión MQTT en 5 segundos...");
mqtt.disconnect();
delay(5000); // espera 5 segundos
retries--;
if (retries == 0) {
// La conexión falla y espera a que se reinicie
while (1);
}
}
Serial.println("MQTT Conectado!"); 
}
