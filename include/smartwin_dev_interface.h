#ifndef _SWDEV_INTERFACE_H_
#define _SWDEV_INTERFACE_H_

#include "future_def.h"

extern "C" {

    int swdev_init();
    int swdev_deinit();

    /**
     * @brief 设置通讯方式 (命令字: 0x11)
     * @param[in] mode 通讯模式 @see SDK_COMMUNICATION_MODE_NONE, SDK_COMMUNICATION_MODE_GPRS, SDK_COMMUNICATION_MODE_WIFI, SDK_COMMUNICATION_MODE_BLUETOOTH
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int swdev_set_communication_mode(uint8_t mode);

    /**
     * @brief 获取网络制式 (命令字: 0x12)
     * @param[out] mode 网络制式
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int swdev_get_network_mode(uint8_t* mode);

    /**
     * @brief 读取系统版本信息 (命令字: 0x17)
     * @param[in] type 版本类型 @see SDK_VERSION_TYPE_HARDWARE, SDK_VERSION_TYPE_BOOT, SDK_VERSION_TYPE_KERNEL, SDK_VERSION_TYPE_SYSTEM, SDK_VERSION_TYPE_LIBRARY, SDK_VERSION_TYPE_SDK
     * @param[out] version 版本信息
     * @param[out] length 版本信息长度
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int swdev_get_system_version(uint8_t type, uint8_t* version, uint8_t* length);

    /**
     * @brief 获取硬件序列号 (命令字: 0x18)
     * @param[out] serial_number 硬件序列号
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int swdev_get_hardware_serial_number(uint8_t* serial_number);

    /**
     * @brief 获取设备型号 (命令字: 0x19)
     * @param[out] model 设备型号
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int swdev_get_device_model(uint8_t* model);

    /**
     * @brief 获取客户自定义序列号 (命令字: 0x1A)
     * @param[out] custom_serial_number 客户自定义序列号
     * @param[out] length 客户自定义序列号长度
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int swdev_get_customer_serial_number(uint8_t* custom_serial_number, uint8_t* length);




















}


#endif
