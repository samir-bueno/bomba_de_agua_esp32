#include <WiFi.h>
#include <PubSubClient.h>


const int Trigger = 19; //Pin digital 2 para el Trigger del sensor
const int Echo = 22; //Pin digital 3 para el Echo del sensor
const int RELE = 16;

const int ROJO = 32;
const int AMARILLO = 33;
const int VERDE = 25;

// Configuración WiFi
const char* NOMBRE_WIFI = "PEINE-2";
const char* CONTRASENNA_WIFI = "etecPeine2";


// Configuración MQTT
const char* mqtt_server = "10.56.2.15"; // IP del broker MQTT

int umbralOn = 8;
int umbralOff = 12;
bool bombaEncendida = false;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {

pinMode(Trigger, OUTPUT); //pin como salida
pinMode(ROJO, OUTPUT); //pin como salida
pinMode(AMARILLO, OUTPUT); //pin como salida
pinMode(VERDE, OUTPUT); //pin como salida
pinMode(Echo, INPUT); //pin como entrada
pinMode(RELE, OUTPUT);

digitalWrite(Trigger, LOW);//Inicializamos el pin con 0

WiFi.begin(NOMBRE_WIFI, CONTRASENNA_WIFI);
Serial.begin(115200);
Serial.print("Conectando...");


while(WiFi.status() != WL_CONNECTED)
{
delay(1000);
Serial.print(".");
}
//Wifi conectado
Serial.println(".");
Serial.println(WiFi.localIP());
Serial.println("Conectado");


client.setServer(mqtt_server, 1883);
}


void loop() {
// Reconectar MQTT si es necesario
if (!client.connected()) {
while (!client.connected()) {
Serial.print("Conectando al broker MQTT...");
if (client.connect("ESP32Cliente")) {
Serial.println("Conectado");
client.subscribe("tanque/distancia");
client.publish("tanque/distancia", "ESP32 conectado");
} else {
Serial.print("Fallo (rc=");
Serial.print(client.state());
Serial.println("), reintentando en 5 segundos...");
delay(5000);

}
}
}
client.loop();


long t;
long d;

digitalWrite(Trigger, HIGH);
delayMicroseconds(10); //Enviamos un pulso de 10us
digitalWrite(Trigger, LOW);

t = pulseIn(Echo, HIGH); //obtenemos el ancho del pulso
d = t/59; //escalamos el tiempo a una distancia en cm

Serial.print("Distancia: ");
Serial.print(d);
String distanciaStr = String(d);
client.publish("tanque/distancia", distanciaStr.c_str());
Serial.print("cm");
delay(500);
Serial.println();

if (!bombaEncendida && d < umbralOn)
{
bombaEncendida = true;
digitalWrite(RELE, HIGH); // Encender bomba
digitalWrite(VERDE, LOW);
digitalWrite(AMARILLO, LOW);
digitalWrite(ROJO, HIGH); // Indicar que la bomba está encendida
}
else if (bombaEncendida && d >= umbralOff)
{
bombaEncendida = false;
digitalWrite(RELE, LOW); // Apagar bomba
digitalWrite(VERDE, HIGH); // Indicar que la bomba está apagada
digitalWrite(AMARILLO, LOW);
digitalWrite(ROJO, LOW);
}
else if (d >= umbralOn && d < umbralOff)
{
if (bombaEncendida) {
digitalWrite(RELE, HIGH); // Asegurar que la bomba sigue encendida
digitalWrite(VERDE, LOW);
digitalWrite(AMARILLO, HIGH); // Indicador amarillo de que está en rango medio
digitalWrite(ROJO, LOW);
}
else {
digitalWrite(RELE, LOW); // Asegurar que la bomba sigue apagada
digitalWrite(VERDE, HIGH);
digitalWrite(AMARILLO, LOW); // Indicador verde de que está en rango medio
digitalWrite(ROJO, LOW);
}
}
}