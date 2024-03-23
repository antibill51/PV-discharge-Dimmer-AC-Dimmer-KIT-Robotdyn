#ifndef TASK_DALLAS
#define TASK_DALLAS

#ifdef STANDALONE
#include <Pinger.h>
Pinger pinger;
#endif

extern AsyncMqttClient  client;
extern DallasTemperature sensors;
extern bool AP; // mode point d'accès
extern Mqtt mqtt_config; // configuration mqtt
extern byte present; // capteur dallas présent ou non 
extern String logs; // logs
extern byte security; // sécurité
extern DeviceAddress addr[15]; 
extern float previous_celsius[15]; // température précédente
extern IPAddress gatewayIP;
extern Config config; 
extern MQTT devicetemp[15];
extern String devAddrNames[15];
extern int deviceCount; // nombre de sonde(s) dallas détectée(s)

int dallas_error[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; // compteur d'erreur dallas
int gw_error = 0;   // compteur d'erreur gateway

float CheckTemperature(String label, byte deviceAddress[12]);
// void dallaspresent ();

/// @brief / task executé toute les n secondes pour publier la température ( voir déclaration task dans main )
void mqttdallas() {
  if ( present == 1 ) {
    sensors.requestTemperatures();
    delay(200);
    for (int a = 0; a < deviceCount; a++) {
      sysvar.celsius[a]=CheckTemperature("temp_" + devAddrNames[a],addr[a]);
      //gestion des erreurs DS18B20
      if ( (sysvar.celsius[a] == -127.00) || (sysvar.celsius[a] == -255.00) || (sysvar.celsius[a] > 200.00) ) {
        sysvar.celsius[a]=previous_celsius[a];
        dallas_error[a] ++; // incrémente le compteur d'erreur
        logging.Set_log_init("Dallas" + String(a) + " : échec "+ String(dallas_error[a]) + "\r\n",true);
      }
      else {
        sysvar.celsius[a] = (roundf(sysvar.celsius[a] * 10) / 10 ) + 0.1; // pour les valeurs min
        dallas_error[a] = 0; // remise à zéro du compteur d'erreur
      }   
    }
    if (!AP && mqtt_config.mqtt) {
      if ( sysvar.celsius[sysvar.dallas_maitre] != previous_celsius[sysvar.dallas_maitre]  || sysvar.celsius[sysvar.dallas_maitre] != 0.99) {
        Mqtt_send_DOMOTICZ(String(config.IDXTemp), String(sysvar.celsius[sysvar.dallas_maitre]),"Temperature");
      }

      if (!discovery_temp) {
        discovery_temp = true;
        device_dimmer_alarm_temp.HA_discovery();
        for (int i = 0; i < deviceCount; i++) {
          device_temp[i].HA_discovery();
        }
        device_dimmer_maxtemp.HA_discovery();
        device_dimmer_alarm_temp.send(stringboolMQTT(sysvar.security));
        device_dimmer_maxtemp.send(String(config.maxtemp)); 
      }


      for (int a = 0; a < deviceCount; a++) {
        if ( sysvar.celsius[a] != previous_celsius[a] || sysvar.celsius[a] != 0.99) {
          device_temp[a].send(String(sysvar.celsius[a]));
          previous_celsius[a]=sysvar.celsius[a];
          // logging.Set_log_init("Dallas " + String(a) + " temp : "+ String(sysvar.celsius[a]) +"\r\n");
        }
      }
    }          
    // sysvar.celsius=CheckTemperature("Inside : ", addr); 
  
    // // arrondi à 1 décimale 
    // if ( (sysvar.celsius == -127.00) || (sysvar.celsius == -255.00) ) {
    // sysvar.celsius=previous_celsius;
    // dallas_error ++; // incrémente le compteur d'erreur
    // }
    // else { 
    //   sysvar.celsius = (roundf(sysvar.celsius * 10) / 10 ) + 0.1; // pour les valeurs min
    //   dallas_error = 0; // remise à zéro du compteur d'erreur
    // }
    // if (mqtt_config.mqtt)  { 
    //   if ( sysvar.celsius != previous_celsius ) {
    //     // envoie des infos en mqtt dans ce cas
    //     // mqtt(String(config.IDXTemp), String(sysvar.celsius),"Temperature");
    //     Mqtt_send_DOMOTICZ(String(config.IDXTemp), String(sysvar.celsius),"Temperature");
    //     // if ( mqtt_config.HA ) { device_temp.send(String(sysvar.celsius)); }
    //     device_temp.send(String(sysvar.celsius));
    //     logging.Set_log_init("Dallas temp : "+ String(sysvar.celsius) +"\r\n");
    //   }
  }
  //// détection sécurité température
  if  ( sysvar.celsius[sysvar.dallas_maitre] >= config.maxtemp ) {
    // coupure du dimmer
    DEBUG_PRINTLN("détection sécurité température");

        unified_dimmer.set_power(0);
        unified_dimmer.dimmer_off();
        
      
      if ( strcmp(config.child,"") != 0 && strcmp(config.child,"none") != 0 && strcmp(config.mode,"off") != 0){
                //logging.Set_log_init( "Consigne temp atteinte - Puissance locale à 0 - le reste va aux enfants\r\n" );
      }
      else {
        sysvar.puissance=0;
        //unified_dimmer.set_power(0); // déjà fait 8 lignes au dessus
              //logging.Set_log_init( "Consigne temp atteinte - Puissance locale à 0 - pas d'enfant à servir\r\n" );
      }
    
    if ( mqtt_config.mqtt ) {
      // mqtt(String(config.IDX), "0","pourcent");
      Mqtt_send_DOMOTICZ(String(config.IDX), "0","pourcent");
    }
    // if ( mqtt_config.HA ) { 
    //   device_dimmer.send("0"); 
    //   device_dimmer_power.send("0");
    // }
    device_dimmer.send("0"); 
    device_dimmer_send_power.send("0");
    device_dimmer_power.send("0");
  }
  
  previous_celsius[sysvar.dallas_maitre]=sysvar.celsius[sysvar.dallas_maitre];

  // si trop d'erreur dallas, on remonte en mqtt
  for (int a = 0; a < deviceCount; a++) {
    if ( dallas_error[a] > 5 ) {
      DEBUG_PRINTLN("détection perte sonde dallas");
      // mqtt(String(config.IDXAlarme), String("Dallas perdue"),"Dallas perdue");
      Mqtt_send_DOMOTICZ(String(config.IDXAlarme), String("Dallas perdue"),"Dallas perdue");
      logging.Set_log_init("Dallas perdue !!!\r\n",true);
      dallas_error[a] = 0; // remise à zéro du compteur d'erreur
      ///mise en sécurité
      sysvar.celsius[a] = float(99.99);  
      previous_celsius[a]=sysvar.celsius[a];
      if (a == sysvar.dallas_maitre) {
        unified_dimmer.set_power(0);
      }
      }
   }

  #ifdef STANDALONE
    if ( pinger.Ping(WiFi.gatewayIP())) {
      //Serial.println("Ping OK");
      //ESP.restart();
      gw_error = 0; // remise à zéro du compteur d'erreur
    }
    else {
      //Serial.println("perte de ping");
      //ESP.restart();
      gw_error ++; // incrémente le compteur d'erreur
    }

  /// si GW perdu, reboot de l'ESP après 2 minutes
    if ( gw_error > 8 ) {
      DEBUG_PRINTLN("détection perte gateway");
      // ESP.restart();
      config.restart = true;
    }
  #endif

}
 
    //***********************************
    //************* récupération d'une température du 18b20
    //***********************************

float CheckTemperature(String label, byte deviceAddress[12]){

  // sensors.requestTemperatures();
  // delay(200);
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print(label);
  if ( (tempC == -127.00) || (tempC == -255.00) ) {
    delay(250);
    //// cas d'une sonde trop longue à préparer les valeurs 
     /// attente de 187ms ( temps de réponse de la sonde )
    tempC = sensors.getTempC(deviceAddress);
  }  
  return (tempC); 
}


#endif