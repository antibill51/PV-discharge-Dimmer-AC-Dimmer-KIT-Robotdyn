#ifndef HA_FUNCTIONS
#define HA_FUNCTIONS

#include <espMqttClientAsync.h>

extern espMqttClientAsync  client;
extern Config config; // NOSONAR
extern Mqtt mqtt_config; // NOSONAR
extern System sysvar;
// extern DeviceAddress addr[MAX_DALLAS];  // array of (up to) 15 temperature sensors
extern String devAddrNames[MAX_DALLAS];  // array of (up to) 15 temperature sensors
extern int deviceCount ; // nombre de sonde(s) dallas détectée(s)
extern String stringBool(bool mybool);
extern Logs logging; 

struct MQTT
{
  private:int MQTT_INTERVAL = 60;
  /* MQTT */
  private:String name; 
  public:void Set_name(String setter) {name=setter; }

  private:char default_entity_id[30]; 
  public:void Set_default_entity_id(String setter) {
    snprintf(default_entity_id, sizeof(default_entity_id), "%s", setter.c_str());}

  private:String dev_cla; 
  public:void Set_dev_cla(String setter) {dev_cla=setter; }

  private:String unit_of_meas; 
  public:void Set_unit_of_meas(String setter) {unit_of_meas=setter; }

  private:String stat_cla; 
  public:void Set_stat_cla(String setter) {stat_cla=setter; }

  private:String entity_category; 
  public:void Set_entity_category(String setter) {entity_category=setter; }
  
  private:char entity_type[14]; 
  public:void Set_entity_type(String setter) {
    snprintf(entity_type, sizeof(entity_type), "%s", setter.c_str());}

  private:String icon; 
  public:void Set_icon(String setter) {icon=setter; }
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
  if (setter) {retain=stringBool(setter); }
  }

  private:String expire_after; 
  public:void Set_expire_after(bool setter) {
    if (setter) {expire_after=R"("exp_aft": ")" + String(MQTT_INTERVAL) + R"(", )"; }
  }


// private:String node_mac = WiFi.macAddress().substring(12,14)+ WiFi.macAddress().substring(15,17);
// private:String topic_Xlyric = "Xlyric/"+ (config.say_my_name) +"/";

