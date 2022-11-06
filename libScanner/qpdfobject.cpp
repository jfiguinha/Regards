#include <header.h>
#include <qpdf/QPDFObject.hh>
#ifndef wxMSVC_VERSION
void QPDFObject::setDescription(QPDF* qpdf, std::string const& description)
{
    this->owning_qpdf = qpdf;
    this->object_description = description;
}
#endif
