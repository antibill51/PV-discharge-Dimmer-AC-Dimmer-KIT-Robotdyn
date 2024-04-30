#ifndef HA_FUNCTIONS
#define HA_FUNCTIONS

#include <AsyncMqttClient.h>

extern AsyncMqttClient  client;
extern Config config;
extern Mqtt mqtt_config;
extern System sysvar;
// extern DeviceAddress addr[MAX_DALLAS];  // array of (up to) 15 temperature sensors
extern String devAddrNames[MAX_DALLAS];  // array of (up to) 15 temperature sensors
extern int deviceCount ; // nombre de sonde(s) dallas détectée(s)
// String stringBool(bool mybool);

struct MQTT
{
    private:int MQTT_INTERVAL = 60;
    /* MQTT */
    private:String name; 
    public:void Set_name(String setter) {name=setter; }

  private:String object_id; 
  public:void Set_object_id(String setter) {object_id=setter; }

  private:String dev_cla; 
  public:void Set_dev_cla(String setter) {dev_cla=setter; }

  private:String unit_of_meas; 
  public:void Set_unit_of_meas(String setter) {unit_of_meas=setter; }

  private:String stat_cla; 
  public:void Set_stat_cla(String setter) {stat_cla=setter; }

  private:String entity_category; 
  public:void Set_entity_category(String setter) {entity_category=setter; }
  
  private:String entity_type; 
  public:void Set_entity_type(String setter) {entity_type=setter; }

  private:String icon; 
  public:void Set_icon(String setter) {icon=R"("ic": ")" + setter + R"(",)"; }
  //{icon="\"ic\": \""+ setter +"\", "; }

  private:String min; 
  public:void Set_entity_valuemin(String setter) {min=setter; }

  private:String max; 
  public:void Set_entity_valuemax(String setter) {max=setter; }

  private:String step; 
  public:void Set_entity_valuestep(String setter) {step=setter; }

  private:String entity_option; 
  public:void Set_entity_option(String setter) {entity_option=setter; }

    private:bool retain_flag; 
    public:void Set_retain_flag(bool setter) {retain_flag=setter; }

    private:int qos; 
    public:void Set_entity_qos(int setter) {qos=setter; }

    private:String retain; 
    public:void Set_retain(bool setter) {
    if (setter) {retain=R"("ret":true,)"; }
    // if (setter) {retain="\"ret\":true,"; }
  }

  private:String expire_after; 
  public:void Set_expire_after(bool setter) {
    if (setter) {expire_after=R"("exp_aft": ")"+ String(MQTT_INTERVAL) +R"(",)"; }
    // if (setter) {expire_after="\"exp_aft\": \""+ String(MQTT_INTERVAL) +"\", "; }

  }

