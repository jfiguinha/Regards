#ifndef wxMSVC_VERSION
#include <qpdf/QPDFObject.hh>

void QPDFObject::setDescription(QPDF* qpdf, std::string const& description)
{
    this->owning_qpdf = qpdf;
    this->object_description = description;
#endif
}