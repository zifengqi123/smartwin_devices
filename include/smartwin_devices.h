#ifndef  SMARTWIN_DEVICES_H_
#define  SMARTWIN_DEVICES_H_

#include <stdint.h>
#include <stdio.h>
#include "smartwin_def.h"
#include "smartwin_comm.h"
#include <vector>
#include <mutex>
using namespace std;

namespace smartwin {

class smartwin_devices {

private:
    smartwin_devices();
    ~smartwin_devices();
    smartwin_devices(const smartwin_devices&) = delete;
    smartwin_devices& operator=(const smartwin_devices&) = delete;

    smartwin_comm* _comm;

    int recv_timeout = 2000;  //ms

    pthread_mutex_t recv_list_mutex_;
    std::vector<std::vector<uint8_t>> recv_list;

    std::vector<std::vector<uint8_t>> keyinput_list;
    std::vector<std::vector<uint8_t>> search_card_list;
    std::vector<std::vector<uint8_t>> tpinput_list;
    std::vector<std::vector<uint8_t>> icstatus_list;

    pthread_mutex_t keyinput_list_mutex_;
    pthread_mutex_t search_card_list_mutex_;
    pthread_mutex_t tpinput_list_mutex_;
    pthread_mutex_t icstatus_list_mutex_;

public:
    static smartwin_devices* getInstance() {
        static smartwin_devices instance;
        
        return &instance;
    }

    int send_request_cmd(uint8_t cmd, std::vector<uint8_t> params);
    int recv_from_list(int8_t cmd, std::vector<uint8_t> &buf);

    std::vector<uint8_t> lvar_to_vector(std::vector<uint8_t> buf);
    std::vector<uint8_t> llvar_to_vector(std::vector<uint8_t> buf);
    
    /**
     * @brief 设置通讯方式 (命令字: 0x11)
     * @param[in] mode 通讯模式 @see SDK_COMMUNICATION_MODE_NONE, SDK_COMMUNICATION_MODE_GPRS, SDK_COMMUNICATION_MODE_WIFI, SDK_COMMUNICATION_MODE_BLUETOOTH
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int set_communication_mode(uint8_t mode);

    /**
     * @brief 获取网络制式 (命令字: 0x12)
     * @param[out] mode 网络制式
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int get_network_mode(uint8_t& mode);

    /**
     * @brief 读取系统版本信息 (命令字: 0x17)
     * @param[in] type 版本类型 @see SDK_VERSION_TYPE_HARDWARE, SDK_VERSION_TYPE_BOOT, SDK_VERSION_TYPE_KERNEL, SDK_VERSION_TYPE_SYSTEM, SDK_VERSION_TYPE_LIBRARY, SDK_VERSION_TYPE_SDK
     * @param[out] version 版本信息
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int get_system_version(uint8_t type, std::vector<uint8_t>& version);

    /**
     * @brief 获取硬件序列号 (命令字: 0x18)
     * @param[out] serial_number 硬件序列号
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int get_hardware_serial_number(std::vector<uint8_t>& serial_number);

    /**
     * @brief 获取设备型号 (命令字: 0x19)
     * @param[out] model 设备型号
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int get_device_model(uint8_t &model);

    /**
     * @brief 获取客户自定义序列号 (命令字: 0x1A)
     * @param[out] custom_serial_number 客户自定义序列号
     * @return 成功返回SDK_OK，失败返回错误码
     */           
    int get_customer_serial_number(std::vector<uint8_t>& custom_serial_number);

    /**
     * @brief 设置时钟 (命令字: 0x1B)
     * @param[in] time 时间
     * 格 式 为YYMMDDhhmmss,BCD码,共6个字节
     * 有效时间范围:2000-1-1 ~ 2099-12-31
     * @return 成功返回SDK_OK，失败返回错误码
     */     
    int set_clock(std::vector<uint8_t> time);

    /**
     * @brief 获取时钟 (命令字: 0x1C)
     * @param[out] time 时间
     * @return 成功返回SDK_OK，失败返回错误码
     */ 
    int get_clock(std::vector<uint8_t>& time);

    /**
     * @brief 蜂鸣 (命令字: 0x20)
     * @param[in] type 类型 @see SDK_BEEP_NORMAL, SDK_BEEP_ABNORMAL
     * @return 成功返回SDK_OK，失败返回错误码
     */     
    int beep(uint8_t type);

