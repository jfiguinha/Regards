#ifndef QPDF_BOOL_HH
#define QPDF_BOOL_HH

#include <qpdf/QPDFObject.hh>

class QPDF_Bool: public QPDFObject
{
  public:
    QPDF_Bool(bool val);
    virtual ~QPDF_Bool();
    virtual std::string unparse();
    virtual JSON getJSON();
    virtual QPDFObject::object_type_e getTypeCode() const;
    virtual char const* getTypeName() const;
    bool getVal() const;

  private:
    bool val;
};

#endif // QPDF_BOOL_HH
