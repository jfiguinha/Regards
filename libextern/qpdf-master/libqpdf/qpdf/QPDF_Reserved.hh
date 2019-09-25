#ifndef QPDF_RESERVED_HH
#define QPDF_RESERVED_HH

#include <qpdf/QPDFObject.hh>

class QPDF_Reserved: public QPDFObject
{
  public:
    virtual ~QPDF_Reserved();
    virtual std::string unparse();
    virtual JSON getJSON();
    virtual QPDFObject::object_type_e getTypeCode() const;
    virtual char const* getTypeName() const;
};

#endif // QPDF_RESERVED_HH