private:
  void createHA_sensor_type(JsonObject& root) {

// char node_mac[21];
char uniq_id[62]; // (MODIFIE) Pour garder l'ID unique sans préfixe
char def_ent_id[80]; // (MODIFIE) Taille augmentée pour accueillir "domaine.id"
char topic_Xlyric[40]; // 8 + config.say_my_name
char stat_t[100]; 
char avty_t[46]; // 6 + topic_Xlyric
char value_template[47]; // 17 + default_entity_id

char cmd_t[93]; //9+topic_Xlyric + entity_type + default_entity_id -2

      // MODIFICATION ICI: On sépare le uniq_id du default_entity_id pour lui ajouter son domaine.
      snprintf(uniq_id, sizeof(uniq_id), "%s-%s", config.say_my_name, default_entity_id);
      snprintf(def_ent_id, sizeof(def_ent_id), "%s.%s", entity_type, uniq_id);
      
      snprintf(topic_Xlyric, sizeof(topic_Xlyric), "Xlyric/%s/", config.say_my_name);
      
      snprintf(stat_t, sizeof(stat_t), "%s%s/%s/state", topic_Xlyric, entity_type, default_entity_id );
      
      snprintf(avty_t, sizeof(avty_t), "%sstatus", topic_Xlyric);

      root["name"] = name;
      root["def_ent_id"] = def_ent_id;
      root["uniq_id"] = uniq_id; // (MODIFIE) On passe uniq_id plutôt que def_ent_id
      root["stat_t"] =  stat_t;
      root["avty_t"] = avty_t;
      if (!strcmp(entity_type, "button") == 0) {
        snprintf(value_template, sizeof(value_template), "{{ value_json.%s }}", default_entity_id);
      }
      if (!strcmp(entity_type, "sensor") == 0 && !strcmp(entity_type, "binary_sensor") == 0) {
      snprintf(cmd_t, sizeof(cmd_t), "%scommand/%s/%s", topic_Xlyric,entity_type,default_entity_id );
      }
      
      if (strcmp(entity_type, "sensor") == 0) {
          root["dev_cla"] = dev_cla;
          root["unit_of_meas"] = unit_of_meas;
          root["stat_cla"] = stat_cla;
          root["val_tpl"] = value_template;
      }
      else if (strcmp(entity_type, "switch") == 0) {
          char pl_on[44]; //14+default_entity_id
          char pl_off[44]; //14+default_entity_id
          snprintf(pl_on, sizeof(pl_on), "{ \"%s\" : \"1\"  } ", default_entity_id);
          snprintf(pl_off, sizeof(pl_off), "{ \"%s\" : \"0\"  } ", default_entity_id);

          root["val_tpl"] = value_template;
          root["pl"] = value_template;
          root["pl_on"] = pl_on;
          root["pl_off"] = pl_off;
          root["stat_on"] = 1;
          root["stat_off"] = 0;
          root["qos"] = 1;
          root["cmd_t"] = cmd_t;
      } 
      else if (strcmp(entity_type, "number") == 0 || strcmp(entity_type, "select") == 0) {
          char cmd_tpl[50]; //20+default_entity_id
          snprintf(cmd_tpl, sizeof(cmd_tpl), "{\"%s\": {{ value }} }", default_entity_id );
          root["val_tpl"] = value_template;
          root["cmd_t"] = cmd_t;
          root["cmd_tpl"] = cmd_tpl;
          root["entity_category"] = entity_category;

          if (strcmp(entity_type, "number") == 0) {
              root["max"] = max;
              root["min"] = min;
              root["step"] = step;
          } 
          else if (strcmp(entity_type, "select") == 0) {
            // JsonArray options = root.createNestedArray("options");
            JsonArray options = root["options"].to<JsonArray>();
            options.add("off");
            options.add("delester");
            options.add("equal");
          }
      } 
      else if (strcmp(entity_type, "binary_sensor") == 0) {
          root["dev_cla"] = dev_cla;
          root["pl_on"] = "true";
          root["pl_off"] = "false";
          root["val_tpl"] = value_template;
      }
      else if (strcmp(entity_type, "button") == 0) {
        char pl_prs[44]; //14+default_entity_id    
          snprintf(pl_prs, sizeof(pl_prs), "{\"%s\": \"1\" }", default_entity_id );
          root["entity_category"] = entity_category;
          root["cmd_t"] = cmd_t;
          root["pl_prs"] = pl_prs;
      }
      if (!icon.isEmpty()) {
          root["ic"] = icon;
      }
        if (!retain.isEmpty()) {
          root["ret"] = retain;
      }
      if (!expire_after.isEmpty()) {
          root["exp_aft"] = String(MQTT_INTERVAL);
      }
  }

  String getIPaddress() {
      return WiFi.localIP().toString();
  }

  void createHA_device_declare(JsonObject& root) { 
      root["ids"] = config.say_my_name;
      root["name"] = config.say_my_name;
      root["sw"] = VERSION;
      root["mdl"] = "ESP8266 " + getIPaddress();
      root["mf"] = "Cyril Poissonnier";
      root["cu"] = "http://" + getIPaddress();
  }

public:
  void HA_discovery(){
    if (client.connected() && config.HA){
      JsonDocument device;
      JsonObject root = device.to<JsonObject>();

      char topic[97]; // 23 + entity_type + config.say_my_name + default_entity_id -2
      snprintf(topic, sizeof(topic), "homeassistant/%s/%s/%s/config", entity_type,config.say_my_name,default_entity_id );

      JsonObject deviceObj = root["device"].to<JsonObject>(); // Création d'un objet JSON imbriqué pour "device"
      createHA_device_declare(deviceObj);

      createHA_sensor_type(root); // Appel de la fonction pour créer les données relatives au capteur

      char output[700];
      serializeJson(root, output);

      int status;
      status = client.publish(topic, 1, false, output);

      if (status == 0) {
        logging.Set_log_init("MQTT ERROR : discovery not sended for ",true);
        logging.Set_log_init(default_entity_id);
        logging.Set_log_init("\r\n");
      }
    }
  }

    public:void send(String value){
      if (client.connected()){
    if (config.JEEDOM || config.HA) {

      char topic[100]; 
      snprintf(topic, sizeof(topic), "Xlyric/%s/%s/%s/state",config.say_my_name, entity_type, default_entity_id );

      char message[100]; 
      snprintf(message, sizeof(message),"{\"%s\":\"%s\"}" ,default_entity_id, value.c_str());
      int status;
      status = client.publish(topic ,qos, retain_flag , message);  
      if (status == 0) {
        logging.Set_log_init("MQTT ERROR : discovery not sended for ",true);
        logging.Set_log_init(default_entity_id);
        logging.Set_log_init("\r\n");
      }
    }
      }
  } 
};

