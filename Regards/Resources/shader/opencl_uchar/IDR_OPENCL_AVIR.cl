/**
 * @file avir_opencl_kernels.cl
 *
 * Portage OpenCL (brique autonome) du coeur de redimensionnement de la
 * librairie AVIR (avir.h / avir_dil.h). Voir COpenCLAvirResizer.h pour la
 * description precise de la portee et des simplifications assumees par
 * rapport a l'implementation CPU d'origine.
 *
 * Format : RGBA float32 (float4), image lineaire ligne par ligne.
 */

typedef struct
{
    int srcStart;   ///< Index du premier pixel source utilise (peut etre < 0).
    int tapCount;   ///< Nombre de coefficients pour ce pixel de sortie.
    int coefOffset; ///< Offset dans le buffer de coefficients aplatis.
} ResizeTap;

// --- Gamma sRGB ---------------------------------------------------------

inline float srgb_to_linear( float c )
{
    return ( c <= 0.04045f ) ? ( c / 12.92f )
                              : pow( ( c + 0.055f ) / 1.055f, 2.4f );
}

inline float linear_to_srgb( float c )
{
    return ( c <= 0.0031308f ) ? ( c * 12.92f )
                                : ( 1.055f * pow( c, 1.0f / 2.4f ) - 0.055f );
}

__kernel void k_linearize_srgb( __global float4* buf, const int count )
{
    const int i = get_global_id( 0 );
    if( i >= count ) return;

    const float4 p = buf[ i ];
    buf[ i ] = (float4)( srgb_to_linear( p.x ), srgb_to_linear( p.y ),
        srgb_to_linear( p.z ), p.w );
}

__kernel void k_delinearize_srgb( __global float4* buf, const int count )
{
    const int i = get_global_id( 0 );
    if( i >= count ) return;

    const float4 p = buf[ i ];
    buf[ i ] = (float4)( linear_to_srgb( p.x ), linear_to_srgb( p.y ),
        linear_to_srgb( p.z ), p.w );
}

// --- Resize separable (convolution a taps variables par pixel) ---------
//
// Un work-item par pixel de sortie. Bords geres par extension (clamp) des
// coordonnees source, comme la replication de bord utilisee par AVIR.

__kernel void k_resize_h(
    __global const float4* src, const int srcW, const int srcH,
    __global float4* dst, const int dstW,
    __global const ResizeTap* taps,   // dstW entrees
    __global const float* coefs )
{
    const int x = get_global_id( 0 );
    const int y = get_global_id( 1 );
    if( x >= dstW || y >= srcH ) return;

    const ResizeTap t = taps[ x ];
    const __global float4* row = src + (size_t) y * srcW;
    float4 acc = (float4)( 0.0f );

    for( int k = 0; k < t.tapCount; ++k )
    {
        const int sx = clamp( t.srcStart + k, 0, srcW - 1 );
        acc += row[ sx ] * coefs[ t.coefOffset + k ];
    }

    dst[ (size_t) y * dstW + x ] = acc;
}

__kernel void k_resize_v(
    __global const float4* src, const int srcW, const int srcH,
    __global float4* dst, const int dstH,
    __global const ResizeTap* taps,   // dstH entrees
    __global const float* coefs )
{
    const int x = get_global_id( 0 );
    const int y = get_global_id( 1 );
    if( x >= srcW || y >= dstH ) return;

    const ResizeTap t = taps[ y ];
    float4 acc = (float4)( 0.0f );

    for( int k = 0; k < t.tapCount; ++k )
    {
        const int sy = clamp( t.srcStart + k, 0, srcH - 1 );
        acc += src[ (size_t) sy * srcW + x ] * coefs[ t.coefOffset + k ];
    }

    dst[ (size_t) y * srcW + x ] = acc;
}

// --- Nettete (unsharp mask separable, noyau fixe partage) --------------
//
// dst = center + (center - flou) * amount. 'kernel1d' est un filtre passe-
// bas (le "flou"), construit cote CPU (COpenCLAvirResizer::BuildSharpenKernel).

__kernel void k_sharpen_h(
    __global const float4* src, const int w, const int h,
    __global float4* dst,
    __global const float* kernel1d, const int klen, const float amount )
{
    const int x = get_global_id( 0 );
    const int y = get_global_id( 1 );
    if( x >= w || y >= h ) return;

    const int half_ = klen / 2;
    const __global float4* row = src + (size_t) y * w;
    float4 blur = (float4)( 0.0f );

    for( int k = 0; k < klen; ++k )
    {
        const int sx = clamp( x + k - half_, 0, w - 1 );
        blur += row[ sx ] * kernel1d[ k ];
    }

    const float4 center = row[ x ];
    dst[ (size_t) y * w + x ] = center + ( center - blur ) * amount;
}

__kernel void k_sharpen_v(
    __global const float4* src, const int w, const int h,
    __global float4* dst,
    __global const float* kernel1d, const int klen, const float amount )
{
    const int x = get_global_id( 0 );
    const int y = get_global_id( 1 );
    if( x >= w || y >= h ) return;

    const int half_ = klen / 2;
    float4 blur = (float4)( 0.0f );

    for( int k = 0; k < klen; ++k )
    {
        const int sy = clamp( y + k - half_, 0, h - 1 );
        blur += src[ (size_t) sy * w + x ] * kernel1d[ k ];
    }

    const float4 center = src[ (size_t) y * w + x ];
    dst[ (size_t) y * w + x ] = center + ( center - blur ) * amount;
}

// --- Dithering -----------------------------------------------------------

__kernel void k_dither_round( __global float4* buf, const int count,
    const float peak )
{
    const int i = get_global_id( 0 );
    if( i >= count ) return;

    buf[ i ] = clamp( round( buf[ i ] ), 0.0f, peak );
}

// Diffusion d'erreur, poids identiques a CImageResizerDithererErrdINL
// d'AVIR (0.364842 / 0.207305 / 0.063011). Sequentiel par nature : chaque
// pixel depend de l'erreur accumulee des pixels precedents (meme ligne et
// ligne precedente), donc un seul work-item traite toute l'image. Le cout
// est negligeable devant la convolution de resize, ce n'est pas le goulot
// d'etranglement du pipeline.

__kernel void k_dither_errdiffusion(
    __global float4* buf, const int w, const int h,
    __global float4* errBuf,   // meme taille que buf, doit etre initialise a 0
    const float peak )
{
    if( get_global_id( 0 ) != 0 ) return;

    for( int y = 0; y < h; ++y )
    {
        __global float4* row = buf + (size_t) y * w;
        __global float4* erow = errBuf + (size_t) y * w;
        __global float4* nrow = ( y + 1 < h ) ? erow + w : 0;

        for( int x = 0; x < w; ++x )
        {
            const float4 val = row[ x ] + erow[ x ];
            erow[ x ] = (float4)( 0.0f );

            const float4 rounded = clamp( round( val ), 0.0f, peak );
            const float4 noise = val - rounded;
            row[ x ] = rounded;

            if( x + 1 < w )
                row[ x + 1 ] += noise * 0.364842f;

            if( nrow )
            {
                if( x > 0 )     nrow[ x - 1 ] += noise * 0.207305f;
                                 nrow[ x ]     += noise * 0.364842f;
                if( x + 1 < w )  nrow[ x + 1 ] += noise * 0.063011f;
            }
        }
    }
}
