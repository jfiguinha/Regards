// vim: ft=c

// Note: changes in included files won't trigger rebuilding
//#include "config.h"

#ifndef CONFIG_H
#define CONFIG_H

#define PROFILE "original"

#define ENABLE_PROFILING 0
#define UNROLL 1

#define DISTANCE_IN_SPARSE 0

#define BLOCK_SIZE 8
#define BLOCK_SIZE_HALF 4
#define BLOCK_SIZE_SQ 64

#define WINDOW_SIZE 39
#define WINDOW_SIZE_HALF 19

#define STEP_SIZE 3
// Multiple of STEP_SIZE
#define SPLIT_SIZE_X (3*STEP_SIZE)
#define SPLIT_SIZE_Y (3*STEP_SIZE)

#define WINDOW_STEP_SIZE_1 1
#define WINDOW_STEP_SIZE_2 1

#define MAX_BLOCK_COUNT_1 16
// 32 causes crash on CPU
#define MAX_BLOCK_COUNT_2 16

#define USE_KAISER_WINDOW 1

#define DCT_1D 0
#define HAAR_1D 1
#define TRANSFORM_METHOD_1D HAAR_1D

#define D_THRESHOLD_1 (3 * 2500)
#define D_THRESHOLD_2 (3 * 400)

#endif


//#include "dct.h"

#ifndef DCT_H
#define DCT_H

// cos(3*PI/16)
#define C3A 0.83146961230254523707878837761791f
// sin(3*PI/16)
#define C3B 0.55557023301960222474283081394853f
// cos(PI/16)
#define C1A 0.98078528040323044912618223613424f
// sin(PI/16)
#define C1B 0.19509032201612826784828486847702f
// sqrt(2) * cos(3*PI/8)
// NOT: sqrt(2) * cos(PI/16)
#define S2C3A 0.54119610014619698439972320536639f
// sqrt(2) * sin(3*PI/8)
// NOT: sqrt(2) * sin(PI/16)
#define S2C3B 1.3065629648763765278566431734272f
// 1 / sqrt(8)
#define C_NORM_1D 0.35355339059327376220042218105242f
// 1 / 8
#define C_NORM_2D 0.125f

#ifndef M_SQRT2_F
#   define M_SQRT2_F 1.4142135623730950488016887242097f
#endif

inline void dct(const float in[8], float out[8], bool normalize) {

    float st2[8], st3[2];
    float tmp;

    // Stage 1

    out[0] = in[0] + in[7];
    out[1] = in[1] + in[6];
    out[2] = in[2] + in[5];
    out[3] = in[3] + in[4];

    out[4] = in[3] - in[4];
    out[5] = in[2] - in[5];
    out[6] = in[1] - in[6];
    out[7] = in[0] - in[7];

    // Stage 2

    st2[0] = out[0] + out[3];
    st2[1] = out[1] + out[2];
    st2[2] = out[1] - out[2];
    st2[3] = out[0] - out[3];

    tmp = C3A * (out[4] + out[7]);

    st2[4] = tmp + (C3B-C3A)*out[7];
    st2[7] = tmp - (C3A+C3B)*out[4];

    tmp = C1A * (out[5] + out[6]);

    st2[5] = tmp + (C1B-C1A)*out[6];
    st2[6] = tmp - (C1A+C1B)*out[5];

    // Stage 3

    out[0] = st2[0] + st2[1];
    out[4] = st2[0] - st2[1];

    tmp = S2C3A * (st2[2] + st2[3]);

    out[2] = tmp + (S2C3B-S2C3A)*st2[3];
    out[6] = tmp - (S2C3A+S2C3B)*st2[2];

    st3[0] = st2[4] + st2[6];
    st3[1] = st2[5] + st2[7];
    out[3] = st2[7] - st2[5];
    out[5] = st2[4] - st2[6];

    // Stage 4

    out[7] = st3[1] - st3[0];
    out[3] *= M_SQRT2_F;
    out[5] *= M_SQRT2_F;
    out[1] = st3[0] + st3[1];

    if (normalize)
        for (int i = 0; i < 8; i++)
            out[i] *= C_NORM_1D;
}