    /**
     * @brief 蜂鸣器按指定的频率发声 (命令字: 0x21)
     * @param[in] frequency 频率
     * @param[in] duration 持续发声时间,单位:ms
     * @return 成功返回SDK_OK，失败返回错误码
     */  
    int beep_frequency(uint32_t frequency, uint32_t duration);

    /**
     * @brief 点亮LED指示灯 (命令字: 0x22)
     * @param[in] color LED颜色 @see SDK_LED_ID_BLUE, SDK_LED_ID_YELLOW, SDK_LED_ID_GREEN, SDK_LED_ID_RED, SDK_LED_ID_ALL
     * @return 成功返回SDK_OK，失败返回错误码
     */  
    int led_on(uint32_t color);

    /**
     * @brief 熄灭LED指示灯 (命令字: 0x23)
     * @param[in] color LED颜色 @see SDK_LED_ID_BLUE, SDK_LED_ID_YELLOW, SDK_LED_ID_GREEN, SDK_LED_ID_RED, SDK_LED_ID_ALL
     * @return 成功返回SDK_OK，失败返回错误码
     */ 
    int led_off(uint32_t color);

    /**
     * @brief LED指示灯闪烁 (命令字: 0x24)
     * @param[in] color LED颜色 @see SDK_LED_ID_BLUE, SDK_LED_ID_YELLOW, SDK_LED_ID_GREEN, SDK_LED_ID_RED, SDK_LED_ID_ALL
     * @param[in] duration 闪烁的周期,单位:ms
     * @return 成功返回SDK_OK，失败返回错误码
     */ 
    int led_flash(uint32_t color, uint32_t duration);

    /**
     * @brief 系统复位 (命令字: 0x25)
     * @return 成功返回SDK_OK，失败返回错误码
     */ 
    int system_reset();

    /**
     * @brief 系统关机 (命令字: 0x26)
     * @return 成功返回SDK_OK，失败返回错误码
     */ 
    int system_shutdown();

    /**
     * @brief 设置终端序列号 (命令字: 0x28)
     * @param[in] serial_number 终端序列号, 终端序列号数据(SN),最大32字节
     * @param[in] custom_serial_number 客户自定义序列号, 客户自定义序列号数据(CSN),最大32字节
     * @return 成功返回SDK_OK，失败返回错误码
     */  
    int set_terminal_serial_number(std::vector<uint8_t> serial_number, std::vector<uint8_t> custom_serial_number);

    /**
     * @brief 获取芯片序列号 (命令字: 0x29)
     * @param serial_number 芯片序列号
     * @return 成功返回SDK_OK，失败返回错误码
     */ 
    int get_chip_serial_number(std::vector<uint8_t>& serial_number); 

    /**
     * @brief 使能下位机立即进入休眠模式 (命令字: 0x2A)
     * @param wakeup_type 唤醒类型 @see SDK_SLEEP_MODE_WAKEUP_TYPE_ANY_KEY, SDK_SLEEP_MODE_WAKEUP_TYPE_IO, SDK_SLEEP_MODE_WAKEUP_TYPE_IC, SDK_SLEEP_MODE_WAKEUP_TYPE_MAG, SDK_SLEEP_MODE_WAKEUP_TYPE_ICC, SDK_SLEEP_MODE_WAKEUP_TYPE_TP
     * @return 成功返回SDK_OK，失败返回错误码
     */ 
    int set_enable_sleep_mode(uint32_t wakeup_type);   

    /**
     * @brief 下位机进入BOOT或查询状态 (命令字: 0x2F)
     * @param type 类型
     * 0x00 查询下位机的状态
     * 0x01 使能下位机进入BOOT模式
     * @param state 状态
     * @return 成功返回SDK_OK，失败返回错误码
     */ 
    int get_enter_boot_state(uint8_t type, uint32_t& state); 

