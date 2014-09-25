/* 
 * File:   Pages.cpp
 * Author: david
 * 
 * Created on 23 September 2014, 17:18
 */

#include "Pages.h"

char indexPage[]=
"<html><title>lamp</title><body>"
"<script type=\"text/javascript\" src=\"/functions.js\" ></script>"
"<H1>Lamp</H1>"
"red:<input type=number onchange=\"setR(this.value); updLeds();\" id=rIp ><br>"
"green:<input type=number onchange=\"setG(this.value); updLeds();\" id=gIp ><br>"
"blue:<input type=number onchange=\"setB(this.value); updLeds();\" id=bIp ><br>"
"yellow:<input type=number onchange=\"setY(this.value); updLeds();\" id=wIp ><br>"
"white:<input type=number onchange=\"setW(this.value); updLeds();\" id=yIp ><br>"
"";

char functionsJS[]=
"Leds={r:0, g:0, b:0, y:0, w:0};"
"function setR(v){Leds.r=v}"
"function setG(v){Leds.g=v}"
"function setB(v){Leds.b=v}"
"function setY(v){Leds.y=v}"
"function setW(v){Leds.w=v}"
"function updLeds(){"
"aj=new XMLHttpRequest();"
"aj.open('GET', '/set?r='+Leds.r+'&g='+Leds.g'+'&b='+Leds.b'+'&y='+Leds.y'&w='+Leds.y, true);"
"aj.send();}"
"gebid=document.getElementById"
"function updIps(){"
    "aj=new XMLHttpRequest();"
    "aj.onreadystatechange=function(){"
    "if(aj.readyState==4 && aj.status==200){"
        "Leds=eval(aj.responseText);"
        "gebid('rIp').value=Leds.r;"
        "gebid('gIp').value=Leds.g;"
        "gebid('bIp').value=Leds.b;"
        "gebid('yIp').value=Leds.y;"
        "gebid('wIp').value=Leds.w;"
    "}}"
    "aj.send();"
"};"
"setInterval(updIps, 5000);"
"updIps();"
;

HttpPage index(indexPage, sizeof(indexPage));

void addPages(HttpServer& server){
//    HttpPage* index = new HttpPage(indexPage, sizeof(indexPage));
    server.addPage("/", &index);
    server.addPage("/index.php", &index);
    server.addPage("/index.html",&index);

    server.addPage("/functions.js", new HttpPage(functionsJS, sizeof(functionsJS)));

}
