#ifndef __JPEG2BMP_H_
#define __JPEG2BMP_H_

#include <stdio.h>

#include "decode.h"

#define EXIT                exit(0)

/*
 * Size of JPEG file
 */
#define JPEG_FILE_SIZE  (90 * 59)

/*
 * Size of output buffer
 */
#define BMP_OUT_SIZE  (90 * 59)

int jpeg2bmp_main ();

extern int OutData_image_width;
extern int OutData_image_height;
extern int OutData_comp_vpos[RGB_NUM];
extern int OutData_comp_hpos[RGB_NUM];
extern unsigned char OutData_comp_buf[RGB_NUM][BMP_OUT_SIZE];

#endif
