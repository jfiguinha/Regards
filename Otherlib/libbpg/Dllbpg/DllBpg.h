// Le bloc ifdef suivant est la façon standard de créer des macros qui facilitent l'exportation 
// à partir d'une DLL. Tous les fichiers contenus dans cette DLL sont compilés avec le symbole DLLFORMATBPG_EXPORTS
// défini sur la ligne de commande. Ce symbole ne doit pas être défini pour un projet
// qui utilisent cette DLL. De cette manière, les autres projets dont les fichiers sources comprennent ce fichier considèrent les fonctions 
// DLLFORMATBPG_API comme étant importées à partir d'une DLL, tandis que cette DLL considère les symboles
// définis avec cette macro comme étant exportés.
#ifdef DLLFORMATBPG_EXPORTS
#define DLLFORMATBPG_API __declspec(dllexport)
#else
#define DLLFORMATBPG_API __declspec(dllimport)
#endif

#include <stdint.h>

DLLFORMATBPG_API int BPG_GetDimensions(uint8_t * buf, size_t buf_len, int & width, int & height);
DLLFORMATBPG_API int BPG_GetPictureRGBA(uint8_t * buf, size_t buf_len, uint8_t * data, size_t data_len, int & width, int & height, bool flip);
DLLFORMATBPG_API int BPG_GetPictureBGRA(uint8_t * buf, size_t buf_len, uint8_t * data, size_t data_len, int & width, int & height, bool flip);
DLLFORMATBPG_API int BPG_SavePNGPicture(uint8_t * buf, size_t buf_len, int compress_level, int lossless_mode, int bit_depth, const char * filename);
DLLFORMATBPG_API uint8_t * BPG_ReadToPNGPicture(uint8_t * buf, size_t buf_len, size_t * buf_out, int bit_depth, int * returnValue);

