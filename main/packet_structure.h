
#include <stdio.h>
#include <stdint.h>


struct s1
{

    uint8_t *frame_header_version;
    uint8_t command_word;
    uint8_t *data_length;
    uint8_t dpID;
    uint8_t type_of_data;
    uint8_t *functional_length;
    uint8_t functional_instruction;
    uint8_t checksum;
};


enum dpID
{
    power = 0x01,
    set_temp,
    operating_mode=0x04,
    wind_speed,
    fault_alarm_1=0x14,
    temp_unit_conversion=0x65,
    strong=0x67,
    eco,
    child_lock,
    sleep=0x6a,
    health,
    very_silent,
    ambient_temp,
    t_on_timer,
    model_logo,
    light=0x71,
    vertical_swing,
    horizontal_swing,
    mildew_proof,
    electric_heating,
    automatic_heating,
    t_off_timer,
    fault_alarm_2,
};


enum data_type
{
    Value,
    Enum,
    Fault,
    Bool,
    
};


enum func
{
    Hearbeat_detection,
    Query_product_info,
    query_of_working_mode_of_MCU_setting_module,
    report_of_wifi_status,
    resest_wifi,
    reset_wifi_selection_mode,
    query_MCU_working_status,
    upgrade_start,
    upgrade_package_transfer,
    get_local_time,
    wifi_function_production_test,
};

enum base_protocol
{
    frame_heade_ver,
    com_word,
    data_len,
    dat,
    chksum,

};