inline void idct(const float in[8], float out[8], bool normalize) {

    float st1[8], st4[2];
    float tmp;

    // Stage 4

    st4[0] = in[1] - in[7];
    st1[5] = in[3] * M_SQRT2_F;
    st1[6] = in[5] * M_SQRT2_F;
    st4[1] = in[1] + in[7];

    // Stage 3

    out[0] = in[0] + in[4];
    out[1] = in[0] - in[4];

    tmp = S2C3A * (in[2] + in[6]);

    out[2] = tmp - (S2C3A+S2C3B)*in[6];
    out[3] = tmp + (S2C3B-S2C3A)*in[2];

    out[4] = st4[0] + st1[6];
    out[5] = st4[1] - st1[5];
    out[6] = st4[0] - st1[6];
    out[7] = st1[5] + st4[1];

    // Stage 2

    st1[0] = out[0] + out[3];
    st1[1] = out[1] + out[2];
    st1[2] = out[1] - out[2];
    st1[3] = out[0] - out[3];

    tmp = C3A * (out[4] + out[7]);

    st1[4] = tmp - (C3A+C3B)*out[7];
    st1[7] = tmp + (C3B-C3A)*out[4];

    tmp = C1A * (out[5] + out[6]);

    st1[5] = tmp - (C1A+C1B)*out[6];
    st1[6] = tmp + (C1B-C1A)*out[5];

    // Stage 1

    out[0] = st1[0] + st1[7];
    out[1] = st1[1] + st1[6];
    out[2] = st1[2] + st1[5];
    out[3] = st1[3] + st1[4];

    out[4] = st1[3] - st1[4];
    out[5] = st1[2] - st1[5];
    out[6] = st1[1] - st1[6];
    out[7] = st1[0] - st1[7];

    if (normalize)
        for (int i = 0; i < 8; i++)
            out[i] *= C_NORM_1D;
}

inline void transpose(float in[8][8], float out[8][8]) {
    int i, j;
    for (j = 0; j < 8; j++) {
        for (i = 0; i < 8; i++) {
            out[j][i] = in[i][j];
        }
    }
}

inline void dct2(float in[8][8], float out[8][8]) {
    int i, j;

    float res[8][8];

#if 1

    // Process rows
    for (j = 0; j < 8; j++) {

        float st2[8], st3[2];
        float tmp;

        // Stage 1

        res[j][0] = in[j][0] + in[j][7];
        res[j][1] = in[j][1] + in[j][6];
        res[j][2] = in[j][2] + in[j][5];
        res[j][3] = in[j][3] + in[j][4];

        res[j][4] = in[j][3] - in[j][4];
        res[j][5] = in[j][2] - in[j][5];
        res[j][6] = in[j][1] - in[j][6];
        res[j][7] = in[j][0] - in[j][7];

        // Stage 2

        st2[0] = res[j][0] + res[j][3];
        st2[1] = res[j][1] + res[j][2];
        st2[2] = res[j][1] - res[j][2];
        st2[3] = res[j][0] - res[j][3];

        tmp = C3A * (res[j][4] + res[j][7]);

        st2[4] = tmp + (C3B-C3A)*res[j][7];
        st2[7] = tmp - (C3A+C3B)*res[j][4];

        tmp = C1A * (res[j][5] + res[j][6]);

        st2[5] = tmp + (C1B-C1A)*res[j][6];
        st2[6] = tmp - (C1A+C1B)*res[j][5];

        // Stage 3

        res[j][0] = st2[0] + st2[1];
        res[j][4] = st2[0] - st2[1];

        tmp = S2C3A * (st2[2] + st2[3]);

        res[j][2] = tmp + (S2C3B-S2C3A)*st2[3];
        res[j][6] = tmp - (S2C3A+S2C3B)*st2[2];

        st3[0] = st2[4] + st2[6];
        st3[1] = st2[5] + st2[7];
        res[j][3] = st2[7] - st2[5];
        res[j][5] = st2[4] - st2[6];

        // Stage 4

        res[j][7] = st3[1] - st3[0];
        res[j][3] *= M_SQRT2_F;
        res[j][5] *= M_SQRT2_F;
        res[j][1] = st3[0] + st3[1];
    }

    // Process columns
    for (i = 0; i < 8; i++) {

        float st2[8], st3[2];
        float tmp;

        // Stage 1

        out[i][0] = res[0][i] + res[7][i];
        out[i][1] = res[1][i] + res[6][i];
        out[i][2] = res[2][i] + res[5][i];
        out[i][3] = res[3][i] + res[4][i];

        out[i][4] = res[3][i] - res[4][i];
        out[i][5] = res[2][i] - res[5][i];
        out[i][6] = res[1][i] - res[6][i];
        out[i][7] = res[0][i] - res[7][i];

        // Stage 2

        st2[0] = out[i][0] + out[i][3];
        st2[1] = out[i][1] + out[i][2];
        st2[2] = out[i][1] - out[i][2];
        st2[3] = out[i][0] - out[i][3];

        tmp = C3A * (out[i][4] + out[i][7]);

        st2[4] = tmp + (C3B-C3A)*out[i][7];
        st2[7] = tmp - (C3A+C3B)*out[i][4];

        tmp = C1A * (out[i][5] + out[i][6]);

        st2[5] = tmp + (C1B-C1A)*out[i][6];
        st2[6] = tmp - (C1A+C1B)*out[i][5];

        // Stage 3

        out[i][0] = st2[0] + st2[1];
        out[i][4] = st2[0] - st2[1];

        tmp = S2C3A * (st2[2] + st2[3]);

        out[i][2] = tmp + (S2C3B-S2C3A)*st2[3];
        out[i][6] = tmp - (S2C3A+S2C3B)*st2[2];

        st3[0] = st2[4] + st2[6];
        st3[1] = st2[5] + st2[7];
        out[i][3] = st2[7] - st2[5];
        out[i][5] = st2[4] - st2[6];

        // Stage 4

        out[i][7] = st3[1] - st3[0];
        out[i][3] *= M_SQRT2_F;
        out[i][5] *= M_SQRT2_F;
        out[i][1] = st3[0] + st3[1];

        // Normalize
        for (j = 0; j < 8; j++) {
            out[i][j] *= C_NORM_2D;
        }
    }

#else

    // Process rows
    for (j = 0; j < 8; j++) {
        dct(in[j], res[j], false);
    }

    transpose(res, out);

    // Process columns
    for (j = 0; j < 8; j++) {
        dct(out[j], res[j], false);
    }

    // Normalize
    for (j = 0; j < 8; j++) {
        for (i = 0; i < 8; i++) {
            out[j][i] = res[j][i] * C_NORM_2D;
        }
    }

#endif
}

