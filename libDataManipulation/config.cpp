/*	
	Copyright(c) 2010 Christian Riess <christian.riess@cs.fau.de>
	and Johannes Jordan <johannes.jordan@cs.fau.de>.

	This file may be licensed under the terms of of the GNU General Public
	License, version 3, as published by the Free Software Foundation. You can
	find it here: http://www.gnu.org/licenses/gpl.html
*/

#include <header.h>
#include "config.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#ifdef VOLE_GUI
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#endif // VOLE_GUI

#ifdef WITH_BOOST
using namespace boost::program_options;
#endif // WITH_BOOST

namespace vole {

Config::Config(const std::string& p) : prefix(p), prefix_enabled(!p.empty())
{
	if (prefix_enabled)
		prefix.push_back('.');
		
#ifdef WITH_BOOST
	initMandatoryBoostOptions();
#endif // WITH_BOOST
#ifdef VOLE_GUI
	configWidget = NULL;
#endif // VOLE_GUI
}

bool Config::readConfig(const char *filename) {
#ifdef WITH_BOOST
	return parseOptionsDescription(filename);
#endif // WITH_BOOST

	std::cerr << "No options reader implemented - big mayhem in common/config.cpp" << std::endl;
	return false; 
}

#ifdef VOLE_GUI
void Config::initConfigWidget() {
	if (configWidget != NULL) return;
	configWidget = new QWidget();
	// append everything that could possibly be configured
	layout = new QHBoxLayout();
	QVBoxLayout *globalValues = new QVBoxLayout();
	QLabel *verbosity = new QLabel("verbosity");
	globalValues->addWidget(verbosity);
	globalValues->addStretch();
	updateComputation = new QPushButton("Update!");
	globalValues->addWidget(updateComputation);
	layout->addLayout(globalValues);
	// TODO: in subclasses, you have to set the layout:
	// configWidget->setLayout(layout);
}
#endif // VOLE_GUI

const char* Config::key(const char *k) const {
	if (!prefix_enabled)
		return k;
	/* note: the boost parser enables shortcut keys (single character) by adding
	   ",<keychar>" to the key description. It does not support these with
	   a prefix. So they are stripped here. */
	std::string full = prefix, key = k;
	full += key.substr(0, key.find_first_of(','));
	return _strdup(full.c_str());
	/* note this is a waste of memory, but only a small one and happens only
	   once. we have no other good option with add_options() only taking char* */
}

bool Config::storeConfig(const char *filename) {
	std::ofstream out;
	out.open(filename);
	// TODO ordentliche Fehlerabfrage
	if (!out.good())
		return false;
	out << getString() << std::endl;
	out.close();
	return true;
}

unsigned long int Config::configHash(vole::HashMethod method) {
	// collect string from relevant parameters, hash it
	std::string current_config = getString();
	return Hashes::getHash(current_config.c_str(), method);
}

#ifdef WITH_BOOST
void Config::initMandatoryBoostOptions() {
	options.add_options()
		(key("verbose,V"), value(&verbosity)->default_value(1),
		 "Verbosity Level: 0 = silent, 1 = normal, 2 = much output, 3 = insane")
	;
}

bool Config::parseOptionsDescription(const char *filename, variables_map *vm)
{
	std::ifstream file(filename, std::ios_base::in);
	if (!file.good()) {
		std::cerr << "*** Error: File " << filename
		          << " could not be read!" << std::endl;
		return false;
	}

	try {
		bool vm_created = false;
		if (vm == NULL) {
			vm = new variables_map();
			vm_created = true;
		}
		store(parse_config_file(file, options), *vm);
		file.close();
		notify(*vm);
		if (vm_created)
			delete vm;
		return true;
	} catch (std::exception& e) {
		std::cerr << "*** Error reading configuration:\n    " << e.what();
		std::cerr << std::endl << std::endl;
		return false;
	}

}
#endif // WITH_BOOST


}
