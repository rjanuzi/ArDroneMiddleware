#include "ihm/ihm.h"
#include "navdata_client/navdata_middleware.h"
#include <ardrone_api.h>

navdata_middleware_tm_t last_tm_data = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};


C_RESULT navdata_middleware_init( mobile_config_t* cfg )
{
  return C_OK;
}


/*============================================================================*/
/**
  * Navdata handling callback
  * This callback is registered to ARDroneTool in the BEGIN_NAVDATA_HANDLER_TABLE in navdata_client.c
  * @param pnd Pointer to the ready-to-use navdata structure
  * @return Always returns C_OK.
  */
C_RESULT navdata_middleware_process( const navdata_unpacked_t* const pnd )
{
        /* Update as ultimas telemetrias */
        last_tm_data.alt = ((float) pnd->navdata_altitude.altitude_raw);
        last_tm_data.lat = ((float) pnd->navdata_phys_measures.phys_accs[ACC_X]) / KIHM_MIL_F;
        last_tm_data.lon = ((float) pnd->navdata_phys_measures.phys_accs[ACC_Y]) / KIHM_MIL_F;;
        last_tm_data.vel_x = (float) pnd->navdata_demo.vx;
        last_tm_data.vel_y = (float) pnd->navdata_demo.vy;
        last_tm_data.vel_z = (float) pnd->navdata_demo.vz;
        last_tm_data.pitch = ((float) pnd->navdata_demo.theta) / KIHM_MIL_F;
        last_tm_data.roll = ((float) pnd->navdata_demo.phi) / KIHM_MIL_F;
        last_tm_data.yaw = ((float) pnd->navdata_demo.psi) / KIHM_MIL_F;
        last_tm_data.temp = (float) (pnd->navdata_phys_measures.accs_temp);
        last_tm_data.bat = (float) pnd->navdata_demo.vbat_flying_percentage;
        last_tm_data.press = (float) (pnd->navdata_pressure_raw.Pression_meas / 100.0); // Pressure in mBar;
        navdata_middlware_print_tm_data(last_tm_data);

        return C_OK;
}

C_RESULT navdata_middleware_release( void )
{
  return C_OK;
}

void navdata_middlware_print_tm_data(navdata_middleware_tm_t dataToPrint)
{
        printf("\n\nNavdata:");
        printf("\n\tAltitude: %.4f", dataToPrint.alt);
        printf("\n\tLatitude: %.4f", dataToPrint.lat);
        printf("\n\tLongitude: %.4f", dataToPrint.lon);
        printf("\n\tVelocidade (x): %.4f", dataToPrint.vel_x);
        printf("\n\tVelocidade (y): %.4f", dataToPrint.vel_y);
        printf("\n\tVelocidade (z): %.4f", dataToPrint.vel_z);
        printf("\n\tPitch: %.4f", dataToPrint.pitch);
        printf("\n\tRoll: %.4f", dataToPrint.roll);
        printf("\n\tYaw: %.4f", dataToPrint.yaw);
        printf("\n\tTemperatura: %.4f", dataToPrint.temp);
        printf("\n\tBateria: %.4f", dataToPrint.bat);
        printf("\n\tPressao: %.4f mBar", dataToPrint.press);
}

