#ifndef QPDF_REAL_HH
#define QPDF_REAL_HH

#include <qpdf/QPDFObject.hh>

class QPDF_Real: public QPDFObject
{
  public:
    QPDF_Real(std::string const& val);
    QPDF_Real(double value, int decimal_places = 0);
    virtual ~QPDF_Real();
    virtual std::string unparse();
    virtual JSON getJSON();
    virtual QPDFObject::object_type_e getTypeCode() const;
    virtual char const* getTypeName() const;
    std::string getVal();

  private:
    // Store reals as strings to avoid roundoff errors.
    std::string val;
};

#endif // QPDF_REAL_HH
