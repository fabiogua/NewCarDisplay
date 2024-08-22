#ifndef HARDWARE_DISPLAY_H
#define HARDWARE_DISPLAY_H

#define LVGL_TICK 10     // Time tick value for lvgl in ms (1-10msa)
#define TICKER_TIME 0.01 // modified to miliseconds

Ticker ticker; // Initialize your system tick

#include "interface/ICommunication.h"
#include "lvgl.h"

#include "../lib/hal_stm_lvgl/touchpad/touchpad.h"
#include "../lib/hal_stm_lvgl/tft/tft.h"

#include "../lib/display/src/ui.h"

#define MIN_BATTERY_TEMP 0.0
#define MAX_BATTERY_TEMP 60.0

#define MIN_BATTERY_VOLTAGE 307.2
#define MAX_BATTERY_VOLTAGE 403.2

#define MIN_AIR_TEMP 0.0
#define MAX_AIR_TEMP 60.0

#define MIN_MOTOR_TEMP 0.0
#define MAX_MOTOR_TEMP 60.0

#define MAX_POWER 120000
#define MAX_RECU_POWER -15000

typedef uint8_t display_status_t;

class HardwareDisplay : public ICommunication
{
public:
    HardwareDisplay()
    {
        setComponentType(COMPONENT_DISPLAY);
        setObjectType(OBJECT_HARDWARE);

        setup();
    }

    HardwareDisplay(id_sub_component_t componentSubId) : HardwareDisplay()
    {
        setComponentSubId(componentSubId);
    }

    void lv_task()
    {
        lv_task_handler();
    }

    void setCurrent(uint16_t value)
    {
        _current = value;
    }

    void setBatteryTemperature(float value)
    {
        _batteryTemperature = value;
        lv_label_set_text_fmt(ui_BatteryTempLabel, "%d°C", (uint32_t)_batteryTemperature);
        lv_bar_set_value(ui_BatteryTempBar, (uint8_t)((_batteryTemperature - MIN_BATTERY_TEMP) / (MAX_BATTERY_TEMP - MIN_BATTERY_TEMP) * 100.0), LV_ANIM_OFF);
    }

    void setBatteryVoltage(float value)
    {
        _batteryVoltage = value;
        uint8_t batteryPercentage = (uint8_t)((_batteryVoltage - MIN_BATTERY_VOLTAGE) / (MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE) * 100.0);
        lv_bar_set_value(ui_BatteryPercentBar, batteryPercentage, LV_ANIM_OFF);
        lv_bar_set_value(ui_BatteryPercentBar1, batteryPercentage, LV_ANIM_OFF);
        lv_bar_set_value(ui_BatteryVoltageBar, batteryPercentage, LV_ANIM_OFF);
        lv_arc_set_value(ui_BatteryArc, batteryPercentage);
        lv_label_set_text_fmt(ui_BatteryPercentLabel, "%d%%", batteryPercentage);
        lv_label_set_text_fmt(ui_BatteryPercentLabel1, "%d%%", batteryPercentage);
        lv_label_set_text_fmt(ui_BatteryVoltageLabel, "%dV", (uint32_t)_batteryVoltage);
    }

    void setMaxCellVoltage(float value)
    {
        _maxCellVoltage = value;
        lv_label_set_text_fmt(ui_MaxCellVoltageLabel, "%dV", (uint32_t)_maxCellVoltage);
    }

    void setMinCellVoltage(float value)
    {
        _minCellVoltage = value;
        lv_label_set_text_fmt(ui_MinCellVoltageLabel, "%dV", (uint32_t)_minCellVoltage);
    }

    void setMinCellTemperature(float value)
    {
        _minCellTemperature = value;
        lv_label_set_text_fmt(ui_MinCellTempLabel, "%d°C", (uint32_t)_minCellTemperature);
    }

    void setMaxCellTemperature(float value)
    {
        _maxCellTemperature = value;
        lv_label_set_text_fmt(ui_MaxCellTempLabel, "%d°C", (uint32_t)_maxCellTemperature);
    }

    void setAirTemperature(float value)
    {
        _airTemperature = value;
        lv_label_set_text_fmt(ui_AirTempLabel, "%d°C", (uint32_t)_airTemperature);
        lv_bar_set_value(ui_AirTempBar, (uint8_t)((_airTemperature - MIN_AIR_TEMP) / (MAX_AIR_TEMP - MIN_AIR_TEMP) * 100.0), LV_ANIM_OFF);
    }

    void setMotorTemperature(float value)
    {
        _motorTemperature = value;
        lv_label_set_text_fmt(ui_MotorTempLabel, "%d°C", (uint32_t)_motorTemperature);
        lv_bar_set_value(ui_MotorTempBar, (uint8_t)((_motorTemperature - MIN_MOTOR_TEMP) / (MAX_MOTOR_TEMP - MIN_MOTOR_TEMP) * 100.0), LV_ANIM_OFF);
    }