inline void idct2(float in[8][8], float out[8][8]) {
    int i, j;

    float res[8][8];

#if 1

    // Process rows
    for (j = 0; j < 8; j++) {

        float st1[8], st4[2];
        float tmp;

        // Stage 4

        st4[0] = in[j][1] - in[j][7];
        st1[5] = in[j][3] * M_SQRT2_F;
        st1[6] = in[j][5] * M_SQRT2_F;
        st4[1] = in[j][1] + in[j][7];

        // Stage 3

        res[j][0] = in[j][0] + in[j][4];
        res[j][1] = in[j][0] - in[j][4];

        tmp = S2C3A * (in[j][2] + in[j][6]);

        res[j][2] = tmp - (S2C3A+S2C3B)*in[j][6];
        res[j][3] = tmp + (S2C3B-S2C3A)*in[j][2];

        res[j][4] = st4[0] + st1[6];
        res[j][5] = st4[1] - st1[5];
        res[j][6] = st4[0] - st1[6];
        res[j][7] = st1[5] + st4[1];

        // Stage 2

        st1[0] = res[j][0] + res[j][3];
        st1[1] = res[j][1] + res[j][2];
        st1[2] = res[j][1] - res[j][2];
        st1[3] = res[j][0] - res[j][3];

        tmp = C3A * (res[j][4] + res[j][7]);

        st1[4] = tmp - (C3A+C3B)*res[j][7];
        st1[7] = tmp + (C3B-C3A)*res[j][4];

        tmp = C1A * (res[j][5] + res[j][6]);

        st1[5] = tmp - (C1A+C1B)*res[j][6];
        st1[6] = tmp + (C1B-C1A)*res[j][5];

        // Stage 1

        res[j][0] = st1[0] + st1[7];
        res[j][1] = st1[1] + st1[6];
        res[j][2] = st1[2] + st1[5];
        res[j][3] = st1[3] + st1[4];

        res[j][4] = st1[3] - st1[4];
        res[j][5] = st1[2] - st1[5];
        res[j][6] = st1[1] - st1[6];
        res[j][7] = st1[0] - st1[7];
    }

    // Process columns
    for (i = 0; i < 8; i++) {

        float st1[8], st4[2];
        float tmp;

        // Stage 4

        st4[0] = res[1][i] - res[7][i];
        st1[5] = res[3][i] * M_SQRT2_F;
        st1[6] = res[5][i] * M_SQRT2_F;
        st4[1] = res[1][i] + res[7][i];

        // Stage 3

        out[i][0] = res[0][i] + res[4][i];
        out[i][1] = res[0][i] - res[4][i];

        tmp = S2C3A * (res[2][i] + res[6][i]);

        out[i][2] = tmp - (S2C3A+S2C3B)*res[6][i];
        out[i][3] = tmp + (S2C3B-S2C3A)*res[2][i];

        out[i][4] = st4[0] + st1[6];
        out[i][5] = st4[1] - st1[5];
        out[i][6] = st4[0] - st1[6];
        out[i][7] = st1[5] + st4[1];

        // Stage 2

        st1[0] = out[i][0] + out[i][3];
        st1[1] = out[i][1] + out[i][2];
        st1[2] = out[i][1] - out[i][2];
        st1[3] = out[i][0] - out[i][3];

        tmp = C3A * (out[i][4] + out[i][7]);

        st1[4] = tmp - (C3A+C3B)*out[i][7];
        st1[7] = tmp + (C3B-C3A)*out[i][4];

        tmp = C1A * (out[i][5] + out[i][6]);

        st1[5] = tmp - (C1A+C1B)*out[i][6];
        st1[6] = tmp + (C1B-C1A)*out[i][5];

        // Stage 1

        out[i][0] = st1[0] + st1[7];
        out[i][1] = st1[1] + st1[6];
        out[i][2] = st1[2] + st1[5];
        out[i][3] = st1[3] + st1[4];

        out[i][4] = st1[3] - st1[4];
        out[i][5] = st1[2] - st1[5];
        out[i][6] = st1[1] - st1[6];
        out[i][7] = st1[0] - st1[7];

        // Normalize
        for (j = 0; j < 8; j++) {
            out[i][j] *= C_NORM_2D;
        }
    }

#else

    // Process rows
    for (j = 0; j < 8; j++) {
        idct(in[j], res[j], false);
    }

    transpose(res, out);

    // Process columns
    for (j = 0; j < 8; j++) {
        idct(out[j], res[j], false);
    }

    // Normalize
    for (j = 0; j < 8; j++) {
        for (i = 0; i < 8; i++) {
            out[j][i] = res[j][i] * C_NORM_2D;
        }
    }

#endif
}

