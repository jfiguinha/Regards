#ifndef wxMSVC_VERSION
#include <qpdf/QPDFObject.hh>

void QPDFObject::setDescription(QPDF* qpdf, const std::string& description)
{
	this->owning_qpdf = qpdf;
	this->object_description = description;
#endif
}
