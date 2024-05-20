#ifndef TASK_GET_MQTT
#define TASK_GET_MQTT


// Web services
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h> 
#include "config/config.h"
#include "function/mqtt.h"
// #include "function/unified_dimmer.h"

#if defined(ESP32) || defined(ESP32ETH)
// Web services
  #include "WiFi.h"
  #include <AsyncTCP.h>
  #include "HTTPClient.h"
#else
// Web services
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESP8266HTTPClient.h> 
#endif


extern Config config; 
extern System sysvar;
extern HTTPClient http;
extern WiFiClient domotic_client;
// extern bool mqttConnected;
extern int deviceCount; 
extern MQTT device_dimmer_child_mode;

extern MQTT device_dimmer;
extern MQTT device_dimmer_maxtemp;
extern MQTT device_dimmer_maxpow;
extern MQTT device_dimmer_minpow;
extern MQTT device_dimmer_starting_pow;
extern MQTT device_dimmer_maxtemp;
extern MQTT device_dimmer_on_off;
extern MQTT device_dimmer_alarm_temp;
extern MQTT device_dimmer_power;
extern MQTT device_dimmer_send_power; 
extern MQTT device_dimmer_total_power;
extern MQTT device_dimmer_charge1;
extern MQTT device_dimmer_charge2;
extern MQTT device_dimmer_charge3;
extern MQTT device_temp[MAX_DALLAS];
extern MQTT device_relay1;
extern MQTT device_relay2;
extern MQTT device_cooler;
extern MQTT device_dimmer_alarm_temp_clear;

extern bool discovery_temp; 
extern bool alerte; 
extern byte security; // sécurité
extern Logs logging; 
extern String devAddrNames[MAX_DALLAS];
extern espMqttClientAsync client; 



void HA_send_all(){

  if (config.HA) {
    logging.Set_log_init("Send HA values \r\n",true);

    device_dimmer_on_off.send(String(config.dimmer_on_off));     // don't send to restore last value from MQTT
    device_relay1.send(String(sysvar.relay1));
    device_relay2.send(String(sysvar.relay2));
    // TODO : Add option to select value at restart ( ON/OFF/LAST). Maybe do the same for relays.
    // device_dimmer.send(String(sysvar.puissance));
    // device_dimmer_power.send(String(sysvar.puissance* config.charge/100));
    // device_dimmer_send_power.send(String(sysvar.puissance));
    // device_dimmer_total_power.send(String(sysvar.puissance_cumul + (sysvar.puissance * config.charge/100)));
    device_cooler.send(stringBool(sysvar.cooler));
    device_dimmer_starting_pow.send(String(config.startingpow));
    device_dimmer_minpow.send(String(config.minpow));
    device_dimmer_maxpow.send(String(config.maxpow));
    device_dimmer_charge1.send(String(config.charge1));
    device_dimmer_charge2.send(String(config.charge2));
    device_dimmer_charge3.send(String(config.charge3));
    device_dimmer_child_mode.send(String(config.mode));

    if (discovery_temp) {
        device_dimmer_alarm_temp.send(stringBool(sysvar.security));
        device_dimmer_maxtemp.send(String(config.maxtemp)); 
    }
  }

}

#endif