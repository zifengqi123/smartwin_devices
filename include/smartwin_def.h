#ifndef __SMARTWIN_DEF_H__
#define __SMARTWIN_DEF_H__

/**
 * @brief SDK返回值定义
 */
#define SDK_OK                              (0)     /**< 成功 */
#define SDK_ERROR                           (-1)    /**< 失败 */
#define SDK_PARAMERR                        (-2)    /**< 参数错误 */
#define SDK_ESC                             (-120)  /**< 取消退出 */
#define SDK_TIMEOUT                         (-121)  /**< 超时 */

/**
 * @brief 磁条卡错误码定义
 */ 
#define ERROR_MAGNETIC_OTHER                (-1101)  // 失败
#define ERROR_MAGNETIC_PARAMERR             (-1102)  // 参数错误
#define ERROR_MAGNETIC_NOCARD               (-1103)  // 未刷卡

/**
 * @brief IC卡错误码定义
 */
#define ERROR_IC_OTHER                      (-1201)  // 失败
#define ERROR_IC_PARAMERR                   (-1202)  // 参数错误
#define ERROR_IC_NOCARD                     (-1203)  // 卡不在座子上
#define ERROR_IC_NOPOWER                    (-1204)  // 卡未上电
#define ERROR_IC_TIMEOUT                    (-1205)  // 超时错误,或无响应(L1 TIME OUT ERROR)
#define ERROR_IC_PROTOCOL                   (-1206)  // 协议错误（L1 PROTOCOL ERROR）
#define ERROR_IC_TRANSFER                   (-1207)  // 传输错误

/**
 * @brief 非接卡错误码定义
 */
#define ERROR_ICC_OTHER                      (-1301)  // 失败
#define ERROR_ICC_PARAMERR                   (-1302)  // 参数错误
#define ERROR_ICC_NOCARD                     (-1303)  // 未搜寻到卡片
#define ERROR_ICC_MULTICARD         (-1304)  // 感应区内卡片过多（多卡冲突）
#define ERROR_ICC_TIMEOUT           (-1305)  // 超时错误(L1 TIME OUT ERROR)
#define ERROR_ICC_PROTOCOL          (-1306)  // 协议错误(L1 PROTOCOL ERROR)
#define ERROR_ICC_TRANSFER          (-1307)  // 传输错误,任何其他错误

/**
 * @brief 打印机错误码定义
 */
#define ERROR_PRINTER_OTHER             (-1401)  // 失败， 或内存不足
#define ERROR_PRINTER_PARAMERR          (-1402)  // 参数错误
#define ERROR_PRINTER_PAPER_LOW         (-1403)  // 打印机缺纸
#define ERROR_PRINTER_TEMP_HIGH         (-1404)  // 温度过高
#define ERROR_PRINTER_DEVICE_FAULT      (-1405)  // 打印机设备故障

/**
 * @brief 密码键盘错误码定义
 */
#define ERROR_KEYPAD_OTHER             (-1501)  // 其它错误
#define ERROR_KEYPAD_PARAMERR          (-1502)  // 参数错误
#define ERROR_KEYPAD_CONFIRM           (-1503)  // 持卡人直接按确认（无密码）
#define ERROR_KEYPAD_CANCEL            (-1504)  // 用户取消输入PIN
#define ERROR_KEYPAD_TIMEOUT           (-1505)  // 超时退出
#define ERROR_KEYPAD_PINOVERLIMIT      (-1506)  // 输入 PIN 一小时不能超过 120 次
#define ERROR_KEYPAD_KEYREPEAT         (-1507)  // 密钥值重复
#define ERROR_KEYPAD_INVALIDKEYINDEX   (-1508)  // 无效密钥索引
#define ERROR_KEYPAD_NOKEY             (-1509)  // 未设置密钥
#define ERROR_KEYPAD_MAINKEYERR        (-1510)  // 主密钥校验错
#define ERROR_KEYPAD_PINKEYERR         (-1511)  // PIN密钥校验错
#define ERROR_KEYPAD_TDKEYERR          (-1512)  // TD密钥校验错
#define ERROR_KEYPAD_MACKEYERR         (-1513)  // MAC密钥校验错
#define ERROR_KEYPAD_INITFAIL           (-1520)  // 密码键盘初始化失败
#define ERROR_KEYPAD_RANDOMNUMFAIL      (-1521)  // 获取随机数失败
#define ERROR_KEYPAD_TERMINAL_DESTROYED (-1522)  // 终端已自毁
#define ERROR_KEYPAD_KEYTRIGGERDISABLE  (-1523)  // 密钥触发禁用





/**
 * @brief 终端信息TAG
 */
