#include "smartwin_devices.h"
#include "smartwin_cmd.h"

namespace smartwin {

smartwin_devices::smartwin_devices() {

    pthread_mutex_init(&keyinput_list_mutex_, NULL);
    pthread_mutex_init(&search_card_list_mutex_, NULL);
    pthread_mutex_init(&tpinput_list_mutex_, NULL);
    pthread_mutex_init(&icstatus_list_mutex_, NULL);
    pthread_mutex_init(&recv_list_mutex_, NULL);

    if(_comm == nullptr) {
        // 安全芯片端口: /dev/ttyS1, 波特率: 460800
        _comm = new smartwin_comm("/dev/ttyS1", 460800, 500, [&](std::vector<uint8_t> buf) {

            printf("callback: %s\n", _comm->printBuf("recv: ", buf).c_str());

            // 读取键盘输入
            if (CMD_READ_KEYBOARD_INPUT == buf[0] && 0x4F == buf[1]) {
                pthread_mutex_lock(&keyinput_list_mutex_);
                keyinput_list.push_back(buf);
                pthread_mutex_unlock(&keyinput_list_mutex_);
            }
            else if (CMD_SEARCH_CARD_START == buf[0] && 0x4F == buf[1]) {
                pthread_mutex_lock(&search_card_list_mutex_);
                search_card_list.push_back(buf);
                pthread_mutex_unlock(&search_card_list_mutex_);
            } 
            else if (CMD_GET_TOUCH_COORDINATE == buf[0] && 0x4F == buf[1])
            {
                pthread_mutex_lock(&tpinput_list_mutex_);
                tpinput_list.push_back(buf);
                pthread_mutex_unlock(&tpinput_list_mutex_);
            }
            else if (CMD_CHECK_IC_STATUS == buf[0] && 0x4F == buf[1])
            {
                pthread_mutex_lock(&icstatus_list_mutex_);
                icstatus_list.push_back(buf);
                pthread_mutex_unlock(&icstatus_list_mutex_);
            }
            else {
                pthread_mutex_lock(&recv_list_mutex_);
                recv_list.push_back(buf);
                pthread_mutex_unlock(&recv_list_mutex_);
            }
        });
    }
}

smartwin_devices::~smartwin_devices() {
    if(_comm != nullptr) {
        delete _comm;
    }
}

int smartwin_devices::send_request_cmd(uint8_t cmd, std::vector<uint8_t> params){
    std::vector<uint8_t> buf;

    buf.push_back(cmd);
    buf.push_back(0x2F);
    buf.push_back(params.size()/256);
    buf.push_back(params.size()%256);

    for(auto param : params) {
        buf.push_back(param);
    }
    return _comm->sendcmd(buf);
}

int smartwin_devices::recv_from_list(int8_t cmd, std::vector<uint8_t> &buf){
    int ret = SDK_TIMEOUT;
    int timeout = recv_timeout;
    while (timeout > 0)
    {
        if (recv_list.size() > 0)
        {
            for(auto recv_buf : recv_list) {
                if (recv_buf[0] == cmd && recv_buf[1] == 0x4F)
                {
                    std::vector<uint8_t> tmp = recv_buf;

                    pthread_mutex_lock(&recv_list_mutex_);
                    recv_list.erase(recv_list.begin());
                    pthread_mutex_unlock(&recv_list_mutex_);

                    int ln = tmp[2] * 256 + tmp[3];
                    if(ln >= 4) {

                        ret = tmp[4];
                        ret = (ret<<8) + tmp[5];
                        ret = (ret<<8) + tmp[6];
                        ret = (ret<<8) + tmp[7];

                        if(ret == 0) {
                            buf = std::vector<uint8_t>(tmp.begin() + 8, tmp.begin() + 8 + ln - 4);
                        }
                    }

                    printf("recv costed time: %d ms\n", recv_timeout - timeout);

                    return ret; 
                }
                else {
                    pthread_mutex_lock(&recv_list_mutex_);
                    recv_list.erase(recv_list.begin());
                    pthread_mutex_unlock(&recv_list_mutex_);
                }
            }
        }
        
        usleep(1000);
        timeout--;
    }

    printf("Err. recv timeout: %d ms\n", recv_timeout - timeout);
    
    return ret;
}

std::vector<uint8_t> smartwin_devices::lvar_to_vector(std::vector<uint8_t> buf) {
    if (buf.size() < 1)
    {
        return std::vector<uint8_t>();
    }   

    int ln = buf[0];
    if (buf.size() < 1 + ln) {
        return std::vector<uint8_t>();
    }
    return std::vector<uint8_t>(buf.begin() + 1, buf.begin() + 1 + ln);
}

std::vector<uint8_t> smartwin_devices::llvar_to_vector(std::vector<uint8_t> buf) {
    if (buf.size() < 2)
    {
        return std::vector<uint8_t>();
    }
    
    int ln = buf[0] * 256 + buf[1];
    if (buf.size() < 2 + ln) {
        return std::vector<uint8_t>();
    }
    return std::vector<uint8_t>(buf.begin() + 2, buf.begin() + 2 + ln);
}

int smartwin_devices::set_communication_mode(uint8_t mode) {
    send_request_cmd(CMD_SET_COMM_MODE, {mode});
    
    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SET_COMM_MODE, buf);

    return ret;
}

int smartwin_devices::get_network_mode(uint8_t &mode) {
    send_request_cmd(CMD_GET_NETWORK_MODE, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_GET_NETWORK_MODE, buf);

    if (ret == SDK_OK) {
        mode = buf[0];
    }
    
    return ret;
}

int smartwin_devices::get_system_version(uint8_t type, std::vector<uint8_t>& version) {
    send_request_cmd(CMD_GET_SYSTEM_VERSION, {type});
    
    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_GET_SYSTEM_VERSION, buf);
    if(ret == SDK_OK) {
        version = lvar_to_vector(buf);
        return SDK_OK;
    }
    return SDK_ERROR;
}

int smartwin_devices::get_hardware_serial_number(std::vector<uint8_t>& serial_number) {
    send_request_cmd(CMD_GET_HARDWARE_SERIAL_NUMBER, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_GET_HARDWARE_SERIAL_NUMBER, buf);
    if(ret == SDK_OK) {
        serial_number = lvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::get_device_model(uint8_t &model) {
    send_request_cmd(CMD_GET_DEVICE_MODEL, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_GET_DEVICE_MODEL, buf);
    if(ret == SDK_OK) {
        model = buf[0];
    }
    return ret;
}

int smartwin_devices::get_customer_serial_number(std::vector<uint8_t>& custom_serial_number) {
    send_request_cmd(CMD_GET_CUSTOMER_SERIAL_NUMBER, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_GET_CUSTOMER_SERIAL_NUMBER, buf);
    if(ret == SDK_OK) {
        custom_serial_number = lvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::set_clock(std::vector<uint8_t> time) {
    send_request_cmd(CMD_SET_CLOCK, time);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SET_CLOCK, buf);
    return ret;
}

int smartwin_devices::get_clock(std::vector<uint8_t>& time) {
    send_request_cmd(CMD_GET_CLOCK, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_GET_CLOCK, buf);
    if(ret == SDK_OK) {
        time = buf;
    }
    return ret;
}

int smartwin_devices::beep(uint8_t type) {
    send_request_cmd(CMD_BEEP, {type});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_BEEP, buf);
    return ret;
}

int smartwin_devices::beep_frequency(uint32_t frequency, uint32_t duration) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((frequency>>24)&0xFF));
    tmp.push_back((uint8_t)((frequency>>16)&0xFF));
    tmp.push_back((uint8_t)((frequency>>8)&0xFF));
    tmp.push_back((uint8_t)(frequency&0xFF));
    tmp.push_back((uint8_t)((duration>>24)&0xFF));
    tmp.push_back((uint8_t)((duration>>16)&0xFF));
    tmp.push_back((uint8_t)((duration>>8)&0xFF));
    tmp.push_back((uint8_t)(duration&0xFF));
    send_request_cmd(CMD_BEEP_FREQUENCY, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_BEEP_FREQUENCY, buf);
    return ret;
}

  
int smartwin_devices::led_on(uint32_t color) {
    std::vector<uint8_t> tmp;   
    tmp.push_back((uint8_t)((color>>24)&0xFF));
    tmp.push_back((uint8_t)((color>>16)&0xFF));
    tmp.push_back((uint8_t)((color>>8)&0xFF));
    tmp.push_back((uint8_t)(color&0xFF));
    send_request_cmd(CMD_LED_ON, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_LED_ON, buf);
    return ret;
}

