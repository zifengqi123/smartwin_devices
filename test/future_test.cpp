#include "smartwin_devices.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

// export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH
smartwin::smartwin_devices* _devices = smartwin::smartwin_devices::getInstance();

std::string vect2str(std::vector<uint8_t> buf) {
    std::string str = "";
    for(int i = 0; i < buf.size(); i++) {
        char tmp[4] = {0};
        sprintf(tmp, "%02X ", buf[i]);
        str += tmp;
    }  
    return str;
}

void test_communication_mode()
{
    int ret = _devices->set_communication_mode(0);
    if (ret != 0) {
        printf("ERROR: set_communication_mode ret: %d\n", ret);
        return;
    }

    uint8_t mode = 0;
    ret = _devices->get_network_mode(mode);
    if (ret != 0) {
        printf("ERROR: get_network_mode ret: %d\n", ret);
        return;
    }
    printf("get_network_mode ret: %d, mode: %d\n", ret, mode);
    
}

void test_version()
{
    std::vector<uint8_t> version;
    int ret = _devices->get_system_version(0x00, version);
    if (ret != 0) {
        printf("ERROR: get_system_version ret: %d\n", ret);
        return;
    }
    printf("get_system_version ret: %d, version: %s\n", ret, version.data());

    std::vector<uint8_t> serial_number = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30};
    std::vector<uint8_t> custom_serial_number = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x31};
    ret = _devices->set_terminal_serial_number(serial_number, custom_serial_number);
    if (ret != 0) {
        printf("ERROR: set_terminal_serial_number ret: %d\n", ret);
        return;
    }
    printf("set_terminal_serial_number ret: %d\n", ret);

    sleep(3);

    ret = _devices->get_hardware_serial_number(version);
    if (ret != 0) {
        printf("ERROR: get_hardware_serial_number ret: %d\n", ret);
        return;
    }
    printf("get_hardware_serial_number ret: %d, serial_number: %s\n", ret, version.data());

    ret = _devices->get_customer_serial_number(version);
    if (ret != 0) {
        printf("ERROR: get_customer_serial_number ret: %d\n", ret);
        return;
    }
    printf("get_customer_serial_number ret: %d, customer_serial_number: %s\n", ret, vect2str(version).c_str());

    uint8_t model = 0;
    ret = _devices->get_device_model(model);
    if (ret != 0) {
        printf("ERROR: get_device_model ret: %d\n", ret);
        return;
    }
    printf("get_device_model ret: %d, device_model: %d\n", ret, model);

    ret = _devices->get_chip_serial_number(version);
    if (ret != 0) {
        printf("ERROR: get_chip_serial_number ret: %d\n", ret);
        return;
    }
    printf("get_chip_serial_number ret: %d, chip_serial_number: %s\n", ret, vect2str(version).c_str());
}


void test_clock()
{
    std::vector<uint8_t> time = {0x20,0x25,0x03,0x23,0x17,0x50};
    int ret = _devices->set_clock(time);
    if (ret != 0) {
        printf("ERROR: set_clock ret: %d\n", ret);
        return;
    }
    printf("set_clock ret: %d\n", ret);

    sleep(3);

    ret = _devices->get_clock(time);
    if (ret != 0) {
        printf("ERROR: get_clock ret: %d\n", ret);
        return;
    }
    printf("get_clock ret: %d, time: %d-%d-%d %d:%d:%d\n", ret, time[0], time[1], time[2], time[3], time[4], time[5]);
}

void test_beep()
{
    int ret = _devices->beep(SDK_BEEP_ABNORMAL);
    if (ret != 0) {
        printf("ERROR: beep ret: %d\n", ret);
        return;
    }
    printf("beep ret: %d\n", ret);
    sleep(3);

    ret = _devices->beep_frequency(1000, 1000);
    if (ret != 0) {
        printf("ERROR: beep_frequency ret: %d\n", ret);
        return;
    }
    printf("beep_frequency ret: %d\n", ret);
    sleep(3);

    ret = _devices->beep(SDK_BEEP_NORMAL);
    if (ret != 0) {
        printf("ERROR: beep ret: %d\n", ret);
        return;
    }
    printf("beep ret: %d\n", ret);
    
}

