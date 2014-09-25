/* 
 * File:   Service.h
 * Author: david
 *
 * Created on 23 September 2014, 01:15
 */

#ifndef SERVICE_H
#define	SERVICE_H

class Service {
public:
    Service();
    virtual void tick()=0;
    virtual ~Service();
private:

};

#endif	/* SERVICE_H */

