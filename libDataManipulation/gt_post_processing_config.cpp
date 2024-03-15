#include <header.h>
#include "gt_post_processing_config.h"

namespace vole { namespace cmfdgt {

GtPostProcessingConfig::GtPostProcessingConfig(const std::string &prefix) : Config(prefix) {
	#ifdef WITH_BOOST
		initBoostOptions();
	#endif // WITH_BOOST
}

std::string GtPostProcessingConfig::getString() const {
	std::stringstream s;

	if (prefix_enabled)
		s << "[" << prefix << "]" << std::endl; 

	s	<< "ground_truth=" << ground_truth_file << " # ground truth file for post-processing" << std::endl
		<< "postprocessed=" << pp_ground_truth_file << 
			   " # post-processed ground truth file (i.e. the output)" << std::endl
		<< "support_size=" << support_size << " # area of support for a copy-move detector (e.g. 8, for 8x8 squares)" << std::endl
		<< "support_ratio=" << support_ratio << " # percent of pixels that should be copied within the support region" << std::endl
		;
	return s.str();
}

#ifdef WITH_BOOST
void GtPostProcessingConfig::initBoostOptions() {
	options.add_options()
		(key("ground_truth"), value(&ground_truth_file)->default_value(""),
			"ground truth file for post-processing")
		(key("postprocessed"), value(&pp_ground_truth_file)->default_value(""),
			"post-processed ground truth file (i.e. the output)")
		(key("support_size"), value(&support_size)->default_value(7),
			"area of support for a copy-move detector (e.g. 8, for 8x8 squares)")
		(key("support_ratio"), value(&support_ratio)->default_value(50),
			"percent of pixels that should be copied within the support region")
	;
}
#endif // WITH_BOOST



}}