#endif


//#include "haar.h"

#ifndef HAAR_H
#define HAAR_H

#define INV_SQRT2_F 0.70710678118654752440084436210485f

inline void haar(float x[8], float y[8]) {
  int i, j;
  int k = 8;

#pragma unroll
  for (j = 0; j < 3; j++) {
    int k2 = k;
    k >>= 1;

#pragma unroll
    for (i = 0; i < k; i++) {
      int i2 = i << 1;
      int i21 = i2 + 1;
      y[i]   = ( x[i2] + x[i21] ) * INV_SQRT2_F;
      y[i+k] = ( x[i2] - x[i21] ) * INV_SQRT2_F;
    }

#pragma unroll
    for (i = 0; i < k2; i++) {
      x[i] = y[i];
    }
  }
}

inline void ihaar(float x[8], float y[8]) {
  int i, j;
  int k = 1;

#pragma unroll
  for (j = 0; j < 3; j++) {

#pragma unroll
    for (i = 0; i < k; i++) {
      int i2 = i << 1;
      int ik = i + k;
      y[i2]   = ( x[i] + x[ik] ) * INV_SQRT2_F;
      y[i2+1] = ( x[i] - x[ik] ) * INV_SQRT2_F;
    }

    k <<= 1;

#pragma unroll
    for (i = 0; i < k; i++) {
      x[i] = y[i];
    }
  }
}

#endif


__constant sampler_t sampler =
      CLK_NORMALIZED_COORDS_FALSE
//    | CLK_ADDRESS_CLAMP_TO_EDGE // Clamp to edge value
    | CLK_ADDRESS_CLAMP // Clamp to zeros
    | CLK_FILTER_NEAREST;

#if USE_KAISER_WINDOW
#   define KAISER(x, y) kaiser_b_2[y*8 + x]
__constant float kaiser_b_2[] = {
    0.1924f, 0.2989f, 0.3846f, 0.4325f, 0.4325f, 0.3846f, 0.2989f, 0.1924f,
    0.2989f, 0.4642f, 0.5974f, 0.6717f, 0.6717f, 0.5974f, 0.4642f, 0.2989f,
    0.3846f, 0.5974f, 0.7688f, 0.8644f, 0.8644f, 0.7688f, 0.5974f, 0.3846f,
    0.4325f, 0.6717f, 0.8644f, 0.9718f, 0.9718f, 0.8644f, 0.6717f, 0.4325f,
    0.4325f, 0.6717f, 0.8644f, 0.9718f, 0.9718f, 0.8644f, 0.6717f, 0.4325f,
    0.3846f, 0.5974f, 0.7688f, 0.8644f, 0.8644f, 0.7688f, 0.5974f, 0.3846f,
    0.2989f, 0.4642f, 0.5974f, 0.6717f, 0.6717f, 0.5974f, 0.4642f, 0.2989f,
    0.1924f, 0.2989f, 0.3846f, 0.4325f, 0.4325f, 0.3846f, 0.2989f, 0.1924f
};
#endif

