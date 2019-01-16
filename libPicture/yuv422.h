#pragma once
#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

static long U[256], V[256], Y1[256], Y2[256];

void init_yuv422p_table(void)
{
	int i;

	// Initialize table
	for (i = 0; i < 256; i++)
	{
		V[i] = 15938 * i - 2221300;
		U[i] = 20238 * i - 2771300;
		Y1[i] = 11644 * i;
		Y2[i] = 19837 * i - 311710;
	}
}

/**
w
+--------------------+
|Y0Y1Y2Y3...         |
|...                 |   h
|...                 |
|                    |
+--------------------+
|U0U1      |
|...       |   h
|...       |
|          |
+----------+
|V0V1      |
|...       |  h
|...       |
|          |
+----------+
w/2
*/
void yuv422p_to_rgb32(unsigned char* yuv422p, unsigned char* rgb, int width, int height)
{
	int y, cb, cr;
	int r, g, b;
	int i = 0;
	unsigned char* p_y;
	unsigned char* p_u;
	unsigned char* p_v;
	unsigned char* p_rgb;
	static int init_yuv422p = 0;    // just do it once

	p_y = yuv422p;
	p_u = p_y + width * height;
	p_v = p_u + width * height / 2;
	p_rgb = rgb;

	if (init_yuv422p == 0)
	{
		init_yuv422p_table();
		init_yuv422p = 1;
	}

	for (i = 0; i < width * height / 2; i++)
	{
		y = p_y[0];
		cb = p_u[0];
		cr = p_v[0];

		r = MAX(0, MIN(255, (V[cr] + Y1[y]) / 10000));   //R value
		b = MAX(0, MIN(255, (U[cb] + Y1[y]) / 10000));   //B value
		g = MAX(0, MIN(255, (Y2[y] - 5094 * (r)-1942 * (b)) / 10000)); //G value

		p_rgb[0] = r;
		p_rgb[1] = g;
		p_rgb[2] = b;
		p_rgb[3] = 0;

		y = p_y[1];
		cb = p_u[0];
		cr = p_v[0];
		r = MAX(0, MIN(255, (V[cr] + Y1[y]) / 10000));   //R value
		b = MAX(0, MIN(255, (U[cb] + Y1[y]) / 10000));   //B value
		g = MAX(0, MIN(255, (Y2[y] - 5094 * (r)-1942 * (b)) / 10000)); //G value

		p_rgb[4] = r;
		p_rgb[5] = g;
		p_rgb[6] = b;
		p_rgb[7] = 0;

		p_y += 2;
		p_u += 1;
		p_v += 1;
		p_rgb += 8;
	}
}