    void calcPower()
    {
        _power = _batteryVoltage * _current;
        if (_power >= 0)
        {
            lv_arc_set_value(ui_PowerArc, (uint8_t)((_power - 0) / MAX_POWER - 0) * 100.0);
            lv_arc_set_value(ui_PowerArc1, (uint8_t)((_power - 0) / MAX_POWER - 0) * 100.0);
        }
        else
        {
            lv_arc_set_value(ui_RecuArc, (uint8_t)((_power - MAX_RECU_POWER) / 0 - MAX_RECU_POWER) * 100.0);
            lv_arc_set_value(ui_RecuArc1, (uint8_t)((_power - MAX_RECU_POWER) / 0 - MAX_RECU_POWER) * 100.0);
        }
    }

    void setState(uint8_t value)
    {
        _state = value;

        switch (_state)
        {
        case 0:
            lv_label_set_text(ui_StateLabel, "OFF");
            lv_label_set_text(ui_StateLabel1, "OFF");
            break;
        case 1:
            lv_label_set_text(ui_StateLabel, "LV NOT CALIBRATED");
            lv_label_set_text(ui_StateLabel1, "LV NOT CALIBRATED");
            break;
        case 2:
            lv_label_set_text(ui_StateLabel, "LV CALIBRATED");
            lv_label_set_text(ui_StateLabel1, "LV CALIBRATED");
            break;
        case 3:
            lv_label_set_text(ui_StateLabel, "HV NOT CALIBRATED");
            lv_label_set_text(ui_StateLabel1, "HV NOT CALIBRATED");
            break;
        case 4:
            lv_label_set_text(ui_StateLabel, "HV CALIBRATED");
            lv_label_set_text(ui_StateLabel1, "HV CALIBRATED");
            break;
        case 5:
            lv_label_set_text(ui_StateLabel, "LV CALIBRATING");
            lv_label_set_text(ui_StateLabel1, "LV CALIBRATING");
            break;
        case 6:
            lv_label_set_text(ui_StateLabel, "HV CALIBRATING");
            lv_label_set_text(ui_StateLabel1, "HV CALIBRATING");
            break;
        case 7:
            lv_label_set_text(ui_StateLabel, "DRIVE");
            lv_label_set_text(ui_StateLabel1, "DRIVE");
            break;
        case 8:
            lv_label_set_text(ui_StateLabel, "LAUNCH CONTROL");
            lv_label_set_text(ui_StateLabel1, "LAUNCH CONTROL");
            break;
        case 9:
            lv_label_set_text(ui_StateLabel, "CAR ERROR");
            lv_label_set_text(ui_StateLabel1, "CAR ERROR");
            break;
        default:
            lv_label_set_text(ui_StateLabel, "UNKNOWN");
            lv_label_set_text(ui_StateLabel1, "UNKNOWN");
            break;
        }
    }
    void setPowermode(uint8_t value)
    {
        _powermode = value;
        switch (_powermode)
        {
        case 0:
            // 40KW
            lv_label_set_text(ui_PowermodeLabel, "40KW");
            lv_label_set_text(ui_PowermodeLabel1, "40KW");
            break;
        case 1:
            // 60KW
            lv_label_set_text(ui_PowermodeLabel, "60KW");
            lv_label_set_text(ui_PowermodeLabel1, "60KW");
            break;
        case 2:
            // 80KW
            lv_label_set_text(ui_PowermodeLabel, "80KW");
            lv_label_set_text(ui_PowermodeLabel1, "80KW");
            break;
        case 3:
            // 60KW LC
            lv_label_set_text(ui_PowermodeLabel, "60KW LC");
            lv_label_set_text(ui_PowermodeLabel1, "60KW LC");
            break;
        case 4:
            // 80KW LC
            lv_label_set_text(ui_PowermodeLabel, "80KW LC");
            lv_label_set_text(ui_PowermodeLabel1, "80KW LC");
            break;
        default:
            // UNKNOWN
            lv_label_set_text(ui_PowermodeLabel, "UNKNOWN");
            lv_label_set_text(ui_PowermodeLabel1, "UNKNOWN");
            break;
        }
    }

    void setShutdownError(uint8_t value)
    {
        // 0noerror, 1 IMD ERROR, 2 BSPD, 3 PRE BSPD, 4 TS on
        _shutdownError = value;
        switch (_shutdownError)
        {
        case 0:
            lv_label_set_text(ui_ShutdownLabel, "NO ERROR");
            lv_label_set_text(ui_ShutdownLabel1, "NO ERROR");
            break;
        case 1:
            lv_label_set_text(ui_ShutdownLabel, "IMD ERROR");
            lv_label_set_text(ui_ShutdownLabel1, "IMD ERROR");
            break;
        case 2:
            lv_label_set_text(ui_ShutdownLabel, "BSPD");
            lv_label_set_text(ui_ShutdownLabel1, "BSPD");
            break;
        case 3:
            lv_label_set_text(ui_ShutdownLabel, "PRE BSPD");
            lv_label_set_text(ui_ShutdownLabel1, "PRE BSPD");
            break;
        case 4:
            lv_label_set_text(ui_ShutdownLabel, "TS ON");
            lv_label_set_text(ui_ShutdownLabel1, "TS ON");
            break;
        default:
            lv_label_set_text(ui_ShutdownLabel, "UNKNOWN");
            lv_label_set_text(ui_ShutdownLabel1, "UNKNOWN");
            break;
        }
    }