/// création des sensors
MQTT device_dimmer; 
MQTT device_temp[MAX_DALLAS];  // NO SONAR
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
    device_dimmer.Set_default_entity_id("power");
    device_dimmer.Set_unit_of_meas("%");
    device_dimmer.Set_stat_cla("measurement");
    device_dimmer.Set_dev_cla("power_factor"); 
    device_dimmer.Set_icon("mdi:percent");
    device_dimmer.Set_entity_type("sensor");
    device_dimmer.Set_entity_qos(0);
    device_dimmer.Set_retain_flag(false);

    device_dimmer_power.Set_name("Watt");
    device_dimmer_power.Set_default_entity_id("watt");
    device_dimmer_power.Set_unit_of_meas("W");
    device_dimmer_power.Set_stat_cla("measurement");
    device_dimmer_power.Set_dev_cla("power");
    device_dimmer_power.Set_icon("mdi:home-lightning-bolt-outline");
    device_dimmer_power.Set_entity_type("sensor");
    device_dimmer_power.Set_entity_qos(0);
    device_dimmer_power.Set_retain_flag(false);

    device_dimmer_total_power.Set_name("Watt total");
    device_dimmer_total_power.Set_default_entity_id("watt_total");
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
      device_temp[i].Set_default_entity_id(String(objectid));
      device_temp[i].Set_unit_of_meas("°C");
      device_temp[i].Set_stat_cla("measurement");
      device_temp[i].Set_dev_cla("temperature");
      device_temp[i].Set_entity_type("sensor");
      device_temp[i].Set_entity_qos(1);
      device_temp[i].Set_retain_flag(true);
    }
    /// création des switch
    device_relay1.Set_name("Relais 1");
    device_relay1.Set_default_entity_id("relay1");
    device_relay1.Set_entity_type("switch");
    device_relay1.Set_entity_qos(0);
    device_relay1.Set_retain_flag(true);
    device_relay1.Set_retain(true);

    device_relay2.Set_name("Relais 2");
    device_relay2.Set_default_entity_id("relay2");
    device_relay2.Set_entity_type("switch");
    device_relay2.Set_entity_qos(0);
    device_relay2.Set_retain_flag(true);
    device_relay2.Set_retain(true);

    device_dimmer_on_off.Set_name("Dimmer");
    device_dimmer_on_off.Set_default_entity_id("on_off");
    device_dimmer_on_off.Set_entity_type("switch");
    device_dimmer_on_off.Set_entity_qos(0);
    device_dimmer_on_off.Set_retain_flag(true);
    device_dimmer_on_off.Set_retain(true);
  
    /// création des button
    device_dimmer_save.Set_name("Sauvegarder");
    device_dimmer_save.Set_default_entity_id("save");
    device_dimmer_save.Set_entity_type("button");
    device_dimmer_save.Set_entity_category("config");
    device_dimmer_save.Set_entity_qos(0);
    device_dimmer_save.Set_retain_flag(false);

    device_dimmer_alarm_temp_clear.Set_name("RAZ surchauffe");
    device_dimmer_alarm_temp_clear.Set_default_entity_id("reset_alarm");
    device_dimmer_alarm_temp_clear.Set_entity_type("button");
    device_dimmer_alarm_temp_clear.Set_entity_category("config");
    device_dimmer_alarm_temp_clear.Set_entity_qos(0);
    device_dimmer_alarm_temp_clear.Set_retain_flag(false);

    /// création des number
    device_dimmer_starting_pow.Set_name("Puissance de demarrage");
    device_dimmer_starting_pow.Set_default_entity_id("starting_power");
    device_dimmer_starting_pow.Set_entity_type("number");
    device_dimmer_starting_pow.Set_entity_category("config");
    device_dimmer_starting_pow.Set_entity_valuemin("-100");
    device_dimmer_starting_pow.Set_entity_valuemax("500");
    device_dimmer_starting_pow.Set_entity_valuestep("1");
    device_dimmer_starting_pow.Set_entity_qos(0);
    device_dimmer_starting_pow.Set_retain_flag(false);

    device_dimmer_minpow.Set_name("Puissance mini");
    device_dimmer_minpow.Set_default_entity_id("minpow");
    device_dimmer_minpow.Set_entity_type("number");
    device_dimmer_minpow.Set_entity_category("config");
    device_dimmer_minpow.Set_entity_valuemin("0");
    device_dimmer_minpow.Set_entity_valuemax("100");
    device_dimmer_minpow.Set_entity_valuestep("1");
    device_dimmer_minpow.Set_entity_qos(0);
    device_dimmer_minpow.Set_retain_flag(false);

    device_dimmer_maxpow.Set_name("Puissance maxi");
    device_dimmer_maxpow.Set_default_entity_id("maxpow");
    device_dimmer_maxpow.Set_entity_type("number");
    device_dimmer_maxpow.Set_entity_category("config");
    device_dimmer_maxpow.Set_entity_valuemin("0");
    device_dimmer_maxpow.Set_entity_valuemax("100");
    device_dimmer_maxpow.Set_entity_valuestep("1");
    device_dimmer_maxpow.Set_entity_qos(0);
    device_dimmer_maxpow.Set_retain_flag(false);

    device_dimmer_maxtemp.Set_name("Température maxi");
    device_dimmer_maxtemp.Set_default_entity_id("maxtemp");
    device_dimmer_maxtemp.Set_entity_type("number");
    device_dimmer_maxtemp.Set_entity_category("config");
    device_dimmer_maxtemp.Set_entity_valuemin("0");
    device_dimmer_maxtemp.Set_entity_valuemax("90");
    device_dimmer_maxtemp.Set_entity_valuestep("1");
    device_dimmer_maxtemp.Set_entity_qos(0);
    device_dimmer_maxtemp.Set_retain_flag(false);

    device_dimmer_send_power.Set_name("Puissance dimmer");
    device_dimmer_send_power.Set_default_entity_id("powdimmer");
    device_dimmer_send_power.Set_entity_type("number");
    device_dimmer_send_power.Set_entity_category("config");
    device_dimmer_send_power.Set_entity_valuemin("0");
    device_dimmer_send_power.Set_entity_valuemax("100");
    device_dimmer_send_power.Set_entity_valuestep("1");
    device_dimmer_send_power.Set_entity_qos(0);
    device_dimmer_send_power.Set_retain_flag(false);

    device_dimmer_charge1.Set_name("Charge 1");
    device_dimmer_charge1.Set_default_entity_id("charge1");
    device_dimmer_charge1.Set_entity_type("number");
    device_dimmer_charge1.Set_entity_category("config");
    device_dimmer_charge1.Set_entity_valuemin("0");
    device_dimmer_charge1.Set_entity_valuemax("3000");
    device_dimmer_charge1.Set_entity_valuestep("50");
    device_dimmer_charge1.Set_entity_qos(0);
    device_dimmer_charge1.Set_retain_flag(false);

    device_dimmer_charge2.Set_name("Charge 2");
    device_dimmer_charge2.Set_default_entity_id("charge2");
    device_dimmer_charge2.Set_entity_type("number");
    device_dimmer_charge2.Set_entity_category("config");
    device_dimmer_charge2.Set_entity_valuemin("0");
    device_dimmer_charge2.Set_entity_valuemax("3000");
    device_dimmer_charge2.Set_entity_valuestep("50");
    device_dimmer_charge2.Set_entity_qos(0);
    device_dimmer_charge2.Set_retain_flag(false);

    device_dimmer_charge3.Set_name("Charge 3");
    device_dimmer_charge3.Set_default_entity_id("charge3");
    device_dimmer_charge3.Set_entity_type("number");
    device_dimmer_charge3.Set_entity_category("config");
    device_dimmer_charge3.Set_entity_valuemin("0");
    device_dimmer_charge3.Set_entity_valuemax("3000");
    device_dimmer_charge3.Set_entity_valuestep("50");
    device_dimmer_charge3.Set_entity_qos(0);
    device_dimmer_charge3.Set_retain_flag(false);

    /// création des select
    device_dimmer_child_mode.Set_name("Mode");
    device_dimmer_child_mode.Set_default_entity_id("child_mode");
    device_dimmer_child_mode.Set_entity_type("select");
    device_dimmer_child_mode.Set_entity_category("config");
    device_dimmer_child_mode.Set_entity_qos(0);
    device_dimmer_child_mode.Set_retain_flag(false);

    // création des binary_sensor
    device_dimmer_alarm_temp.Set_name("Surchauffe");
    device_dimmer_alarm_temp.Set_default_entity_id("alarm_temp");
    device_dimmer_alarm_temp.Set_entity_type("binary_sensor");
    device_dimmer_alarm_temp.Set_entity_category("diagnostic");
    device_dimmer_alarm_temp.Set_dev_cla("problem");
    device_dimmer_alarm_temp.Set_entity_qos(0);
    device_dimmer_alarm_temp.Set_retain_flag(false);

    device_cooler.Set_name("Ventillateur");
    device_cooler.Set_default_entity_id("cooler");
    device_cooler.Set_entity_type("binary_sensor");
    device_cooler.Set_entity_category("diagnostic");
    device_cooler.Set_dev_cla("running");
    device_cooler.Set_entity_qos(0);
    device_cooler.Set_retain_flag(false);
  }
}
#endif
