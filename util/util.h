#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include "opencv2/core.hpp"

class Util
{
public:
    static void maxDistIdx(const std::vector<double>& values, int* index);
    static void thresholdMinMax(cv::InputArray img, cv::OutputArray bin, double minVal, double maxVal);

    static uchar getMainBgGrayColor(cv::Mat ucharGray);

    static void horizonBinProjection(const cv::Mat& src, cv::Mat& dst);
    static void verticalBinProjection(const cv::Mat& src, cv::Mat& dst);

    /*
     * return mat.ncols is gray.nrow
     * return mat.row1 is main bgcolor, row2 is second bgcolor
     */
    static cv::Mat getHoriProjectionBgColor(cv::Mat gray);

    static void removeHoriProjectionBg(cv::Mat gray, cv::Mat bgColor);

    static void getCellRectWithoutBorder(
            const cv::Mat& bin,
            float horiBorderMinRatio, float vertBorderMinRatio,
            float cellMinWidthRatio, float cellMinHeightRatio,
            std::vector<std::vector<cv::Rect> >& cellRects);
    static std::vector<cv::Point2i> getHoriCellSegments(const cv::Mat& horiProjection, float horiBorderMinRatio, float cellMinHeightRatio);
    static std::vector<cv::Point2i> getVertCellSegments(const cv::Mat& vertProjection, float vertBorderMinRatio, float cellMinWidthRatio);

    static void rotateClockWise90(cv::Mat &src);
};

#endif // UTIL_H
