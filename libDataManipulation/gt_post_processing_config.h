#pragma once

#include "config.h"

namespace vole { namespace cmfdgt {

class GtPostProcessingConfig : public Config {
public:
	GtPostProcessingConfig(const std::string &prefix = std::string());

	//! ground truth file for post-processing
	std::string ground_truth_file;

	//! post-processed ground truth file (i.e. the output)
	std::string pp_ground_truth_file;

	int support_size;
	int support_ratio;

	virtual std::string getString() const;

	protected:
	#ifdef WITH_BOOST
		virtual void initBoostOptions();
	#endif // WITH_BOOST


};

}}



