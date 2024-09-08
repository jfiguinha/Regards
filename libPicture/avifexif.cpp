#include "header.h"
#include "avifexif.h"
#include <avif/avif.h>

CAvifExif::CAvifExif()
{
    decoder = avifDecoderCreate();   
	decoded = avifImageCreateEmpty();  

}
CAvifExif::~CAvifExif()
{
    avifImageDestroy(decoded);
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
        
    result = avifDecoderRead(decoder, decoded);
	if (result != AVIF_RESULT_OK) {
		fprintf(stderr, "Cannot open file for read: %s\n", filename);
        return false;
	}
    
    return true;
        
}

void CAvifExif::GetMetadataAvif( uint8_t*& data, unsigned int& size)
{
      
    int itemSize = decoded->exif.size;
    // Request item data.
    if (size > 0)
    {
        memcpy(data, decoded->exif.data, itemSize);
    }
    else
        size = itemSize;
            
}
