#ifndef __SMARTWIN_CMD_H__
#define __SMARTWIN_CMD_H__


/**
 * @brief 命令字定义
 */
#define CMD_SET_COMM_MODE                           0x11    /**< 设置通讯方式 */
#define CMD_GET_NETWORK_MODE                        0x12    /**< 获取网络制式 */
#define CMD_GET_SYSTEM_VERSION                      0x17    /**< 读取系统版本信息 */
#define CMD_GET_HARDWARE_SERIAL_NUMBER              0x18    /**< 获取硬件序列号 */
#define CMD_GET_DEVICE_MODEL                        0x19    /**< 获取设备型号 */
#define CMD_GET_CUSTOMER_SERIAL_NUMBER              0x1A    /**< 获取客户自定义序列号 */
#define CMD_SET_CLOCK                               0x1B    /**< 设置时钟 */
#define CMD_GET_CLOCK                               0x1C    /**< 获取时钟 */
#define CMD_BEEP                                    0x20    /**< 蜂鸣 */
#define CMD_BEEP_FREQUENCY                          0x21    /**< 蜂鸣器按指定的频率发声 */
#define CMD_LED_ON                                  0x22    /**< 点亮LED指示灯 */
#define CMD_LED_OFF                                 0x23    /**< 熄灭LED指示灯 */
#define CMD_LED_FLASH                               0x24    /**< LED指示灯闪烁 */
#define CMD_SYSTEM_RESET                            0x25    /**< 系统复位 */
#define CMD_SYSTEM_SHUTDOWN                         0x26    /**< 系统关机 */
#define CMD_SET_TERMINAL_SERIAL_NUMBER              0x28    /**< 设置终端序列号 */
#define CMD_GET_CHIP_SERIAL_NUMBER                  0x29    /**< 获取芯片序列号 */
#define CMD_ENABLE_SLEEP_MODE                       0x2A    /**< 使能下位机立即进入休眠模式 */
#define CMD_ENTER_BOOT_OR_QUERY_STATE               0x2F    /**< 下位机进入BOOT或查询状态 */
#define CMD_OPEN_KEYBOARD                           0x30    /**< 打开键盘 */
#define CMD_CLOSE_KEYBOARD                          0x31    /**< 关闭键盘 */
#define CMD_READ_KEYBOARD_INPUT                     0x32    /**< 读取键盘输入 */
#define CMD_CLEAR_KEYBOARD_CACHE                    0x33    /**< 清除按键缓存 */
#define CMD_SET_KEYBOARD_SOUND                      0x34    /**< 设置打开关闭按键音 */
#define CMD_SET_KEYBOARD_BACKLIGHT                  0x35    /**< 设置按键背光 */
#define CMD_OPEN_TP                                 0x3A    /**< 打开TP */
#define CMD_CLOSE_TP                                0x3B    /**< 关闭TP */
#define CMD_CHECK_TP_SUPPORT                        0x3C    /**< 检查是否支持TP */
#define CMD_GET_TOUCH_COORDINATE                    0x3E    /**< 获取触控坐标值 */
#define CMD_SET_TOUCH_PARAMETER                     0x3F    /**< 设置触控参数 */
#define CMD_OPEN_MAGNETIC_STRIPE_CARD               0x40    /**< 打开磁条卡设备 */
#define CMD_CLOSE_MAGNETIC_STRIPE_CARD              0x41    /**< 关闭磁条卡设备 */
#define CMD_CHECK_MAGNETIC_STRIPE_CARD              0x42    /**< 检查是否有刷磁条卡 */
#define CMD_READ_MAGNETIC_STRIPE_CARD_DATA          0x43    /**< 读取磁条卡数据 */
#define CMD_CLEAR_MAGNETIC_STRIPE_CARD_DATA         0x44    /**< 清除磁卡数据缓冲 */
#define CMD_FORMAT_MAGNETIC_STRIPE_CARD_DATA        0x45    /**< 格式化磁道信息 */
#define CMD_OPEN_IC_CARD_MODULE                     0x4A    /**< 打开IC卡模块 */
#define CMD_CLOSE_IC_CARD_MODULE                    0x4B    /**< 关闭IC卡模块 */
#define CMD_CHECK_IC_STATUS                         0x4C    /**< 检查ICC状态 */
#define CMD_IC_CARD_RESET                           0x4D    /**< IC卡上电复位 */
#define CMD_IC_CARD_MODULE_POWER_OFF                0x4E    /**< IC卡模块下电 */
#define CMD_IC_CARD_SEND_APDU_COMMAND               0x4F    /**< IC卡发送APDU命令 */
#define CMD_ICC_OPEN_MODULE                         0x50    /**< 打开非接模块 */
#define CMD_ICC_CLOSE_MODULE                        0x51    /**< 关闭非接模块 */
#define CMD_ICC_SEARCH_CARD_ACTIVATION              0x52    /**< 寻卡激活 */
#define CMD_ICC_SEND_APDU_COMMAND                   0x53    /**< 发送APDU命令 */
#define CMD_MIFARE_CARD_AUTHENTICATION              0x54    /**< Mifare卡认证 */
#define CMD_MIFARE_CARD_OPERATION                   0x55    /**< Mifare卡操作 */
#define CMD_SEARCH_CARD_START                       0x46    /**< 单独寻卡报文 */
#define CMD_SEARCH_CARD_RESULT                      0x47    /**< 发送寻卡结果报文 */
#define CMD_SEARCH_CARD_STOP                        0x48    /**< 结束寻卡 */
#define CMD_SCAN_OPEN                               0x5A    /**< 打开扫码 */
#define CMD_SCAN_CLOSE                              0x5B    /**< 关闭扫码 */
#define CMD_READ_SCAN_DATA                          0x5C    /**< 读取扫码数据 */
#define CMD_CHECK_PRINTER_SUPPORT                   0x61    /**< 检查是否支持打印机 */
#define CMD_PRINTER_OPEN                            0x62    /**< 打开打印机 */
#define CMD_PRINTER_CLOSE                           0x63    /**< 关闭打印机 */
#define CMD_QUERY_PRINTER_STATUS                    0x64    /**< 查询打印机状态 */
#define CMD_SET_PRINTER_GRAY                         0x65    /**< 设置打印灰度 */
#define CMD_PAPER_FEED                               0x6B    /**< 走纸 */
#define CMD_PRINT_BITMAP_DATA                        0x6C    /**< 打印位图数据 */
#define CMD_KEYPAD_OPEN_PASSWORD                     0x70    /**< 打开密码键盘 */
#define CMD_KEYPAD_CLOSE_PASSWORD                    0x71    /**< 关闭密码键盘 */
#define CMD_KEYPAD_GET_RANDOM_NUMBER                 0x72    /**< 获取随机数 */
#define CMD_KEYPAD_UPDATE_MASTER_KEY                 0x73    /**< 更新主密钥 */
#define CMD_KEYPAD_UPDATE_WORK_KEY                   0x74    /**< 更新工作密钥 */
#define CMD_KEYPAD_ENCRYPT_DATA                       0x75    /**< 加密数据 */
#define CMD_KEYPAD_ENCRYPT_MAGNETIC_STRIPE_DATA     0x76    /**< 加密磁道数据 */
#define CMD_KEYPAD_CALCULATE_MAC                    0x77    /**< 计算MAC */
#define CMD_KEYPAD_INPUT_ONLINE_PIN                 0x78    /**< 输入联机PIN */
#define CMD_KEYPAD_GEN_RSA_KEY_PAIR_OUTPUT_PUBLIC_KEY 0x7B   /**< 生成RSA密钥对输出公钥 */
#define CMD_KEYPAD_ENCRYPT_RSA_PRIVATE_KEY          0x7C    /**< 使用已生成RSA私钥加密 */
#define CMD_KEYPAD_ENCRYPT_HARDWARE_SERIAL_NUMBER   0x7D    /**< 硬件序列号加密 */
#define CMD_KEYPAD_CHECK_TRIGGER_STATUS             0x84    /**< 查看触发状态 */
#define CMD_KEYPAD_SET_TRIGGER_STATUS               0x85    /**< 设置触发状态 */
#define CMD_KEYPAD_RELEASE_TRIGGER                  0x86    /**< 解触发 */
#define CMD_KEYPAD_CHECK_KEY                        0x87    /**< 密钥检查 */
#define CMD_KEYPAD_SM3_HASH                         0x95    /**< SM3哈希算法 */
#define CMD_KEYPAD_DES_ENCRYPT_DECRYPT              0x96    /**< DES加解密算法 */
#define CMD_KEYPAD_AES_ENCRYPT_DECRYPT              0x97    /**< AES加解密算法 */
#define CMD_KEYPAD_SM4_ENCRYPT_DECRYPT              0x98    /**< SM4加解密算法 */
#define CMD_KEYPAD_SM2_ENCRYPT_DECRYPT              0x99    /**< SM2加解密算法 */
#define CMD_KEYPAD_SM2_SIGN                         0x9A    /**< SM2签名算法 */
#define CMD_KEYPAD_SM2_VERIFY                       0x9B    /**< SM2验签算法 */
#define CMD_FILE_DOWNLOAD_START                     0xAA    /**< 文件下载启动 */
#define CMD_FILE_DOWNLOAD                           0xAB    /**< 文件下载 */
#define CMD_INTERNAL_AUTH                           0xA0    /**< 内部认证 */
#define CMD_EXTERNAL_AUTH_DOWNLOAD_HARDWARE_SERIAL  0xA1    /**< 外部认证后硬件序列号下载 */
#define CMD_EXTERNAL_AUTH_UNLOCK                    0xA7    /**< 外部认证后解锁 */
#define CMD_EXTERNAL_AUTH_ENCRYPT_CHIP_ID           0xA8    /**< 外部认证后加密芯片ID */
#define CMD_EXTERNAL_AUTH_RESET_BOOT                0xA9    /**< 外部认证后重置Boot */


#endif