    /**
     * @brief 打开键盘 (命令字: 0x30)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keyboard_open();

    /**
     * @brief 关闭键盘 (命令字: 0x31)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keyboard_close();

    /**
     * @brief 清除按键缓存 (命令字: 0x33)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keyboard_clear_cache();

    /**
     * @brief 读取键盘输入 (命令字: 0x32)
     * @param[out] key 按键
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keyboard_get_input(uint8_t& key);

    /**
     * @brief 设置打开关闭按键音 (命令字: 0x34)
     * @param[in] sound 按键音开关 @see SDK_KEYBOARD_SOUND_OFF, SDK_KEYBOARD_SOUND_ON
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keyboard_set_sound(uint8_t sound);

    /**
     * @brief 设置按键背光 (命令字: 0x35)
     * @param[in] backlight 背光值 @see SDK_KEYBOARD_BACKLIGHT_OFF, SDK_KEYBOARD_BACKLIGHT_ON
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keyboard_set_backlight(uint8_t backlight);

    /**
     * @brief 打开TP (命令字: 0x3A)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int tp_open();

    /**
     * @brief 关闭TP (命令字: 0x3B)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int tp_close();

    /**
     * @brief 检查是否支持TP (命令字: 0x3C)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int tp_check_support();

    /**
     * @brief 获取触控坐标值 (命令字: 0x3E) 
     * @param[out] x X坐标
     * @param[out] y Y坐标
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int tp_get_touch_coordinate(uint32_t &x, uint32_t &y);

    /**
     * @brief 设置触控参数 (命令字: 0x3F)
     * @param[in] start_x 有效X起始坐标 
     * @param[in] start_y 有效Y起始坐标
     * @param[in] end_x 有效X结束坐标
     * @param[in] end_y 有效Y结束坐标
     * @param[in] interval 上报坐标时间最小间隔（ms）
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int tp_set_parameter(uint32_t start_x, uint32_t start_y, uint32_t end_x, uint32_t end_y, uint32_t interval);

    /**
     * @brief 打开磁条卡设备 (命令字: 0x40)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int magnetic_stripe_card_open();

    /**
     * @brief 关闭磁条卡设备 (命令字: 0x41)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int magnetic_stripe_card_close();

    /**
     * @brief 检查是否有刷磁条卡 (命令字: 0x42)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int magnetic_stripe_card_check();

    /**
     * @brief 读取磁条卡数据 (命令字: 0x43)
     * @param[out] track1 磁道1数据 不带起始符和结束符
     * @param[out] track2 磁道2数据 不带起始符和结束符
     * @param[out] track3 磁道3数据 不带起始符和结束符
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int magnetic_stripe_card_read_data(std::vector<uint8_t>& track1, std::vector<uint8_t>& track2, 
        std::vector<uint8_t>& track3);

    /**
     * @brief 清除磁卡数据缓冲 (命令字: 0x44)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int magnetic_stripe_card_clear_data();

    /**
     * @brief 格式化磁道信息 (命令字: 0x45)
     * @param[in] track1 磁道1数据
     * @param[in] track2 磁道2数据
     * @param[in] track3 磁道3数据
     * @param[out] card_number 卡号
     * @param[out] valid_date 有效期
     * @param[out] card_holder_name 持卡人姓名
     * @param[out] service_code 服务代码
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int magnetic_stripe_card_format_data(std::vector<uint8_t> track1, std::vector<uint8_t> track2, 
        std::vector<uint8_t> track3, std::vector<uint8_t>& card_number, std::vector<uint8_t>& valid_date,
        std::vector<uint8_t>& card_holder_name, std::vector<uint8_t>& service_code);

    /**
     * @brief 打开IC卡模块 (命令字: 0x4A)
     * @param[in] card_type 卡类型 @see SDK_CARD_TYPE_CPU, SDK_CARD_TYPE_AT24CXX, SDK_CARD_TYPE_MEMORY
     * @param[in] card_seat 卡座号 @see SDK_CARD_SEAT_STANDARD, SDK_CARD_SEAT_SAM1, SDK_CARD_SEAT_SAM2, SDK_CARD_SEAT_SAM3
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int ic_card_open(uint8_t card_type, uint8_t card_seat);

    /**
     * @brief 关闭IC卡模块 (命令字: 0x4B)
     * @param[in] card_type 卡类型 @see SDK_CARD_TYPE_CPU, SDK_CARD_TYPE_AT24CXX, SDK_CARD_TYPE_MEMORY
     * @param[in] card_seat 卡座号 @see SDK_CARD_SEAT_STANDARD, SDK_CARD_SEAT_SAM1, SDK_CARD_SEAT_SAM2, SDK_CARD_SEAT_SAM3
     * @return 成功返回SDK_OK，失败返回错误码
     */    
    int ic_card_close(uint8_t card_type, uint8_t card_seat);

