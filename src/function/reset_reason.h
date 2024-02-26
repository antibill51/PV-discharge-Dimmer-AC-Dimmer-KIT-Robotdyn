#ifndef RESET_REASON
#define RESET_REASON

/// config
// #include "config/enums.h"
#define BOOT_REASON_MESSAGE_SIZE 150

#if defined(ARDUINO_ARCH_ESP32)
#if CONFIG_IDF_TARGET_ESP32 // ESP32/PICO-D4
#include "esp32/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32C6
#include "esp32c6/rom/rtc.h"
#elif CONFIG_IDF_TARGET_ESP32H2
#include "esp32h2/rom/rtc.h"
#else 
#error Target CONFIG_IDF_TARGET is not supported
#endif
        void esp32_cpu_error(int cpu, char *buffer, int bufferlength){

            switch (rtc_get_reset_reason(cpu))
            {
            case 1:
                snprintf(buffer, bufferlength, "POWERON_RESET (Vbat power on reset)");
                break;
            case 3:
                snprintf(buffer, bufferlength, "SW_RESET (Software reset digital core)");
                break;
            case 4:
                snprintf(buffer, bufferlength, "OWDT_RESET (Legacy watch dog reset digital core)");
                break;
            case 5:
                snprintf(buffer, bufferlength, "DEEPSLEEP_RESET (Deep Sleep reset digital core)");
                break;
            case 6:
                snprintf(buffer, bufferlength, "SDIO_RESET (Reset by SLC module, reset digital core)");
                break;
            case 7:
                snprintf(buffer, bufferlength, "TG0WDT_SYS_RESET (Timer Group0 Watch dog reset digital core)");
                break;
            case 8:
                snprintf(buffer, bufferlength, "TG1WDT_SYS_RESET (Timer Group1 Watch dog reset digital core)");
                break;
            case 9:
                snprintf(buffer, bufferlength, "RTCWDT_SYS_RESET (RTC Watch dog Reset digital core)");
                break;
            case 10:
                snprintf(buffer, bufferlength, "INTRUSION_RESET (Instrusion tested to reset CPU)");
                break;
            case 11:
                snprintf(buffer, bufferlength, "TGWDT_CPU_RESET (Time Group reset CPU)");
                break;
            case 12:
                snprintf(buffer, bufferlength, "SW_CPU_RESET (Software reset CPU)");
                break;
            case 13:
                snprintf(buffer, bufferlength, "RTCWDT_CPU_RESET (RTC Watch dog Reset CPU)");
                break;
            case 14:
                snprintf(buffer, bufferlength, "EXT_CPU_RESET (for APP CPU, reseted by PRO CPU)");
                break;
            case 15:
                snprintf(buffer, bufferlength, "RTCWDT_BROWN_OUT_RESET (Reset when the vdd voltage is not stable)");
                break;
            case 16:
                snprintf(buffer, bufferlength, "RTCWDT_RTC_RESET (RTC Watch dog reset digital core and rtc module)");
                break;
            default :
                snprintf(buffer, bufferlength, "NO_MEAN");
                break;
            }
        }


#endif
    void SendBootReasonMessage()
    {
    char bootReasonMessage [BOOT_REASON_MESSAGE_SIZE];
    #if defined(ARDUINO_ARCH_ESP32)
        Serial.println("Reason for reset:");

        for (int i = 0; i < 2; i++) {

            String cpu = "CPU" + String(i);
            Serial.println(cpu);
            esp32_cpu_error(i, bootReasonMessage, BOOT_REASON_MESSAGE_SIZE);
            Serial.println(bootReasonMessage);
            logging.Set_log_init("Reason for reset ",false);
            logging.Set_log_init(cpu.c_str());
            logging.Set_log_init(" : ");
            logging.Set_log_init(String(bootReasonMessage).c_str());
            logging.Set_log_init("\r\n");
        }
    #endif

    #if defined(ARDUINO_ARCH_ESP8266)

        rst_info *resetInfo;

        resetInfo = ESP.getResetInfoPtr();

        switch (resetInfo->reason)
        {

        case REASON_DEFAULT_RST:
            snprintf(bootReasonMessage, BOOT_REASON_MESSAGE_SIZE, "Normal startup by power on");
            break;

        case REASON_WDT_RST:
            snprintf(bootReasonMessage, BOOT_REASON_MESSAGE_SIZE, "Hardware watch dog reset");
            break;

        case REASON_EXCEPTION_RST:
            snprintf(bootReasonMessage, BOOT_REASON_MESSAGE_SIZE, "Exception reset, GPIO status won't change");
            break;

        case REASON_SOFT_WDT_RST:
            snprintf(bootReasonMessage, BOOT_REASON_MESSAGE_SIZE, "Software watch dog reset, GPIO status won't change");
            break;

        case REASON_SOFT_RESTART:
            snprintf(bootReasonMessage, BOOT_REASON_MESSAGE_SIZE, "Software restart ,system_restart , GPIO status won't change");
            break;

        case REASON_DEEP_SLEEP_AWAKE:
            snprintf(bootReasonMessage, BOOT_REASON_MESSAGE_SIZE, "Wake up from deep-sleep");
            break;

        case REASON_EXT_SYS_RST:
            snprintf(bootReasonMessage, BOOT_REASON_MESSAGE_SIZE, "External system reset");
            break;

        default:
            snprintf(bootReasonMessage, BOOT_REASON_MESSAGE_SIZE, "Unknown reset cause %d", resetInfo->reason);
            break;
        };
        Serial.println(bootReasonMessage);
        logging.Set_log_init("Reason for reset: ",false);
        logging.Set_log_init(String(bootReasonMessage).c_str());
        logging.Set_log_init("\r\n");
    #endif
    }
#endif
