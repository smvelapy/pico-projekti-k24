#include <stddef.h>

int webhandler_setup();
void webhandler_getsettings();
void webhandler_post_sensordata(float temp, float hum);
extern void (*webhandler_getsettings_callback) (char *buffer, size_t size);