    /**
     * @brief 检查ICC状态 (命令字: 0x4C)
     * @param[in] card_type 卡类型 @see SDK_CARD_TYPE_CPU, SDK_CARD_TYPE_AT24CXX, SDK_CARD_TYPE_MEMORY
     * @param[in] card_seat 卡座号 @see SDK_CARD_SEAT_STANDARD, SDK_CARD_SEAT_SAM1, SDK_CARD_SEAT_SAM2, SDK_CARD_SEAT_SAM3
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int ic_card_check_status(uint8_t card_type, uint8_t card_seat);


    /**
     * @brief IC卡上电复位 (命令字: 0x4D)
     * @param[in] card_type 卡类型 @see SDK_CARD_TYPE_CPU, SDK_CARD_TYPE_AT24CXX, SDK_CARD_TYPE_MEMORY
     * @param[in] card_seat 卡座号 @see SDK_CARD_SEAT_STANDARD, SDK_CARD_SEAT_SAM1, SDK_CARD_SEAT_SAM2, SDK_CARD_SEAT_SAM3
     * @param[out] reset_data 复位数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int ic_card_reset(uint8_t card_type, uint8_t card_seat, std::vector<uint8_t>& reset_data);

    /**
     * @brief IC卡模块下电 (命令字: 0x4E)
     * @param[in] card_type 卡类型 @see SDK_CARD_TYPE_CPU, SDK_CARD_TYPE_AT24CXX, SDK_CARD_TYPE_MEMORY
     * @param[in] card_seat 卡座号 @see SDK_CARD_SEAT_STANDARD, SDK_CARD_SEAT_SAM1, SDK_CARD_SEAT_SAM2, SDK_CARD_SEAT_SAM3
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int ic_card_power_off(uint8_t card_type, uint8_t card_seat);

    /**
     * @brief IC卡发送APDU命令 (命令字: 0x4F)
     * @param[in] card_type 卡类型 @see SDK_CARD_TYPE_CPU, SDK_CARD_TYPE_AT24CXX, SDK_CARD_TYPE_MEMORY
     * @param[in] card_seat 卡座号 @see SDK_CARD_SEAT_STANDARD, SDK_CARD_SEAT_SAM1, SDK_CARD_SEAT_SAM2, SDK_CARD_SEAT_SAM3
     * @param[in] apdu_command APDU命令数据
     * @param[out] card_return_data 卡片返回数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int ic_card_send_apdu_command(uint8_t card_type, uint8_t card_seat, 
        std::vector<uint8_t> apdu_command, std::vector<uint8_t> card_return_data);

    /**
     * @brief 打开非接模块 (命令字: 0x50)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int icc_open_module();

    /**
     * @brief 关闭非接模块 (命令字: 0x51)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int icc_close_module();

    /**
     * @brief 寻卡激活 (命令字: 0x52)
     * @param[in] card_type 卡类型,支持多种,按位表示,用或"|"的方式 @see SDK_ICC_TYPE_A, SDK_ICC_TYPE_B, SDK_ICC_TYPE_M1, SDK_ICC_TYPE_C
     * @param[out] card_type_return 读到的非接卡类型
     * @param[out] serial_number 序列号
     * @param[out] cid CID
     * @param[out] card_response_info 卡片响应信息等内容的缓冲区
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int icc_search_card_activation(uint8_t card_type, uint8_t &card_type_return, std::vector<uint8_t>& serial_number, 
            std::vector<uint8_t>& cid, std::vector<uint8_t>& card_response_info);

    /**
     * @brief 发送APDU命令 (命令字: 0x53)
     * @param apdu_command APDU命令数据
     * @param apdu_command_len APDU命令长度
     * @return 成功返回SDK_OK，失败返回错误码
     * 卡片返回数据
     */
    int icc_send_apdu_command(std::vector<uint8_t> apdu_command, std::vector<uint8_t> card_return_data);