int smartwin_devices::led_off(uint32_t color) {
    std::vector<uint8_t> tmp;   
    tmp.push_back((uint8_t)((color>>24)&0xFF));
    tmp.push_back((uint8_t)((color>>16)&0xFF));
    tmp.push_back((uint8_t)((color>>8)&0xFF));
    tmp.push_back((uint8_t)(color&0xFF));
    send_request_cmd(CMD_LED_OFF, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_LED_OFF, buf);
    return ret;
}

int smartwin_devices::led_flash(uint32_t color, uint32_t duration) {
    std::vector<uint8_t> tmp;   
    tmp.push_back((uint8_t)((color>>24)&0xFF));
    tmp.push_back((uint8_t)((color>>16)&0xFF));
    tmp.push_back((uint8_t)((color>>8)&0xFF));
    tmp.push_back((uint8_t)(color&0xFF));
    tmp.push_back((uint8_t)((duration>>24)&0xFF));
    tmp.push_back((uint8_t)((duration>>16)&0xFF));
    tmp.push_back((uint8_t)((duration>>8)&0xFF));
    tmp.push_back((uint8_t)(duration&0xFF));
    send_request_cmd(CMD_LED_FLASH, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_LED_FLASH, buf);
    return ret;
}

int smartwin_devices::system_reset() {
    send_request_cmd(CMD_SYSTEM_RESET, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SYSTEM_RESET, buf);
    return ret;
}

int smartwin_devices::system_shutdown() {
    send_request_cmd(CMD_SYSTEM_SHUTDOWN, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SYSTEM_SHUTDOWN, buf);
    return ret;
}

int smartwin_devices::set_terminal_serial_number(std::vector<uint8_t> serial_number, std::vector<uint8_t> custom_serial_number) {
    std::vector<uint8_t> tmp;
    tmp.push_back(serial_number.size());
    for(size_t i = 0; i < serial_number.size(); i++) {
        tmp.push_back(serial_number[i]);
    }
    tmp.push_back(custom_serial_number.size());
    for(size_t i = 0; i < custom_serial_number.size(); i++) {
        tmp.push_back(custom_serial_number[i]);
    }
    send_request_cmd(CMD_SET_TERMINAL_SERIAL_NUMBER, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SET_TERMINAL_SERIAL_NUMBER, buf);
    return ret;
}

int smartwin_devices::get_chip_serial_number(std::vector<uint8_t>& serial_number) {     
    send_request_cmd(CMD_GET_CHIP_SERIAL_NUMBER, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_GET_CHIP_SERIAL_NUMBER, buf);
    if(ret == SDK_OK) {
        serial_number = lvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::set_enable_sleep_mode(uint32_t wakeup_type) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((wakeup_type>>24)&0xFF));
    tmp.push_back((uint8_t)((wakeup_type>>16)&0xFF));
    tmp.push_back((uint8_t)((wakeup_type>>8)&0xFF));
    tmp.push_back((uint8_t)(wakeup_type&0xFF));
                
    send_request_cmd(CMD_ENABLE_SLEEP_MODE, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_ENABLE_SLEEP_MODE, buf);
    return ret;
}

int smartwin_devices::get_enter_boot_state(uint8_t type, uint32_t& state) {
    send_request_cmd(CMD_ENTER_BOOT_OR_QUERY_STATE, {type});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_ENTER_BOOT_OR_QUERY_STATE, buf);
    if(ret == SDK_OK) {
        state = buf[0] * 256 * 256 * 256 + buf[1] * 256 * 256 + buf[2] * 256 + buf[3];
    }
    return ret;
}       

int smartwin_devices::keyboard_open() {
    pthread_mutex_lock(&keyinput_list_mutex_);
    keyinput_list.clear();
    pthread_mutex_unlock(&keyinput_list_mutex_);

    send_request_cmd(CMD_OPEN_KEYBOARD, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_OPEN_KEYBOARD, buf);
    return ret;
}

int smartwin_devices::keyboard_close() {
    send_request_cmd(CMD_CLOSE_KEYBOARD, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_CLOSE_KEYBOARD, buf);
    return ret; 
}

int smartwin_devices::keyboard_clear_cache() {
    pthread_mutex_lock(&keyinput_list_mutex_);
    keyinput_list.clear();
    pthread_mutex_unlock(&keyinput_list_mutex_);
    
    send_request_cmd(CMD_CLEAR_KEYBOARD_CACHE, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_CLEAR_KEYBOARD_CACHE, buf);
    return ret;
}

int smartwin_devices::keyboard_get_input(uint8_t& key) {
    int ret = SDK_OK;

    if(keyinput_list.size() > 0) {
        std::vector<uint8_t> buf = keyinput_list[0];

        pthread_mutex_lock(&keyinput_list_mutex_);
        keyinput_list.erase(keyinput_list.begin());
        pthread_mutex_unlock(&keyinput_list_mutex_);

        uint32_t key_tmp = buf[4];
        key_tmp = (key_tmp<<8) + buf[5];
        key_tmp = (key_tmp<<8) + buf[6];
        key_tmp = (key_tmp<<8) + buf[7];
        key = (uint8_t)key_tmp;

    } else {
        ret = SDK_ERROR;
    }
    return ret;
}

int smartwin_devices::keyboard_set_sound(uint8_t sound) {
    send_request_cmd(CMD_SET_KEYBOARD_SOUND, {sound});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SET_KEYBOARD_SOUND, buf);
    return ret;
}

int smartwin_devices::keyboard_set_backlight(uint8_t backlight) {
    send_request_cmd(CMD_SET_KEYBOARD_BACKLIGHT, {backlight});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SET_KEYBOARD_BACKLIGHT, buf);
    return ret;
}

/**
 * @brief TP主动上报
 */
#define SDK_TP_REPORT_NO  (0x01)              /**< 不主动上报 */
#define SDK_TP_REPORT_YES  (0x00)            /**< 主动上报 */