inline void threshold_2d(float in[8][8], const float fSigma) {
    int i, j;
	float tau_2D = (2.0f * fSigma);
    for (j = 0; j < BLOCK_SIZE; j++) {
        for (i = 0; i < BLOCK_SIZE; i++) {
            if (fabs(in[j][i]) <= tau_2D) in[j][i] = 0.0f;
        }
    }
}

inline void threshold_1d(float in[8], int* weight_count, int block_count, const float fSigma) {
    int i;
	float tau_1D = 2.7f * fSigma;
	if(fSigma > 40)
	{
		tau_1D = 2.8f * fSigma;
	}
	
    for (i = 0; i < BLOCK_SIZE; i++) {
        //if (fabs(in[i]) <= tau_1D * sqrt((float)block_count)) in[i] = 0.0f;
        if (fabs(in[i]) <= tau_1D) in[i] = 0.0f;
        else (*weight_count)++;
    }
}

__kernel void calc_distances(
    const __global float * input, 
    const int width, 
    const int height,
    __global short* similar_coords,
    __global uchar* block_counts,
    const int threshold,
    const int max_block_count,
    const int window_step_size
) {

    const short2 gid = {get_global_id(0) * STEP_SIZE, get_global_id(1) * STEP_SIZE};
    const size_t tot_globals = get_global_size(0) * get_global_size(1);
    const size_t global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);
    // Use this if you want to crash AMD's clBuildProgram()
    //int distances[MAX_BLOCK_COUNT] = { [0 ... MAX_BLOCK_COUNT-1] = INT_MAX };
    int distances[MAX_BLOCK_COUNT_2];
    short2 positions[MAX_BLOCK_COUNT_2];
    short block_count = 0;

    for (int n = 0; n < max_block_count; n++)
        distances[n] = INT_MAX;

    // Start block matching from window
    for (int j = WINDOW_SIZE_HALF % window_step_size; j < WINDOW_SIZE; j += window_step_size) {
        for (int i = WINDOW_SIZE_HALF % window_step_size; i < WINDOW_SIZE; i += window_step_size) {

            // Create reference block
            uchar ref[BLOCK_SIZE][BLOCK_SIZE];

            // Interestingly moving this outside outer for loops reduces performance significantly
            for (int y = 0; y < BLOCK_SIZE; y++) {
                for (int x = 0; x < BLOCK_SIZE; x++) {
                    const int2 ref_pos = {gid.x + x, gid.y + y};
                    int tid = ref_pos.y * width + ref_pos.x;
                    //float4 value = input[tid];
                    ref[y][x] = input[tid];//(uchar)read_imageui(input, sampler, ref_pos).s0;
                }
            }

            int d = 0;

            // Create matching block
            for (int y = 0; y < BLOCK_SIZE; y++) {
                for (int x = 0; x < BLOCK_SIZE; x++) {
                    const int2 pos = {gid.x + i + x - WINDOW_SIZE_HALF,
                                      gid.y + j + y - WINDOW_SIZE_HALF};
                    int tid = pos.y * width + pos.x;
                    const uchar b = (uchar)input[tid];//read_imageui(input, sampler, pos).s0;
                    d += (ref[y][x]-b) * (ref[y][x]-b);
                }
            }

            // Only count block if it's distance is under threshold and
            // is smaller than any already found block.
            if (d <= threshold) {
                for (int n = 0; n < max_block_count; n++) {
                    if (d < distances[n]) {
                        for (int k = max_block_count-1; k > n; k--) {
                            distances[k] = distances[k-1];
                            positions[k] = positions[k-1];
                        }
                        block_count++;
                        distances[n] = d;
                        positions[n].x = i;
                        positions[n].y = j;
                        break;
                    }
                }
            }
        }
    }

    if (block_count > max_block_count) block_count = max_block_count;
#if 0
    else {
        uchar n = 1;
        while ((n << 1) <= block_count) n <<= 1;
        block_count = n;
    }
#endif

    block_counts[global_id] = block_count;

    /* Nearest block coordinates are written as such into memory:
     *  most similar              second most similar      - sorted order of similarity
     * | x0 | y0 | x0 | y0 | ... | x1 | y1 | x1 | y1 | ... - coords for similar block
     *  t0        t1        tn    t0        t1        tn   - thread id
     *  R00       R01       Rmn   R00       R01       Rmn  - reference block id
     */
    for (int n = 0; n < block_count; n++) {
        // SoA or AoS?
        const int ind = 2 * (n * tot_globals + global_id);
        similar_coords[ind] = positions[n].x;
        similar_coords[ind + 1] = positions[n].y;
    }
}


