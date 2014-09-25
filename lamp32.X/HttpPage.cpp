/* 
 * File:   HttpPage.cpp
 * Author: david
 * 
 * Created on 23 September 2014, 12:36
 */

#include "HttpPage.h"

HttpPage::HttpPage(char* content, int length):
    content(content),
    contentLength(length)
{
//    debug_str("making page \r\n");
}

HttpPage::~HttpPage() {
}

void HttpPage::sendHeader(socket& socket, uns16 code)
{
    switch (code){
        case 200:
        {
            char buf[] = "HTTP/1.1 200 OK\r\n"
                         "Connection: Close\r\n"
                         "Content-length:         "; // left space for content length


            int l = sprintf(buf+sizeof(buf)-9, "%i\r\n\r\n", contentLength);
            l=sizeof(buf)-9+l;

                socket.send(buf, l);
        }
        break;
        case 404:
            char buf[]="HTTP/1.1 404 Not Found\r\n"
            "Content-Length: 0\r\n\r\n ";
            socket.send(buf, sizeof(buf)-1);

    }
}
void HttpPage::sendContent(socket& socket)
{
        socket.send(content, contentLength);
}
