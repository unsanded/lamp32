/* 
 * File:   HttpServer.cpp
 * Author: david
 * 
 * Created on 23 September 2014, 00:44
 */

#include "HttpServer.h"

#include "string"

using namespace std;

HttpServer::HttpServer(uns16 port):
sock(SOCK_STREAM)
{
    ip_addr_t zero;
    
    sock.listen(port);
}

HttpServer::~HttpServer() {
}

void HttpServer::tick(){
    socket newsock(sock.accept());
    if(!newsock) return;

    newsock.autoclose=false;

    int32 len=0;
    len=newsock.recv(buf, 100, 20);
    debug_int_hex_16bit(len>>16);
    debug_int_hex_16bit(len);
    debug_nl();
    if(len<=0)
        len=0;
    while(len<30){
        len+=newsock.recv(&buf[len], 100-len, 20);
        debug_var(len);
        delay_ms(70);
    }

    buf[len]=0;
    debug_str("http got ");
    debug_int_hex_16bit(len);
    debug_str(" chars\r\n");

    if(len<20) return;

    int reqPos;
    if(!strncmp("GET", buf, 3)){
        char* pageName = &buf[4];

        debug_str(pageName);

        char* in=pageName;
        debug_putc(*in);
        while((*in)!=' ' && (*in)!='?' && (*in)!='\n'){
            in++;
            debug_putc(*in);
        }
        *in=0;
        debug_nl();
        debug_str(pageName);

        HttpPage* page=pages[pageName];
        if(page){
            page->sendHeader(newsock, 200);
            page->sendContent(newsock);
            delay_ms(100);
        }
    }
}


void HttpServer::addPage(std::string name, HttpPage* page){
    pages.insert(make_pair(name, page));

}
