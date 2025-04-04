#include "smartwin_dev_interface.h"
#include "future_devices.h"

extern "C" {
    
static future::future_devices* _swdev_t = NULL;

int swdev_init() {
    _swdev_t = future::future_devices::getInstance();
    return 0;
}

int swdev_deinit() {
    _swdev_t = NULL;
    return 0;
}

int swdev_set_communication_mode(uint8_t mode) {
    int ret = _swdev_t->set_communication_mode(mode);
    return ret;
}

int swdev_get_network_mode(uint8_t* mode) {
    int ret = _swdev_t->get_network_mode(*mode);
    return ret;
}

int swdev_get_system_version(uint8_t type, uint8_t* version) {
    std::vector<uint8_t> version_buf;
    int ret = _swdev_t->get_system_version(type, version_buf);
    if(ret == SDK_OK) {
        *version = version_buf[0];
    }
    return ret;
}

int swdev_get_hardware_serial_number(uint8_t* serial_number, uint8_t* length) {
    std::vector<uint8_t> serial_number_buf;
    int ret = _swdev_t->get_hardware_serial_number(serial_number_buf);
    if(ret == SDK_OK) {
        *serial_number = serial_number_buf.size();
        memcpy(serial_number, serial_number_buf.data(), serial_number_buf.size());
    }
    return ret;
}

int swdev_get_device_model(uint8_t* model) {
    return _swdev_t->get_device_model(*model);
}

int swdev_get_customer_serial_number(uint8_t* custom_serial_number, uint8_t* length) {
    std::vector<uint8_t> custom_serial_number_buf;
    int ret = _swdev_t->get_customer_serial_number(custom_serial_number_buf);
    if(ret == SDK_OK) {
        *length = custom_serial_number_buf.size();
        memcpy(custom_serial_number, custom_serial_number_buf.data(), custom_serial_number_buf.size());
    }
    return ret;
}



}