int smartwin_devices::tp_open() {
    pthread_mutex_lock(&tpinput_list_mutex_);
    tpinput_list.clear();
    pthread_mutex_unlock(&tpinput_list_mutex_);

    send_request_cmd(CMD_OPEN_TP, {SDK_TP_REPORT_YES});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_OPEN_TP, buf);
    return ret;
}

int smartwin_devices::tp_close() {

    send_request_cmd(CMD_CLOSE_TP, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_CLOSE_TP, buf);
    return ret;
}

int smartwin_devices::tp_check_support() {
    send_request_cmd(CMD_CHECK_TP_SUPPORT, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_CHECK_TP_SUPPORT, buf);
    return ret;
}

int smartwin_devices::tp_get_touch_coordinate(uint32_t &x, uint32_t &y) {
    // send_request_cmd(CMD_GET_TOUCH_COORDINATE, {});

    // std::vector<uint8_t> buf;
    // int ret = recv_from_list  (CMD_GET_TOUCH_COORDINATE, buf);
    // if(ret == SDK_OK) {
    //     x = buf[0] * 256 * 256 * 256 + buf[1] * 256 * 256 + buf[2] * 256 + buf[3];
    //     y = buf[4] * 256 * 256 * 256 + buf[5] * 256 * 256 + buf[6] * 256 + buf[7];
    // }

    int ret = SDK_ERROR;
    if(tpinput_list.size() > 0) {
        std::vector<uint8_t> buf = tpinput_list[tpinput_list.size() - 1];

        pthread_mutex_lock(&tpinput_list_mutex_);
        tpinput_list.clear();
        pthread_mutex_unlock(&tpinput_list_mutex_);

        x = buf[4] * 256 + buf[5];
        y = buf[6] * 256 + buf[7];

        y = 239 - y;        //将触摸原点从左下角调整为左上角

        if(y < 0) {
            y = 0;
        }
        if(y > 239) {
            y = 239;
        }
        if(x < 0) {
            x = 0;
        }
        if(x > 319) {
            x = 319;
        }
        ret = SDK_OK;
    }

    return ret;
}

int smartwin_devices::tp_set_parameter(uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y, uint32_t interval) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((start_x>>24)&0xFF));
    tmp.push_back((uint8_t)((start_x>>16)&0xFF));
    tmp.push_back((uint8_t)((start_x>>8)&0xFF));
    tmp.push_back((uint8_t)(start_x&0xFF));
    tmp.push_back((uint8_t)((start_y>>24)&0xFF));   
    tmp.push_back((uint8_t)((start_y>>16)&0xFF));
    tmp.push_back((uint8_t)((start_y>>8)&0xFF));
    tmp.push_back((uint8_t)(start_y&0xFF));
    tmp.push_back((uint8_t)((end_x>>24)&0xFF));
    tmp.push_back((uint8_t)((end_x>>16)&0xFF));
    tmp.push_back((uint8_t)((end_x>>8)&0xFF));
    tmp.push_back((uint8_t)(end_x&0xFF));   
    tmp.push_back((uint8_t)((end_y>>24)&0xFF));
    tmp.push_back((uint8_t)((end_y>>16)&0xFF));
    tmp.push_back((uint8_t)((end_y>>8)&0xFF));
    tmp.push_back((uint8_t)(end_y&0xFF));
    tmp.push_back((uint8_t)((interval>>24)&0xFF));
    tmp.push_back((uint8_t)((interval>>16)&0xFF));
    tmp.push_back((uint8_t)((interval>>8)&0xFF));
    tmp.push_back((uint8_t)(interval&0xFF));

    send_request_cmd(CMD_SET_TOUCH_PARAMETER, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SET_TOUCH_PARAMETER, buf);
    return ret;
}

int smartwin_devices::magnetic_stripe_card_open() {
    send_request_cmd(CMD_OPEN_MAGNETIC_STRIPE_CARD, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_OPEN_MAGNETIC_STRIPE_CARD, buf);
    return ret;
}

int smartwin_devices::magnetic_stripe_card_close() {
    send_request_cmd(CMD_CLOSE_MAGNETIC_STRIPE_CARD, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_CLOSE_MAGNETIC_STRIPE_CARD, buf);
    return ret;
}

int smartwin_devices::magnetic_stripe_card_check() {
    send_request_cmd(CMD_CHECK_MAGNETIC_STRIPE_CARD, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_CHECK_MAGNETIC_STRIPE_CARD, buf);
    return ret;
}

int smartwin_devices::magnetic_stripe_card_read_data(std::vector<uint8_t>& track1, std::vector<uint8_t>& track2, 
                                                  std::vector<uint8_t>& track3) {
    send_request_cmd(CMD_READ_MAGNETIC_STRIPE_CARD_DATA, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_READ_MAGNETIC_STRIPE_CARD_DATA, buf);  
    if(ret == SDK_OK) {
        int index = 0;
        uint8_t ln = buf[index++];
        track1 = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
        index += ln;
        ln = buf[index++];
        track2 = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
        index += ln;
        ln = buf[index++];
        track3 = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
    }
    return ret;
}

int smartwin_devices::magnetic_stripe_card_clear_data() {
    send_request_cmd(CMD_CLEAR_MAGNETIC_STRIPE_CARD_DATA, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_CLEAR_MAGNETIC_STRIPE_CARD_DATA, buf);
    return ret;
}

int smartwin_devices::magnetic_stripe_card_format_data(std::vector<uint8_t> track1, std::vector<uint8_t> track2, 
        std::vector<uint8_t> track3, std::vector<uint8_t>& card_number, std::vector<uint8_t>& valid_date,
        std::vector<uint8_t>& card_holder_name, std::vector<uint8_t>& service_code) {
    std::vector<uint8_t> tmp;
    tmp.push_back(track1.size()); 
    for(size_t i = 0; i < track1.size(); i++) {
        tmp.push_back(track1[i]);
    }
    tmp.push_back(track2.size());
    for(size_t i = 0; i < track2.size(); i++) {
        tmp.push_back(track2[i]);
    }
    tmp.push_back(track3.size());
    for(size_t i = 0; i < track3.size(); i++) {
        tmp.push_back(track3[i]);
    }

    send_request_cmd(CMD_FORMAT_MAGNETIC_STRIPE_CARD_DATA, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_FORMAT_MAGNETIC_STRIPE_CARD_DATA, buf);
    if(ret == SDK_OK) {
        int index = 0;
        uint8_t ln = buf[index++];
        card_number = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
        index += ln;
        ln = buf[index++];
        valid_date = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
        index += ln;
        ln = buf[index++];
        card_holder_name = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
        index += ln;
        ln = buf[index++];
        service_code = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
    }
    return ret;
}

int smartwin_devices::ic_card_open(uint8_t card_type, uint8_t card_seat) {
    std::vector<uint8_t> tmp;
    tmp.push_back(card_type);
    tmp.push_back(card_seat);
    send_request_cmd(CMD_OPEN_IC_CARD_MODULE, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_OPEN_IC_CARD_MODULE, buf);
    return ret;
}

int smartwin_devices::ic_card_close(uint8_t card_type, uint8_t card_seat) {
    std::vector<uint8_t> tmp;
    tmp.push_back(card_type);
    tmp.push_back(card_seat);
    send_request_cmd(CMD_CLOSE_IC_CARD_MODULE, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_CLOSE_IC_CARD_MODULE, buf);
    return ret;
}

