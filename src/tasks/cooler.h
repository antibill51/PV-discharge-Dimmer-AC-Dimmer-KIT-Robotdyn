#ifndef TASK_COOLER 
#define TASK_COOLER

#include "Arduino.h"

extern System sysvar;
extern Config config;
extern Mqtt mqtt_config;
extern String logs;
extern bool AP; // mode point d'accès
extern MQTT device_cooler;
extern byte security;
extern Programme programme;

unsigned long lastCoolerOffTime = 0;
const unsigned long cooldownDuration = 60 * 1000; // 1 minute en millisecondes

void cooler() {
                      
    bool cooler_change = sysvar.cooler ;

    /// controle du cooler 
    if (config.dimmer_on_off == 1){
        if ( ( sysvar.puissance > config.minpow && sysvar.celsius[sysvar.dallas_maitre] < config.maxtemp && security == 0 ) || ( programme.run == true )) {
            sysvar.cooler = 1;
        } else {
            sysvar.cooler = 0;
        }
    } 
    else {
        sysvar.cooler = 0;
    }

    if ( cooler_change != sysvar.cooler ) {
        if ( sysvar.cooler == 1 ) {
        digitalWrite(COOLER, HIGH);
        // envoie mqtt
        if ( mqtt_config.mqtt ) {  device_cooler.send(stringbool(true));  }
        } else {
        lastCoolerOffTime = millis(); // on enregistre le temps d'arret pour le cooldown
        }

    }

    if (sysvar.cooler == 0 && millis() - lastCoolerOffTime >= cooldownDuration && digitalRead(COOLER) == HIGH && programme.run == false) {
        digitalWrite(COOLER, LOW); // Éteindre le ventilateur après X secondes (cooldownDuration)
    
        if ( mqtt_config.mqtt ) {  device_cooler.send(stringbool(false));  }
        }
    
    
    ///ajout d'envoie MQTT pour test fuite mémoire
  //  client.publish(String("memory/dimmer-"+dimmername).c_str(), 0,true, String(ESP.getFreeHeap()).c_str());
    client.publish((topic_Xlyric+"memory").c_str(),1,true, String(ESP.getFreeHeap()).c_str());

 // pas besoin de tempo pour l'arret, vu que c'est toute les 15 secondes la task 
}

#endif

