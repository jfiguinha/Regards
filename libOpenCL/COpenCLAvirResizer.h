/**
 * @file COpenCLAvirResizer.h
 *
 * Brique OpenCL autonome (non encore integree a Regards) portant le coeur
 * de redimensionnement de la librairie AVIR (avir.h / avir_dil.h) sur GPU.
 *
 * PORTEE ET LIMITES ASSUMEES PAR RAPPORT A avir.h :
 *
 *  - Pipeline a un seul etage de convolution separable (H puis V), avec un
 *    filtre sinc fenetre "Peaked Cosine" identique en formule a celui
 *    d'AVIR (CDSPWindowGenPeakedCosine / CDSPPeakedCosineLPF), mais calcule
 *    EXACTEMENT pour chaque pixel de sortie cote CPU, sans passer par la
 *    table compressee + interpolation d'AVIR (CDSPFracFilterBankLin) :
 *    cette compression n'a d'interet que pour la boucle chaude CPU
 *    d'origine, elle est inutile ici puisque le calcul ne tourne qu'une
 *    seule fois par redimensionnement, avant l'envoi au GPU.
 *  - Pas de construction adaptative multi-etapes : pas de pre-filtre
 *    demi-bande pour les forts ratios de reduction, pas de doResize2().
 *    Un seul filtre est utilise, dont la longueur augmente avec le ratio
 *    de reduction pour eviter le repliement.
 *  - Le filtre de "nettete" n'est PAS un portage bit-exact du filtre de
 *    correction d'AVIR (coefficients issus d'une optimisation numerique
 *    empirique propre a son implementation). C'est un filtre unsharp-mask
 *    generique sinc-fenetre, intensite reglable via 'sharpenAmount'.
 *  - Le dithering par diffusion d'erreur reprend les memes poids qu'AVIR
 *    (0.364842 / 0.207305 / 0.063011) mais tourne dans un seul work-item
 *    OpenCL (sequentiel par nature, comme cote CPU : cf. isRecursive()
 *    dans avir.h).
 *
 * Format image : RGBA float32 (4 floats contigus par pixel), en memoire
 * lineaire ligne par ligne. La conversion depuis/vers un format 8 bits ou
 * autre est a la charge de l'appelant.
 *
 * Cette classe utilise l'API OpenCL C brute (pas de dependance a
 * COpenCLContext ni au reste de Regards) pour rester testable seule ;
 * l'integration (contexte/queue partages, format de buffer image de
 * Regards, etc.) sera a faire dans un second temps.
 */

#pragma once

#include <vector>
#include <cstdint>

#ifdef __APPLE__
	#include <OpenCL/opencl.h>
#else
	#include <CL/cl.h>
#endif

namespace Regards
{
	namespace OpenCL
	{
		class COpenCLContext;

		struct SAvirResizeParams
		{
			double alpha = 4.76449;        ///< Parametre Alpha de la fenetre Peaked Cosine (cf. AVIR LPFltAlpha).
			double baseTaps = 7.56;        ///< Largeur totale de base du filtre en 1:1 (cf. AVIR LPFltBaseLen).
			double cutoffMult = 0.79285;   ///< Multiplicateur de frequence de coupure (cf. AVIR LPFltCutoffMult).

			bool  linearizeGamma = true;   ///< Linearise (sRGB->lineaire) avant, re-gamma apres.

			bool  sharpen = false;         ///< Applique le filtre de nettete (seulement si agrandissement).
			float sharpenAmount = 0.35f;

			bool  dither = false;
			bool  ditherErrorDiffusion = true; ///< false = arrondi simple (parallele, plus rapide).

			float peakValue = 1.0f;        ///< Valeur max apres clamp (1.0 pour du float normalise 0..1).
		};

		class COpenCLAvirResizer
		{
		public:
			COpenCLAvirResizer(COpenCLContext* openCLContext);
			~COpenCLAvirResizer();

			COpenCLAvirResizer(const COpenCLAvirResizer&) = delete;
			COpenCLAvirResizer& operator=(const COpenCLAvirResizer&) = delete;

			/// Compile avir_opencl_kernels.cl et cree les kernels. A appeler une fois.
			bool Init();

			/// srcRGBA : buffer hote, srcWidth*srcHeight*4 floats (RGBA).
			/// dstRGBA : buffer hote DEJA ALLOUE, dstWidth*dstHeight*4 floats.
			bool Resize(
				const cv::UMat& src,
				cv::UMat& dest,
				const SAvirResizeParams& params);

			bool IsInit()
			{
				return isInit;
			}

			bool IsOk()
			{
				return isOk;
			}

		private:

			struct SResizeTap
			{
				cl_int srcStart;
				cl_int tapCount;
				cl_int coefOffset;
			};

			bool isOk = false;
			bool isInit = false;
			COpenCLContext* openCLContext = nullptr;
			cv::ocl::Program m_program;
			cl_kernel m_kLinearize = nullptr;
			cl_kernel m_kDelinearize = nullptr;
			cl_kernel m_kResizeH = nullptr;
			cl_kernel m_kResizeV = nullptr;
			cl_kernel m_kSharpenH = nullptr;
			cl_kernel m_kSharpenV = nullptr;
			cl_kernel m_kDitherRound = nullptr;
			cl_kernel m_kDitherErrDiff = nullptr;

			/// Construit le banc de filtres (un par pixel de sortie) pour un axe.
			void BuildAxisFilters(
				int srcLen, int dstLen,
				const SAvirResizeParams& params,
				std::vector<SResizeTap>& outTaps,
				std::vector<float>& outCoefs) const;

			/// Construit un petit noyau passe-bas symetrique (klen impair) utilise
			/// pour l'unsharp mask.
			std::vector<float> BuildSharpenKernel(int klen) const;

			static double PeakedCosineWindow(double x, double len2, double alpha);

			cl_kernel CreateKernel(const char* name);
			void ReleaseKernels();
		};
	}
}
