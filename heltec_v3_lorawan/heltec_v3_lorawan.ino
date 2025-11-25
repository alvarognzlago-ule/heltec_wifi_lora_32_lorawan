/*
 * Heltec LoRaWAN Example - Official Structure (Restored)
 * Board: Heltec WiFi LoRa 32 V3
 * Region: EU868
 */

#include "LoRaWan_APP.h"
#include <Arduino.h>

/* OTAA PARA HELTEC ESP32 V3 */
// DevEUI (MSB) - Tal cual TTN: 70B3D57ED0074427
uint8_t devEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x07, 0x44, 0x27 };

// AppEUI (MSB) - Tal cual TTN: 0000000000000000
uint8_t appEui[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// AppKey (MSB) - Tal cual TTN: 012FF11AFC349535D7CD25C31624A866
uint8_t appKey[] = { 0x01, 0x2F, 0xF1, 0x1A, 0xFC, 0x34, 0x95, 0x35, 0xD7, 0xCD, 0x25, 0xC3, 0x16, 0x24, 0xA8, 0x66 };

/* ABP (Requerido por librería) */
uint8_t nwkSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t appSKey[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint32_t devAddr =  (uint32_t)0x00000000;

/* LoRaWAN Channel Mask */
uint16_t userChannelsMask[6] = { 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/* LoRaWAN Region and Class */
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;
DeviceClass_t  loraWanClass = CLASS_A;

/* Settings */
uint32_t appTxDutyCycle = 15000; 
bool overTheAirActivation = true;
bool loraWanAdr = true;
bool isTxConfirmed = true;
uint8_t appPort = 2;
uint8_t confirmedNbTrials = 4;

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
    appDataSize = 4;
    appData[0] = 0x00;
    appData[1] = 0x01;
    appData[2] = 0x02;
    appData[3] = 0x03;
}

void setup() {
  Serial.begin(115200);
  
  // RETRASO DE SEGURIDAD para evitar el crash loop
  delay(3000); 
  Serial.println("Iniciando Heltec WiFi LoRa 32 V3 (Oficial)...");

  // Esta función es necesaria para que la radio funcione bien (RX/TX)
  // El delay anterior debería evitar el "Guru Meditation Error"
  Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);
}

void loop() {
  switch( deviceState )
  {
    case DEVICE_STATE_INIT:
    {
#if(LORAWAN_DEVEUI_AUTO)
      LoRaWAN.generateDeveuiByChipID();
#endif
      LoRaWAN.init(loraWanClass,loraWanRegion);
      LoRaWAN.setDefaultDR(3);
      break;
    }
    case DEVICE_STATE_JOIN:
    {
      LoRaWAN.join();
      break;
    }
    case DEVICE_STATE_SEND:
    {
      prepareTxFrame( appPort );
      LoRaWAN.send();
      deviceState = DEVICE_STATE_CYCLE;
      break;
    }
    case DEVICE_STATE_CYCLE:
    {
      txDutyCycleTime = appTxDutyCycle + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
      LoRaWAN.cycle(txDutyCycleTime);
      deviceState = DEVICE_STATE_SLEEP;
      break;
    }
    case DEVICE_STATE_SLEEP:
    {
      LoRaWAN.sleep(loraWanClass);
      break;
    }
    default:
    {
      deviceState = DEVICE_STATE_INIT;
      break;
    }
  }
}
