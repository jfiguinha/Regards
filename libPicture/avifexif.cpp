#include "header.h"
#include "avifexif.h"
#include <avif/avif.h>

CAvifExif::CAvifExif()
{
    decoder = avifDecoderCreate();   

}
CAvifExif::~CAvifExif()
{
    avifDecoderDestroy(decoder); 
}

bool CAvifExif::InitAvif(const char * filename)
{
    printf("CAvif::GetMetadata : %s has Exif \n", filename);

    avifResult result;
    result = avifDecoderSetIOFile(decoder, filename);  
        
	if (result != AVIF_RESULT_OK) {
		fprintf(stderr, "Cannot open file for read: %s\n", filename);
        return false;
	}

    result = avifDecoderParse(decoder);
	if (result != AVIF_RESULT_OK) {
		fprintf(stderr, "Cannot open file for read: %s\n", filename);
        return false;
	}
    
    return true;
        
}

void CAvifExif::GetMetadataAvif( uint8_t*& data, unsigned int& size)
{
     
    int itemSize = decoder->image->exif.size;
    // Request item data.
    if (size > 0)
    {
        memcpy(data, decoder->image->exif.data, itemSize);
    }
    else
        size = itemSize;
            
}