void test_led()
{
    int ret = _devices->led_on(0x0F);
    if (ret != 0) {
        printf("ERROR: led_on ret: %d\n", ret);
        return;
    }
    printf("led_on ret: %d\n", ret);
    sleep(10);
    ret = _devices->led_off(0x0F);
    if (ret != 0) {
        printf("ERROR: led_off ret: %d\n", ret);
        return;
    }
    printf("led_off ret: %d\n", ret);
    sleep(10);
    ret = _devices->led_flash(0x0F, 1000);
    if (ret != 0) {
        printf("ERROR: led_flash ret: %d\n", ret);
        return;
    }
    printf("led_flash ret: %d\n", ret);
    sleep(10);
    ret = _devices->led_off(0x0F);
    if (ret != 0) {
        printf("ERROR: led_off ret: %d\n", ret);
        return;
    }
    printf("led_off ret: %d\n", ret);
}

void test_system_reset()
{
    int ret = _devices->system_reset();
    if (ret != 0) {
        printf("ERROR: system_reset ret: %d\n", ret);
        return;
    }
    printf("system_reset ret: %d\n", ret);
}

void test_system_shutdown()
{
    int ret = _devices->system_shutdown();
    if (ret != 0) {
        printf("ERROR: system_shutdown ret: %d\n", ret);
        return;
    }
    printf("system_shutdown ret: %d\n", ret);
}   


void test_set_enable_sleep_mode()
{
    int ret = _devices->set_enable_sleep_mode(0);
    if (ret != 0) {
        printf("ERROR: set_enable_sleep_mode ret: %d\n", ret);
        return;
    }
    printf("set_enable_sleep_mode ret: %d\n", ret);
}

void test_get_enter_boot_state()
{
    uint32_t state = 0;
    int ret = _devices->get_enter_boot_state(0, state);
    if (ret != 0) {
        printf("ERROR: get_enter_boot_state ret: %d\n", ret);
        return;
    }
    printf("get_enter_boot_state ret: %d, state: %d\n", ret, state);
}

void test_keyboard()
{
    int ret = _devices->keyboard_open();
    if (ret != 0) {
        printf("ERROR: keyboard_open ret: %d\n", ret);
        return;
    }
    ret = _devices->keyboard_set_sound(1);
    if (ret != 0) {
        printf("ERROR: keyboard_set_sound ret: %d\n", ret);
        return;
    }
    printf("keyboard_set_sound ret: %d\n", ret);
    ret = _devices->keyboard_set_backlight(1);
    if (ret != 0) {
        printf("ERROR: keyboard_set_backlight ret: %d\n", ret);
        return;
    }
    printf("keyboard_set_backlight ret: %d\n", ret);

    sleep(30);

    ret = _devices->keyboard_clear_cache();
    if (ret != 0) {
        printf("ERROR: keyboard_clear_cache ret: %d\n", ret);
        return;
    }
    printf("keyboard_clear_cache ret: %d\n", ret);

    int cnt = 10;

    printf("keyboard_get_input start, input 10 cnt\n");
    while(cnt > 0) {
        uint8_t key = 0;
        ret = _devices->keyboard_get_input(key);
        if (ret == 0) {
            printf("keyboard_get_input ret: %d, key: %d\n", ret, key);
            cnt--;
        }
    }

    ret = _devices->keyboard_set_sound(0);
    if (ret != 0) {
        printf("ERROR: keyboard_set_sound ret: %d\n", ret);
        return;
    }
    printf("keyboard_set_sound ret: %d\n", ret);
    ret = _devices->keyboard_set_backlight(0);
    if (ret != 0) {
        printf("ERROR: keyboard_set_backlight ret: %d\n", ret);
        return;
    }

    printf("keyboard_open end\n");
    ret = _devices->keyboard_close();
    if (ret != 0) {
        printf("ERROR: keyboard_close ret: %d\n", ret);
        return;
    }
    printf("keyboard_close ret: %d\n", ret);
}


void test_tp()
{
    int ret = _devices->tp_check_support();
    if (ret != 0) {
        printf("ERROR: tp_check_support ret: %d\n", ret);
        return;
    }
    printf("tp_check_support ret: %d\n", ret);    
    ret = _devices->tp_set_parameter(0, 0, 319, 239, 20);
    if (ret != 0) {
        printf("ERROR: tp_set_parameter ret: %d\n", ret);
        return;
    }
    printf("tp_set_parameter ret: %d\n", ret);  

    ret = _devices->tp_open();
    if (ret != 0) {
        printf("ERROR: tp_open ret: %d\n", ret);
        return;
    }
    printf("tp_open ret: %d\n", ret);

    sleep(3);
    uint32_t x = 0;
    uint32_t y = 0;
    int cnt = 100;
    printf("tp_get_touch_coordinate start, get 100 cnt\n");
    while (cnt > 0) {
        ret = _devices->tp_get_touch_coordinate(x, y);
        if (ret == 0) {
            printf("tp_get_touch_coordinate ret: %d, x: %d, y: %d\n", ret, x, y);
            cnt--;
        }
    }
    printf("tp_get_touch_coordinate end\n");

    ret = _devices->tp_close();
    if (ret != 0) {
        printf("ERROR: tp_close ret: %d\n", ret);
        return;
    }
    printf("tp_close ret: %d\n", ret);

    sleep(3);
}