#define ACCU(x, y) accumulator[y][x]
#define WM(x, y) weight_map[y][x]

__kernel void bm3d_basic_filter(
    __global float * output,
    const __global float *input,  
    const int width, 
    const int height,
	const float fSigma,
    __global short* similar_coords,
    __global uchar* block_counts,
    const int global_size_x_d,
    const int tot_globals_d)
{
#if 1
    const int2 gid = {get_global_id(0) * SPLIT_SIZE_X, get_global_id(1) * SPLIT_SIZE_Y};
    if (gid.x > width-1 || gid.y > height-1) return;
    //const size_t tot_globals = get_global_size(0) * get_global_size(1);
    //const size_t global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);

#if 1
    const int2 back_limit = {max(gid.x - WINDOW_SIZE_HALF, 0),
                             max(gid.y - WINDOW_SIZE_HALF, 0)};
    const int2 front_limit = {min(gid.x + SPLIT_SIZE_X - 1 + WINDOW_SIZE_HALF, width-1),
                              min(gid.y + SPLIT_SIZE_Y - 1 + WINDOW_SIZE_HALF, height-1)};
#else
    const int2 back_limit = gid;
    const int2 front_limit = gid;
#endif

	float variance = fSigma * fSigma;
    float accumulator[SPLIT_SIZE_Y][SPLIT_SIZE_X] = {{0.0f}};
    float weight_map[SPLIT_SIZE_Y][SPLIT_SIZE_X] = {{0.0f}};

    int ri = gid.x;
    int rj = gid.y;

    while (ri - STEP_SIZE >= back_limit.x) ri -= STEP_SIZE;
    while (rj - STEP_SIZE >= back_limit.y) rj -= STEP_SIZE;

    const int ri_min = ri;
    const int rj_min = rj;

    // Loop through all reference blocks that can contribute to a split block.
    for (rj = rj_min; rj <= front_limit.y; rj += STEP_SIZE) {
        for (ri = ri_min; ri <= front_limit.x; ri += STEP_SIZE) {

            const int rgid = (rj/STEP_SIZE)*global_size_x_d + (ri/STEP_SIZE);

            float stack[MAX_BLOCK_COUNT_1][BLOCK_SIZE][BLOCK_SIZE];

            const int block_count = block_counts[rgid];
            int weight_count = 0;

            // Build stack of similar blocks
            for (int n = 0; n < block_count; n++) {
                float block[BLOCK_SIZE][BLOCK_SIZE];
                for (int j = 0; j < BLOCK_SIZE; j++) {
                    for (int i = 0; i < BLOCK_SIZE; i++) {
                        const int2 pos = {ri - WINDOW_SIZE_HALF + similar_coords[2*(n*tot_globals_d + rgid)] + i,
                                          rj - WINDOW_SIZE_HALF + similar_coords[2*(n*tot_globals_d + rgid) + 1] + j};

                        int tid = pos.y * width + pos.x;
                        block[j][i] = input[tid];//(float)read_imageui(input, sampler, pos).s0;
                    }
                }

                dct2(block, stack[n]);
				if(fSigma > 40)
					threshold_2d(stack[n], fSigma);

            }

            // Do collaborative filtering
            for (int j = 0; j < BLOCK_SIZE; j++) {
                for (int i = 0; i < BLOCK_SIZE; i++) {

                    int blocks_left = block_count;
                    int k = 0;

                    // Process only max 8 layers at the time because of 8-point DCT
                    while (blocks_left > 0) 
                    {
                        //float pipe[8];// = { 0.0f };
                        float tr_pipe[8];
                        float _pipe[8] = { 0.0f };

                        for (int n = 0; n < min(blocks_left, 8); n++) {
                            _pipe[n] = stack[k*8 + n][j][i];
                        }

#if TRANSFORM_METHOD_1D == DCT_1D
                        dct(_pipe, tr_pipe, true);
                        threshold_1d(tr_pipe, &weight_count, block_count, fSigma);
                        idct(tr_pipe, _pipe, true);
#elif TRANSFORM_METHOD_1D == HAAR_1D
                        haar(_pipe, tr_pipe);
                        threshold_1d(tr_pipe, &weight_count, block_count, fSigma);
                        ihaar(tr_pipe, _pipe);
#endif

                        for (int n = 0; n < min(blocks_left, 8); n++) {
                            stack[k*8 + n][j][i] = _pipe[n];
                        }

                        k++;
                        blocks_left -= 8;
                    }
                }
            }

            // Convert weight count to weight multiplier
            const float wx = (weight_count >= 1) ? (1.0f / (variance * (float)weight_count)) : 1.0f;

            // Relocate stack blocks to their positions in split rectangle
            for (int n = 0; n < block_count; n++) {
                float block[BLOCK_SIZE][BLOCK_SIZE];
                idct2(stack[n], block);

                for (int j = 0; j < BLOCK_SIZE; j++) {
                    for (int i = 0; i < BLOCK_SIZE; i++) {
                        const int2 pixel_offset = {ri - gid.x, rj - gid.y};
                        const int2 pixel_pos = {similar_coords[2*(n*tot_globals_d + rgid)] - WINDOW_SIZE_HALF + i + pixel_offset.x,
                                                similar_coords[2*(n*tot_globals_d + rgid) + 1] - WINDOW_SIZE_HALF + j + pixel_offset.y};

                        if (pixel_pos.x >= 0 && pixel_pos.y >= 0 && pixel_pos.x < SPLIT_SIZE_X && pixel_pos.y < SPLIT_SIZE_Y) {
#if USE_KAISER_WINDOW
                            const float pixel_wx = wx * KAISER(i, j);
#else
                            const float pixel_wx = wx;
#endif
                            ACCU(pixel_pos.x, pixel_pos.y) += block[j][i] * pixel_wx;
                            WM(pixel_pos.x, pixel_pos.y) += pixel_wx;
                        }
                    }
                }
            }
        }
    }

#if UNROLL
#   pragma unroll
#endif
    for (int j = 0; j < SPLIT_SIZE_Y; j++) {
#if UNROLL
#   pragma unroll
#endif
        for (int i = 0; i < SPLIT_SIZE_X; i++) {
            const int2 pos = {gid.x + i, gid.y + j};
            if (pos.x < width && pos.y < height) {
                // Normalize aggregation output
                uchar pixel_value = convert_uchar_sat(
                    ACCU(i, j) / WM(i, j)
                );
                //if (WM(i, j) == 0) pixel_value = 255;
                //if (pixel_value == 0) pixel_value = 255;
                
                int tid = pos.y * width + pos.x;
                output[tid] = pixel_value;
                //write_imageui(output, pos, pixel_value);
            }
        }
    }
#endif
}

