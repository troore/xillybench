/*
 *  Transformation: JPEG -> BMP
 *  
 *  @(#) $Id: jpeg2bmp.c,v 1.2 2003/07/18 10:19:21 honda Exp $ 
 */

/*
 * Buffer for reading JPEG file
 */
unsigned char JpegFileBuf[JPEG_FILE_SIZE];


	int
jpeg2bmp_main ()
{
	int ci;
	unsigned char *c;
	int i, j;

	/*
	 * Store input data in buffer
	 */
	c = JpegFileBuf;
	for (i = 0; i < JPEGSIZE; i++)
		
		{        ci = hana_jpg[i];
			*c++ = ci;
		}

	jpeg_read (JpegFileBuf);

	for (i = 0; i < RGB_NUM; i++)
	{
		for (j = 0; j < BMP_OUT_SIZE; j++)
		{
			if (OutData_comp_buf[i][j] != hana_bmp[i][j])
			{
				main_result++;
			}
		}
	}
	if (OutData_image_width != out_width)
	{
		main_result++;
	}
	if (OutData_image_height != out_length)
	{
		main_result++;
	}
	return (0);
}
