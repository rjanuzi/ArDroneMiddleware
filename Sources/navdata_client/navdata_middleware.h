#ifndef _NAVDATA_MIDDLEWARE_H_
#define _NAVDATA_MIDDLEWARE_H_

#include "common/mobile_config.h"
#include <ardrone_tool/Navdata/ardrone_navdata_client.h>

#define NAVDATA_MIDDLEWARE_TM_JSON_TEMPLATE     "{\"alt\" : \"%.4f\",\"lat\" : \"%.4f\",\"lon\" : \"%.4f\",\"vel_x\" : \"%.4f\",\"vel_y\" : \"%.4f\",\"vel_z\" : \"%.4f\",\"pitch\" : \"%.4f\",\"roll\" : \"%.4f\",\"yaw\" : \"%.4f\",\"temp\" : \"%.4f\",\"bat\" : \"%.4f\",\"press\" : \"%.4f\"}"

#define NAVDATA_MIDDLEWARE_TM_JSON_MAX_SIZE     300
#define NAVDATA_MIDDLEWARE_GENERATE_TM_JSON (tm_data, json_string_out) {sprintf(json_string_out, NAVDATA_MIDDLEWARE_TM_JSON_TEMPLATE, tm_data.alt, tm_data.lat, tm_data.lon, tm_data.vel_x, tm_data.vel_y, tm_data.vel_z, tm_data.pitch, tm_data.roll, tm_data.yaw, tm_data.temp, tm_data.bat, tm_data.press);}

typedef struct navdata_middleware_tm_s 
{
        float alt;
        float lat;
        float lon;
        float vel_x;
        float vel_y;
        float vel_z;
        float pitch;
        float roll;
        float yaw;
        float temp;
        float bat;
        float press;
} navdata_middleware_tm_t;

C_RESULT navdata_middleware_init( mobile_config_t* cfg );
C_RESULT navdata_middleware_process( const navdata_unpacked_t* const navdata );
C_RESULT navdata_middleware_release( void );

void navdata_middlware_print_tm_data(navdata_middleware_tm_t dataToPrint);

#endif // _NAVDATA_MIDDLEWARE_H_
