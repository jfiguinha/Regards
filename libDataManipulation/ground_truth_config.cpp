#include <header.h>
#include "ground_truth_config.h"

namespace vole {

GroundTruthConfig::GroundTruthConfig(const std::string &prefix) : Config(prefix) {
	#ifdef WITH_BOOST
		initBoostOptions();
	#endif // WITH_BOOST
}

std::string GroundTruthConfig::getString() const {
	std::stringstream s;

	if (prefix_enabled)
		s << "[" << prefix << "]" << std::endl; 

	s	<< "orig=" << orig_file << " # Image to process" << std::endl
		<< "copy=" << copy_file << 
			   " # image containing the copy-move forgery" << std::endl
		<< "snippet=" << snippet_file << 
			   " # manipulated region" << std::endl
		<< "alpha_snippet=" << snippet_alpha_file << 
			   " # snippet alpha channel file" << std::endl
		<< "full_snippet_alpha=" << full_snippet_alpha_file << 
			" # alpha channel of the snippet in the dimensions of the full image" << std::endl
		<< "ground_truth=" << ground_truth_file << " # ground truth for the copied region" << std::endl
		<< "l1tol=" << l1_tolerance << " # pixel distance in one channel: within this distance, consider a pixel a match" << std::endl
		<< "prune=" << prune_factor << " # divisor for upper limit on the match badness. If your match is not found, lower this factor" << std::endl
		<< "opaque=" << opaque_intensity << " # alpha channel intensity that should be considered opaque (between 0 and 255)" << std::endl
		<< "l2dist=" << (use_l2 ? 1 : 0) << " # Use L2-distance instead of L1" << std::endl
		;
	return s.str();
}



#ifdef WITH_BOOST
void GroundTruthConfig::initBoostOptions() {
	options.add_options()
		(key("orig"), value(&orig_file)->default_value(""),
			"Image to process")
		(key("copy"), value(&copy_file)->default_value(""),
			"image containing the copy-move forgery")
		(key("snippet"), value(&snippet_file)->default_value(""),
			"manipulated region")
		(key("alpha_snippet"), value(&snippet_alpha_file)->default_value(""),
			"snippet alpha channel file")
		(key("full_snippet_alpha"), value(&full_snippet_alpha_file)->default_value(""),
			"alpha channel of the snippet in the dimensions of the full image")
		(key("ground_truth"), value(&ground_truth_file)->default_value(""),
			"ground truth for the copied region")
		(key("l1tol"), value(&l1_tolerance)->default_value(15),
			"pixel distance in one channel: within this distance, consider a pixel a match")
		(key("prune"), value(&prune_factor)->default_value(4.0, "4.0"),
			"divisor for upper limit on the match badness. If your match is not found, lower this factor")
		(key("opaque"), value(&opaque_intensity)->default_value(255),
			"alpha channel intensity that should be considered opaque (between 0 and 255)")
		(key("l2dist"), bool_switch(&use_l2)->default_value(false), "Use L2-distance instead of L1")
	;
}
#endif // WITH_BOOST

}