void test_magnetic_stripe_card()
{
    int ret = _devices->magnetic_stripe_card_open();
    if (ret != 0) {
        printf("ERROR: magnetic_stripe_card_open ret: %d\n", ret);
        return;
    }
    printf("magnetic_stripe_card_open ret: %d\n", ret);

    ret = _devices->magnetic_stripe_card_clear_data();
    if (ret != 0) {
        printf("ERROR: magnetic_stripe_card_clear_data ret: %d\n", ret);
        return;
    }
    printf("magnetic_stripe_card_clear_data ret: %d\n", ret);

    sleep(3);

    std::vector<uint8_t> tk1;
    std::vector<uint8_t> tk2;
    std::vector<uint8_t> tk3;
    ret = _devices->magnetic_stripe_card_read_data(tk1, tk2, tk3);
    if (ret != 0) {
        printf("ERROR: magnetic_stripe_card_read_data ret: %d\n", ret);
        return;
    }
    printf("magnetic_stripe_card_read_data ret: %d\n", ret);
    printf("tk1: %s\n", vect2str(tk1).c_str());
    printf("tk2: %s\n", vect2str(tk2).c_str());
    printf("tk3: %s\n", vect2str(tk3).c_str());

    std::vector<uint8_t> card_number;
    std::vector<uint8_t> valid_date;
    std::vector<uint8_t> card_holder_name;
    std::vector<uint8_t> service_code;
    ret = _devices->magnetic_stripe_card_format_data(tk1, tk2, tk3, card_number, valid_date, card_holder_name, service_code);
    if (ret != 0) {
        printf("ERROR: magnetic_stripe_card_format_data ret: %d\n", ret);
        return;
    }
    printf("magnetic_stripe_card_format_data ret: %d\n", ret);
    printf("card_number: %s\n", card_number.data());
    printf("valid_date: %s\n", valid_date.data());
    printf("card_holder_name: %s\n", card_holder_name.data());
    printf("service_code: %s\n", service_code.data());


    sleep(3);

    ret = _devices->magnetic_stripe_card_close();
    if (ret != 0) {
        printf("ERROR: magnetic_stripe_card_close ret: %d\n", ret);
        return;
    }
    printf("magnetic_stripe_card_close ret: %d\n", ret);


}

void test_ic()
{
    int ret = _devices->ic_card_open(0, 0);
    if (ret != 0) {
        printf("ERROR: ic_card_open ret: %d\n", ret);
        return;
    }
    printf("ic_card_open ret: %d\n", ret);

    while (true) {
        ret = _devices->ic_card_check_status(0, 0);
        if (ret == 0) {
            printf("ic_card_check_status ret: %d\n", ret);
            break;
        } 
        sleep(1);
    }

    std::vector<uint8_t> data;
    ret = _devices->ic_card_reset(0, 0, data);
    if (ret != 0) {
        printf("ERROR: ic_card_reset ret: %d\n", ret);
        return;
    }
    printf("ic_card_reset ret: %d\n", ret);
    printf("data: %s\n", vect2str(data).c_str());

    std::vector<uint8_t> apdu_command = {0x00, 0xB0, 0x00, 0x00, 0x02, 0x10, 0x00};
    std::vector<uint8_t> card_return_data;
    ret = _devices->ic_card_send_apdu_command(0, 0, apdu_command, card_return_data);
    if (ret != 0) {
        printf("ERROR: ic_card_send_apdu_command ret: %d\n", ret);
        return;
    }
    printf("ic_card_send_apdu_command ret: %d\n", ret);
    printf("card_return_data: %s\n", vect2str(card_return_data).c_str());

    ret = _devices->ic_card_power_off(0, 0);
    if (ret != 0) {
        printf("ERROR: ic_card_module_power_off ret: %d\n", ret);
        return;
    }
    printf("ic_card_module_power_off ret: %d\n", ret);

    
}

