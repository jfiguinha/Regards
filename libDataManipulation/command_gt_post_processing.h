#pragma once

#include "command.h"
#include "gt_post_processing_config.h"

namespace vole { namespace cmfdgt {

class CommandGtPostProcessing : public Command
{
public:
	CommandGtPostProcessing();

	int execute();

	void printHelp() const;
	void printShortHelp() const;

	GtPostProcessingConfig config;
};

}}

