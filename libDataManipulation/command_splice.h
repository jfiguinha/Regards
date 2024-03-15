#pragma once

#include "command.h"
#include "splice_config.h"


namespace vole { namespace cmfd_gt {

/** Splices an image and a number of snippets and generates the attached ground
 * truth. It applies additionally rotation, scaling, Gaussian noise and JPEG
 * artifacts, depending on the configuration values.
 */
class CommandSplice : public vole::Command
{
public:
	CommandSplice();

	int execute();

	void printHelp() const;
	void printShortHelp() const;

	SpliceConfig config;

private:
//	void parseOptions(void);
	std::pair<std::string, std::string> get_output_file_names();

	std::vector<int> insert_positions;
	std::vector<cv::Mat_<cv::Vec3b> > snippets;
	std::vector<cv::Mat_<uchar> > snippets_alpha;
};

} }

