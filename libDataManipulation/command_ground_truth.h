#pragma once

#include "command.h"
#include "ground_truth_config.h"

namespace vole {

class GroundTruth : public Command
{
public:
	GroundTruth();

	int execute();

	void printHelp() const;
	void printShortHelp() const;

	GroundTruthConfig config;
private:

};

}