#define INFO_MERCHANT_NAME      (0xA0) // 平台名称
#define INFO_MNT_VER            (0xA1) // mnt版本
#define INFO_HARDWARE_VER       (0xA2) // 硬件版本号(参数文件控制)
#define INFO_BOOT_VER           (0xA3) // BOOT版本号
#define INFO_APP_VER            (0xA4) // 应用版本
#define INFO_SN                 (0xA5) // SN
#define INFO_DATE               (0xA6) // 出厂日期
#define INFO_GSM_VER            (0xA7) // GPRS模块版本
#define INFO_SDK_VER            (0xA8) // SDK VER
#define INFO_IMEI               (0xA9) // IMEI
#define INFO_ICCID              (0xAA) // ICCID
#define INFO_IMSI               (0xAB) // IMSI
#define INFO_TERM_TYPE          (0xAC) // 终端类型
#define INFO_OPTION             (0xAD) // 硬件选配信息(TP/SCAN/WIFI)
#define INFO_EFLASH             (0xAE) // 外部FLAHS型号
#define INFO_CSN                (0xAF) // 客户CSN号
#define INFO_LCD                (0xB0)  // LCD类型
#define INFO_RES                (0xB1)  // 资源名称



/**
 * @brief 按键值定义, 中文注释
 */
#define KEY_0                   0x30            /**< 0 */
#define KEY_1                   0x31            /**< 1 */
#define KEY_2                   0x32            /**< 2 */
#define KEY_3                   0x33            /**< 3 */
#define KEY_4                   0x34            /**< 4 */
#define KEY_5                   0x35            /**< 5 */
#define KEY_6                   0x36            /**< 6 */
#define KEY_7                   0x37            /**< 7 */       
#define KEY_8                   0x38            /**< 8 */
#define KEY_9                   0x39            /**< 9 */
#define KEY_LETTER              0x07            /**< 字母 */
#define KEY_BACKSPACE           0x08            /**< 退格 */
#define KEY_CONFIRM             0x0D            /**< 确认 */
#define KEY_LEFT                0x12            /**< 向左 */
#define KEY_RIGHT               0x13            /**< 向右 */
#define KEY_MENU                0x14            /**< 菜单 */
#define KEY_FUNCTION            0x15            /**< 功能 */
#define KEY_PRINT_UP            0x19            /**< 打印向上走纸 */
#define KEY_CANCEL              0x2E            //0x1B            /**< 取消 */
#define KEY_UP                  0x26            /**< 向上 */
#define KEY_DOWN                0x28            /**< 向下 */
#define KEY_STAR                0x2A            /**< '*'键 */
#define KEY_CLEAR               0x1B            //0x2E            /**< 清除 */
#define KEY_INVALID             0xFF            /**< 无效按键 */
#define KEY_TIMEOUT             0x00            /**< 超时无按键 */
#define KEY_OTHER               0x00            /**< 其他 */

/**
 * @brief 算法模式
 */
#define SDK_MATH_MODE_ECB  (0x00)            /**< ECB模式 */
#define SDK_MATH_MODE_CBC  (0x01)            /**< CBC模式 */
#define SDK_MATH_MODE_CFB  (0x02)            /**< CFB模式 */
#define SDK_MATH_MODE_OFB  (0x03)            /**< OFB模式 */

/**
 * @brief 加解密模式
 */
#define SDK_MATH_DECRYPT   (0x00)            /**< 解密 */
#define SDK_MATH_ENCRYPT   (0x01)            /**< 加密 */

/**
 * @brief 解锁模式
 */
#define SDK_UNLOCK_MODE_UNLOCK   (0x01)        /**< 解锁 */
#define SDK_UNLOCK_MODE_LOCK     (0x00)        /**< 锁定 */

/**
 * @brief 写入模式
 */
#define SDK_PED_DECRYPT  (0x00)              /**< 解密 */
#define SDK_PED_ENCRYPT  (0x01)              /**< 加密 */
#define SDK_PED_PLAINTEXT  (0x02)            /**< 明文 */
#define SDK_PED_DES  (0x00)                  /**< DES */
#define SDK_PED_SM4  (0x80)                  /**< SM4 */
#define SDK_PED_AES  (0x40)                  /**< AES */
#define SDK_PED_XOR  (0xC0)                  /**< XOR */

/**
 * @brief 打印对齐方式
 */
#define SDK_PRINT_ALIGN_LEFT  (0x01)          /**< 左对齐 */    
#define SDK_PRINT_ALIGN_RIGHT  (0x02)          /**< 右对齐 */
#define SDK_PRINT_ALIGN_CENTER  (0x04)          /**< 居中对齐 */

/**
 * @brief 寻卡方式
 */