    void setSpeed(float value)
    {
        _speed = value;
        lv_label_set_text_fmt(ui_SpeedLabel, "%d", (uint8_t)_speed);
    }
    void setGas(float value)
    {
        _gas = value;
        lv_bar_set_value(ui_GasBar, (uint8_t)(_gas * 100), LV_ANIM_OFF);
    }
    void setBrake(float value)
    {
        _brake = value;
        lv_bar_set_value(ui_BrakeBar, (uint8_t)(_brake * 100), LV_ANIM_OFF);
    }

    void setStatus(display_status_t status)
    {
        _status = status;
    }

    display_status_t getStatus()
    {
        return _status;
    }

    message_build_result_t buildMessage(CarMessage &carMessage)
    {
        car_sub_message_t subMessage;
        subMessage.length = 2;
        subMessage.data[0] = 0;
        subMessage.data[1] = 1;
        carMessage.addSubMessage(subMessage);

        return MESSAGE_BUILD_OK;
    }

    message_parse_result_t parseMessage(CarMessage &carMessage)
    {
        message_parse_result_t result = MESSAGE_PARSE_OK;
        for (car_sub_message_t &subMessage : carMessage.subMessages)
        {
            if (subMessage.length != 7) // not a valid message
                    result = MESSAGE_PARSE_ERROR;

            uint8_t msgId;

            printf("Message received\n");
            printf("Data: %d %d %d %d %d %d %d\n", subMessage.data[0], subMessage.data[1], subMessage.data[2], subMessage.data[3], subMessage.data[4], subMessage.data[5], subMessage.data[6]);

            // msgId = subMessage.data[0];
            // if (msgId == 0)
            // {

            //     // subMessage.data[1] = (uint8_t)_status;

            //     setShutdownError(subMessage.data[2]);

            //     uint16_t speed = (uint16_t)subMessage.data[3] | (subMessage.data[4] << 8);
            //     setSpeed((float)speed / 100.0);

            //     uint16_t current = (uint16_t)subMessage.data[5] | (subMessage.data[6] << 8);
            //     setCurrent(current);
            // }
            // else if (msgId == 1)
            // {

            //     uint16_t motorTemp = (uint16_t)subMessage.data[1] << 8 | subMessage.data[2];
            //     setMotorTemperature((float)motorTemp / 100.0);

            //     uint16_t airTemp = (uint16_t)subMessage.data[3] << 8 | subMessage.data[4];
            //     setAirTemperature((float)airTemp / 100.0);

            //     uint16_t dcVoltage = (uint16_t)subMessage.data[5] << 8 | subMessage.data[6];
            //     setBatteryVoltage((float)dcVoltage / 100.0);
            // }
            // else if (msgId == 2)
            // {
            //     setCurrent(subMessage.data[1] << 8 | subMessage.data[2]);
            //     setState(subMessage.data[3]);
            //     setPowermode(subMessage.data[4]);
            //     calcPower();
            // }
            // else if (msgId == 3)
            // {
            //     uint16_t gas = (uint16_t)subMessage.data[1] | (subMessage.data[2] << 8);
            //     setGas((float)gas / 65535.0);

            //     uint16_t brake = (uint16_t)subMessage.data[3] | (subMessage.data[4] << 8);
            //     setBrake((float)brake / 65535.0);
            // }
            // else
            //     result = MESSAGE_PARSE_ERROR;
        }

        return result;
    }

    static void lv_ticker_func()
    {
        lv_tick_inc(LVGL_TICK); 
    }

    void setup()
    {
        lv_init();
        tft_init();      // Initialize diplay
        touchpad_init(); // Initialize touchpad

        ticker.attach(callback(&lv_ticker_func), TICKER_TIME); // Attach callback to ticker
        ui_init();
    }

private:
    display_status_t _status = 0;
    float _batteryVoltage = 403.2f;
    float _maxCellVoltage = 4.2f;
    float _minCellVoltage = 3.2f;
    float _minCellTemperature = 0.0f;
    float _maxCellTemperature = 40.0f;
    float _batteryTemperature = 40.0f;
    bool _imd = true;
    bool _bms = true;
    float _motorTemperature = 40.0f;
    float _airTemperature = 40.0f;
    float _speed = 23.4f;
    uint8_t _state = 0;
    uint8_t _powermode = 0;
    uint32_t _power = 0;
    uint16_t _current;
    uint8_t _shutdownError = 0;
    float _gas = 0.0f;
    float _brake = 0.0f;
};

#endif // HARDWARE_DISPLAY_H