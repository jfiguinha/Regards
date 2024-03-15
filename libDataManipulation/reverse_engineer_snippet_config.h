#pragma once
#include "config.h"

namespace vole { namespace cmfdgt {

class ReverseEngineerSnippetConfig : public vole::Config {
public:	
	ReverseEngineerSnippetConfig(const std::string &prefix = std::string());

	//! input file name
	std::string orig_file;
	//! file name of full image including the tampered region
	std::string copy_file;
	//! file name of manipulated region
	std::string snippet_file;
	//! alpha threshold for the overlay
	std::string snippet_alpha_file;
	//! alpha threshold for the overlay (whole image)
	std::string full_snippet_alpha_file;

	virtual std::string getString() const;

	protected:
	#ifdef WITH_BOOST
		virtual void initBoostOptions();
	#endif // WITH_BOOST
};

} }

