/* This file was taken from modified dcraw published by Paul Lee
   on January 23, 2009, taking dcraw ver.8.90/rev.1.417
   as basis.
   http://sites.google.com/site/demosaicalgorithms/modified-dcraw

   As modified dcraw source code was published, the release under
   GPL Version 2 or later option could be applied, so this file
   is taken under this premise.
*/

/*
    Copyright (C) 2009 Paul Lee

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

/*
   Adaptive Homogeneity-Directed interpolation is based on
   the work of Keigo Hirakawa, Thomas Parks, and Paul Lee.
 */
/*
Adaptive Homogeneity-Directed interpolation is based on
the work of Keigo Hirakawa, Thomas Parks, and Paul Lee.

AHD interpolation with built-in anti-aliasing feature
*/
#define TS 256		/* Tile Size */

void CLASS ahd_interpolate_mod()
{
	int i, j, k, top, left, row, col, tr, tc, c, d, val, hm[2];
	ushort (*pix)[4], (*rix)[3];
	static const int dir[4] = { -1, 1, -TS, TS };
	unsigned ldiff[2][4], abdiff[2][4], leps, abeps;
	float r, cbrt[0x10000], xyz[3], xyz_cam[3][4];
	ushort (*rgb)[TS][TS][3];
	short (*lab)[TS][TS][3], (*lix)[3];
	char (*homo)[TS][TS], *buffer;

#ifdef DCRAW_VERBOSE
	if (verbose) fprintf (stderr,_("AHD interpolation (modified for anti-aliasing)...\n"));
#endif

	for (i=0; i < 0x10000; i++) {
		r = i / 65535.0;
		cbrt[i] = r > 0.008856 ? pow((double)r,1/3.0) : 7.787*r + 16/116.0;
	}
	for (i=0; i < 3; i++)
		for (j=0; j < colors; j++)
			for (xyz_cam[i][j] = k=0; k < 3; k++)
				xyz_cam[i][j] += xyz_rgb[i][k] * rgb_cam[k][j] / d65_white[i];

	border_interpolate(6);
	buffer = (char *) malloc (26*TS*TS);		/* 1664 kB */
	merror (buffer, "ahd_interpolate()");
	rgb  = (ushort(*)[TS][TS][3]) buffer;
	lab  = (short (*)[TS][TS][3])(buffer + 12*TS*TS);
	homo = (char  (*)[TS][TS])   (buffer + 24*TS*TS);

	for (top=3; top < height-6; top += TS-7)
		for (left=3; left < width-6; left += TS-7) {

			/*  Interpolate green horizontally and vertically:		*/
			for (row = top; row < top+TS && row < height-3; row++) {
				col = left + (FC(row,left) & 1);
				for (c = FC(row,col); col < left+TS && col < width-3; col+=2) {
					pix = image + row*width+col;
					val = ((pix[-1][1] + pix[0][c] + pix[1][1]) * 2
						- pix[-2][c] - pix[2][c] + 2) >> 2;
					if (val < 0 || val > 65535) {
						val = (pix[-3][1] + pix[3][1] +
							18*(2*pix[0][c] - pix[-2][c] - pix[2][c]) +
							63*(pix[-1][1] + pix[1][1]) + 64) >> 7;
						if (val < 0 || val > 65535) {
							val = (4*(pix[-1][1] + pix[1][1]) +
								2*pix[0][c]-pix[-2][c]-pix[2][c] + 4) >> 3;
							if (val < 0 || val > 65535)
								val = (pix[-1][1] + pix[1][1] + 1) >> 1; }}
					rgb[0][row-top][col-left][1] = val;
					val = ((pix[-width][1] + pix[0][c] + pix[width][1]) * 2
						- pix[-2*width][c] - pix[2*width][c] + 2) >> 2;
					if (val < 0 || val > 65535) {
						val = (pix[-3*width][1] + pix[3*width][1] +
							18*(2*pix[0][c] - pix[-2*width][c] - pix[2*width][c]) +
							63*(pix[-width][1] + pix[width][1]) + 64) >> 7;
						if (val < 0 || val > 65535) {
							val = (4*(pix[-width][1] + pix[width][1]) +
								2*pix[0][c]-pix[-2*width][c]-pix[2*width][c] + 4) >> 3;
							if (val < 0 || val > 65535)
								val = (pix[-width][1] + pix[width][1] + 1) >> 1; }}
					rgb[1][row-top][col-left][1] = val;
				}
			}
			/*  Interpolate red and blue, and convert to CIELab:		*/
			for (d=0; d < 2; d++)
				for (row=top+1; row < top+TS-1 && row < height-4; row++)
					for (col=left+1; col < left+TS-1 && col < width-4; col++) {
						pix = image + row*width+col;
						rix = &rgb[d][row-top][col-left];
						lix = &lab[d][row-top][col-left];
						if ((c = 2 - FC(row,col)) == 1) {
							c = FC(row+1,col);
							val = pix[0][1] + (( pix[-1][2-c] + pix[1][2-c]
							- rix[-1][1] - rix[1][1] + 1) >> 1);
							if (val < 0 || val > 65535)
								val = (pix[-1][2-c] + pix[1][2-c] + 1) >> 1;
							rix[0][2-c] = val;
							val = pix[0][1] + (( pix[-width][c] + pix[width][c]
							- rix[-TS][1] - rix[TS][1] + 1) >> 1);
							if (val < 0 || val > 65535)
								val = (pix[-width][c] + pix[width][c] + 1) >> 1;
						} else {
							val = rix[0][1] + (( pix[-width-1][c] + pix[-width+1][c]
							+ pix[+width-1][c] + pix[+width+1][c]
							- rix[-TS-1][1] - rix[-TS+1][1]
							- rix[+TS-1][1] - rix[+TS+1][1] + 2) >> 2);
							if (val < 0 || val > 65535)
								val = (pix[-width-1][c] + pix[-width+1][c] +
								pix[ width-1][c] + pix[ width+1][c] + 2) >> 2; }
						rix[0][c] = val;
						c = FC(row,col);
						rix[0][c] = pix[0][c];
						xyz[0] = xyz[1] = xyz[2] = 0.5;
						FORCC {
							xyz[0] += xyz_cam[0][c] * rix[0][c];
							xyz[1] += xyz_cam[1][c] * rix[0][c];
							xyz[2] += xyz_cam[2][c] * rix[0][c];
						}
						xyz[0] = cbrt[CLIP((int) xyz[0])];
						xyz[1] = cbrt[CLIP((int) xyz[1])];
						xyz[2] = cbrt[CLIP((int) xyz[2])];
						lix[0][0] = 64 * (116 * xyz[1] - 16);
						lix[0][1] = 64 * 500 * (xyz[0] - xyz[1]);
						lix[0][2] = 64 * 200 * (xyz[1] - xyz[2]);
					}
					/*  Build homogeneity maps from the CIELab images:		*/
					memset (homo, 0, 2*TS*TS);
					for (row=top+2; row < top+TS-2 && row < height-5; row++) {
						tr = row-top;
						for (col=left+2; col < left+TS-2 && col < width-5; col++) {
							tc = col-left;
							for (d=0; d < 2; d++) {
								lix = &lab[d][tr][tc];
								for (i=0; i < 4; i++) {
									ldiff[d][i] = ABS(lix[0][0]-lix[dir[i]][0]);
									abdiff[d][i] = SQR(lix[0][1]-lix[dir[i]][1])
										+ SQR(lix[0][2]-lix[dir[i]][2]);
								}
							}
							leps = MIN(MAX(ldiff[0][0],ldiff[0][1]),
								MAX(ldiff[1][2],ldiff[1][3]));
							abeps = MIN(MAX(abdiff[0][0],abdiff[0][1]),
								MAX(abdiff[1][2],abdiff[1][3]));
							for (d=0; d < 2; d++)
								for (i=0; i < 4; i++)
									if (ldiff[d][i] <= leps && abdiff[d][i] <= abeps)
										homo[d][tr][tc]++;
						}
					}
					/*  Combine the most homogenous pixels for the final result:	*/
					for (row=top+3; row < top+TS-3 && row < height-6; row++) {
						tr = row-top;
						for (col=left+3; col < left+TS-3 && col < width-6; col++) {
							tc = col-left;
							for (d=0; d < 2; d++)
								for (hm[d]=0, i=tr-1; i <= tr+1; i++)
									for (j=tc-1; j <= tc+1; j++)
										hm[d] += homo[d][i][j];
							if (hm[0] != hm[1])
								FORC3 image[row*width+col][c] = rgb[hm[1] > hm[0]][tr][tc][c];
							else
								FORC3 image[row*width+col][c] =
								(rgb[0][tr][tc][c] + rgb[1][tr][tc][c] + 1) >> 1;
						}
					}
		}
		free (buffer);

}
#undef TS
