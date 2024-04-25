#include <stddef.h>

int webhandler_setup();
void webhandler_getsettings(unsigned char sensor_readed);
void webhandler_post_sensordata(unsigned char sensor_type, float temp, float hum);
extern void (*webhandler_getsettings_callback) (char *buffer, size_t size);
