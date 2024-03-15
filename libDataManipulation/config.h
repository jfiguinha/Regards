/*	
	Copyright(c) 2010 Christian Riess <christian.riess@cs.fau.de>
	and Johannes Jordan <johannes.jordan@cs.fau.de>.

	This file may be licensed under the terms of of the GNU General Public
	License, version 3, as published by the Free Software Foundation. You can
	find it here: http://www.gnu.org/licenses/gpl.html
*/

#pragma once

#ifdef WITH_BOOST
#include <boost/program_options.hpp>
#endif // WITH_BOOST

#ifdef VOLE_GUI
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#endif // VOLE_GUI

#include "hashes.h"


#ifdef WITH_BOOST
using namespace boost::program_options;
#endif // WITH_BOOST


namespace vole {

	/* base class that exposes configuration handling */
	class Config {
	public:
		Config(const std::string &prefix = std::string());
		virtual ~Config() {}

		virtual bool readConfig(const char *filename);

		bool storeConfig(const char *filename);
		virtual std::string getString() const = 0;

		virtual unsigned long int configHash(vole::HashMethod method = vole::HASH_djb2);

		#ifdef WITH_BOOST
			// takes a variables_map as optional argument, because there may be already one in use
			bool parseOptionsDescription(const char *filename,
				boost::program_options::variables_map *vm = NULL);

			boost::program_options::options_description options;
		#endif // WITH_BOOST

		/// helper function to be used in initBoostOptions
		const char* key(const char *key) const;

		#ifdef VOLE_GUI
			virtual QWidget *getConfigWidget() { return NULL; /* FIXME make this pure virtual */}
			virtual void updateValuesFromWidget() { return; /* FIXME make this pure virtual */ }
			QPushButton *getUpdateButton() { return updateComputation; }
		#endif // VOLE_GUI

		/// verbosity level: 0 = silent, 1 = normal, 2 = a lot, 3 = insane
		int verbosity;
		/// config option prefix (may be empty)
		std::string prefix;
		/// cache for faster operation
		bool prefix_enabled;

	protected:

		#ifdef WITH_BOOST
			virtual void initMandatoryBoostOptions();
		#endif // WITH_BOOST
	
		#ifdef VOLE_GUI
			void initConfigWidget();

			QWidget *configWidget;
			QHBoxLayout *layout;
			QPushButton *updateComputation;
		#endif // VOLE_GUI
		/*
		// TODO these lines serve only as a template for the migration - delete
		// them soon
		// graphical output on runtime?
		bool graphical;
		// image to be processed
		std::string inputfile;
		// working directory
		std::string outputdir;
		// subcommand (if required for a command)
		std::string method;
		// verbosity level: 0 = silent, 1 = normal, 2 = a lot, 3 = insane
		int verbosity;
		*/
	};
}


/* this is some macro trickery (just leave it as is) to make ENUMs
   work for reading (program_options) and writing (to stream) */
#ifdef WITH_BOOST
#if BOOST_VERSION < 104200
#define ENUM_MAGIC(ENUM) \
	const char* ENUM ## Str[] = ENUM ## String;\
	void validate(boost::any& v, const std::vector<std::string>& values, \
	               ENUM* target_type, int) \
	{ \
		validators::check_first_occurrence(v); \
		const std::string& s = validators::get_single_string(values); \
		for (unsigned int i = 0; i < sizeof(ENUM ## Str)/sizeof(char*); ++i) { \
			if (strcmp(ENUM ## Str[i], s.c_str()) == 0) { \
				v = boost::any((ENUM)i); \
				return; \
			} \
		} \
		throw validation_error("invalid value"); \
	} \
	std::ostream& operator<<(std::ostream& o, ENUM e)  \
	{	o << ENUM ## Str[e]; return o;  }
#else	// only the exception throw is changed
#define ENUM_MAGIC(ENUM) \
	const char* ENUM ## Str[] = ENUM ## String;\
	void validate(boost::any& v, const std::vector<std::string>& values, \
	               ENUM* target_type, int) \
	{ \
		validators::check_first_occurrence(v); \
		const std::string& s = validators::get_single_string(values); \
		for (unsigned int i = 0; i < sizeof(ENUM ## Str)/sizeof(char*); ++i) { \
			if (strcmp(ENUM ## Str[i], s.c_str()) == 0) { \
				v = boost::any((ENUM)i); \
				return; \
			} \
		} \
		throw validation_error(validation_error::invalid_option_value); \
	} \
	std::ostream& operator<<(std::ostream& o, ENUM e)  \
	{	o << ENUM ## Str[e]; return o;  }
#endif // BOOST_VERSION
#else
#define ENUM_MAGIC(ENUM) \
	const char* ENUM ## Str[] = ENUM ## String;\
	std::ostream& operator<<(std::ostream& o, ENUM e)  \
	{	o << ENUM ## Str[e]; return o;  }
#endif // WITH_BOOST


