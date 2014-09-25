/* 
 * File:   HttpServer.h
 * Author: david
 *
 * Created on 23 September 2014, 00:44
 */

#include "socket.hpp"
#include "main.h"
#include "Service.h"
#include "HttpPage.h"

#include <map>
#include <string>

#ifndef HTTPSERVER_H
#define	HTTPSERVER_H

class HttpServer: public Service {
public:
    HttpServer(uns16 port=80);
    virtual void tick();
    virtual ~HttpServer();
    void addPage(std::string name, HttpPage* page);
private:
    socket sock;
    char buf[128];
    std::map<std::string, HttpPage*> pages;
    
    

    

};

#endif	/* HTTPSERVER_H */