#define SDK_SWIPE_CARD_HAND     (0x01)          /**< 手动寻卡 */
#define SDK_SWIPE_CARD_MAG      (0x02)          /**< 磁条卡寻卡 */
#define SDK_SWIPE_CARD_ICC      (0x04)          /**< IC卡寻卡 */
#define SDK_SWIPE_CARD_RF       (0x08)          /**< 非接卡寻卡 */

/**
 * @brief 非接卡类型
 */
#define SDK_ICC_TYPE_A  (0x01)                /**< A卡 */
#define SDK_ICC_TYPE_B  (0x02)                /**< B卡 */
#define SDK_ICC_TYPE_M1  (0x04)                /**< M1卡 */
#define SDK_ICC_TYPE_C  (0x08)                /**< C卡 */

/**
 * @brief 卡类型
 */
#define SDK_CARD_TYPE_CPU  (0x00)              /**< CPU卡 */
#define SDK_CARD_TYPE_AT24CXX  (0x01)          /**< AT24CXX */
#define SDK_CARD_TYPE_MEMORY  (0x02)          /**< 内存卡 */

/**
 * @brief 卡座号
 */
#define SDK_CARD_SEAT_STANDARD  (0x00)          /**< 标准卡座 */
#define SDK_CARD_SEAT_SAM1  (0x01)
#define SDK_CARD_SEAT_SAM2  (0x02)
#define SDK_CARD_SEAT_SAM3  (0x03)

/**
 * @brief 密钥类型
 */
#define SDK_KEY_TYPE_A  'A'                /**< A卡 */
#define SDK_KEY_TYPE_B  'B'                /**< B卡 */

/**
 * @brief 非接卡操作指令
 */
#define SDK_MIFARE_CARD_OPERATION_READ          'R'    /**< 读取 */
#define SDK_MIFARE_CARD_OPERATION_WRITE         'W'    /**< 写入 */
#define SDK_MIFARE_CARD_OPERATION_INCREMENT     '+'    /**< 增加 */
#define SDK_MIFARE_CARD_OPERATION_DECREMENT     '-'    /**< 减少 */
#define SDK_MIFARE_CARD_OPERATION_TRANSFER      '>'    /**< 转移 */


/**
 * @brief 键盘背光状态
 */
#define SDK_KEYBOARD_BACKLIGHT_OFF  (0x00)        /**< 关闭 */
#define SDK_KEYBOARD_BACKLIGHT_ON  (0x01)


/**
 * @brief 键盘按键音状态
 */
#define SDK_KEYBOARD_SOUND_OFF  (0x00)          /**< 关闭 */
#define SDK_KEYBOARD_SOUND_ON  (0x01)           /**< 开启 */

/**
 * @brief 低功耗状态唤醒类型
 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_ANY_KEY  (0x01)  /**< 任意按键 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_IO  (0x02)      /**< IO口 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_IC  (0x04)      /**< IC卡 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_MAG  (0x08)      /**< 磁条卡 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_ICC  (0x10)      /**< 非接卡 */
#define SDK_SLEEP_MODE_WAKEUP_TYPE_TP  (0x20)

/**
 * @brief 指示灯 ID,按位控制
 */
#define SDK_LED_ID_BLUE  (0x01)              /**< 蓝色 */
#define SDK_LED_ID_YELLOW  (0x02)
#define SDK_LED_ID_GREEN  (0x04)
#define SDK_LED_ID_RED  (0x08)
#define SDK_LED_ID_ALL  (0x0F)

/**
 * @brief 蜂鸣器鸣叫类型
 */
#define SDK_BEEP_NORMAL  (0x00)              /**< 正常 1声 */
#define SDK_BEEP_ABNORMAL  (0x01)            /**< 异常 3声 */


/**
 * @brief 版本类型
 */
#define SDK_VERSION_TYPE_HARDWARE  (0x00)    /**< 硬件(内部版本号) */
#define SDK_VERSION_TYPE_BOOT  (0x01)        /**< Boot 版本号 */
#define SDK_VERSION_TYPE_KERNEL  (0x02)      /**< 内核版本号 */
#define SDK_VERSION_TYPE_SYSTEM  (0x03)      /**< 系统版本号 */
#define SDK_VERSION_TYPE_LIBRARY  (0x04)     /**< 底层库版本 */
#define SDK_VERSION_TYPE_SDK  (0x05)         /**< SDK 版本号 */

/**
 * @brief 通讯模式
 */
#define SDK_COMMUNICATION_MODE_NONE  (0x00)    /**< 不需要检测通讯模块 */
#define SDK_COMMUNICATION_MODE_GPRS  (0x01)    /**< GPRS,4G 等 */
#define SDK_COMMUNICATION_MODE_WIFI  (0x02)    /**< WIFI */
#define SDK_COMMUNICATION_MODE_BLUETOOTH  (0x04)    /**< 蓝牙 */





#endif