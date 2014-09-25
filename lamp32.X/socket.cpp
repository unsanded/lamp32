/* 
 * File:   socket.cpp
 * Author: david
 * 
 * Created on 22 September 2014, 14:29
 */

#include "socket.hpp"

socket::socket(uns8 type, uns8 domain, uns8 protocol)
{
    debug_str("create\r\n");
    refcount = new int(1);
    *refcount=1;
    sd=cc3000_socket(domain, type, protocol);
    listening=false;
    connected=false;
    valid= (sd>=0);
    autoclose=true;

}
socket::socket(const socket& orig){
    debug_str("COPY SD: ");
    sd=orig.sd;
    debug_int_hex(sd);
    debug_nl();
    
    connected=orig.connected;
    valid    =orig.valid;
    listening=orig.listening;
    autoclose=orig.autoclose;
    refcount =orig.refcount;
    (*refcount)++;
    debug_var(*refcount);
    debug_var(refcount);
    bob =orig.bob;
    port=orig.port;

}

void socket::send(const char* data, int length){

    debug_str("SEND SD:");
    debug_int_hex(sd);
    debug_nl();
    if(connected && valid)
            cc3000_send(sd,(unsigned char* ) data, length, 0);
    else
        debug_str("SOCKET UNCONNECTED SEND");

}

socket& socket::operator<<(const char* data)
{
    if(connected && valid)
            cc3000_send(sd,(unsigned char* ) data, strlen(data), 0);
    else
        debug_str("SOCKET UNCONNECTED SEND");
    return *this;
}

int32 socket::recv(char* data, int length, int timeout){
    int32 rlen;
    if(connected && valid)
        rlen= cc3000_recv(sd, (unsigned char*) data, length, 0, timeout);
    else
        rlen= -1;

    return rlen;
}


socket::~socket() {
    (*refcount)--;//lesson learned ;) brackets are important :|
    
    if(*refcount==0){
        delete refcount;
            debug_str("Deleting ");
            debug_int_hex(sd);
        if(autoclose && valid){
            debug_str(" adn closing");
            cc3000_socket_close(sd);
        }
            debug_nl();
    }
    else
        debug_var(*refcount);
}

bool socket::connect(uns16 port, ip_addr_t addr)
{
    uns8 status = cc3000_connect(sd, port, addr);
    if(!status)
    {
        connected=true;
        bob=addr;
    }
}
bool socket::listen(uns16 port){
    ip_addr_t zero;
    zero.ip[0]=0;
    zero.ip[1]=0;
    zero.ip[2]=0;
    zero.ip[3]=0;
    return listen(port, zero);
}
bool socket::listen(uns16 port, ip_addr_t addr)
{
    uns8 status;
    if(valid){
        status=cc3000_bind(sd, addr, port);
        status=cc3000_listen(sd);
    }
    else
        debug_str("LISTEN ON INVALID SOCKET");
    bob=addr;
    this->port=port;
    listening=true;
    return status!=0;
}

socket socket::accept(){
    socket res;// starts of as invalid socket
    if(!listening) return res;

    int newsd=cc3000_accept(sd);
    if(newsd<0) return res;
    res.valid=true;
    res.bob.ip[0]=wlan_rx_buffer[14];
    res.bob.ip[1]=wlan_rx_buffer[13];
    res.bob.ip[2]=wlan_rx_buffer[12];
    res.bob.ip[3]=wlan_rx_buffer[11];

    res.sd=newsd;
    res.autoclose=true;
    res.valid=true;
    res.connected=true;
    res.port=port;
    
    return res;

}
socket::operator bool(){
    return valid;
}


socket::socket(){
    valid=false;
    debug_str("create\r\n");
    refcount= new int(1);
    *refcount=1;// one reference: this
}


