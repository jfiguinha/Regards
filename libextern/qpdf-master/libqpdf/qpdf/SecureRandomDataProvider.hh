#ifndef SECURERANDOMDATAPROVIDER_HH
#define SECURERANDOMDATAPROVIDER_HH

#include <qpdf/RandomDataProvider.hh>
#include <qpdf/DLL.h>

class SecureRandomDataProvider: public RandomDataProvider
{
  public:
    QPDF_DLL
    SecureRandomDataProvider();
    QPDF_DLL
    virtual ~SecureRandomDataProvider();

    QPDF_DLL
    virtual void provideRandomData(unsigned char* data, size_t len);

    QPDF_DLL
    static RandomDataProvider* getInstance();
};

#endif // SECURERANDOMDATAPROVIDER_HH
