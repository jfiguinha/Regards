#pragma once
/*
OpenCL Interpolation Performance Validation Module

Add this to: libOpenCL/OpenCLEffectVideo.h or create new file
*/

#include <chrono>
#include <vector>
#include <cstring>
#include <cstdio>

namespace Regards::OpenCL {

    class InterpolationProfiler {
    public:
        struct FrameStats {
            int width, height;
            int filterType;
            int angle;
            double processingTimeMs;
            bool optimized;
        };

        static InterpolationProfiler& getInstance() {
            static InterpolationProfiler instance;
            return instance;
        }

        // Call before interpolation
        void StartProfiling(int w, int h, int filter, int angle, bool opt) {
            currentStats.width = w;
            currentStats.height = h;
            currentStats.filterType = filter;
            currentStats.angle = angle;
            currentStats.optimized = opt;
            startTime = std::chrono::high_resolution_clock::now();
        }

        // Call after interpolation
        void EndProfiling() {
            auto endTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
            currentStats.processingTimeMs = duration.count();
            frameStats.push_back(currentStats);

            // Auto print after every 30 frames
            if (frameStats.size() % 30 == 0) {
                PrintStats();
            }
        }

        void PrintStats() const {
            if (frameStats.empty()) return;

            printf("\n========== OpenCL Interpolation Stats ==========\n");
            printf("Total Frames: %zu\n", frameStats.size());

            double totalMs = 0.0;
            double minMs = 999999.0;
            double maxMs = 0.0;

            for (const auto& stat : frameStats) {
                totalMs += stat.processingTimeMs;
                minMs = (std::min)(minMs, stat.processingTimeMs);
                maxMs = (std::max)(maxMs, stat.processingTimeMs);
            }

            double avgMs = totalMs / frameStats.size();
            printf("Average Time: %.2f ms\n", avgMs);
            printf("Min Time:     %.2f ms\n", minMs);
            printf("Max Time:     %.2f ms\n", maxMs);

            // FPS calculation
            double avgFps = 1000.0 / avgMs;
            printf("Avg FPS:      %.1f\n", avgFps);

            // Last frame info
            const auto& last = frameStats.back();
            printf("\nLast Frame:\n");
            printf("  Resolution: %dx%d\n", last.width, last.height);
            printf("  Filter: %d, Angle: %d°\n", last.filterType, last.angle);
            printf("  Time: %.2f ms %s\n", last.processingTimeMs, last.optimized ? "[OPTIMIZED]" : "[ORIGINAL]");

            printf("================================================\n\n");
        }

        void ClearStats() {
            frameStats.clear();
        }

    private:
        std::vector<FrameStats> frameStats;
        FrameStats currentStats;
        std::chrono::high_resolution_clock::time_point startTime;
    };

    // Validation function to compare original vs optimized results
    class InterpolationValidator {
    public:
        struct ValidationResult {
            bool pixelsPerfectMatch;
            int totalPixels;
            int differentPixels;
            int maxChannelDiff;
            double errorPercentage;
        };

        static ValidationResult ValidateInterpolation(
            const uint* originalOutput,
            const uint* optimizedOutput,
            int width, int height,
            int pixelTolerance = 1  // Allow ±1 per channel for floating point rounding
        ) {
            ValidationResult result{};
            result.totalPixels = width * height;

            int diffPixels = 0;
            int maxDiff = 0;

            for (int i = 0; i < result.totalPixels; i++) {
                uint orig = originalOutput[i];
                uint opt = optimizedOutput[i];

                if (orig != opt) {
                    // Check channel differences
                    int r_diff = abs((int)(orig & 0xFF) - (int)(opt & 0xFF));
                    int g_diff = abs((int)((orig >> 8) & 0xFF) - (int)((opt >> 8) & 0xFF));
                    int b_diff = abs((int)((orig >> 16) & 0xFF) - (int)((opt >> 16) & 0xFF));
                    int a_diff = abs((int)((orig >> 24) & 0xFF) - (int)((opt >> 24) & 0xFF));

                    int channelMax = (std::max)({ r_diff, g_diff, b_diff, a_diff });
                    maxDiff = (std::max)(maxDiff, channelMax);

                    if (channelMax > pixelTolerance) {
                        diffPixels++;
                    }
                }
            }

            result.pixelsPerfectMatch = (diffPixels == 0);
            result.differentPixels = diffPixels;
            result.maxChannelDiff = maxDiff;
            result.errorPercentage = (double)diffPixels / result.totalPixels * 100.0;

            return result;
        }

        static void PrintValidationResult(const ValidationResult& result) {
            printf("\n========== Interpolation Validation ==========\n");
            printf("Total Pixels:      %d\n", result.totalPixels);
            printf("Pixels Matching:   %s\n", result.pixelsPerfectMatch ? "YES ✓" : "NO ✗");
            printf("Different Pixels:  %d (%.4f%%)\n", result.differentPixels, result.errorPercentage);
            printf("Max Channel Diff:  %d\n", result.maxChannelDiff);

            if (result.pixelsPerfectMatch) {
                printf("\n✓ VALIDATION PASSED - Results are identical\n");
            } else if (result.errorPercentage < 0.01) {
                printf("\n✓ VALIDATION PASSED - Floating point rounding acceptable\n");
            } else {
                printf("\n✗ VALIDATION FAILED - Significant difference detected\n");
            }
            printf("=============================================\n\n");
        }
    };

} // namespace Regards::OpenCL

/*
USAGE:

// In COpenCLEffectVideo::InterpolationZoomBicubic()

auto& profiler = Regards::OpenCL::InterpolationProfiler::getInstance();
profiler.StartProfiling(widthOut, heightOut, type, angle, true); // true = optimized

// ... run interpolation kernel ...

profiler.EndProfiling();

// Validation (for testing)
if (enableValidation) {
    auto result = Regards::OpenCL::InterpolationValidator::ValidateInterpolation(
        originalBuffer, optimizedBuffer, widthOut, heightOut
    );
    Regards::OpenCL::InterpolationValidator::PrintValidationResult(result);
}

// Print accumulated stats every 30 frames automatically
// Or manually call:
profiler.PrintStats();
*/