__kernel void bm3d_wiener_filter(
    __global float * output,
    const __global float * input, 
    const __global float * basic,  
    int width, 
    int height,
	const float fSigma,
    __global short* similar_coords,
    __global uchar* block_counts,
    const int global_size_x_d,
    const int tot_globals_d
) {
#if 1
    const int2 gid = {get_global_id(0) * SPLIT_SIZE_X, get_global_id(1) * SPLIT_SIZE_Y};
    if (gid.x > width-1 || gid.y > height-1) return;
    //const size_t tot_globals = get_global_size(0) * get_global_size(1);
    //const size_t global_id = get_global_id(1)*get_global_size(0) + get_global_id(0);

#if 1
    const int2 back_limit = {max(gid.x - WINDOW_SIZE_HALF, 0),
                             max(gid.y - WINDOW_SIZE_HALF, 0)};
    const int2 front_limit = {min(gid.x + SPLIT_SIZE_X - 1 + WINDOW_SIZE_HALF, width-1),
                              min(gid.y + SPLIT_SIZE_Y - 1 + WINDOW_SIZE_HALF, height-1)};
#else
    const int2 back_limit = gid;
    const int2 front_limit = gid;
#endif

    float accumulator[SPLIT_SIZE_Y][SPLIT_SIZE_X] = {{0.0f}};
    float weight_map[SPLIT_SIZE_Y][SPLIT_SIZE_X] = {{0.0f}};
	float variance = fSigma * fSigma;
    int ri = gid.x;
    int rj = gid.y;

    while (ri - STEP_SIZE >= back_limit.x) ri -= STEP_SIZE;
    while (rj - STEP_SIZE >= back_limit.y) rj -= STEP_SIZE;

    const int ri_min = ri;
    const int rj_min = rj;

    for (rj = rj_min; rj <= front_limit.y; rj += STEP_SIZE) {
        for (ri = ri_min; ri <= front_limit.x; ri += STEP_SIZE) {
            const int rgid = (rj/STEP_SIZE)*global_size_x_d + (ri/STEP_SIZE);

            float basic_stack[MAX_BLOCK_COUNT_2][BLOCK_SIZE][BLOCK_SIZE];
            float noise_stack[MAX_BLOCK_COUNT_2][BLOCK_SIZE][BLOCK_SIZE];

            const int block_count = block_counts[rgid];
            float sumsqr_weights = 0.0f;

            // Build stack
            for (int n = 0; n < block_count; n++) {
                float basic_block[BLOCK_SIZE][BLOCK_SIZE];
                float noise_block[BLOCK_SIZE][BLOCK_SIZE];

                for (int j = 0; j < BLOCK_SIZE; j++) {
                    for (int i = 0; i < BLOCK_SIZE; i++) {
                        const int2 pos = {ri - WINDOW_SIZE_HALF + similar_coords[2*(n*tot_globals_d + rgid)] + i,
                                          rj - WINDOW_SIZE_HALF + similar_coords[2*(n*tot_globals_d + rgid) + 1] + j};


                        int tid = pos.y * width + pos.x;
                        basic_block[j][i] = (float)input[tid];//read_imageui(basic, sampler, pos).s0;
                        noise_block[j][i] = (float)basic[tid];//read_imageui(input, sampler, pos).s0;
                    }
                }

                dct2(basic_block, basic_stack[n]);
                dct2(noise_block, noise_stack[n]);
            }

            for (int j = 0; j < BLOCK_SIZE; j++) {
                for (int i = 0; i < BLOCK_SIZE; i++) {

                    int blocks_left = block_count;
                    int k = 0;

                    while (blocks_left > 0) {

                        float _pipe[8]= {0.0f};
                        float pipe2[8] = {0.0f};
                        float tr_basic_pipe[8];
                        float tr_noise_pipe[8];

                        for (int n = 0; n < min(blocks_left, 8); n++) {
                            _pipe[n] = basic_stack[k*8 + n][j][i];
                            pipe2[n] = noise_stack[k*8 + n][j][i];
                        }

#if TRANSFORM_METHOD_1D == DCT_1D
                        dct(_pipe, tr_basic_pipe, true);
                        dct(pipe2, tr_noise_pipe, true);
#elif TRANSFORM_METHOD_1D == HAAR_1D
                        haar(_pipe, tr_basic_pipe);
                        haar(pipe2, tr_noise_pipe);
#endif

                        for (int n = 0; n < 8; n++) {
                            // weights
                            _pipe[n] = tr_basic_pipe[n]*tr_basic_pipe[n] / (tr_basic_pipe[n]*tr_basic_pipe[n] + variance);
                            sumsqr_weights += _pipe[n]*_pipe[n];
                            pipe2[n] = _pipe[n]*tr_noise_pipe[n];
                        }

#if TRANSFORM_METHOD_1D == DCT_1D
                        idct(pipe2, _pipe, true);
#elif TRANSFORM_METHOD_1D == HAAR_1D
                        ihaar(pipe2, _pipe);
#endif

                        for (int n = 0; n < 8; n++) {
                            noise_stack[k*8 + n][j][i] = _pipe[n];
                        }

                        k++;
                        blocks_left -= 8;
                    }
                }
            }

            const float wx = 1.0f / (variance * sumsqr_weights);

            for (int n = 0; n < block_count; n++) {
                float block[BLOCK_SIZE][BLOCK_SIZE];
                idct2(noise_stack[n], block);

                for (int j = 0; j < BLOCK_SIZE; j++) {
                    for (int i = 0; i < BLOCK_SIZE; i++) {
                        const int2 pixel_offset = {ri - gid.x, rj - gid.y};
                        const int2 pixel_pos = {similar_coords[2*(n*tot_globals_d + rgid)] - WINDOW_SIZE_HALF + i + pixel_offset.x,
                                                similar_coords[2*(n*tot_globals_d + rgid) + 1] - WINDOW_SIZE_HALF + j + pixel_offset.y};

                        if (pixel_pos.x >= 0 && pixel_pos.y >= 0 && pixel_pos.x < SPLIT_SIZE_X && pixel_pos.y < SPLIT_SIZE_Y) {
#if USE_KAISER_WINDOW
                            const float pixel_wx = wx * KAISER(i, j);
#else
                            const float pixel_wx = wx;
#endif
                            ACCU(pixel_pos.x, pixel_pos.y) += block[j][i] * pixel_wx;
                            WM(pixel_pos.x, pixel_pos.y) += pixel_wx;
                        }
                    }
                }
            }
        }
    }

#if UNROLL
#   pragma unroll
#endif
    for (int j = 0; j < SPLIT_SIZE_Y; j++) {
#if UNROLL
#   pragma unroll
#endif
        for (int i = 0; i < SPLIT_SIZE_X; i++) {
            const int2 pos = {gid.x + i, gid.y + j};
            if (pos.x < width && pos.y < height)
            {
                
                const uchar pixel_value = convert_uchar_sat(
                    ACCU(i, j) / WM(i, j)
                );

                int tid = pos.y * width + pos.x;
                output[tid] = pixel_value;
                //write_imageui(output, pos, pixel_value);
            }
        }
    }
#endif
}

