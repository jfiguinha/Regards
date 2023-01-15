/**********************************************************************
 * File:        tesseractmain.cpp
 * Description: Main program for merge of tess and editor.
 * Author:      Ray Smith
 *
 * (C) Copyright 1992, Hewlett-Packard Ltd.
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 ** http://www.apache.org/licenses/LICENSE-2.0
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 *
 **********************************************************************/
#include <header.h>
#include <tesseract/ocrclass.h>
#include "ExportOcr.h"
#include <wx/progdlg.h>
using namespace Regards::Scanner;
tesseract::TessBaseAPI CExportOcr::api;
// Include automatically generated configuration file if running autoconf
#ifdef HAVE_CONFIG_H
#include "config_auto.h"
#endif
//#include <tesseract/dict.h>
//#include <tesseract/simddetect.h>
#include <cerrno>               // for errno

#include <tesseract/renderer.h>
#include <leptonica/allheaders.h>
//#include "dict.h"
#if defined(USE_OPENCL)
#include "openclwrapper.h"      // for OpenclDevice
#endif

#if defined(HAVE_LIBARCHIVE)
#include <archive.h>
#endif

#if defined(_WIN32)
#include <io.h>
#if defined(HAVE_TIFFIO_H)

#include <tiffio.h>


static void Win32ErrorHandler(const char* module, const char* fmt,
	va_list ap) {
	if (module != nullptr) {
		fprintf(stderr, "%s: ", module);
	}
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, ".\n");
}

static void Win32WarningHandler(const char* module, const char* fmt,
	va_list ap) {
	if (module != nullptr) {
		fprintf(stderr, "%s: ", module);
	}
	fprintf(stderr, "Warning, ");
	vfprintf(stderr, fmt, ap);
	fprintf(stderr, ".\n");
}

#endif /* HAVE_TIFFIO_H */
#endif   // _WIN32

/*
static void SetVariablesFromCLArgs(tesseract::TessBaseAPI* api, int argc,
	char** argv) {
	char opt1[256], opt2[255];
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
			strncpy(opt1, argv[i + 1], 255);
			opt1[255] = '\0';
			char* p = strchr(opt1, '=');
			if (!p) {
				fprintf(stderr, "Missing = in configvar assignment\n");
				exit(EXIT_FAILURE);
			}
			*p = 0;
			strncpy(opt2, strchr(argv[i + 1], '=') + 1, sizeof(opt2) - 1);
			opt2[254] = 0;
			++i;

			if (!api->SetVariable(opt1, opt2)) {
				fprintf(stderr, "Could not set option: %s=%s\n", opt1, opt2);
			}
		}
	}
}
*/
static void PrintLangsList(tesseract::TessBaseAPI* api)
{
#ifndef __WXGTK__
	std::vector<std::string> languages;
#else
	GenericVector<STRING> languages;
#endif
	api->GetAvailableLanguagesAsVector(&languages);
	printf("List of available languages (%d):\n", languages.size());
	for (int index = 0; index < languages.size(); ++index)
	{
#ifndef __WXGTK__
		std::string& lang = languages[index];
#else
		STRING& lang = languages[index];
#endif
		printf("%s\n", lang.c_str());
	}
	api->End();
}


/**
 * We have 2 possible sources of pagesegmode: a config file and
 * the command line. For backwards compatibility reasons, the
 * default in tesseract is tesseract::PSM_SINGLE_BLOCK, but the
 * default for this program is tesseract::PSM_AUTO. We will let
 * the config file take priority, so the command-line default
 * can take priority over the tesseract default, so we use the
 * value from the command line only if the retrieved mode
 * is still tesseract::PSM_SINGLE_BLOCK, indicating no change
 * in any config file. Therefore the only way to force
 * tesseract::PSM_SINGLE_BLOCK is from the command line.
 * It would be simpler if we could set the value before Init,
 * but that doesn't work.
 */
static void FixPageSegMode(tesseract::TessBaseAPI* api,
                           tesseract::PageSegMode pagesegmode)
{
	if (api->GetPageSegMode() == tesseract::PSM_SINGLE_BLOCK)
		api->SetPageSegMode(pagesegmode);
}

static void checkArgValues(int arg, const char* mode, int count)
{
	if (arg >= count || arg < 0)
	{
		printf("Invalid %s value, please enter a number between 0-%d\n", mode, count - 1);
		exit(EXIT_SUCCESS);
	}
}

