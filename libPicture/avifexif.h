struct avifDecoder;


class CAvifExif
{
public:
    CAvifExif();
    ~CAvifExif();
    bool InitAvif(const char * filename);
    void GetMetadataAvif( uint8_t * & buffer, unsigned int& size);
private:
    avifDecoder* decoder;   

    
};