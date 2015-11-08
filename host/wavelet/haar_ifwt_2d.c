
/*
   Calculate one iteration of the Haar inverse FWT in 1-dimension.
 */

void Haar_inverse_pass_1d (int n, float * s)
{
	int i;
	int npts;
	float * r = NULL;

	npts = powerof2 (n);
	r = (float *) malloc (sizeof(float) * npts);

	for (i = 0;  i < npts/2;  i++)
	{
		r[2*i]   = s[i] + s[i + npts/2];
		r[2*i+1] = s[i] - s[i + npts/2]; 
	}

	for (i = 0;  i < npts;  i++)
	{
		s[i] = r[i];
	}

	free (r);   r = NULL;
}


/*
   Calculate one iteration of the Haar inverse FWT in 2-dimensions.
 */
void Haar_inverse_pass_2d (int n, float ** s)
{
	int i, j;
	int npts;
	float * c = NULL;

	npts = powerof2 (n);

	for (i = 0;  i < npts;  i++)
	{
		Haar_inverse_pass_1d (n, s[i]);
	}

	c = (float *) malloc (sizeof(float) * npts);

	for (j = 0;  j < npts;  j++)
	{
		for (i = 0;  i < npts;  i++)
			c[i] = s[i][j];
		Haar_inverse_pass_1d (n, c);
		for (i = 0;  i < npts;  i++)
			s[i][j] = c[i];
	}

	free (c);   c = NULL;
}

/*
   Calculate the Haar inverse fast wavelet transform in 2-dimensions.
 */
void haar_inverse_fwt_2d(int n, float **s)
{
	int m;
//	int npts;

//	npts = n * n;

	for (m = 1; m <= n; m++) {
		Haar_inverse_pass_2d (m, s);
	}
}