  private:String HA_sensor_type() {
    //   String topic = "homeassistant/"+ entity_type +"/"+ node_id +"/";
    //   String topic_Xlyric = "Xlyric/"+ node_id +"/";
    //   String info;
    //   if (entity_type == "sensor") {
    //           info =         "\"dev_cla\": \""+dev_cla+"\","
    //         "\"unit_of_meas\": \""+unit_of_meas+"\","
    //         "\"stat_cla\": \""+stat_cla+"\"," 
    //         "\"value_template\": \"{{ value_json."+ object_id +" }}\","; 
    //   }
    //   else if (entity_type == "switch") { 
    //           info =         "\"val_tpl\": \"{{ value_json."+ object_id +" }}\","
    //         "\"pl\":  \"{{ value_json."+ object_id +" }}\","
    //         "\"pl_on\": \"{ \\\""+object_id+"\\\" : \\\"1\\\"  } \","
    //         "\"pl_off\": \"{ \\\""+object_id+"\\\" : \\\"0\\\"  } \","
    //         "\"stat_on\":1,"
    //         "\"stat_off\":0,"
    //       "\"qos\":1,"
    //       "\"cmd_t\": \""+ topic_Xlyric + "command/" +  entity_type + "/" + object_id + "\",";
    //   } 
    //   else if (entity_type == "number") { 
    //         info =         "\"val_tpl\": \"{{ value_json."+ object_id +" }}\","
    //       "\"cmd_t\": \""+ topic_Xlyric + "command/" +  entity_type + "/" + object_id + "\","
    //         "\"cmd_tpl\": \"{ \\\""+object_id+"\\\" : {{ value }} } \"," 
    //       "\"entity_category\": \""+ entity_category + "\","
    //       "\"max\": \""+max+"\","
    //       "\"min\": \""+min+"\","
    //       "\"step\": \""+step+"\",";
    //   } 
    //   else if (entity_type == "select") { 
    //         info =         "\"val_tpl\": \"{{ value_json."+ object_id +" }}\","
    //       "\"cmd_t\": \""+ topic_Xlyric + "command/" +  entity_type + "/" + object_id + "\","
    //         "\"cmd_tpl\": \"{ \\\""+object_id+"\\\" : \\\"{{ value }}\\\" } \"," 
    //       "\"entity_category\": \""+ entity_category + "\","
    //       "\"options\": ["+ entity_option + "],";
    //   } 
    //   else if (entity_type == "binary_sensor") { 
    //           info =         "\"dev_cla\": \""+dev_cla+"\","
    //         "\"pl_on\":\"true\","
    //         "\"pl_off\":\"false\","
    //         "\"val_tpl\": \"{{ value_json."+ object_id +" }}\",";
    //   }
    //   else if (entity_type == "button") { 
    //         info =            "\"entity_category\": \""+ entity_category + "\","
    //       "\"cmd_t\": \""+ topic_Xlyric + "command/" +  entity_type + "/" + object_id + "\","
    //         "\"pl_prs\": \"{ \\\""+object_id+"\\\" : \\\"1\\\"  } \",";
    //   }
    //   return info;

      String topic = R"(homeassistant/)" + entity_type + R"(/)" + node_id + R"(/)";
      String topic_Xlyric = R"(Xlyric/)" + node_id + R"(/)";
      String info;
      if (entity_type == "sensor") {
          info = R"("dev_cla": ")" + dev_cla + R"(",
                "unit_of_meas": ")" + unit_of_meas + R"(",
                "stat_cla": ")" + stat_cla + R"(",
                "value_template": "{{ value_json.)" + object_id + R"( }}",)";
      }
      else if (entity_type == "switch") { 
          info = R"("val_tpl": "{{ value_json.)" + object_id + R"( }}",
                "pl":  "{{ value_json.)" + object_id + R"( }}",
                "pl_on": "{ \")" + object_id + R"(\": \"1\"  } ",
                "pl_off": "{ \")" + object_id + R"(\": \"0\"  } ",
                "stat_on":1,
                "stat_off":0,
                "qos":1,
                "cmd_t": ")" + topic_Xlyric + R"(command/)" +  entity_type + R"(/)" + object_id + R"(",)";
      } 
      else if (entity_type == "number") { 
          info = R"("val_tpl": "{{ value_json.)" + object_id + R"( }}",
                "cmd_t": ")" + topic_Xlyric + R"(command/)" +  entity_type + R"(/)" + object_id + R"(",
                "cmd_tpl": "{ \")" + object_id + R"(\": {{ value }} } ",
                "entity_category": ")" + entity_category + R"(",
                "max": ")" + max + R"(",
                "min": ")" + min + R"(",
                "step": ")" + step + R"(",)";
      } 
      else if (entity_type == "select") { 
          info = R"("val_tpl": "{{ value_json.)" + object_id + R"( }}",
                "cmd_t": ")" + topic_Xlyric + R"(command/)" +  entity_type + R"(/)" + object_id + R"(",
                "cmd_tpl": "{ \")" + object_id + R"(\": {{ value }} } ",
                "entity_category": ")" + entity_category + R"(",
                "options": [)" + entity_option + R"(],)";
      } 
      else if (entity_type == "binary_sensor") { 
          info = R"("dev_cla": ")" + dev_cla + R"(",
                "pl_on":"true",
                "pl_off":"false",
                "val_tpl": "{{ value_json.)" + object_id + R"( }}",)";
      }
      else if (entity_type == "button") { 
          info = R"("entity_category": ")" + entity_category + R"(",
                "cmd_t": ")" + topic_Xlyric + R"(command/)" +  entity_type + R"(/)" + object_id + R"(",
                "pl_prs": "{ \"")" + object_id + R"(": \""1\"  } ",)";
      }
      return info;


    }



  private:String IPaddress = WiFi.localIP().toString();
  
    private:String node_mac = WiFi.macAddress().substring(12,14)+ WiFi.macAddress().substring(15,17);
    // setter mod_mac
    public:void Set_node_mac(String setter) {node_mac=setter; }
       
    private:String node_id = String("Dimmer-") + node_mac; 
    // private:String topic_switch = "homeassistant/switch/"+ node_id +"/";
    // private:String topic_switch_state = "homeassistant/switch/";
    private:String HA_device_declare() { 
              String IPaddress = WiFi.localIP().toString();
              String info = R"("dev": {
                      "ids": ")" + node_id + R"(",
                      "name": ")" + node_id + R"(",
                      "sw": "Dimmer )" + String(VERSION) + R"(",
                      "mdl": "ESP8266 )" + IPaddress + R"(",
                      "mf": "Cyril Poissonnier",
                      "cu": "http://)" + IPaddress + R"("
                      })";
              // String info =         "\"dev\": {"
            //   "\"ids\": \""+ node_id + "\","
            //   "\"name\": \""+ node_id + "\","
            //   "\"sw\": \"Dimmer "+ String(VERSION) +"\","
            //   "\"mdl\": \"ESP8266 " + IPaddress + "\","
            //   "\"mf\": \"Cyril Poissonnier\","
            //   "\"cu\": \"http://"+ IPaddress +"\""
            // "}"; 
            return info;
            }


  public:void HA_discovery(){
      String topic = "homeassistant/"+ entity_type +"/"+ node_id +"/";
      String topic_Xlyric = "Xlyric/"+ node_id +"/";
      String device = R"({"name": ")" + name + R"(", 
                  "obj_id": "Dimmer-)" + node_mac + R"(-)" + object_id + R"(", 
                  "uniq_id": ")" + node_mac + R"(-)" + object_id + R"(", 
                  "stat_t": ")" + topic_Xlyric + R"(sensors/)" + object_id + R"(/state", 
                  "avty_t": ")" + topic_Xlyric + R"(status",)"
                  + HA_sensor_type()
                  + icon
                  + retain
                  + expire_after
                  + HA_device_declare() + 
                    "}";
      // String device= "{\"name\": \""+ name + "\"," 
      //       "\"obj_id\": \"Dimmer-"+ node_mac +"-"+ object_id + "\"," 
      //       "\"uniq_id\": \""+ node_mac + "-" + object_id +"\","
      //     "\"stat_t\": \""+ topic_Xlyric + "sensors/" + object_id +"/state\"," 
      //     "\"avty_t\": \""+ topic_Xlyric + "status\","
      //     + HA_sensor_type()
      //       + icon
      //       + retain
      //       + expire_after
      //     + HA_device_declare() + 
      //       "}";

       if (strlen(object_id.c_str()) > 0) {
       client.publish(String(topic+object_id+"/config").c_str() ,1,false, device.c_str()); // déclaration autoconf dimmer
       }  
       else {
        client.publish(String(topic+"config").c_str() ,1,true, device.c_str()); // déclaration autoconf dimmer
       }
 
    }

    public:void send(String value){
    if (config.JEEDOM || config.HA) {
      String topic = R"(Xlyric/)" + node_id + R"(/sensors/)";
      String message = R"(  { ")" + object_id + R"(" : ")" + value.c_str() + R"("  } )";
      // String topic = "Xlyric/"+ node_id +"/sensors/";
      // String message = "  { \""+object_id+"\" : \"" + value.c_str() + "\"  } ";
      client.publish(String(topic + object_id + "/state").c_str() ,qos, retain_flag , message.c_str());
    }
  } 
};