void test_icc()
{
    int ret = _devices->icc_open_module();
    if (ret != 0) {
        printf("ERROR: icc_open_module ret: %d\n", ret);
        return;
    }
    printf("icc_open_module ret: %d\n", ret);

    uint8_t card_type = 0;
    std::vector<uint8_t> serial_number;
    std::vector<uint8_t> cid;
    std::vector<uint8_t> card_response_info;
    ret = _devices->icc_search_card_activation(0, card_type, serial_number, cid, card_response_info);
    if (ret != 0) {
        printf("ERROR: icc_search_card_activation ret: %d\n", ret);
        return;
    }
    printf("icc_search_card_activation ret: %d\n", ret);

    std::vector<uint8_t> apdu_command = {0x00, 0xB0, 0x00, 0x00, 0x02, 0x10, 0x00};
    std::vector<uint8_t> card_return_data;
    ret = _devices->icc_send_apdu_command(apdu_command, card_return_data);
    if (ret != 0) {
        printf("ERROR: icc_send_apdu_command ret: %d\n", ret);
        return;
    }
    printf("icc_send_apdu_command ret: %d\n", ret);
    printf("card_return_data: %s\n", vect2str(card_return_data).c_str());
    ret = _devices->icc_close_module();
    if (ret != 0) {
        printf("ERROR: icc_close_module ret: %d\n", ret);
        return;
    }
}


void test_mifare_card()
{
    std::vector<uint8_t> uid;
    std::vector<uint8_t> auth_key = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};

    int ret = _devices->ic_card_open(SDK_CARD_TYPE_MEMORY, SDK_CARD_SEAT_SAM1);
    if (ret != 0) {
        printf("ERROR: ic_card_open ret: %d\n", ret);
        return;
    }
    printf("ic_card_open ret: %d\n", ret);

    ret = _devices->ic_card_reset(SDK_CARD_TYPE_MEMORY, SDK_CARD_SEAT_SAM1, uid);
    if (ret != 0) {
        printf("ERROR: ic_card_reset ret: %d\n", ret);
        return;
    }
    printf("ic_card_reset ret: %d\n", ret);

    ret = _devices->mifare_card_authentication(0, SDK_KEY_TYPE_A, uid, auth_key);
    if (ret != 0) {
        printf("ERROR: mifare_card_authentication ret: %d\n", ret);
        return;
    }
    printf("mifare_card_authentication ret: %d\n", ret);
    
    std::vector<uint8_t> response_data;
    ret = _devices->mifare_card_operation(SDK_MIFARE_CARD_OPERATION_READ, 0, 0, {}, response_data);
    if (ret != 0) {
        printf("ERROR: mifare_card_operation ret: %d\n", ret);
        return;
    }
    printf("mifare_card_operation ret: %d\n", ret);
    printf("response_data: %s\n", vect2str(response_data).c_str());

    ret = _devices->ic_card_close(SDK_CARD_TYPE_MEMORY, SDK_CARD_SEAT_SAM1);
    if (ret != 0) {
        printf("ERROR: ic_card_close ret: %d\n", ret);
        return;
    }
    printf("ic_card_close ret: %d\n", ret);
}

void test_search_card()
{
    int ret = _devices->search_card_start(SDK_SWIPE_CARD_HAND | SDK_SWIPE_CARD_ICC | SDK_SWIPE_CARD_RF | SDK_SWIPE_CARD_MAG, 
        10000);
    if (ret != 0) {
        printf("ERROR: search_card_start ret: %d\n", ret);
        return;
    }
    printf("search_card_start ret: %d\n", ret);
    int cnt = 10;
    while (cnt > 0) {
        uint8_t type = 0;
        uint8_t key = 0;
        ret = _devices->search_card_get_status(type, key);
        if (ret == 0) {
            printf("search_card_get_status ret: %d, type: %d, key: %d\n", ret, type, key);
            break;
        }
        sleep(1);
        cnt--;
    }
    
    ret = _devices->search_card_stop();
    if (ret != 0) {
        printf("ERROR: search_card_stop ret: %d\n", ret);
        return;
    }
    printf("search_card_stop ret: %d\n", ret);
}

void test_scan()
{
    int ret = _devices->scan_open();
    if (ret != 0) {
        printf("ERROR: scan_open ret: %d\n", ret);
        return;
    }
    printf("scan_open ret: %d\n", ret);

    sleep(1);

    std::vector<uint8_t> data;
    ret = _devices->scan_read_data(10000, data);
    if (ret != 0) {
        printf("ERROR: scan_read_data ret: %d\n", ret);
        return;
    }
    printf("scan_read_data ret: %d\n", ret);
    printf("data: %s\n", vect2str(data).c_str());

    ret = _devices->scan_close();
    if (ret != 0) {
        printf("ERROR: scan_close ret: %d\n", ret);
        return;
    }
    printf("scan_close ret: %d\n", ret);
}

