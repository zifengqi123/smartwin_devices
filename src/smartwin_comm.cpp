#include "smartwin_comm.h"

namespace smartwin {

smartwin_comm::smartwin_comm(std::string port_name, int baudrate, int timeout,
        std::function<void(std::vector<uint8_t>)> callback) {  

    recv_callback_ = callback;

    printf("smartwin_comm port_name: %s, baudrate: %d\n", port_name.c_str(), baudrate);

    serial::Timeout to = serial::Timeout::simpleTimeout(timeout);
    to.write_timeout_constant = timeout;
    to.read_timeout_constant = timeout;
    to.read_timeout_multiplier = 10;

    _serial = new serial::Serial();
    _serial->setPort(port_name);
    _serial->setBaudrate(baudrate);
    _serial->setTimeout(to);
    
    try
    {
        _serial->open();
    }
    catch(serial::IOException& e)
    {
        printf("Err.Unable to open port. %s, err: %s\n", port_name.c_str(), e.what());
        return ;
    }

    thread_flag_ = true;

    pthread_mutex_init(&cmd_recv_mutex_, NULL);
    pthread_create(&cmd_recv_thread_, NULL, &smartwin_comm::cmd_recv_thread_func, this);   

}

smartwin_comm::~smartwin_comm() {
    thread_flag_ = false;
    pthread_mutex_destroy(&cmd_recv_mutex_);
    pthread_join(cmd_recv_thread_, NULL);
    delete _serial;
}

std::string smartwin_comm::printBuf(std::string t_str, std::vector<uint8_t> buf) {
    return printBuf(t_str, buf.data(), buf.size());
}

std::string smartwin_comm::printBuf(std::string t_str, uint8_t* buf, int ln) {
    std::string str = t_str;
    for(int i = 0; i < ln; i++) {
        char tmp[4] = {0};
        sprintf(tmp, "%02X ", buf[i]);
        str += tmp;
    }  
    return str;
}   

uint8_t smartwin_comm::xor_check(std::vector<uint8_t> buf) {
    uint8_t xor_value = 0;
    for(size_t i = 0; i < buf.size(); i++) {
        xor_value ^= buf[i];
    }
    return xor_value;
}

int smartwin_comm::sendcmd(std::vector<uint8_t> buf) {
    if(!_serial->isOpen()) {
        return -110;
    }

    std::vector<uint8_t> sb;

    sb.push_back(0x02);

    for(size_t i = 0; i < buf.size(); i++) {
        sb.push_back(buf[i]);
    }

    sb.push_back(0x03);
    sb.push_back(xor_check(buf));

    pthread_mutex_lock(&cmd_recv_mutex_);
    int ret = _serial->write(sb.data(), sb.size());
    pthread_mutex_unlock(&cmd_recv_mutex_);

#ifdef SERIAL_DEBUG_INFO
    printf("sendcmd ret: %d, %s\n", ret, printBuf("send: ", sb).c_str());
#endif


    if(ret != sb.size()) {
        printf("send Error: %d, %s\n", ret, printBuf("send: ", sb).c_str());
        return -1;
    }
    return 0;
}


void* smartwin_comm::cmd_recv_thread_func(void* arg) {
    smartwin_comm* comm = (smartwin_comm*)arg;
    printf("cmd_recv_thread_func start. %d\n", comm->thread_flag_);

    while(comm->thread_flag_) {

        std::vector<uint8_t> recv_buf;
        try
        {
            size_t num = comm->_serial->available();
            if(num > 0)
            {
                pthread_mutex_lock(&comm->cmd_recv_mutex_);
                // printf("_serial->available num: %d\n", num);

                num = comm->_serial->read(comm->t_buffer, 1);
                // printf("=1. _serial->available num: %d\n", num);

                if(num > 0) {
                    if(comm->t_buffer[0] == 0x02) {

                        num = comm->_serial->read(comm->t_buffer, 4);
                        // printf("=4. _serial->available num: %d\n", num);

                        if(num == 4) {
                            recv_buf.push_back(comm->t_buffer[0]);
                            recv_buf.push_back(comm->t_buffer[1]);
                            recv_buf.push_back(comm->t_buffer[2]);
                            recv_buf.push_back(comm->t_buffer[3]);

                            size_t ln = comm->t_buffer[2] * 256 + comm->t_buffer[3];

                            num = comm->_serial->read(comm->t_buffer, ln);

                            // printf("=%d. _serial->available num: %d\n", ln, num);
                            if(num == ln) {
                                recv_buf.insert(recv_buf.end(), comm->t_buffer, comm->t_buffer + num);

                                num = comm->_serial->read(comm->t_buffer, 2);
                                // printf("=2. _serial->available num: %d\n", num);

                                if(num == 2) {
                                    if(comm->t_buffer[0] == 0x03 &&
                                        comm->t_buffer[1] == comm->xor_check(recv_buf)) {

                                        if(comm->recv_callback_) {
                                            comm->recv_callback_(recv_buf);
                                        }
                                    }
                                    else {
                                        printf("%s\n", comm->printBuf("recv check error: ", recv_buf).c_str());
                                    }
                                }
                            }
                        }
                    }
                }
                pthread_mutex_unlock(&comm->cmd_recv_mutex_);

            }
        }
        catch(serial::IOException &err)
        {
            printf("read_some err: %s\n", err.what());
        }
        
        usleep(20*1000);
    }

    return nullptr;
}

}
