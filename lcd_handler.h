#include <stdlib.h>

void lcdhandler_init();
void lcdhandler_clear();
int lcdhandler_write_line(const char *msg, unsigned char line_index);