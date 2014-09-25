#include "cc3000.h"

#ifndef __cc3000_general_h
#define __cc3000_general_h

#ifdef	__cplusplus
extern "C" {
#endif


/** 
 
    \brief Read the buffer size from the module
 
    Read the buffer size from the module
 
*/
uns8 cc3000_general_read_buffer_size(uns8 *free_buffers, uns16 *buffer_length);

/** 
 
    \brief Startup module 
 
	Start module, ready to connect
	
	\param patches_request 0 for startup without patches, 1 to start with patches

*/

void cc3000_general_startup(uns8 patches_request);

/** 
 
    \brief Shutdown module
 
	Shutdown module into power down state
	
*/


void cc3000_general_shutdown();

#ifdef	__cplusplus
}
#endif


#endif
