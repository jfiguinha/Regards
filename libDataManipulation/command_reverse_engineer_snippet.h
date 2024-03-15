#pragma once

#include "command.h"
#include "reverse_engineer_snippet_config.h"

namespace vole {
namespace cmfdgt {

class CommandReverseEngineerSnippet : public vole::Command {
public:
	CommandReverseEngineerSnippet();

	int execute();

	void printHelp() const;
	void printShortHelp() const;

	ReverseEngineerSnippetConfig config;
private:

};

}
}

