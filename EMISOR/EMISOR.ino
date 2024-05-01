
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 23        // Pin del DHT11 conectado al pin 23 del ESP32
#define DHTTYPE DHT11   // Tipo de sensor DHT (DHT11)

DHT_Unified dht(DHTPIN, DHTTYPE);

String lora_band = "915000000"; // Frecuencia de operación de la banda LoRa (Hz), depende del país/región
String lora_networkid = "18";   // Identificación de la red LoRa
String lora_address = "1";      // Dirección del módulo LoRa transmisor
String lora_RX_address = "2";   // Dirección del módulo LoRa receptor

void setup() {
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // Inicialización del puerto serie 2 (pin 16 para RXD, 17 para TXD UART del ESP32) a 115200 baudios
  
  dht.begin(); // Inicialización del sensor DHT11
  delay(1000); // Esperar 1 segundo a que el sensor se inicialice

  delay(1000);
  // Configuración de parámetros para el módulo LoRa a través del puerto serie 2
  Serial2.println("AT+BAND=" + lora_band); // Configurar la banda de frecuencia
  delay(500);
  Serial2.println("AT+ADDRESS=" + lora_address); // Configurar la dirección del módulo LoRa
  delay(500);
  Serial2.println("AT+NETWORKID=" + lora_networkid); // Configurar la identificación de la red LoRa
  delay(1000);
}

void loop() {
  // Lectura de temperatura y humedad del sensor DHT11
  sensors_event_t event;
  dht.temperature().getEvent(&event); // Obtener la temperatura del evento
  float temperature = event.temperature; // Almacenar la temperatura en una variable
  dht.humidity().getEvent(&event); // Obtener la humedad del evento
  float humidity = event.relative_humidity; // Almacenar la humedad en una variable

  // Enviar datos de temperatura y humedad a través de LoRa
  String data = "Temperatura:" + String(temperature) + "C,Humedad:" + String(humidity) + "%"; // Crear una cadena con los datos
  Serial2.print("AT+SEND=" + lora_RX_address + ","); // Comando para enviar datos a través de LoRa
  Serial2.print(data.length()); // Enviar la longitud de la cadena de datos
  Serial2.println("," + data); // Enviar los datos a través del puerto serie 2

  delay(500); // Esperar 0.5 segundos antes de enviar datos nuevamente
}
