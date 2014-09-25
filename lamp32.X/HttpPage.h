/* 
 * File:   HttpPage.h
 * Author: david
 *
 * Created on 23 September 2014, 12:36
 */

#ifndef HTTPPAGE_H
#define	HTTPPAGE_H
#include <string>

#include "socket.hpp"

using namespace std;
class HttpPage {
public:
    HttpPage(char* content, int length);

    virtual void sendHeader(socket& socket, uns16 code);
    virtual void sendContent(socket& socket);
    virtual ~HttpPage();
private:
    bool hasContentLength;
    char* content;
    int contentLength;


};

#endif	/* HTTPPAGE_H */

