#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include "opencv2/core.hpp"

class Util
{
public:
    static void maxDistIdx(const std::vector<double>& values, int* index);
    static void thresholdMinMax(cv::InputArray img, cv::OutputArray bin, double minVal, double maxVal);

    static void horizonBinProjection(const cv::Mat& src, cv::Mat& dst);
    static void verticalBinProjection(const cv::Mat& src, cv::Mat& dst);

    static void getCellRectWithoutBorder(
            const cv::Mat& bin,
            float horiBorderMinRatio, float vertBorderMinRatio,
            float cellMinWidthRatio, float cellMinHeightRatio,
            std::vector<std::vector<cv::Rect> >& cellRects);
    static std::vector<cv::Point2i> getHoriCellSegments(const cv::Mat& horiProjection, float horiBorderMinRatio, float cellMinHeightRatio);
    static std::vector<cv::Point2i> getVertCellSegments(const cv::Mat& vertProjection, float vertBorderMinRatio, float cellMinWidthRatio);
};

#endif // UTIL_H
