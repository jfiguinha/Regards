#include <header.h>
#include "splice_config.h"

namespace vole {

SpliceConfig::SpliceConfig(const std::string &prefix) : Config(prefix) {
	#ifdef WITH_BOOST
		initBoostOptions();
	#endif // WITH_BOOST
}

std::string SpliceConfig::getString() const {
	std::stringstream s;

	if (prefix_enabled)
		s << "[" << prefix << "]" << std::endl; 

	s	<< "local_noise=" << (local_noise ? 1:0) << " # add noise only to the snippet" << std::endl
		<< "global_noise=" << (global_noise ? 1:0) << " # add noise to the whole image" << std::endl
		<< "rot=" << (rot ? 1:0) << " # add rotation" << std::endl
		<< "scale=" << (scale ? 1:0) << " # add scaling" << std::endl
		<< "jpeg=" << (jpeg ? 1:0) << " # add JPEG compression" << std::endl
		<< "orig=" << orig_file << " # Original (untampered) image file" << std::endl
		<< "output=" << output_directory << " # Working directory" << std::endl
		<< "file_prefix=" << file_prefix << " # prefix for the output file name" << std::endl
		<< "snippet=" << snippet_file << " # manipulated region" << std::endl
		<< "alpha_snippet=" << snippet_alpha_file << " # snippet alpha channel file" << std::endl
		<< "local_noise_std_dev=" << local_noise_std_dev << 
			   " # Standard deviation of Gaussian noise if noise should be added to the snippet" << std::endl
		<< "global_noise_std_dev=" << global_noise_std_dev << 
			   " # Standard deviation of Gaussian noise if noise should be added to the whole_image" << std::endl
		<< "insert_positions=" << insert_positions <<
				" # insert position(s) of the snippet(s) (','-separated list, in the order x11,y11,x12,y12,x21...)" << std::endl
		<< "source_positions=" << source_positions <<
			" # source position(s) of the snippet(s) (','-separated list, in the order x1,y1,x2,y2,x3,...)" << std::endl

		<< "angle=" << angle << " # angle for rotation of the manipulated region" << std::endl
		<< "jquality=" << quality << " # quality factor for JPEG compression" << std::endl

		// params
 		<< "lnoise_stddev=" << local_noise_std_dev <<
			" # Standard deviation in per mille (1/1000) of Gaussian noise (added to the snippet)" << std::endl
 		<< "gnoise_stddev=" << global_noise_std_dev <<
			" # Standard deviation in per mille (1/1000) of Gaussian noise (added to the whole image)" << std::endl
		<< "scaling=" << scaling <<
			" # scaling factor of the snippet, per mille (1/1000)" << std::endl
		<< "angle=" << angle <<
			" # angle for rotation of the manipulated region" << std::endl
		<< "jquality=" << quality <<
			" # quality factor for jpeg compression" << std::endl
		;
	return s.str();
}


#ifdef WITH_BOOST
void SpliceConfig::initBoostOptions() {
	options.add_options()
		// operations
		(key("local_noise"), bool_switch(&local_noise)->default_value(false), "add noise only to the snippet")
		(key("global_noise"), bool_switch(&global_noise)->default_value(false), "add noise to the whole image")
		(key("rot"), bool_switch(&rot)->default_value(false), "add rotation")
		(key("scale"), bool_switch(&scale)->default_value(false), "add scaling")
		(key("jpeg"), bool_switch(&jpeg)->default_value(false), "add JPEG compression")

		// files
		(key("orig"), value(&orig_file)->default_value(""), "Original (untampered) image file")
		(key("output,O"), value(&output_directory)->default_value("/tmp/"), "Directory where the image is written to ")
		(key("file_prefix"), value(&file_prefix)->default_value(""), "Prefix for the output file name")
		(key("snippet"), value(&snippet_file)->default_value(""), "manipulated region")
		(key("alpha_snippet"), value(&snippet_alpha_file)->default_value(""), "snippet alpha channel file")

		// snippet source and insertion positions
		(key("source_positions"), value(&source_positions)->default_value(""),
			"source position(s) of the snippet(s) (','-separated list, in the order x1,y1,x2,y2,x3,...)")
		(key("insert_positions"), value(&insert_positions)->default_value(""),
			"insert position(s) of the snippet(s) (','-separated list, in the order x1,y1,x2,y2,x3,...)")

		// params
 		(key("lnoise_stddev"), value(&local_noise_std_dev)->default_value(10),
			"Standard deviation in per mille (1/1000) of Gaussian noise (added to the snippet)")
 		(key("gnoise_stddev"), value(&global_noise_std_dev)->default_value(10),
			"Standard deviation in per mille (1/1000) of Gaussian noise (added to the whole image)")
		(key("scaling"), value(&scaling)->default_value(10),
			"scaling factor of the snippet, per mille (1/1000)")
		(key("angle"), value(&angle)->default_value(10), // e.g. 0
			"angle for rotation of the manipulated region")
		(key("jquality"), value(&quality)->default_value(90), // e.g. 0
			"quality factor for jpeg compression")
	;
}
#endif // WITH_BOOST

}