int smartwin_devices::ic_card_check_status(uint8_t card_type, uint8_t card_seat) {

    std::vector<uint8_t> tmp;
    tmp.push_back(card_type);
    tmp.push_back(card_seat);
    
    pthread_mutex_lock(&icstatus_list_mutex_);
    icstatus_list.clear();
    pthread_mutex_unlock(&icstatus_list_mutex_);

    send_request_cmd(CMD_CHECK_IC_STATUS, tmp);

    int timeout = recv_timeout;
    while(timeout > 0) {
        if (icstatus_list.size() > 0) {
            std::vector<uint8_t> buf = icstatus_list[icstatus_list.size() - 1];

            pthread_mutex_lock(&icstatus_list_mutex_);
            icstatus_list.clear();
            pthread_mutex_unlock(&icstatus_list_mutex_);
            
            int status = buf[4];
            status = (status<<8) + buf[5];
            status = (status<<8) + buf[6];
            status = (status<<8) + buf[7];
            return status;
        }
        else {
            usleep(1000);
            timeout--;
        }
    }

    return SDK_TIMEOUT;
}

int smartwin_devices::ic_card_reset(uint8_t card_type, uint8_t card_seat, 
                                  std::vector<uint8_t>& reset_data) {
    std::vector<uint8_t> tmp;
    tmp.push_back(card_type);
    tmp.push_back(card_seat);
    send_request_cmd(CMD_IC_CARD_RESET, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_IC_CARD_RESET, buf);
    if(ret == SDK_OK) {
        reset_data = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::ic_card_power_off(uint8_t card_type, uint8_t card_seat) {

    std::vector<uint8_t> tmp;
    tmp.push_back(card_type);
    tmp.push_back(card_seat);
    send_request_cmd(CMD_IC_CARD_MODULE_POWER_OFF, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_IC_CARD_MODULE_POWER_OFF, buf);
    return ret;
}

int smartwin_devices::ic_card_send_apdu_command(uint8_t card_seat, 
                    std::vector<uint8_t> apdu_command, std::vector<uint8_t>& card_return_data) {
    std::vector<uint8_t> tmp;
    tmp.push_back(card_seat);

    tmp.push_back((uint8_t)((apdu_command.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(apdu_command.size()&0xFF));

    for(size_t i = 0; i < apdu_command.size(); i++) {
        tmp.push_back(apdu_command[i]);
    }
    send_request_cmd(CMD_IC_CARD_SEND_APDU_COMMAND, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_IC_CARD_SEND_APDU_COMMAND, buf);
    if(ret == SDK_OK) {
        card_return_data = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::icc_open_module() {
    send_request_cmd(CMD_ICC_OPEN_MODULE, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_ICC_OPEN_MODULE, buf);
    return ret;
}

int smartwin_devices::icc_close_module() {
    send_request_cmd(CMD_ICC_CLOSE_MODULE, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_ICC_CLOSE_MODULE, buf);
    return ret;
}

int smartwin_devices::icc_search_card_activation(uint8_t card_type, uint8_t &card_type_return, 
                                              std::vector<uint8_t>& serial_number, std::vector<uint8_t>& cid, 
                                              std::vector<uint8_t>& card_response_info) {
    send_request_cmd(CMD_ICC_SEARCH_CARD_ACTIVATION, {card_type});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_ICC_SEARCH_CARD_ACTIVATION, buf);
    if(ret == SDK_OK) {
        int index = 0;
        card_type_return = buf[index++];
        uint8_t ln = buf[index++];
        serial_number = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
        index += ln;
        ln = buf[index++];
        cid = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
        index += ln;
        ln = buf[index++];
        card_response_info = std::vector<uint8_t>(buf.begin() + index, buf.begin() + index + ln);
    }
    return ret;
}

int smartwin_devices::icc_send_apdu_command(std::vector<uint8_t> apdu_command, std::vector<uint8_t>& card_return_data) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((apdu_command.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(apdu_command.size()&0xFF));
    for(size_t i = 0; i < apdu_command.size(); i++) {
        tmp.push_back(apdu_command[i]);
    }
    send_request_cmd(CMD_ICC_SEND_APDU_COMMAND, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_ICC_SEND_APDU_COMMAND, buf);
    if(ret == SDK_OK) {
        card_return_data = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::mifare_card_authentication(uint8_t block_number, uint8_t key_type, 
                                              std::vector<uint8_t> uid, std::vector<uint8_t> auth_key) {
    std::vector<uint8_t> tmp;
    tmp.push_back(block_number);
    tmp.push_back(key_type);
    tmp.push_back((uint8_t)((uid.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(uid.size()&0xFF));
    for(size_t i = 0; i < uid.size(); i++) {   
        tmp.push_back(uid[i]);
    }
    tmp.push_back((uint8_t)((auth_key.size()>>8)&0xFF));
    for(size_t i = 0; i < auth_key.size(); i++) {
        tmp.push_back(auth_key[i]);
    }
    send_request_cmd(CMD_MIFARE_CARD_AUTHENTICATION, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_MIFARE_CARD_AUTHENTICATION, buf);
    return ret;
}

int smartwin_devices::mifare_card_operation(uint8_t operation_instruction, uint8_t block_number, uint8_t target_block_number, 
        std::vector<uint8_t> data, std::vector<uint8_t>& response_data) {
    std::vector<uint8_t> tmp;
    tmp.push_back(operation_instruction);
    tmp.push_back(block_number);
    tmp.push_back(target_block_number);

    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }
    send_request_cmd(CMD_MIFARE_CARD_OPERATION, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_MIFARE_CARD_OPERATION, buf);
    if(ret == SDK_OK) {
        response_data = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::search_card_start(uint8_t search_mode, uint32_t timeout_ms) {
    pthread_mutex_lock(&search_card_list_mutex_);
    search_card_list.clear();
    pthread_mutex_unlock(&search_card_list_mutex_);

    std::vector<uint8_t> tmp;
    tmp.push_back(search_mode);

    tmp.push_back((uint8_t)((timeout_ms>>24)&0xFF));
    tmp.push_back((uint8_t)((timeout_ms>>16)&0xFF));
    tmp.push_back((uint8_t)((timeout_ms>>8)&0xFF));
    tmp.push_back((uint8_t)(timeout_ms&0xFF));
    send_request_cmd(CMD_SEARCH_CARD_START, tmp);

    // std::vector<uint8_t> buf;
    // int ret = recv_from_list(CMD_SEARCH_CARD_START, buf);
    
    return SDK_OK;
}

int smartwin_devices::search_card_get_status(uint8_t &type, uint8_t &key) {
    int ret = SDK_OK;

    if(search_card_list.size() > 0) {
        std::vector<uint8_t> buf = search_card_list[search_card_list.size() - 1];

        pthread_mutex_lock(&search_card_list_mutex_);
        search_card_list.clear();
        pthread_mutex_unlock(&search_card_list_mutex_);

        int ln = buf[2] * 256 + buf[3];
        if(ln >= 4) {
            int code = buf[4];
            code = (code<<8) + buf[5];
            code = (code<<8) + buf[6];
            code = (code<<8) + buf[7];

            type = buf[8];
            if((type & 0x01) == 0x01 && ln >= 6) {
                key = buf[9];
            }

            ret = code;
        }
    } else {
        ret = SDK_ERROR;
    }
    return ret;
}

int smartwin_devices::search_card_stop() {
    send_request_cmd(CMD_SEARCH_CARD_STOP, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SEARCH_CARD_STOP, buf);
    return ret;
}


int smartwin_devices::scan_open() {
    send_request_cmd(CMD_SCAN_OPEN, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SCAN_OPEN, buf);
    return ret;
}

int smartwin_devices::scan_close() {
    send_request_cmd(CMD_SCAN_CLOSE, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SCAN_CLOSE, buf);
    return ret;
}

int smartwin_devices::scan_read_data(uint32_t timeout_ms, std::vector<uint8_t>& data) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((timeout_ms>>24)&0xFF));
    tmp.push_back((uint8_t)((timeout_ms>>16)&0xFF));
    tmp.push_back((uint8_t)((timeout_ms>>8)&0xFF));
    tmp.push_back((uint8_t)(timeout_ms&0xFF));
    send_request_cmd(CMD_READ_SCAN_DATA, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_READ_SCAN_DATA, buf);
    if(ret == SDK_OK) {
        data = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::printer_check_support() {   
    send_request_cmd(CMD_CHECK_PRINTER_SUPPORT, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_CHECK_PRINTER_SUPPORT, buf);
    return ret;
}

int smartwin_devices::printer_open() {
    send_request_cmd(CMD_PRINTER_OPEN, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_PRINTER_OPEN, buf);
    return ret;
}

int smartwin_devices::printer_close() {
    send_request_cmd(CMD_PRINTER_CLOSE, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_PRINTER_CLOSE, buf);
    return ret;
}

int smartwin_devices::printer_query_status() {
    send_request_cmd(CMD_QUERY_PRINTER_STATUS, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_QUERY_PRINTER_STATUS, buf);
    return ret;
}

int smartwin_devices::printer_set_gray(uint8_t gray) {
    send_request_cmd(CMD_SET_PRINTER_GRAY, {gray});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_SET_PRINTER_GRAY, buf);
    return ret;
}

int smartwin_devices::printer_paper_feed(uint32_t pixel_point) {  
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((pixel_point>>24)&0xFF));
    tmp.push_back((uint8_t)((pixel_point>>16)&0xFF));
    tmp.push_back((uint8_t)((pixel_point>>8)&0xFF));
    tmp.push_back((uint8_t)(pixel_point&0xFF));
    send_request_cmd(CMD_PAPER_FEED, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_PAPER_FEED, buf);
    return ret;
}

int smartwin_devices::printer_print_bitmap_data(uint8_t packet_number, std::vector<uint8_t> bitmap_data, 
        uint32_t bitmap_width, uint32_t bitmap_height, uint8_t bitmap_attribute) {
    std::vector<uint8_t> tmp;
    tmp.push_back(packet_number);
    tmp.push_back((uint8_t)((bitmap_data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(bitmap_data.size()&0xFF));
    for(size_t i = 0; i < bitmap_data.size(); i++) {
        tmp.push_back(bitmap_data[i]);
    }
    tmp.push_back((uint8_t)((bitmap_width>>24)&0xFF));
    tmp.push_back((uint8_t)((bitmap_width>>16)&0xFF));
    tmp.push_back((uint8_t)((bitmap_width>>8)&0xFF));
    tmp.push_back((uint8_t)(bitmap_width&0xFF));
    tmp.push_back((uint8_t)((bitmap_height>>24)&0xFF));
    tmp.push_back((uint8_t)((bitmap_height>>16)&0xFF));
    tmp.push_back((uint8_t)((bitmap_height>>8)&0xFF));
    tmp.push_back((uint8_t)(bitmap_height&0xFF));
    tmp.push_back(bitmap_attribute);

    send_request_cmd(CMD_PRINT_BITMAP_DATA, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_PRINT_BITMAP_DATA, buf);
    return ret;
}

int smartwin_devices::keypad_open() {
    send_request_cmd(CMD_KEYPAD_OPEN_PASSWORD, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_OPEN_PASSWORD, buf);
    return ret;
}

int smartwin_devices::keypad_close() {
    send_request_cmd(CMD_KEYPAD_CLOSE_PASSWORD, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_CLOSE_PASSWORD, buf);
    return ret;
}

int smartwin_devices::keypad_get_random_number(uint32_t length, std::vector<uint8_t>& random_number) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((length>>24)&0xFF));
    tmp.push_back((uint8_t)((length>>16)&0xFF));
    tmp.push_back((uint8_t)((length>>8)&0xFF));
    tmp.push_back((uint8_t)(length&0xFF));
    send_request_cmd(CMD_KEYPAD_GET_RANDOM_NUMBER, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_GET_RANDOM_NUMBER, buf);
    if(ret == SDK_OK) {
        random_number = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_update_master_key(uint32_t master_key_index, std::vector<uint8_t> master_key_data, 
        uint32_t decrypt_master_key_index, uint8_t write_mode) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((master_key_index>>24)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>16)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>8)&0xFF));
    tmp.push_back((uint8_t)(master_key_index&0xFF));

    tmp.push_back((uint8_t)((master_key_data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(master_key_data.size()&0xFF));
    for(size_t i = 0; i < master_key_data.size(); i++) {
        tmp.push_back(master_key_data[i]);
    }

    tmp.push_back((uint8_t)((decrypt_master_key_index>>24)&0xFF));
    tmp.push_back((uint8_t)((decrypt_master_key_index>>16)&0xFF));
    tmp.push_back((uint8_t)((decrypt_master_key_index>>8)&0xFF));
    tmp.push_back((uint8_t)(decrypt_master_key_index&0xFF));

    tmp.push_back(write_mode);
    
    send_request_cmd(CMD_KEYPAD_UPDATE_MASTER_KEY, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_UPDATE_MASTER_KEY, buf);
    return ret;
}

int smartwin_devices::keypad_update_work_key(uint32_t master_key_index, std::vector<uint8_t> pin_key_data, 
        std::vector<uint8_t> mac_key_data, std::vector<uint8_t> tdk_key_data, uint8_t write_mode) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((master_key_index>>24)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>16)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>8)&0xFF));
    tmp.push_back((uint8_t)(master_key_index&0xFF));

    tmp.push_back((uint8_t)((pin_key_data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(pin_key_data.size()&0xFF));
    for(size_t i = 0; i < pin_key_data.size(); i++) {
        tmp.push_back(pin_key_data[i]);
    }

    tmp.push_back((uint8_t)((mac_key_data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(mac_key_data.size()&0xFF));
    for(size_t i = 0; i < mac_key_data.size(); i++) {
        tmp.push_back(mac_key_data[i]);
    }

    tmp.push_back((uint8_t)((tdk_key_data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(tdk_key_data.size()&0xFF));
    for(size_t i = 0; i < tdk_key_data.size(); i++) {
        tmp.push_back(tdk_key_data[i]);
    }

    tmp.push_back(write_mode);
    send_request_cmd(CMD_KEYPAD_UPDATE_WORK_KEY, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_UPDATE_WORK_KEY, buf);
    return ret;
}

int smartwin_devices::keypad_encrypt_data(uint32_t master_key_index, uint8_t work_key_type, std::vector<uint8_t> iv, 
        std::vector<uint8_t> data, uint8_t algorithm_mode, uint8_t mode, std::vector<uint8_t>& encrypted_data) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((master_key_index>>24)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>16)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>8)&0xFF));
    tmp.push_back((uint8_t)(master_key_index&0xFF));

    tmp.push_back(work_key_type);

    tmp.push_back((uint8_t)((iv.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(iv.size()&0xFF));
    for(size_t i = 0; i < iv.size(); i++) {
        tmp.push_back(iv[i]);
    }

    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }

    tmp.push_back(algorithm_mode);
    tmp.push_back(mode);

    send_request_cmd(CMD_KEYPAD_ENCRYPT_DATA, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_ENCRYPT_DATA, buf);
    if(ret == SDK_OK) {
        encrypted_data = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_encrypt_magnetic_stripe_data(uint32_t master_key_index, uint8_t magnetic_stripe_encryption_mode, 
        std::vector<uint8_t> magnetic_stripe_data, std::vector<uint8_t>& encrypted_magnetic_stripe_data) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((master_key_index>>24)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>16)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>8)&0xFF));
    tmp.push_back((uint8_t)(master_key_index&0xFF));    

    tmp.push_back(magnetic_stripe_encryption_mode);

    tmp.push_back((uint8_t)((magnetic_stripe_data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(magnetic_stripe_data.size()&0xFF));
    for(size_t i = 0; i < magnetic_stripe_data.size(); i++) {
        tmp.push_back(magnetic_stripe_data[i]);
    }

    send_request_cmd(CMD_KEYPAD_ENCRYPT_MAGNETIC_STRIPE_DATA, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_ENCRYPT_MAGNETIC_STRIPE_DATA, buf);
    if(ret == SDK_OK) {
        encrypted_magnetic_stripe_data = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_calculate_mac(uint32_t master_key_index, std::vector<uint8_t> data, uint8_t mac_algorithm_mode, 
        std::vector<uint8_t>& mac) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((master_key_index>>24)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>16)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>8)&0xFF));
    tmp.push_back((uint8_t)(master_key_index&0xFF));


    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }
    
    tmp.push_back(mac_algorithm_mode);

    send_request_cmd(CMD_KEYPAD_CALCULATE_MAC, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_CALCULATE_MAC, buf);
    if(ret == SDK_OK) {
        mac = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_input_online_pin(uint32_t master_key_index, std::vector<uint8_t> pin_length, uint8_t row_number, uint8_t column_number, 
        std::vector<uint8_t> card_number, uint8_t encryption_mode, uint32_t wait_input_time, std::vector<uint8_t>& encrypted_pin) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((master_key_index>>24)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>16)&0xFF));
    tmp.push_back((uint8_t)((master_key_index>>8)&0xFF));
    tmp.push_back((uint8_t)(master_key_index&0xFF));

    tmp.push_back((uint8_t)((pin_length.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(pin_length.size()&0xFF));
    for(size_t i = 0; i < pin_length.size(); i++) {
        tmp.push_back(pin_length[i]);
    }

    tmp.push_back((uint8_t)((row_number>>24)&0xFF));
    tmp.push_back((uint8_t)((row_number>>16)&0xFF));
    tmp.push_back((uint8_t)((row_number>>8)&0xFF));
    tmp.push_back((uint8_t)(row_number&0xFF));

    tmp.push_back((uint8_t)((column_number>>24)&0xFF));
    tmp.push_back((uint8_t)((column_number>>16)&0xFF));
    tmp.push_back((uint8_t)((column_number>>8)&0xFF));
    tmp.push_back((uint8_t)(column_number&0xFF));

    tmp.push_back((uint8_t)((card_number.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(card_number.size()&0xFF));
    for(size_t i = 0; i < card_number.size(); i++) {
        tmp.push_back(card_number[i]);
    }

    tmp.push_back(encryption_mode); 

    tmp.push_back((uint8_t)((wait_input_time>>24)&0xFF));
    tmp.push_back((uint8_t)((wait_input_time>>16)&0xFF));
    tmp.push_back((uint8_t)((wait_input_time>>8)&0xFF));
    tmp.push_back((uint8_t)(wait_input_time&0xFF));

    send_request_cmd(CMD_KEYPAD_INPUT_ONLINE_PIN, tmp);   

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_INPUT_ONLINE_PIN, buf);
    if(ret == SDK_OK) {
        encrypted_pin = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_generate_rsa_key_pair_output_public_key(uint32_t expected_key_length, std::vector<uint8_t> public_exponent, 
        std::vector<uint8_t>& public_key) {
    std::vector<uint8_t> tmp;   
    tmp.push_back((uint8_t)((expected_key_length>>24)&0xFF));
    tmp.push_back((uint8_t)((expected_key_length>>16)&0xFF));
    tmp.push_back((uint8_t)((expected_key_length>>8)&0xFF));
    tmp.push_back((uint8_t)(expected_key_length&0xFF));

    tmp.push_back((uint8_t)((public_exponent.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(public_exponent.size()&0xFF));
    for(size_t i = 0; i < public_exponent.size(); i++) {
        tmp.push_back(public_exponent[i]);
    }

    send_request_cmd(CMD_KEYPAD_GEN_RSA_KEY_PAIR_OUTPUT_PUBLIC_KEY, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_GEN_RSA_KEY_PAIR_OUTPUT_PUBLIC_KEY, buf);
    if(ret == SDK_OK) {
        public_key = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_encrypt_rsa_private_key(std::vector<uint8_t> input_data, std::vector<uint8_t>& encrypted_data) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((input_data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(input_data.size()&0xFF));
    for(size_t i = 0; i < input_data.size(); i++) {
        tmp.push_back(input_data[i]);
    }

    send_request_cmd(CMD_KEYPAD_ENCRYPT_RSA_PRIVATE_KEY, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_ENCRYPT_RSA_PRIVATE_KEY, buf);
    if(ret == SDK_OK) {
        encrypted_data = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_encrypt_hardware_serial_number(std::vector<uint8_t> data, uint8_t mode, std::vector<uint8_t>& encrypted_data) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }

    tmp.push_back(mode);

    send_request_cmd(CMD_KEYPAD_ENCRYPT_HARDWARE_SERIAL_NUMBER, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_ENCRYPT_HARDWARE_SERIAL_NUMBER, buf);
    if(ret == SDK_OK) {
        encrypted_data = llvar_to_vector(buf);  
    }
    return ret;
}

int smartwin_devices::keypad_check_trigger_status(uint8_t &reset_check_switch, uint8_t &self_destruction_flag) {
    send_request_cmd(CMD_KEYPAD_CHECK_TRIGGER_STATUS, {});

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_CHECK_TRIGGER_STATUS, buf);
    reset_check_switch = buf[0];
    self_destruction_flag = buf[1];
    return ret;
}

int smartwin_devices::keypad_set_trigger_status(uint32_t debounce_time, uint8_t reset_check_switch) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((debounce_time>>24)&0xFF));
    tmp.push_back((uint8_t)((debounce_time>>16)&0xFF));
    tmp.push_back((uint8_t)((debounce_time>>8)&0xFF));
    tmp.push_back((uint8_t)(debounce_time&0xFF));

    tmp.push_back(reset_check_switch);

    send_request_cmd(CMD_KEYPAD_SET_TRIGGER_STATUS, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_SET_TRIGGER_STATUS, buf);
    return ret;
}

int smartwin_devices::keypad_release_trigger(uint8_t mode) {
    std::vector<uint8_t> tmp;
    tmp.push_back(mode);

    send_request_cmd(CMD_KEYPAD_RELEASE_TRIGGER, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_RELEASE_TRIGGER, buf);
    return ret;
}

int smartwin_devices::keypad_check_key(uint8_t key_type, uint32_t key_index, std::vector<uint8_t>& kcv) {
    std::vector<uint8_t> tmp;
    tmp.push_back(key_type);
    tmp.push_back((uint8_t)((key_index>>24)&0xFF));
    tmp.push_back((uint8_t)((key_index>>16)&0xFF));
    tmp.push_back((uint8_t)((key_index>>8)&0xFF));
    tmp.push_back((uint8_t)(key_index&0xFF));

    send_request_cmd(CMD_KEYPAD_CHECK_KEY, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_CHECK_KEY, buf);
    if(ret == SDK_OK) {
        kcv = lvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_sm3_hash_algorithm(std::vector<uint8_t> data, std::vector<uint8_t>& hash) {
    std::vector<uint8_t> tmp;
    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }

    send_request_cmd(CMD_KEYPAD_SM3_HASH, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_SM3_HASH, buf);
    if(ret == SDK_OK) {
        hash = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_des_encrypt_decrypt_algorithm(uint8_t algorithm_mode, uint8_t encrypt_decrypt_mode, 
        std::vector<uint8_t> iv, std::vector<uint8_t> data, std::vector<uint8_t> key, std::vector<uint8_t>& result) {
    std::vector<uint8_t> tmp;
    tmp.push_back(algorithm_mode);
    tmp.push_back(encrypt_decrypt_mode);

    tmp.push_back((uint8_t)((iv.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(iv.size()&0xFF));
    for(size_t i = 0; i < iv.size(); i++) {
        tmp.push_back(iv[i]);
    }

    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }

    tmp.push_back((uint8_t)((key.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(key.size()&0xFF));
    for(size_t i = 0; i < key.size(); i++) {
        tmp.push_back(key[i]);
    }

    send_request_cmd(CMD_KEYPAD_DES_ENCRYPT_DECRYPT, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_DES_ENCRYPT_DECRYPT, buf);
    if(ret == SDK_OK) {
        result = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_aes_encrypt_decrypt_algorithm(uint8_t algorithm_mode, uint8_t encrypt_decrypt_mode, 
        std::vector<uint8_t> iv, std::vector<uint8_t> data, std::vector<uint8_t> key, std::vector<uint8_t>& result  ) {
    std::vector<uint8_t> tmp;
    tmp.push_back(algorithm_mode);
    tmp.push_back(encrypt_decrypt_mode);

    tmp.push_back((uint8_t)((iv.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(iv.size()&0xFF));
    for(size_t i = 0; i < iv.size(); i++) {
        tmp.push_back(iv[i]);
    }

    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }

    tmp.push_back((uint8_t)((key.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(key.size()&0xFF));
    for(size_t i = 0; i < key.size(); i++) {
        tmp.push_back(key[i]);
    }

    send_request_cmd(CMD_KEYPAD_AES_ENCRYPT_DECRYPT, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_AES_ENCRYPT_DECRYPT, buf);
    if(ret == SDK_OK) {
        result = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_sm4_encrypt_decrypt_algorithm(uint8_t algorithm_mode, uint8_t encrypt_decrypt_mode, 
        std::vector<uint8_t> key, std::vector<uint8_t> iv, std::vector<uint8_t> data, std::vector<uint8_t>& result) {
    std::vector<uint8_t> tmp;
    tmp.push_back(algorithm_mode);
    tmp.push_back(encrypt_decrypt_mode);

    for(size_t i = 0; i < 8; i++) {
        tmp.push_back(key[i]);
    }

    tmp.push_back((uint8_t)((iv.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(iv.size()&0xFF));
    for(size_t i = 0; i < iv.size(); i++) {
        tmp.push_back(iv[i]);
    }

    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }

    send_request_cmd(CMD_KEYPAD_SM4_ENCRYPT_DECRYPT, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_SM4_ENCRYPT_DECRYPT, buf);
    if(ret == SDK_OK) {
        result = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::keypad_sm2_encrypt_decrypt_algorithm(uint8_t encrypt_decrypt_mode, std::vector<uint8_t> data, 
        std::vector<uint8_t> key, std::vector<uint8_t>& result){
    std::vector<uint8_t> tmp;
    tmp.push_back(encrypt_decrypt_mode);

    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]); 
    }

    tmp.push_back((uint8_t)((key.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(key.size()&0xFF));
    for(size_t i = 0; i < key.size(); i++) {
        tmp.push_back(key[i]);
    }

    send_request_cmd(CMD_KEYPAD_SM2_ENCRYPT_DECRYPT, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_SM2_ENCRYPT_DECRYPT, buf);
    if(ret == SDK_OK) {
        result = llvar_to_vector(buf);
    }
    return ret;
}       

int smartwin_devices::keypad_sm2_signature_algorithm(std::vector<uint8_t> public_key, std::vector<uint8_t> private_key, 
        std::vector<uint8_t> uid, std::vector<uint8_t> data, std::vector<uint8_t>& result) {
    std::vector<uint8_t> tmp;
    for(size_t i = 0; i < 64; i++) {
        tmp.push_back(public_key[i]);
    }

    for(size_t i = 0; i < 32; i++) {
        tmp.push_back(private_key[i]);
    }

    tmp.push_back((uint8_t)((uid.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(uid.size()&0xFF));
    for(size_t i = 0; i < uid.size(); i++) {
        tmp.push_back(uid[i]);
    }

    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }

    send_request_cmd(CMD_KEYPAD_SM2_SIGN, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_SM2_SIGN, buf);
    if(ret == SDK_OK) {
        result = std::vector<uint8_t>(buf.begin(), buf.begin() + 64);
    }
    return ret;
}

int smartwin_devices::keypad_sm2_verify_algorithm(std::vector<uint8_t> signature, std::vector<uint8_t> public_key, 
        std::vector<uint8_t> uid, std::vector<uint8_t> data, std::vector<uint8_t>& result) {
    std::vector<uint8_t> tmp;
    for(size_t i = 0; i < 64; i++) {
        tmp.push_back(signature[i]);
    }

    for(size_t i = 0; i < 64; i++) {
        tmp.push_back(public_key[i]);
    }
    
    tmp.push_back((uint8_t)((uid.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(uid.size()&0xFF));
    for(size_t i = 0; i < uid.size(); i++) {
        tmp.push_back(uid[i]);
    }   
    
    tmp.push_back((uint8_t)((data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(data.size()&0xFF));
    for(size_t i = 0; i < data.size(); i++) {
        tmp.push_back(data[i]);
    }

    send_request_cmd(CMD_KEYPAD_SM2_VERIFY, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_KEYPAD_SM2_VERIFY, buf);
    return ret;
}

int smartwin_devices::file_download_start_download(uint8_t file_type, uint8_t compress_format, uint8_t parameter_update, uint8_t reserved, 
        uint32_t file_size, uint32_t file_crc32, std::vector<uint8_t> file_name, uint32_t& single_packet_length, uint32_t& start_offset) {
    std::vector<uint8_t> tmp;
    tmp.push_back(file_type);
    tmp.push_back(compress_format);
    tmp.push_back(parameter_update);
    tmp.push_back(reserved);

    tmp.push_back((uint8_t)((file_size>>24)&0xFF));
    tmp.push_back((uint8_t)((file_size>>16)&0xFF));
    tmp.push_back((uint8_t)((file_size>>8)&0xFF));
    tmp.push_back((uint8_t)(file_size&0xFF));

    tmp.push_back((uint8_t)((file_crc32>>24)&0xFF));
    tmp.push_back((uint8_t)((file_crc32>>16)&0xFF));
    tmp.push_back((uint8_t)((file_crc32>>8)&0xFF));
    tmp.push_back((uint8_t)(file_crc32&0xFF));

    tmp.push_back((uint8_t)(file_name.size()&0xFF));
    for(size_t i = 0; i < file_name.size(); i++) {
        tmp.push_back(file_name[i]);
    }

    send_request_cmd(CMD_FILE_DOWNLOAD_START, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_FILE_DOWNLOAD_START, buf);
    if(ret == SDK_OK) {
        single_packet_length = buf[0] *256 + buf[1];
        start_offset = buf[2] *256*256*256 + buf[3]*256*256 + buf[4] *256 + buf[5];
    }
    return ret;
}

int smartwin_devices::file_download(uint8_t packet_number, uint32_t file_data_offset, std::vector<uint8_t> file_data) {
    std::vector<uint8_t> tmp;
    tmp.push_back(packet_number);
    tmp.push_back((uint8_t)((file_data_offset>>24)&0xFF));
    tmp.push_back((uint8_t)((file_data_offset>>16)&0xFF));
    tmp.push_back((uint8_t)((file_data_offset>>8)&0xFF));
    tmp.push_back((uint8_t)(file_data_offset&0xFF));

    tmp.push_back((uint8_t)((file_data.size()>>8)&0xFF));
    tmp.push_back((uint8_t)(file_data.size()&0xFF));
    for(size_t i = 0; i < file_data.size(); i++) {
        tmp.push_back(file_data[i]);
    }

    send_request_cmd(CMD_FILE_DOWNLOAD, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_FILE_DOWNLOAD, buf);
    return ret;
}

int smartwin_devices::internal_authentication(std::vector<uint8_t> random_number_r1, std::vector<uint8_t>& encrypted_random_number_r1, 
        std::vector<uint8_t>& random_number_r2) {
    std::vector<uint8_t> tmp;

    for(size_t i = 0; i < random_number_r1.size(); i++) {
        tmp.push_back(random_number_r1[i]);
    }

    send_request_cmd(CMD_INTERNAL_AUTH, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_INTERNAL_AUTH, buf);
    if(ret == SDK_OK) {
        encrypted_random_number_r1 = std::vector<uint8_t>(buf.begin(), buf.begin() + 8);
        random_number_r2 = std::vector<uint8_t>(buf.begin() + 8, buf.begin() + 16);
    }
    return ret;
}

int smartwin_devices::external_authentication_hardware_serial_number_download(std::vector<uint8_t> encrypted_random_number_r2, uint8_t self_destruction_reset_flag, 
        uint8_t sn_reset_flag, std::vector<uint8_t> date_time, std::vector<uint8_t> hardware_serial_number, 
        std::vector<uint8_t> hardware_serial_number_key, std::vector<uint8_t> master_key, 
        std::vector<uint8_t> customer_custom_serial_number, std::vector<uint8_t> organization_private_key, 
        std::vector<uint8_t> additional_key, std::vector<uint8_t>& hardware_info) {
    std::vector<uint8_t> tmp;
    for(size_t i = 0; i < 8; i++) {
        tmp.push_back(encrypted_random_number_r2[i]);
    }

    tmp.push_back(self_destruction_reset_flag);
    tmp.push_back(sn_reset_flag);

    for(size_t i = 0; i < 7; i++) {
        tmp.push_back(date_time[i]);
    }
    

    tmp.push_back(hardware_serial_number.size()&0xFF);
    for(size_t i = 0; i < hardware_serial_number.size(); i++) {
        tmp.push_back(hardware_serial_number[i]);
    }

    tmp.push_back(hardware_serial_number_key.size()&0xFF);
    for(size_t i = 0; i < hardware_serial_number_key.size(); i++) {
        tmp.push_back(hardware_serial_number_key[i]);
    }

    tmp.push_back(master_key.size()&0xFF);
    for(size_t i = 0; i < master_key.size(); i++) {
        tmp.push_back(master_key[i]);
    }

    tmp.push_back(customer_custom_serial_number.size()&0xFF);
    for(size_t i = 0; i < customer_custom_serial_number.size(); i++) {
        tmp.push_back(customer_custom_serial_number[i]);
    }

    tmp.push_back(organization_private_key.size()&0xFF);
    for(size_t i = 0; i < organization_private_key.size(); i++) {
        tmp.push_back(organization_private_key[i]);
    }

    tmp.push_back(additional_key.size()>>8&0xFF);
    tmp.push_back(additional_key.size()&0xFF);
    for(size_t i = 0; i < additional_key.size(); i++) {
        tmp.push_back(additional_key[i]);
    }

    send_request_cmd(CMD_EXTERNAL_AUTH_DOWNLOAD_HARDWARE_SERIAL, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_EXTERNAL_AUTH_DOWNLOAD_HARDWARE_SERIAL, buf);
    if(ret == SDK_OK) {
        hardware_info = llvar_to_vector(buf);
    }
    return ret;
}

int smartwin_devices::external_authentication_unlock(std::vector<uint8_t> encrypted_r2_key2, uint8_t self_destruction_reset_flag) {
    std::vector<uint8_t> tmp;   
    for(size_t i = 0; i < 8; i++) {
        tmp.push_back(encrypted_r2_key2[i]);
    }

    tmp.push_back(self_destruction_reset_flag); 

    send_request_cmd(CMD_EXTERNAL_AUTH_UNLOCK, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_EXTERNAL_AUTH_UNLOCK, buf);
    return ret;
}

int smartwin_devices::external_authentication_encrypted_chip_id(std::vector<uint8_t> encrypted_r2_key2) {
    std::vector<uint8_t> tmp;   
    for(size_t i = 0; i < 8; i++) {
        tmp.push_back(encrypted_r2_key2[i]);
    }

    send_request_cmd(CMD_EXTERNAL_AUTH_ENCRYPT_CHIP_ID, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_EXTERNAL_AUTH_ENCRYPT_CHIP_ID, buf);
    return ret;
}

int smartwin_devices::external_authentication_reset_boot(std::vector<uint8_t> encrypted_r2_key2) {
    std::vector<uint8_t> tmp;   
    for(size_t i = 0; i < 8; i++) {
        tmp.push_back(encrypted_r2_key2[i]);
    }

    send_request_cmd(CMD_EXTERNAL_AUTH_RESET_BOOT, tmp);

    std::vector<uint8_t> buf;
    int ret = recv_from_list(CMD_EXTERNAL_AUTH_RESET_BOOT, buf);
    return ret;
}

}