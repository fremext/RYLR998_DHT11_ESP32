
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String lora_band = "915000000"; // Frecuencia de operación de la banda LoRa (Hz), depende del país/región
String lora_networkid = "18";   // Identificación de la red LoRa
String lora_address = "2";      // Dirección del módulo LoRa

int buzzerPin = 23; // Pin digital del buzzer pasivo

void setup() {
  Wire.begin(21, 22); // Iniciar la comunicación I2C con la pantalla OLB ED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Iniciar la pantalla OLED

  Serial2.begin(115200, SERIAL_8N1, 16, 17); // Inicializar puerto serie 2 (pin 16 RXD, 17 TXD UART del ESP32) a 115200 baudios

  // Configurar parámetros del módulo LoRa a través del puerto serie 2
  delay(1000);
  Serial2.println("AT+BAND=" + lora_band); // Configurar la banda de frecuencia
  delay(500);
  Serial2.println("AT+NETWORKID=" + lora_networkid); // Configurar la identificación de la red LoRa
  delay(500);
  Serial2.println("AT+ADDRESS=" + lora_address); // Configurar la dirección del módulo LoRa
  delay(1000);
}

void loop() {
  if (Serial2.available()) {
    // Leer los datos recibidos a través de LoRa
    String receivedData = Serial2.readStringUntil('\n');
   
    // Buscar la posición de "Temperatura:" y extraer el valor de temperatura
    int tempPos = receivedData.indexOf("Temperatura:");
    int humPos = receivedData.indexOf("Humedad:");
    String temperatura = receivedData.substring(tempPos + 12, humPos - 1); // +12 para omitir "Temperatura: " y -1 para omitir el espacio en blanco antes del valor de humedad
    String humedad = receivedData.substring(humPos + 9); // +9 para omitir "Humedad:"

    display.clearDisplay(); // Limpiar la pantalla antes de mostrar nuevos datos
    display.setTextSize(1);  // Establecer el tamaño del texto
    display.setTextColor(SSD1306_WHITE);  // Establecer el color del texto
    display.setCursor(15, 0);  // Establecer la posición del cursor para imprimir el encabezado
    display.println("DHT11_LoRa_7Km:");  // Imprimir el encabezado
    display.setCursor(30, 15);  // Establecer la posición del cursor para imprimir la etiqueta de temperatura
    display.println("Temperatura");  // Imprimir la etiqueta de temperatura
    display.drawRect(37, 25, 50, 12, SSD1306_WHITE);  // Dibujar un rectángulo para resaltar el valor de temperatura
    display.setCursor(42, 27);  // Establecer la posición del cursor para imprimir el valor de temperatura
    display.println(temperatura + "C");  // Imprimir el valor de temperatura
    display.setCursor(42, 40);  // Establecer la posición del cursor para imprimir la etiqueta de humedad
    display.println("Humedad");  // Imprimir la etiqueta de humedad
    display.drawRect(21, 50, 85, 12, SSD1306_WHITE);  // Dibujar un rectángulo para resaltar el valor de humedad
    display.setCursor(25, 52);  // Establecer la posición del cursor para imprimir el valor de humedad
    display.println(humedad + "%");  // Imprimir el valor de humedad

    // Convertir el valor de temperatura a un número decimal
    float tempValue = temperatura.toFloat();
    // Verificar si la temperatura es mayor o igual a 30.00 °C
    if (tempValue >= 30.00) {
      // Activar el buzzer
      tone(buzzerPin, 1000); // Generar una señal de 1000 Hz en el pin del buzzer
    } else {
      // Desactivar el buzzer
      noTone(buzzerPin); // Detener la señal en el pin del buzzer
    }

    // Mostrar en la pantalla OLED
    display.display();
  }
}
