/* 
 * File:   socket.hpp
 * Author: david
 *
 * Created on 22 September 2014, 14:29
 */

#include "cc3000.h"
#include "cc3000_socket.h"

#ifndef SOCKET_HPP
#define	SOCKET_HPP

class socket {
public:
    socket(uns8 type, uns8 domain=AF_INET, uns8 protocol=IPPROTO_IPV4);
    socket(const socket& orig);
    virtual ~socket();

    bool connect(uns16 port, ip_addr_t addr);
    bool listen(uns16 port, ip_addr_t addr);
    bool listen(uns16 port);
    socket accept();

    socket& operator<<(const char* data);
    void send(const char* data, int length);
    int32 recv(char* data, int len, int timeout=20);
    operator bool();
    bool autoclose;
private:

    int* refcount;
    int sd;
    bool valid;
    bool listening;
    bool connected;

    ip_addr_t bob;
    uns16 port;

    socket();


};

#endif	/* SOCKET_HPP */