void test_printer()
{
    int ret = _devices->printer_open();
    if (ret != 0) {
        printf("ERROR: printer_open ret: %d\n", ret);
        return;
    }
    printf("printer_open ret: %d\n", ret);

    ret = _devices->printer_set_gray(60);
    if (ret != 0) {
        printf("ERROR: printer_set_gray ret: %d\n", ret);
        return;
    }
    printf("printer_set_gray ret: %d\n", ret);

    ret = _devices->printer_paper_feed(100);
    if (ret != 0) {
        printf("ERROR: printer_paper_feed ret: %d\n", ret);
        return;
    }
    std::vector<uint8_t> bitmap_data = {0x42, 0x4D, 0xB6, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x04, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 
    0x40, 0x01, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x0C, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for (size_t i = 0; i < 1024 + 3200; i++)
    {
        bitmap_data.push_back(0x00);
    }
    
    ret = _devices->printer_print_bitmap_data(1, bitmap_data, 320, 240, SDK_PRINT_ALIGN_CENTER);
    if (ret != 0) {
        printf("ERROR: printer_print_bitmap_data ret: %d\n", ret);
        return;
    }
    printf("printer_print_bitmap_data ret: %d\n", ret);

    ret = _devices->printer_paper_feed(100);
    if (ret != 0) {
        printf("ERROR: printer_paper_feed ret: %d\n", ret);
        return;
    }
    printf("printer_paper_feed ret: %d\n", ret);


    ret = _devices->printer_close();
    if (ret != 0) {
        printf("ERROR: printer_close ret: %d\n", ret);
        return;
    }
    printf("printer_close ret: %d\n", ret);
}

void test_keypad()
{
    int ret = _devices->keypad_open();
    if (ret != 0) {
        printf("ERROR: keypad_open ret: %d\n", ret);
        return;
    }
    printf("keypad_open ret: %d\n", ret);

    std::vector<uint8_t> data;
    ret = _devices->keypad_get_random_number(8, data);
    if (ret != 0) {
        printf("ERROR: keypad_get_random_number ret: %d\n", ret);
        return;
    }
    printf("keypad_get_random_number ret: %d\n", ret);
    printf("data: %s\n", vect2str(data).c_str());

    ret = _devices->keypad_close();
    if (ret != 0) {
        printf("ERROR: keypad_close ret: %d\n", ret);
        return;
    }
    printf("keypad_close ret: %d\n", ret);
}


int main()
{
    // 菜单

    int choice = 0;
    while (true) {
        printf("+---------------------------------------------------+\n");
        printf("| 请选择测试项：[0. 退出]\n");
        printf("| 1. test_communication_mode      2. test_version\n");
        printf("| 3. test_clock                   4. test_beep\n");
        printf("| 5. test_led                     6. test_system_reset\n");
        printf("| 7. test_system_shutdown         8. test_set_enable_sleep_mode\n");
        printf("| 9. test_get_enter_boot_state   10. test_keyboard\n");
        printf("|11. test_tp                     12. test_magnetic_stripe_card\n");
        printf("|13. test_ic                     14. test_icc\n");
        printf("|15. test_mifare_card            16. test_search_card\n");
        printf("|17. test_scan                   18. test_printer\n");
        printf("|19. test_keypad\n");
        printf("+---------------------------------------------------+\n");

        printf("请输入测试项[0-19]: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                test_communication_mode();
                break;
            case 2:
                test_version();
                break;
            case 3:
                test_clock();
                break;
            case 4:
                test_beep();
                break;
            case 5:
                test_led();
                break;
            case 6:
                test_system_reset();
                break;
            case 7:
                test_system_shutdown();
                break;
            case 8:
                test_set_enable_sleep_mode();
                break;
            case 9:
                test_get_enter_boot_state();
                break;
            case 10:
                test_keyboard();
                break;
            case 11:
                test_tp();
                break;
            case 12:
                test_magnetic_stripe_card();
                break;
            case 13:
                test_ic();
                break;
            case 14:
                test_icc();
                break;
            case 15:
                test_mifare_card();
                break;
            case 16:
                test_search_card();
                break;
            case 17:
                test_scan();
                break;
            case 18:
                test_printer();
                break;
            case 19:
                test_keypad();
                break;
            case 0:
                return 0;
        }     

    }                      
    
    return 0;
}