/// création des sensors
MQTT device_dimmer; 
MQTT device_temp[MAX_DALLAS]; 
MQTT device_dimmer_power;
MQTT device_dimmer_total_power;
MQTT device_dimmer_charge1;
MQTT device_dimmer_charge2;
MQTT device_dimmer_charge3;
/// création des switchs
MQTT device_relay1;
MQTT device_relay2;
MQTT device_dimmer_on_off;

/// création des button
MQTT device_dimmer_save;
MQTT device_dimmer_alarm_temp_clear;

/// création number
MQTT device_dimmer_starting_pow; 
MQTT device_dimmer_maxtemp;
MQTT device_dimmer_minpow;
MQTT device_dimmer_maxpow;
MQTT device_dimmer_send_power; 

/// création select
MQTT device_dimmer_child_mode;

/// création binary_sensor
MQTT device_dimmer_alarm_temp;
MQTT device_cooler;

void devices_init(){
  if (config.HA || config.JEEDOM) {
    /// création des sensors
    device_dimmer.Set_name("Puissance");
    device_dimmer.Set_object_id("power");
    device_dimmer.Set_unit_of_meas("%");
    device_dimmer.Set_stat_cla("measurement");
    device_dimmer.Set_dev_cla("power_factor"); // fix is using native unit of measurement '%' which is not a valid unit for the device class ('power') it is using
    device_dimmer.Set_icon("mdi:percent");
    device_dimmer.Set_entity_type("sensor");
    device_dimmer.Set_entity_qos(0);
    device_dimmer.Set_retain_flag(false);
    // device_dimmer.Set_expire_after(true);

    device_dimmer_power.Set_name("Watt");
    device_dimmer_power.Set_object_id("watt");
    device_dimmer_power.Set_unit_of_meas("W");
    device_dimmer_power.Set_stat_cla("measurement");
    device_dimmer_power.Set_dev_cla("power");
    device_dimmer_power.Set_icon("mdi:home-lightning-bolt-outline");
    device_dimmer_power.Set_entity_type("sensor");
    device_dimmer_power.Set_entity_qos(0);
    device_dimmer_power.Set_retain_flag(false);

    device_dimmer_total_power.Set_name("Watt total");
    device_dimmer_total_power.Set_object_id("watt_total");
    device_dimmer_total_power.Set_unit_of_meas("W");
    device_dimmer_total_power.Set_stat_cla("measurement");
    device_dimmer_total_power.Set_dev_cla("power");
    device_dimmer_total_power.Set_icon("mdi:home-lightning-bolt-outline");
    device_dimmer_total_power.Set_entity_type("sensor");
    device_dimmer_total_power.Set_entity_qos(0);
    device_dimmer_total_power.Set_retain_flag(false);
    
    for (int i = 0; i < deviceCount; i++) {
      String devicename;
      String objectid;
      if ( i == sysvar.dallas_maitre ) {
        devicename = "Température master";
        objectid = "temperature_master";
        }
      else {
        devicename ="Température";
        objectid = "temperature_"+ devAddrNames[i];
        }
      device_temp[i].Set_name(String(devicename));
      device_temp[i].Set_object_id(String(objectid));
      device_temp[i].Set_unit_of_meas("°C");
      device_temp[i].Set_stat_cla("measurement");
      device_temp[i].Set_dev_cla("temperature");
      device_temp[i].Set_entity_type("sensor");
      device_temp[i].Set_entity_qos(1);
      device_temp[i].Set_retain_flag(true);
    }
    /// création des switch
    device_relay1.Set_name("Relais 1");
    device_relay1.Set_object_id("relay1");
    device_relay1.Set_entity_type("switch");
    device_relay1.Set_entity_qos(0);
    device_relay1.Set_retain_flag(false);
    device_relay1.Set_retain(true);

    device_relay2.Set_name("Relais 2");
    device_relay2.Set_object_id("relay2");
    device_relay2.Set_entity_type("switch");
    device_relay2.Set_entity_qos(0);
    device_relay2.Set_retain_flag(false);
    device_relay2.Set_retain(true);

    device_dimmer_on_off.Set_name("Dimmer");
    device_dimmer_on_off.Set_object_id("on_off");
    device_dimmer_on_off.Set_entity_type("switch");
    device_dimmer_on_off.Set_entity_qos(0);
    device_dimmer_on_off.Set_retain_flag(false);
    device_dimmer_on_off.Set_retain(true);
  
    /// création des button
    device_dimmer_save.Set_name("Sauvegarder");
    device_dimmer_save.Set_object_id("save");
    device_dimmer_save.Set_entity_type("button");
    device_dimmer_save.Set_entity_category("config");
    device_dimmer_save.Set_entity_qos(0);
    device_dimmer_save.Set_retain_flag(false);

    device_dimmer_alarm_temp_clear.Set_name("RAZ surchauffe");
    device_dimmer_alarm_temp_clear.Set_object_id("reset_alarm");
    device_dimmer_alarm_temp_clear.Set_entity_type("button");
    device_dimmer_alarm_temp_clear.Set_entity_category("config");
    device_dimmer_alarm_temp_clear.Set_entity_qos(0);
    device_dimmer_alarm_temp_clear.Set_retain_flag(false);


    /// création des number
    device_dimmer_starting_pow.Set_name("Puissance de démarrage");
    device_dimmer_starting_pow.Set_object_id("starting_power");
    device_dimmer_starting_pow.Set_entity_type("number");
    device_dimmer_starting_pow.Set_entity_category("config");
    device_dimmer_starting_pow.Set_entity_valuemin("-100");
    device_dimmer_starting_pow.Set_entity_valuemax("500"); // trop? pas assez? TODO : test sans valeur max?
    device_dimmer_starting_pow.Set_entity_valuestep("1");
    device_dimmer_starting_pow.Set_entity_qos(0);
    device_dimmer_starting_pow.Set_retain_flag(false);

    device_dimmer_minpow.Set_name("Puissance mini");
    device_dimmer_minpow.Set_object_id("minpow");
    device_dimmer_minpow.Set_entity_type("number");
    device_dimmer_minpow.Set_entity_category("config");
    device_dimmer_minpow.Set_entity_valuemin("0");
    device_dimmer_minpow.Set_entity_valuemax("100"); // trop? pas assez? TODO : test sans valeur max?
    device_dimmer_minpow.Set_entity_valuestep("1");
    device_dimmer_minpow.Set_entity_qos(0);
    device_dimmer_minpow.Set_retain_flag(false);

    device_dimmer_maxpow.Set_name("Puissance maxi");
    device_dimmer_maxpow.Set_object_id("maxpow");
    device_dimmer_maxpow.Set_entity_type("number");
    device_dimmer_maxpow.Set_entity_category("config");
    device_dimmer_maxpow.Set_entity_valuemin("0");
    device_dimmer_maxpow.Set_entity_valuemax("100"); // trop? pas assez? TODO : test sans valeur max?
    device_dimmer_maxpow.Set_entity_valuestep("1");
    device_dimmer_maxpow.Set_entity_qos(0);
    device_dimmer_maxpow.Set_retain_flag(false);

    device_dimmer_maxtemp.Set_name("Température maxi");
    device_dimmer_maxtemp.Set_object_id("maxtemp");
    device_dimmer_maxtemp.Set_entity_type("number");
    device_dimmer_maxtemp.Set_entity_category("config");
    device_dimmer_maxtemp.Set_entity_valuemin("0");
    device_dimmer_maxtemp.Set_entity_valuemax("90"); // trop? pas assez? TODO : test sans valeur max?
    device_dimmer_maxtemp.Set_entity_valuestep("1");
    device_dimmer_maxtemp.Set_entity_qos(0);
    device_dimmer_maxtemp.Set_retain_flag(false);

    device_dimmer_send_power.Set_name("Puissance dimmer");
    device_dimmer_send_power.Set_object_id("powdimmer");
    device_dimmer_send_power.Set_entity_type("number");
    device_dimmer_send_power.Set_entity_category("config");
    device_dimmer_send_power.Set_entity_valuemin("0");
    device_dimmer_send_power.Set_entity_valuemax("100"); // trop? pas assez? TODO : test sans valeur max?
    device_dimmer_send_power.Set_entity_valuestep("1");
    device_dimmer_send_power.Set_entity_qos(0);
    device_dimmer_send_power.Set_retain_flag(false);

    device_dimmer_charge1.Set_name("Charge 1");
    device_dimmer_charge1.Set_object_id("charge1");
    device_dimmer_charge1.Set_entity_type("number");
    device_dimmer_charge1.Set_entity_category("config");
    device_dimmer_charge1.Set_entity_valuemin("0");
    device_dimmer_charge1.Set_entity_valuemax("3000");
    device_dimmer_charge1.Set_entity_valuestep("50");
    device_dimmer_charge1.Set_entity_qos(0);
    device_dimmer_charge1.Set_retain_flag(false);

    device_dimmer_charge2.Set_name("Charge 2");
    device_dimmer_charge2.Set_object_id("charge2");
    device_dimmer_charge2.Set_entity_type("number");
    device_dimmer_charge2.Set_entity_category("config");
    device_dimmer_charge2.Set_entity_valuemin("0");
    device_dimmer_charge2.Set_entity_valuemax("3000");
    device_dimmer_charge2.Set_entity_valuestep("50");
    device_dimmer_charge2.Set_entity_qos(0);
    device_dimmer_charge2.Set_retain_flag(false);

    device_dimmer_charge3.Set_name("Charge 3");
    device_dimmer_charge3.Set_object_id("charge3");
    device_dimmer_charge3.Set_entity_type("number");
    device_dimmer_charge3.Set_entity_category("config");
    device_dimmer_charge3.Set_entity_valuemin("0");
    device_dimmer_charge3.Set_entity_valuemax("3000");
    device_dimmer_charge3.Set_entity_valuestep("50");
    device_dimmer_charge3.Set_entity_qos(0);
    device_dimmer_charge3.Set_retain_flag(false);

    /// création des select
    device_dimmer_child_mode.Set_name("Mode");
    device_dimmer_child_mode.Set_object_id("child_mode");
    device_dimmer_child_mode.Set_entity_type("select");
    device_dimmer_child_mode.Set_entity_category("config");
    device_dimmer_child_mode.Set_entity_option(R"("off","delester","equal")");
    // device_dimmer_child_mode.Set_entity_option("\"off\",\"delester\",\"equal\"");
    device_dimmer_child_mode.Set_entity_qos(0);
    device_dimmer_child_mode.Set_retain_flag(false);

    // création des binary_sensor
    device_dimmer_alarm_temp.Set_name("Surchauffe");
    device_dimmer_alarm_temp.Set_object_id("alarm_temp");
    device_dimmer_alarm_temp.Set_entity_type("binary_sensor");
    device_dimmer_alarm_temp.Set_entity_category("diagnostic");
    device_dimmer_alarm_temp.Set_dev_cla("problem");
    device_dimmer_alarm_temp.Set_entity_qos(0);
    device_dimmer_alarm_temp.Set_retain_flag(false);

    device_cooler.Set_name("Ventillateur");
    device_cooler.Set_object_id("cooler");
    device_cooler.Set_entity_type("binary_sensor");
    device_cooler.Set_entity_category("diagnostic");
    device_cooler.Set_dev_cla("running");
    device_cooler.Set_entity_qos(0);
    device_cooler.Set_retain_flag(false);
  }
}
#endif