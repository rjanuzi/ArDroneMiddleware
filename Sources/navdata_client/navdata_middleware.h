#ifndef _NAVDATA_MIDDLEWARE_H_
#define _NAVDATA_MIDDLEWARE_H_

#include "common/mobile_config.h"
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>

C_RESULT navdata_middleware_init( mobile_config_t* cfg );
C_RESULT navdata_middleware_process( const navdata_unpacked_t* const navdata );
C_RESULT navdata_middleware_release( void );

#endif // _NAVDATA_MIDDLEWARE_H_
