#pragma once

#include "config.h"

/**
 * Configuration parameters for the ground truth generation.
 */
namespace vole {

class SpliceConfig : public Config {
public:	
	SpliceConfig(const std::string &prefix = std::string());

	//! Original (untampered) image file
	std::string orig_file;
	//! directory for all intermediate files
	std::string output_directory;
	//! prefix for the output file name
	std::string file_prefix;
	//! subcommand (if required for a command)

	//! add noise only on snippet
	bool local_noise;
	//! add noise afterwards
	bool global_noise;

	//! add rotation
	bool rot;
	//! add scaling
	bool scale;
	//! add jpeg artifacts
	bool jpeg;

	//! file name of manipulated region
	std::string snippet_file;

	//! alpha threshold for the overlay
	std::string snippet_alpha_file;

	//! noise level; Standard deviation of Gaussian noise (for snippet)
	int local_noise_std_dev; // divided by 1000
	int global_noise_std_dev; // divided by 1000

	// rotation section
	int angle;  // in degrees

	// scaling section
	int scaling; // in per mille (1/1000)

	// jpeg section
	int quality;

	// source positions: where do the snippets come from?
	std::string source_positions;
	// insert positions: where should the snippets be inserted?
	std::string insert_positions;

	virtual std::string getString() const;

	protected:
	#ifdef WITH_BOOST
		virtual void initBoostOptions();
	#endif // WITH_BOOST

};

}

