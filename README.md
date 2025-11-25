# Heltec WiFi LoRa 32 V3 - LoRaWAN Basic Sketch

Este sketch proporciona una conexión básica y funcional a **The Things Network (TTN)** utilizando el protocolo LoRaWAN OTAA para la placa **Heltec WiFi LoRa 32 V3**.

Incluye correcciones críticas para evitar reinicios constantes ("Guru Meditation Error") y asegurar que la radio funcione correctamente.

## �️ Requisitos Previos

### Hardware
*   **Placa**: Heltec WiFi LoRa 32 V3 (ESP32-S3 + SX1262)
*   **Antena**: Antena LoRa (868/915 MHz según región) conectada al puerto IPEX

### Software (Librerías y Board Manager)
1.  **Arduino IDE**: Versión 1.8.19 o superior (recomendado 2.0+).
2.  **Gestor de Placas (Board Manager)**:
    *   Paquete a instalar: **esp32** de **Espressif Systems**.
    *   Versión: **3.3.4** (o superior).
3.  **Librería LoRaWAN**:
    *   Este código utiliza la librería nativa `LoRaWan_APP.h` que viene incluida con las definiciones de placa Heltec dentro del paquete esp32. Asegúrate de seleccionar la placa correcta (**Heltec WiFi LoRa 32(V3)**).

## �📋 Cómo configurar para tu dispositivo

Para usar este código en otro dispositivo o con otra aplicación de TTN, debes modificar las siguientes líneas en el archivo `.ino`:

### 1. Credenciales OTAA (Recomendado)
Busca esta sección al principio del código y reemplaza los valores con los de tu consola TTN:

```cpp
/* OTAA PARA HELTEC ESP32 V3 */
// DevEUI (MSB) - Copiar de TTN -> Device -> General Settings -> DevEUI
uint8_t devEui[] = { 0x70, 0xB3, ... }; 

// AppEUI (MSB) - Copiar de TTN (Suele ser todo ceros o JoinEUI)
uint8_t appEui[] = { 0x00, 0x00, ... }; 

// AppKey (MSB) - Copiar de TTN -> Device -> General Settings -> AppKey
uint8_t appKey[] = { 0x01, 0x2F, ... }; 
```

**Nota:** Asegúrate de copiar los valores en formato de array de bytes (ej: `0xAB, 0xCD...`).

### 2. Configuración de Región
Si no estás en Europa (EU868), busca esta línea y cámbiala:

```cpp
/* LoRaWAN Region and Class */
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION; // Se define en el menú de Arduino IDE
```
Asegúrate de seleccionar la región correcta en el menú **Herramientas -> LoRaWan Region** del Arduino IDE.

## ⚠️ Notas Importantes (No borrar)

Este código incluye dos correcciones críticas para la placa Heltec V3. **No elimines estas líneas**:

1.  **Activación de Vext**:
    ```cpp
    pinMode(36, OUTPUT);
    digitalWrite(36, LOW); 
    ```
    Esto enciende la alimentación de la pantalla OLED y la radio LoRa. Sin esto, el dispositivo no transmitirá.

2.  **Retraso de Seguridad**:
    ```cpp
    delay(3000); 
    Mcu.begin(HELTEC_BOARD, SLOW_CLK_TPYE);
    ```
    Este retraso de 3 segundos al inicio es vital para evitar que la placa entre en un bucle de reinicios (Crash Loop) al inicializar el hardware.

## 🚀 Cómo subir el código

1.  Instala la librería **"Heltec ESP32 Dev-Boards"**.
2.  Selecciona la placa: **Heltec WiFi LoRa 32(V3)**.
3.  Configura en Herramientas:
    *   **LoRaWan Region**: EU868 (o tu región).
    *   **LoRaWan Debug Level**: None (o Freq para ver frecuencias).
4.  Conecta la placa y sube el sketch.