    /**
     * @brief Mifare卡认证 (命令字: 0x54)
     * @param[in] block_number 块号
     * @param[in] key_type 密钥类型 @see SDK_KEY_TYPE_A, SDK_KEY_TYPE_B
     * @param[in] uid 卡片序列号（UID）
     * @param[in] auth_key 认证密钥
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int mifare_card_authentication(uint8_t block_number, uint8_t key_type, 
            std::vector<uint8_t> uid, std::vector<uint8_t> auth_key);

    /**
     * @brief Mifare卡操作 (命令字: 0x55)
     * @param[in] operation_instruction 操作指令 @see SDK_MIFARE_CARD_OPERATION_READ, SDK_MIFARE_CARD_OPERATION_WRITE, SDK_MIFARE_CARD_OPERATION_INCREMENT, SDK_MIFARE_CARD_OPERATION_DECREMENT, SDK_MIFARE_CARD_OPERATION_TRANSFER
     * @param[in] block_number 要访问的块号
     * @param[in] target_block_number 目标块号
     * @param[in] data 数据
     * @param[out] response_data 响应数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int mifare_card_operation(uint8_t operation_instruction, uint8_t block_number, uint8_t target_block_number, 
        std::vector<uint8_t> data, std::vector<uint8_t>& response_data);

    /**
     * @brief 单独寻卡报文 (命令字: 0x46)
     * @param[in] search_mode 寻卡方式,按位组合使用 @see SDK_SWIPE_CARD_HAND, SDK_SWIPE_CARD_MAG, SDK_SWIPE_CARD_ICC, SDK_SWIPE_CARD_RF
     * @param[in] timeout_ms 寻卡超时时间 ms
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int search_card_start(uint8_t search_mode, uint32_t timeout_ms);

    /**
     * @brief 获取寻卡状态 (命令字: 0x47)
     * @param[out] type 卡片类型
     * @param[out] key 卡片密钥
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int search_card_get_status(uint8_t &type, uint8_t &key);

    /**
     * @brief 结束寻卡 (命令字: 0x48)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int search_card_stop();

    /**
     * @brief 打开扫码 (命令字: 0x5A)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int scan_open();

    /**
     * @brief 关闭扫码 (命令字: 0x5B)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int scan_close();

    /**
     * @brief 读取扫码数据 (命令字: 0x5C)
     * @param[in] timeout_ms 超时时间
     * @param[out] data 扫码数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int scan_read_data(uint32_t timeout_ms, std::vector<uint8_t>& data);

    /**
     * @brief 检查是否支持打印机 (命令字: 0x61)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int printer_check_support();

    /**
     * @brief 打开打印机 (命令字: 0x62)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int printer_open();

    /**
     * @brief 关闭打印机 (命令字: 0x63)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int printer_close();

    /**
     * @brief 查询打印机状态 (命令字: 0x64)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int printer_query_status();

    /**
     * @brief 设置打印灰度 (命令字: 0x65)
     * @param[in] gray 打印灰度等级
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int printer_set_gray(uint8_t gray);

    /**
     * @brief 走纸 (命令字: 0x6B)
     * @param[in] pixel_point 像素点
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int printer_paper_feed(uint32_t pixel_point);

    /**
     * @brief 打印位图数据 (命令字: 0x6C)
     * @param[in] packet_number 数据包序号 1~255循环，表示为数据包下发过程；= 0 表示最后一包，此时下位机执行打印数据
     * @param[in] bitmap_data 打印位图点阵数据
     * @param[in] bitmap_width 打印位图点阵数据的宽度
     * @param[in] bitmap_height 打印位图点阵数据的高度
     * @param[in] bitmap_attribute 打印属性 @see SDK_PRINT_ALIGN_LEFT, SDK_PRINT_ALIGN_RIGHT, SDK_PRINT_ALIGN_CENTER
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int printer_print_bitmap_data(uint8_t packet_number, std::vector<uint8_t> bitmap_data, 
        uint32_t bitmap_width, uint32_t bitmap_height, uint8_t bitmap_attribute);

    /**
     * @brief 打开密码键盘 (命令字: 0x70)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_open();

    /**
     * @brief 关闭密码键盘 (命令字: 0x71)
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_close();

    /**
     * @brief 获取随机数 (命令字: 0x72)
     * @param[in] length 需要获取的随机数长度
     * @param[out] random_number 随机数
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_get_random_number(uint32_t length, std::vector<uint8_t>& random_number);

    /**
     * @brief 更新主密钥 (命令字: 0x73)
     * @param[in] master_key_index 主密钥索引
     * @param[in] master_key_data 主密钥数据
     * @param[in] decrypt_master_key_index 解(加)密主密钥索引
     * @param[in] write_mode 写入模式 @see SDK_PED_DECRYPT, SDK_PED_ENCRYPT, SDK_PED_PLAINTEXT, SDK_PED_DES, SDK_PED_SM4, SDK_PED_AES, SDK_PED_XOR
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_update_master_key(uint32_t master_key_index, std::vector<uint8_t> master_key_data, 
        uint32_t decrypt_master_key_index, uint8_t write_mode);

    /**
     * @brief 更新工作密钥 (命令字: 0x74)
     * @param[in] 主密钥索引
     * @param[in]  Pin 密钥密文
     * @param[in]  Mac 密钥密文
     * @param[in]  TDK 密钥密文
     * @param[in] 写入模式 @see SDK_PED_DECRYPT, SDK_PED_ENCRYPT, SDK_PED_PLAINTEXT, SDK_PED_DES, SDK_PED_SM4, SDK_PED_AES
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_update_work_key(uint32_t master_key_index, std::vector<uint8_t> pin_key_data, 
        std::vector<uint8_t> mac_key_data, std::vector<uint8_t> tdk_key_data, uint8_t write_mode);

    /**
     * @brief 加解密数据 (命令字: 0x75)
     * @param[in] 主密钥索引
     * @param[in] 工作密钥类型
     * @param[in] 初始向量
     * @param[in] 待加密数据
     * @param[in] 算法模式
     * @param[in] 模式
     * @param[out] 加密数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_encrypt_data(uint32_t master_key_index, uint8_t work_key_type, std::vector<uint8_t> iv, 
        std::vector<uint8_t> data, uint8_t algorithm_mode, uint8_t mode, std::vector<uint8_t>& encrypted_data);

    /**
     * @brief 加密磁道数据 (命令字: 0x76)
     * @param[in] 主密钥索引
     * @param[in] 磁道加密方式
     * @param[in] 待加密的磁道数据
     * @param[out] 加密后的磁道数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_encrypt_magnetic_stripe_data(uint32_t master_key_index, uint8_t magnetic_stripe_encryption_mode, 
        std::vector<uint8_t> magnetic_stripe_data, std::vector<uint8_t>& encrypted_magnetic_stripe_data);

    /**
     * @brief 计算MAC (命令字: 0x77)
     * @param[in] 主密钥索引
     * @param[in] 待计算数据
     * @param[in] MAC 算法模式
     * @param[out] 计算后的MAC
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_calculate_mac(uint32_t master_key_index, std::vector<uint8_t> data, uint8_t mac_algorithm_mode, 
        std::vector<uint8_t>& mac);

    /**
     * @brief 输入联机PIN (命令字: 0x78)
     * @param[in] 主密钥索引
     * @param[in] 支持 PIN 的长度
     * @param[in] 显示*号的行号
     * @param[in] 显示*号的列号
     * @param[in] 卡号
     * @param[in] 加密方式
     * @param[in] 等待输入时间
     * @param[out] 输入的密文PIN
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_input_online_pin(uint32_t master_key_index, std::vector<uint8_t> pin_length, uint8_t row_number, uint8_t column_number, 
        std::vector<uint8_t> card_number, uint8_t encryption_mode, uint32_t wait_input_time, std::vector<uint8_t>& encrypted_pin);

    /**
     * @brief 生成RSA密钥对输出公钥(N+E) (命令字: 0x7B)
     * @param[in] 期望生成 RSA 密钥长度
     * @param[in] 公钥指数
     * @param[out] 公钥模
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_generate_rsa_key_pair_output_public_key(uint32_t expected_key_length, std::vector<uint8_t> public_exponent, 
            std::vector<uint8_t>& public_key);     

    /**
     * @brief 使用已生成RSA私钥加密(N+D) (命令字: 0x7C)
     * @param[in] 输入数据
     * @param[out] 加密后的数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_encrypt_rsa_private_key(std::vector<uint8_t> input_data, std::vector<uint8_t>& encrypted_data);

    /**
     * @brief 硬件序列号加密 (命令字: 0x7D)
     * @param[in] 待加密数据
     * @param[in] 模式
     * @param[out] 加密后的数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_encrypt_hardware_serial_number(std::vector<uint8_t> data, uint8_t mode, std::vector<uint8_t>& encrypted_data);

    /**
     * @brief 查看触发状态 (命令字: 0x84)
     * @param[out] 复位检查开关
     * @param[out] 自毁开启标志
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_check_trigger_status(uint8_t &reset_check_switch, uint8_t &self_destruction_flag);

    /**
     * @brief 设置触发状态 (命令字: 0x85)
     * @param[in] 防抖时间
     * @param[in] 复位检查开关
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_set_trigger_status(uint32_t debounce_time, uint8_t reset_check_switch);

    /**
     * @brief 解触发 (命令字: 0x86)
     * @param[in] 模式
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_release_trigger(uint8_t mode);

    /**
     * @brief 密钥检查 (命令字: 0x87)
     * @param[in] 需要检测密钥的类型
     * @param[in] 需要检测密钥的索引
     * @param[out] KCV值
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_check_key(uint8_t key_type, uint32_t key_index, std::vector<uint8_t>& kcv);

    /**
     * @brief SM3哈希算法 (命令字: 0x95)
     * @param[in] 待计算数据
     * @param[out] 计算后的哈希值
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_sm3_hash_algorithm(std::vector<uint8_t> data, std::vector<uint8_t>& hash);

    /**
     * @brief DES加解密算法 (命令字: 0x96)
     * @param[in] 算法模式 @see SDK_MATH_MODE_ECB, SDK_MATH_MODE_CBC, SDK_MATH_MODE_CFB, SDK_MATH_MODE_OFB
     * @param[in] 加解密模式 @see SDK_MATH_DECRYPT, SDK_MATH_ENCRYPT
     * @param[in] 初始向量
     * @param[in] 待计算数据
     * @param[in] 密钥
     * @param[out] 计算后的结果
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_des_encrypt_decrypt_algorithm(uint8_t algorithm_mode, uint8_t encrypt_decrypt_mode, 
        std::vector<uint8_t> iv, std::vector<uint8_t> data, std::vector<uint8_t> key, std::vector<uint8_t>& result);

    /**
     * @brief AES加解密算法 (命令字: 0x97)
     * @param[in] 算法模式 @see SDK_MATH_MODE_ECB, SDK_MATH_MODE_CBC, SDK_MATH_MODE_CFB, SDK_MATH_MODE_OFB
     * @param[in] 加解密模式 @see SDK_MATH_DECRYPT, SDK_MATH_ENCRYPT
     * @param[in] 初始向量
     * @param[in] 待计算数据
     * @param[in] 密钥
     * @param[out] 计算后的结果
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_aes_encrypt_decrypt_algorithm(uint8_t algorithm_mode, uint8_t encrypt_decrypt_mode, 
        std::vector<uint8_t> iv, std::vector<uint8_t> data, std::vector<uint8_t> key, std::vector<uint8_t>& result);

    /**
     * @brief SM4加解密算法 (命令字: 0x98)
     * @param[in] 算法模式 @see SDK_MATH_MODE_ECB, SDK_MATH_MODE_CBC, SDK_MATH_MODE_CFB, SDK_MATH_MODE_OFB
     * @param[in] 加解密模式 @see SDK_MATH_DECRYPT, SDK_MATH_ENCRYPT
     * @param[in] 密钥
     * @param[in] 初始向量
     * @param[in] 待计算数据
     * @param[out] 计算后的结果
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_sm4_encrypt_decrypt_algorithm(uint8_t algorithm_mode, uint8_t encrypt_decrypt_mode, 
        std::vector<uint8_t> key, std::vector<uint8_t> iv, std::vector<uint8_t> data, std::vector<uint8_t>& result);
    

    /**
     * @brief SM2加解密 (命令字: 0x99)
     * @param[in] 加解密模式 @see SDK_MATH_DECRYPT, SDK_MATH_ENCRYPT
     * @param[in] 待计算数据
     * @param[in] 密钥
     * @param[out] 计算后的结果
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_sm2_encrypt_decrypt_algorithm(uint8_t encrypt_decrypt_mode, std::vector<uint8_t> data, 
        std::vector<uint8_t> key, std::vector<uint8_t>& result);

    /**
     * @brief SM2签名算法 (命令字: 0x9A)
     * @param[in] 公钥
     * @param[in] 私钥
     * @param[in] UID
     * @param[in] 待签名的数据
     * @param[out] 签名后的数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_sm2_signature_algorithm(std::vector<uint8_t> public_key, std::vector<uint8_t> private_key, 
        std::vector<uint8_t> uid, std::vector<uint8_t> data, std::vector<uint8_t>& result);

    /**
     * @brief SM2验签算法 (命令字: 0x9B)
     * @param[in] 验签值
     * @param[in] 公钥
     * @param[in] UID
     * @param[in] 要验签的数据
     * @param[out] 验签后的结果
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int keypad_sm2_verify_algorithm(std::vector<uint8_t> signature, std::vector<uint8_t> public_key, 
        std::vector<uint8_t> uid, std::vector<uint8_t> data, std::vector<uint8_t>& result);

    /**
     * @brief 文件下载启动 (命令字: 0xAA)
     * @param[in] 下载的文件类型
     * @param[in] 压缩格式
     * @param[in] 参数是否可重新更新
     * @param[in] 保留
     * @param[in] 文件字节
     * @param[in] 文件内容CRC32
     * @param[in] 文件名
     * @param[out] 单包长度
     * @param[out] 起始偏移
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int file_download_start_download(uint8_t file_type, uint8_t compress_format, uint8_t parameter_update, uint8_t reserved, 
        uint32_t file_size, uint32_t file_crc32, std::vector<uint8_t> file_name, uint32_t& single_packet_length, uint32_t& start_offset);

    /**
     * @brief 文件下载 (命令字: 0xAB)
     * @param[in] 包序号 1~255循环，当为0时为最后一包
     * @param[in] 文件数据偏移量
     * @param[in] 文件数据
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int file_download(uint8_t packet_number, uint32_t file_data_offset, std::vector<uint8_t> file_data);

    /**
     * @brief 内部认证 (命令字: 0xA0)
     * @param[in] 随机数R1
     * @param[out] 加密随机数R1(Key1)
     * @param[out] 随机数R2
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int internal_authentication(std::vector<uint8_t> random_number_r1, std::vector<uint8_t>& encrypted_random_number_r1, 
        std::vector<uint8_t>& random_number_r2);

    /**
     * @brief 外部认证后硬件序列号下载 (命令字: 0xA1)
     * @param[in] 加密随机数R2(Key2)
     * @param[in] 自毁重置标志
     * @param[in] SN号重置标志
     * @param[in] 日期时间
     * @param[in] 硬件序列号
     * @param[in] 硬件序列号密钥
     * @param[in] 主密钥
     * @param[in] 客户自定义序列号
     * @param[in] 机构私钥
     * @param[in] 附加密钥
     * @param[out] 硬件信息
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int external_authentication_hardware_serial_number_download(std::vector<uint8_t> encrypted_random_number_r2, uint8_t self_destruction_reset_flag, 
        uint8_t sn_reset_flag, std::vector<uint8_t> date_time, std::vector<uint8_t> hardware_serial_number, 
        std::vector<uint8_t> hardware_serial_number_key, std::vector<uint8_t> master_key, 
        std::vector<uint8_t> customer_custom_serial_number, std::vector<uint8_t> organization_private_key, 
        std::vector<uint8_t> additional_key, std::vector<uint8_t>& hardware_info);

    /**
     * @brief 外部认证后解锁 (命令字: 0xA7)
     * @param[in] encrypted_r2_key2 加密随机数R2(Key2)，8字节
     * @param[in] self_destruction_reset_flag 自毁重置标志 @see SDK_UNLOCK_MODE_UNLOCK, SDK_UNLOCK_MODE_LOCK
     * @return 成功返回SDK_OK，锁定返回0x01，其他错误返回错误码
     */
    int external_authentication_unlock(std::vector<uint8_t> encrypted_r2_key2, uint8_t self_destruction_reset_flag);

    /**
     * @brief 外部认证后加密芯片ID (命令字: 0xA8)
     * @param[in] encrypted_r2_key2 加密随机数R2(Key2)，8字节
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int external_authentication_encrypted_chip_id(std::vector<uint8_t> encrypted_r2_key2);

    /**
     * @brief 外部认证后重置Boot (命令字: 0xA9)
     * @param[in] encrypted_r2_key2 加密随机数R2(Key2)，8字节
     * @return 成功返回SDK_OK，失败返回错误码
     */
    int external_authentication_reset_boot(std::vector<uint8_t> encrypted_r2_key2);



};

}

#endif