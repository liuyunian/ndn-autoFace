#ifndef CLIENT_H_
#define CLIENT_H_

#include<iostream>

class Client{
public:
    Client(const std::string & interface, const uint8_t * localMacAddr);
    ~Client();

    void sendMulticastFrame();
    void sendUnicastFrame(const u_int8_t * distAddr);

private:
    std::string m_interface;
    const uint8_t * m_localMacAddr;
};

#endif