// NOTE: arg_i is used here to avoid ugly *i so many times in this function
#ifndef __WXGTK__
static void ParseArgs(const int argc, char** argv, const char** lang,
                      const char** image, const char** outputbase,
                      const char** datapath, l_int32* dpi, bool* list_langs,
                      bool* print_parameters, std::vector<std::string>* vars_vec,
                      std::vector<std::string>* vars_values, l_int32* arg_i,
                      tesseract::PageSegMode* pagesegmode,
                      tesseract::OcrEngineMode* enginemode)
#else
static void ParseArgs(const int argc, char** argv, const char** lang,
	const char** image, const char** outputbase,
	const char** datapath, l_int32* dpi, bool* list_langs,
	bool* print_parameters, GenericVector<STRING> * vars_vec,
	GenericVector<STRING> * vars_values, l_int32* arg_i,
	tesseract::PageSegMode* pagesegmode,
	tesseract::OcrEngineMode* enginemode)
#endif
{
	bool noocr = false;
	int i;
	for (i = 1; i < argc && (*outputbase == nullptr || argv[i][0] == '-'); i++)
	{
		if (*image != nullptr && *outputbase == nullptr)
		{
			// outputbase follows image, don't allow options at that position.
			*outputbase = argv[i];
		}
		else if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "--help") == 0))
		{
			//PrintHelpMessage(argv[0]);
			noocr = true;
		}
		else if (strcmp(argv[i], "--help-extra") == 0)
		{
			//PrintHelpExtra(argv[0]);
			noocr = true;
		}
		else if ((strcmp(argv[i], "--help-psm") == 0))
		{
			//PrintHelpForPSM();
			noocr = true;
#ifndef DISABLED_LEGACY_ENGINE
		}
		else if ((strcmp(argv[i], "--help-oem") == 0))
		{
			//PrintHelpForOEM();
			noocr = true;
#endif
		}
		else if ((strcmp(argv[i], "-v") == 0) ||
			(strcmp(argv[i], "--version") == 0))
		{
			//PrintVersionInfo();
			noocr = true;
		}
		else if (strcmp(argv[i], "-l") == 0 && i + 1 < argc)
		{
			*lang = argv[i + 1];
			++i;
		}
		else if (strcmp(argv[i], "--tessdata-dir") == 0 && i + 1 < argc)
		{
			*datapath = argv[i + 1];
			++i;
		}
		else if (strcmp(argv[i], "--dpi") == 0 && i + 1 < argc)
		{
			*dpi = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--user-words") == 0 && i + 1 < argc)
		{
			vars_vec->push_back("user_words_file");
			vars_values->push_back(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--user-patterns") == 0 && i + 1 < argc)
		{
			vars_vec->push_back("user_patterns_file");
			vars_values->push_back(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--list-langs") == 0)
		{
			noocr = true;
			*list_langs = true;
		}
		else if (strcmp(argv[i], "--psm") == 0 && i + 1 < argc)
		{
			checkArgValues(atoi(argv[i + 1]), "PSM", tesseract::PSM_COUNT);
			*pagesegmode = static_cast<tesseract::PageSegMode>(atoi(argv[i + 1]));
			++i;
		}
		else if (strcmp(argv[i], "--oem") == 0 && i + 1 < argc)
		{
#ifndef DISABLED_LEGACY_ENGINE
			int oem = atoi(argv[i + 1]);
			checkArgValues(oem, "OEM", tesseract::OEM_COUNT);
			*enginemode = static_cast<tesseract::OcrEngineMode>(oem);
#endif
			++i;
		}
		else if (strcmp(argv[i], "--print-parameters") == 0)
		{
			noocr = true;
			*print_parameters = true;
		}
		else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc)
		{
			// handled properly after api init
			++i;
		}
		else if (*image == nullptr)
		{
			*image = argv[i];
		}
		else
		{
			// Unexpected argument.
			fprintf(stderr, "Error, unknown command line argument '%s'\n", argv[i]);
			exit(EXIT_FAILURE);
		}
	}

	*arg_i = i;

	if (*pagesegmode == tesseract::PSM_OSD_ONLY)
	{
		// OSD = orientation and script detection.
		if (*lang != nullptr && strcmp(*lang, "osd"))
		{
			// If the user explicitly specifies a language (other than osd)
			// or a script, only orientation can be detected.
			fprintf(stderr, "Warning, detects only orientation with -l %s\n", *lang);
		}
		else
		{
			// That mode requires osd.traineddata to detect orientation and script.
			*lang = "osd";
		}
	}

	if (*outputbase == nullptr && noocr == false)
	{
		//PrintHelpMessage(argv[0]);
		exit(EXIT_FAILURE);
	}
}

static void PreloadRenderers(
	tesseract::TessBaseAPI* api,
	std::vector<tesseract::TessResultRenderer*>* renderers,
	tesseract::PageSegMode pagesegmode, const char* outputbase)
{
	if (pagesegmode == tesseract::PSM_OSD_ONLY)
	{
#ifndef DISABLED_LEGACY_ENGINE
		renderers->push_back(new tesseract::TessOsdRenderer(outputbase));
#endif  // ndef DISABLED_LEGACY_ENGINE
	}
	else
	{
		bool error = false;
		bool b;
		api->GetBoolVariable("tessedit_create_hocr", &b);
		if (b)
		{
			bool font_info;
			api->GetBoolVariable("hocr_font_info", &font_info);
			auto* renderer =
				new tesseract::TessHOcrRenderer(outputbase, font_info);
			if (renderer->happy())
			{
				renderers->push_back(renderer);
			}
			else
			{
				delete renderer;
				printf("Error, could not create hOCR output file: %s\n",
				       strerror(errno));
				error = true;
			}
		}

		api->GetBoolVariable("tessedit_create_alto", &b);
		if (b)
		{
			auto* renderer =
				new tesseract::TessAltoRenderer(outputbase);
			if (renderer->happy())
			{
				renderers->push_back(renderer);
			}
			else
			{
				delete renderer;
				printf("Error, could not create ALTO output file: %s\n",
				       strerror(errno));
				error = true;
			}
		}

		api->GetBoolVariable("tessedit_create_tsv", &b);
		if (b)
		{
			bool font_info;
			api->GetBoolVariable("hocr_font_info", &font_info);
			auto* renderer =
				new tesseract::TessTsvRenderer(outputbase, font_info);
			if (renderer->happy())
			{
				renderers->push_back(renderer);
			}
			else
			{
				delete renderer;
				printf("Error, could not create TSV output file: %s\n",
				       strerror(errno));
				error = true;
			}
		}

		api->GetBoolVariable("tessedit_create_pdf", &b);
		if (b)
		{
#ifdef WIN32
			//if (_setmode(_fileno(stdout), _O_BINARY) == -1)
			//	printf("ERROR: cin to binary: %s", strerror(errno));
#endif  // WIN32
			bool textonly;
			api->GetBoolVariable("textonly_pdf", &textonly);
			auto* renderer =
				new tesseract::TessPDFRenderer(outputbase, api->GetDatapath(),
				                               textonly);
			if (renderer->happy())
			{
				renderers->push_back(renderer);
			}
			else
			{
				delete renderer;
				printf("Error, could not create PDF output file: %s\n",
				       strerror(errno));
				error = true;
			}
		}

		api->GetBoolVariable("tessedit_write_unlv", &b);
		if (b)
		{
			api->SetVariable("unlv_tilde_crunching", "true");
			auto* renderer =
				new tesseract::TessUnlvRenderer(outputbase);
			if (renderer->happy())
			{
				renderers->push_back(renderer);
			}
			else
			{
				delete renderer;
				printf("Error, could not create UNLV output file: %s\n",
				       strerror(errno));
				error = true;
			}
		}

		api->GetBoolVariable("tessedit_create_lstmbox", &b);
		if (b)
		{
			auto* renderer =
				new tesseract::TessLSTMBoxRenderer(outputbase);
			if (renderer->happy())
			{
				renderers->push_back(renderer);
			}
			else
			{
				delete renderer;
				printf("Error, could not create LSTM BOX output file: %s\n",
				       strerror(errno));
				error = true;
			}
		}

		api->GetBoolVariable("tessedit_create_boxfile", &b);
		if (b)
		{
			auto* renderer =
				new tesseract::TessBoxTextRenderer(outputbase);
			if (renderer->happy())
			{
				renderers->push_back(renderer);
			}
			else
			{
				delete renderer;
				printf("Error, could not create BOX output file: %s\n",
				       strerror(errno));
				error = true;
			}
		}

		api->GetBoolVariable("tessedit_create_wordstrbox", &b);
		if (b)
		{
			auto* renderer =
				new tesseract::TessWordStrBoxRenderer(outputbase);
			if (renderer->happy())
			{
				renderers->push_back(renderer);
			}
			else
			{
				delete renderer;
				printf("Error, could not create WordStr BOX output file: %s\n",
				       strerror(errno));
				error = true;
			}
		}

		api->GetBoolVariable("tessedit_create_txt", &b);
		if (b || (!error && renderers->empty()))
		{
			auto* renderer =
				new tesseract::TessTextRenderer(outputbase);
			if (renderer->happy())
			{
				renderers->push_back(renderer);
			}
			else
			{
				delete renderer;
				printf("Error, could not create TXT output file: %s\n",
				       strerror(errno));
			}
		}
	}

	if (!renderers->empty())
	{
		// Since the PointerVector auto-deletes, null-out the renderers that are
		// added to the root, and leave the root in the vector.
		for (int r = 1; r < renderers->size(); ++r)
		{
			(*renderers)[0]->insert((*renderers)[r]);
			(*renderers)[r] = nullptr;
		}
	}
}
#ifndef __WXGTK__
void CExportOcr::monitorProgress(tesseract::ETEXT_DESC* monitor, int page)
#else
void CExportOcr::monitorProgress(ETEXT_DESC* monitor, int page)
#endif
{
	wxProgressDialog dialog("OCR in progress", "Percent : ", 100, nullptr,
	                        wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_AUTO_HIDE);

	while (true)
	{
		if (false == dialog.Update(monitor[page].progress, "Percent : "))
			break;
		if (monitor[page].progress == 100)
			break;
	}
}

#ifndef __WXGTK__
void CExportOcr::ocrProcess(tesseract::TessBaseAPI* api, tesseract::ETEXT_DESC* monitor)
#else
void CExportOcr::ocrProcess(tesseract::TessBaseAPI* api, ETEXT_DESC* monitor)
#endif
{
	api->Recognize(monitor);
}

/**********************************************************************
 *  main()
 *
 **********************************************************************/

int CExportOcr::ExportOcr(
	int argc, char** argv,
	wxString& errorMessage)
{
	const char* lang = nullptr;
	const char* image = nullptr;
	const char* outputbase = nullptr;
	const char* datapath = nullptr;
	int returnValue = EXIT_SUCCESS;
	bool list_langs = false;
	bool print_parameters = false;
	l_int32 dpi = 0;
	int arg_i = 1;
	tesseract::PageSegMode pagesegmode = tesseract::PSM_AUTO;
#ifdef DISABLED_LEGACY_ENGINE
	auto enginemode = tesseract::OEM_LSTM_ONLY;
#else
	tesseract::OcrEngineMode enginemode = tesseract::OEM_DEFAULT;
#endif
	/* main() calls functions like ParseArgs which call exit().
	 * This results in memory leaks if vars_vec and vars_values are
	 * declared as auto variables (destructor is not called then). */
#ifndef __WXGTK__
	static std::vector<std::string> vars_vec;
	static std::vector<std::string> vars_values;
#else
	static GenericVector<STRING> vars_vec;
	static GenericVector<STRING> vars_values;

#endif

#if !defined(DEBUG)
	// Disable debugging and informational messages from Leptonica.
	setMsgSeverity(L_SEVERITY_ERROR);
#endif

#if defined(HAVE_TIFFIO_H) && defined(_WIN32)
	/* Show libtiff errors and warnings on console (not in GUI). */
	TIFFSetErrorHandler(Win32ErrorHandler);
	TIFFSetWarningHandler(Win32WarningHandler);
#endif // HAVE_TIFFIO_H && _WIN32


	ParseArgs(argc, argv, &lang, &image, &outputbase, &datapath, &dpi,
	          &list_langs, &print_parameters, &vars_vec, &vars_values, &arg_i,
	          &pagesegmode, &enginemode);


	if (lang == nullptr)
	{
		// Set default language if none was given.
		lang = "eng";
	}

	if (image == nullptr && !list_langs && !print_parameters)
		return EXIT_SUCCESS;

	// Call GlobalDawgCache here to create the global DawgCache object before
	// the TessBaseAPI object. This fixes the order of destructor calls:
	// first TessBaseAPI must be destructed, DawgCache must be the last object.
	//	tesseract::Dict::GlobalDawgCache();

	// Avoid memory leak caused by auto variable when return is called.
	// Avoid memory leak caused by auto variable when return is called.

	// Avoid memory leak caused by auto variable when exit() is called.
	std::vector<tesseract::TessResultRenderer*> renderers;

	api.SetOutputName(outputbase);


	const int init_failed = api.Init(datapath, lang, enginemode, &(argv[arg_i]),
	                                 argc - arg_i, &vars_vec, &vars_values, false);

	/*
	char * config[1];
	config[0] = new char[255];
	strcpy(config[0], "pdf");
	const int init_failed = api.Init(datapath, lang, enginemode, config, 1, &vars_vec, &vars_values, false);
	*/
	//SetVariablesFromCLArgs(&api, argc, argv);

	// SIMD settings might be overridden by config variable.
	//tesseract::SIMDDetect::Update();

	try
	{
		if (list_langs)
		{
			PrintLangsList(&api);
			api.End();
			return EXIT_SUCCESS;
		}

		if (init_failed)
		{
			errorMessage = "not initialize tesseract.\n";
			throw("error");
		}

		if (print_parameters)
		{
			FILE* fout = stdout;
			fprintf(stdout, "Tesseract parameters:\n");
			api.PrintVariables(fout);
			api.End();
			return EXIT_SUCCESS;
		}

		FixPageSegMode(&api, pagesegmode);

		if (dpi)
		{
			char dpi_string[255];
			snprintf(dpi_string, 254, "%d", dpi);
			api.SetVariable("user_defined_dpi", dpi_string);
		}

		if (pagesegmode == tesseract::PSM_AUTO_ONLY)
		{
			int ret_val = EXIT_SUCCESS;

			Pix* pixs = pixRead(image);
			if (!pixs)
			{
				errorMessage = "Leptonica can't process input file.\n";
				throw("error");
			}

			api.SetImage(pixs);

			tesseract::Orientation orientation;
			tesseract::WritingDirection direction;
			tesseract::TextlineOrder order;
			float deskew_angle;

			const tesseract::PageIterator* it = api.AnalyseLayout();
			if (it)
			{
				// TODO: Implement output of page segmentation, see documentation
				// ("Automatic page segmentation, but no OSD, or OCR").
				it->Orientation(&orientation, &direction, &order, &deskew_angle);
				printf(
					"Orientation: %d\nWritingDirection: %d\nTextlineOrder: %d\n"
					"Deskew angle: %.4f\n",
					orientation, direction, order, deskew_angle);
			}
			else
			{
				throw("error");
			}

			delete it;

			pixDestroy(&pixs);
			api.End();
			return ret_val;
		}

		// set in_training_mode to true when using one of these configs:
		// ambigs.train, box.train, box.train.stderr, linebox, rebox
		bool b = false;
		bool in_training_mode =
			(api.GetBoolVariable("tessedit_ambigs_training", &b) && b) ||
			(api.GetBoolVariable("tessedit_resegment_from_boxes", &b) && b) ||
			(api.GetBoolVariable("tessedit_make_boxes_from_boxes", &b) && b);

#ifdef DISABLED_LEGACY_ENGINE
		auto cur_psm = api->GetPageSegMode();
		auto osd_warning = std::std::string("");
		if (cur_psm == tesseract::PSM_OSD_ONLY) {
			const char* disabled_osd_msg =
				"\nERROR: The page segmentation mode 0 (OSD Only) is currently disabled.\n\n";
			fprintf(stderr, "%s", disabled_osd_msg);
			return EXIT_FAILURE;
		}
		else if (cur_psm == tesseract::PSM_AUTO_OSD) {
			api->SetPageSegMode(tesseract::PSM_AUTO);
			osd_warning +=
				"\nWarning: The page segmentation mode 1 (Auto+OSD) is currently disabled. "
				"Using PSM 3 (Auto) instead.\n\n";
		}
		else if (cur_psm == tesseract::PSM_SPARSE_TEXT_OSD) {
			api->SetPageSegMode(tesseract::PSM_SPARSE_TEXT);
			osd_warning +=
				"\nWarning: The page segmentation mode 12 (Sparse text + OSD) is currently disabled. "
				"Using PSM 11 (Sparse text) instead.\n\n";
		}
#endif  // def DISABLED_LEGACY_ENGINE


		if (in_training_mode)
		{
			renderers.push_back(nullptr);
		}
		else if (outputbase != nullptr)
		{
			PreloadRenderers(&api, &renderers, pagesegmode, outputbase);
		}

		bool banner = false;
		if (outputbase != nullptr && strcmp(outputbase, "-") &&
			strcmp(outputbase, "stdout"))
		{
			banner = true;
		}

		if (!renderers.empty())
		{
			//if (banner)
			//	PrintBanner();
#ifdef DISABLED_LEGACY_ENGINE
			if (!osd_warning.empty()) {
				fprintf(stderr, "%s", osd_warning.c_str());
			}
#endif
			bool succeed = api.ProcessPages(image, nullptr, 0, renderers[0]);
			if (!succeed)
			{
				errorMessage = "Error during processing.\n";
				throw("error");
			}
		}
	}
	catch (...)
	{
		returnValue = EXIT_FAILURE;
	}

	/*
	for (int i = 0;i < renderers.size();i++)
	{
		delete renderers[i];
	}
	*/
	//std::vector<tesseract::TessResultRenderer> renderers;


	api.End();

	return returnValue;
}
