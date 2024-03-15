#pragma once

/** Container for matches with low error between the snippet and the image (in
 * order to find the snippet positions in the image). The class stores the six
 * best positions in a (primitive) priority queue, such that the best match is
 * always at index 0. */
class GoodMatch {
public:

	GoodMatch(double initialWorst = DBL_MAX) :
		worstPossibleScore(initialWorst), worstScore(initialWorst),
		sixBestScores(6, initialWorst), sixBestPositions(6, cv::Point(-1, -1)) {}
		
	inline double getWorstPossibleScore() { return worstPossibleScore; }
	inline double getWorstScore() { return worstScore; }

	inline std::vector<double> &bestScores() { return sixBestScores; }
	inline std::vector<cv::Point> &bestPositions() { return sixBestPositions; }
	
	inline bool valid() { return sixBestScores[0] < worstPossibleScore; }
	
	inline friend std::ostream& operator<<(std::ostream& o, const GoodMatch& m) {
		for (unsigned int i = 0; i < m.sixBestScores.size(); ++i)
			o << "Score #" << i+1 << ": " << m.sixBestScores[i]
			  << " @ (" << m.sixBestPositions[i].x
			  <<  ","  << m.sixBestPositions[i].y << ")" << std::endl;
		return o;
	}

	void insertScore(double score, cv::Point pos) {
		for (int i = 0; i < 6; ++i) {
			if (sixBestScores[i] > score) {
				// shift all scores behind
				for (int j = 5; j > i; --j) {
					sixBestScores[j] = sixBestScores[j-1];
					sixBestPositions[j] = sixBestPositions[j-1];
				}
				// insert new score
				sixBestScores[i] = score;
				sixBestPositions[i] = pos;
				worstScore = sixBestScores[5];
				break;
			}
		}
	}

protected:
	double worstPossibleScore, worstScore;
	std::vector<double> sixBestScores;
	std::vector<cv::Point> sixBestPositions;
};

