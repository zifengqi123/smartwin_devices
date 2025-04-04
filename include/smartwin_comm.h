#ifndef __SMARTWIN_COMM_H__
#define __SMARTWIN_COMM_H__

#include "serial/serial.h"  
#include <vector>
#include <string>
#include <stdint.h> 
#include <functional>
#include <unistd.h>

#define SERIAL_DEBUG_INFO 1

namespace smartwin {

class smartwin_comm {

private:
    serial::Serial * _serial;

    pthread_t cmd_recv_thread_;
    pthread_mutex_t cmd_recv_mutex_;

    std::function<void(std::vector<uint8_t>)> recv_callback_;

    bool thread_flag_ = false;

public:

    smartwin_comm(std::string port_name, int baudrate, int timeout,
                    std::function<void(std::vector<uint8_t>)> callback);

    ~smartwin_comm();
    
    std::string printBuf(std::string t_str, std::vector<uint8_t> buf);
    std::string printBuf(std::string t_str, uint8_t* buf, int ln);

    uint8_t xor_check(std::vector<uint8_t> buf);
    
    int sendcmd(std::vector<uint8_t> buf);

    static void* cmd_recv_thread_func(void* arg);

    uint8_t t_buffer[1024];

};

}

#endif
