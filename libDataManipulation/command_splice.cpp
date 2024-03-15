#include <header.h>
#include "command_splice.h"
#include "string_conversion.h"
#include "splice_core.h"

#include <fstream>
#include <sstream>

using namespace boost::program_options;

namespace vole { namespace cmfd_gt {

CommandSplice::CommandSplice()
	: Command(
		"splice",
		config,
		"Christian Riess",
		"christian.riess@informatik.uni-erlangen.de")
{
}

int CommandSplice::execute()
{
	if (config.orig_file.length() < 1) { std::cerr << "Add original image (--orig)!" << std::endl; printHelp(); return 1; }
	if (config.snippet_file.length() < 1) { std::cerr << "Add tampering snippet (--snippet)!" << std::endl; printHelp(); return 1; }
	if (config.snippet_alpha_file.length() < 1) { std::cerr << "Add tampered snippet alpha channel (--alpha_snippet)!" << std::endl; printHelp(); return 1; }

	if (config.orig_file.length() < 1) { std::cerr << "Add original image (--orig)!" << std::endl; printHelp(); return 1; }
	if (config.file_prefix.length() < 1) { std::cout << "WARNING: Add file prefix (--file_prefix) to better distinguish files!" << std::endl; }
	if (config.source_positions.length() < 1) { std::cout << "Add source positions (--source_positions)!" << std::endl; return 1; }
	if (config.insert_positions.length() < 1) { std::cout << "Add insert positions (--insert_positions)!" << std::endl; return 1; }

	cv::Mat_<cv::Vec3b> orig = cv::imread(config.orig_file, cv::IMREAD_COLOR);
    if (orig.empty()) { std::cerr << "error loading image " << config.orig_file << std::endl; return 1; }

	// expand file lists
	std::vector<std::string> snippet_files = vole::StringConversion::stringToList(config.snippet_file, ',');
	std::vector<std::string> alpha_snippet_files = vole::StringConversion::stringToList(config.snippet_alpha_file, ',');
	std::vector<int> source_positions = vole::StringConversion::stringToIntList(config.source_positions, ',');
	std::vector<int> insert_positions = vole::StringConversion::stringToIntList(config.insert_positions, ',');

	if (snippet_files.size() != alpha_snippet_files.size()) {
		std::cerr << "error: different number snippet_files (" << snippet_files.size() << ") than alpha channels ("
			<< alpha_snippet_files.size() << "), aborted." << std::endl;
		return 1;
	}
	if (snippet_files.size()*2 != source_positions.size()) {
		std::cerr << "error: positions (" << source_positions.size() << " coordinates) do not match number of snippet_files ("
			<< snippet_files.size() << "), aborted." << std::endl;
		return 1;
	}
	if (snippet_files.size()*2 != insert_positions.size()) {
		std::cerr << "error: positions (" << insert_positions.size() << " coordinates) do not match number of snippet_files ("
			<< snippet_files.size() << "), aborted." << std::endl;
		return 1;
	}
	
	// read all the data
	std::vector<cv::Mat_<cv::Vec3b> > snippets;
	std::vector<cv::Mat_<unsigned char> > alpha_snippets;
	for (unsigned int i = 0; i < snippet_files.size(); ++i) {
		// check snippets
		cv::Mat_<cv::Vec3b> snippet  = cv::imread(snippet_files[i], cv::IMREAD_COLOR);
		cv::Mat_<unsigned char> alpha_snippet = cv::imread(alpha_snippet_files[i], cv::IMREAD_GRAYSCALE);

		if (snippet.empty()) {
			std::cerr << "error loading snippet " << i << ": " << config.snippet_file << ", aborted." << std::endl;
			return 1;
		}
		if (alpha_snippet.empty()) {
			std::cerr << "error loading snippet alpha " << i << ": " << config.snippet_alpha_file << ", aborted." << std::endl;
			return 1;
		}
		if ((snippet.cols != alpha_snippet.cols) || (snippet.rows != alpha_snippet.rows)) {
			std::cerr << "image dimensions of snippet " << snippet_files[i] << " and " << alpha_snippet_files[i]
				<< " do not match, aborted." << std::endl;
			return 1;
		}
		snippets.push_back(snippet);
		alpha_snippets.push_back(alpha_snippet);
	}

	SpliceCore splicing(orig, snippets, alpha_snippets, source_positions, insert_positions, 200 /* FIXME make configurable*/);

	if (config.local_noise) {
		for (int i = 0; i < (int) snippets.size(); ++i) {
			splicing.add_snippet_noise(i, config.local_noise_std_dev/1000.0);
		}
	}

	if (config.rot) {
		for (int i = 0; i < (int) snippets.size(); ++i) {
			splicing.add_rotation(i, config.angle, false);
		}
	}

	if (config.scale) {
		for (int i = 0; i < (int) snippets.size(); ++i) {
			splicing.add_scaling(i, config.scaling / 1000.0, false);
		}
	}

	splicing.splice();

	if (config.jpeg) {
		splicing.add_global_jpeg(config.quality);
	}
	
	if (config.global_noise) {
		splicing.add_global_noise(config.global_noise_std_dev/1000.0);
	}

	cv::Mat_<cv::Vec3b> tampered = splicing.getTamperedImage();
	cv::Mat_<unsigned char> ground_truth = splicing.getAlphaImage();

	std::pair<std::string, std::string> output_files = get_output_file_names();
	cv::imwrite(output_files.first, tampered);
	cv::imwrite(output_files.second, ground_truth);

	cv::Mat_<cv::Vec3b> sanity;
	tampered.copyTo(sanity);
	for (int y = 0; y < sanity.rows; ++y) {
		for (int x = 0; x < sanity.cols; ++x) {
			sanity[y][x][2] = ground_truth[y][x];
		}
	}
	cv::imwrite("/tmp/sanity.png", sanity);

	if (config.verbosity > 0) {
		std::cout << "wrote files." << std::endl
			<< "copy: " << output_files.first << std::endl
			<< "gt:   " << output_files.second << std::endl
			<< "sanity:   " << "/tmp/sanity.png" << std::endl;
	}
	return 0;
}

std::pair<std::string, std::string> CommandSplice::get_output_file_names() {
	std::stringstream tampered, gt;
	tampered << config.output_directory << "/";
	gt << config.output_directory << "/";
	if (config.file_prefix.length() > 0) {
		tampered << config.file_prefix << "_copy";
		gt << config.file_prefix << "_gt";
	} else {
		tampered << "copy";
		gt << "gt";
	}
	if (config.local_noise) {
		tampered << "_ln" << config.local_noise_std_dev;
		gt << "_ln" << config.local_noise_std_dev;
	}

	if (config.rot) {
		tampered << "_r" << config.angle;
		gt << "_r" << config.angle;
	}

	if (config.scale) {
		tampered << "_s" << config.scaling;
		gt << "_s" << config.scaling;
	}

	if (config.jpeg) {
		tampered << "_j" << config.quality;
		gt << "_j" << config.quality;
	}
	
	if (config.global_noise) {
		tampered << "_gn" << config.global_noise_std_dev;
		gt << "_gn" << config.global_noise_std_dev;
	}

	tampered << ".png";
	gt << ".png";

	return std::pair<std::string, std::string>(tampered.str(), gt.str());
}

void CommandSplice::printHelp() const {
	std::cout << "splices images, creates ground truth automatically" << std::endl;
	std::cout << std::endl;
	std::cout << "This command combines an input image and a number of input" << std::endl;
	std::cout << "snippets to a forgery. Optionally, noise, JPEG compression" << std::endl;
	std::cout << "artifacts, rotation and scaling can be applied. Noise and" << std::endl;
	std::cout << "JPEG artifacts can be applied only to the snippets, or to" << std::endl;
	std::cout << "the whole combined image (or to both). Note that all" << std::endl;
	std::cout << "transformations must currently be the same for all inserted" << std::endl;
	std::cout << "snippets. However, this can be changed in a straightforward" << std::endl;
	std::cout << "manner by extending the parameter set to per-snippet parameters." << std::endl;
}

void CommandSplice::printShortHelp() const {
	std::cout << "splices images, creates ground truth automatically" << std::endl;
}

} } // end namespace vole, cmfd_gt
