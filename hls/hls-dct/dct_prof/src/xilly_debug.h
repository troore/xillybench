#ifndef XILLYBUS_DEBUG_H_
#define XILLYBUS_DEBUG_H_

#include "stdint.h"

void xilly_puts(const char *);
void xilly_hexprint(const uint32_t, const int);
void xilly_decprint(const uint32_t, const int);

#endif
