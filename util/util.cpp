#include "util.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <QDebug>

using namespace cv;


void Util::maxDistIdx(const std::vector<double>& values, int *index)
{
    int count = values.size();
    std::vector<double> distances;
    for (int i=0; i<count; i++) {
        double sum = 0;
        for (int j=0; j<count; j++) {
            double dist = values[i] - values[j];
            sum += dist*dist;
        }

        distances.push_back(sum);
    }

    double maxVal = 0;
    int maxIdx;
    minMaxIdx(Mat(distances), 0, &maxVal, 0, &maxIdx);

    *index = maxIdx;
}


void Util::thresholdMinMax(cv::InputArray img, cv::OutputArray bin, double minVal, double maxVal)
{
    threshold(img, bin, maxVal, 0, CV_THRESH_TRUNC);
    threshold(bin, bin, minVal, 255, CV_THRESH_BINARY);
}

void Util::horizonBinProjection(const Mat& src, Mat& dst)
{
    // accept only char type matrices
    CV_Assert(src.depth() != sizeof(uchar));

    dst.create(src.rows, 1, CV_32F);

    int i, j;
    const uchar* p;
    float* p_dst;
    for(i = 0; i < src.rows; i++){
        p = src.ptr<uchar>(i);
        p_dst = dst.ptr<float>(i);
        p_dst[0] = 0;
        for(j = 0; j < src.cols; j++){
            if (p[j]==255) p_dst[0]++;
        }
        p_dst[0] /= src.cols;
    }
}

void Util::verticalBinProjection(const Mat& src, Mat& dst)
{
    // accept only char type matrices
    CV_Assert(src.depth() != sizeof(uchar));

    dst.create(1, src.cols, CV_32F);

    int i, j;
    const uchar* p;
    float* p_dst = dst.ptr<float>(0);
    for(j = 0; j < src.cols; j++){
        p_dst[j] = 0;
        for(i = 0; i < src.rows; i++){
            p = src.ptr<uchar>(i);
            if (p[j]==255) p_dst[j]++;
        }
        p_dst[j]  /= src.rows;
    }
}

void Util::getCellRectWithoutBorder(const Mat &bin, float horiBorderMinRatio, float vertBorderMinRatio, float cellMinWidthRatio, float cellMinHeightRatio, std::vector<std::vector<Rect> > &cellRects)
{
    // find table border
    Mat horiProjection, vertProjection;
    horizonBinProjection(bin, horiProjection);
    verticalBinProjection(bin, vertProjection);

    imshow("horiBorder", horiProjection);
    imshow("vertBorder", vertProjection);

    auto horiCellSegments = getHoriCellSegments(horiProjection, horiBorderMinRatio, cellMinHeightRatio);
    auto vertCellSegments = getVertCellSegments(vertProjection, vertBorderMinRatio, cellMinWidthRatio);

    for (size_t i=0; i<horiCellSegments.size(); i++) {
        std::vector<Rect> rects;
        for (size_t j=0; j<vertCellSegments.size(); j++) {
            rects.push_back(Rect(vertCellSegments[j].x, horiCellSegments[i].x, vertCellSegments[j].y, horiCellSegments[i].y));
        }
        cellRects.push_back(rects);
    }
}

std::vector<cv::Point2i> Util::getHoriCellSegments(const cv::Mat& horiProjection, float horiBorderMinRatio, float cellMinHeightRatio)
{
    bool isLastBorder = false;
    int segmentStart = -1;
    std::vector<Point2i> segments;
    for (int i=0; i<horiProjection.rows; i++) {
        float d = horiProjection.ptr<float>(i)[0];
        bool isBorder = d > horiBorderMinRatio;
        if (isBorder) {
            if (isLastBorder) {
                continue;
            } else {
                // segment finish
                if (i > 0 && segmentStart > -1) {
                    int height = i-segmentStart;
                    if (height > cellMinHeightRatio*horiProjection.rows) {
                        segments.push_back(Point2i(segmentStart, height));
                    }
                }
                isLastBorder = true;
            }
        } else {
            if (isLastBorder) {
                // segment start
                segmentStart = i;
                isLastBorder = false;
            } else {
                continue;
            }
        }
    }

    return segments;
}

std::vector<cv::Point2i> Util::getVertCellSegments(const cv::Mat& vertProjection, float vertBorderMinRatio, float cellMinWidthRatio)
{
    bool isLastBorder = false;
    int segmentStart = -1;
    std::vector<Point2i> segments;
    for (int i=0; i<vertProjection.cols; i++) {
        float vd = vertProjection.ptr<float>(0)[i];
        bool isBorder = vd > vertBorderMinRatio;
        if (isBorder) {
            if (isLastBorder) {
                continue;
            } else {
                // segment finish
                if (i > 0 && segmentStart > -1) {
                    int width = i-segmentStart;
                    if (width > vertProjection.cols * cellMinWidthRatio) {
                        segments.push_back(Point2i(segmentStart, width));
                    }
                }
                isLastBorder = true;
            }
        } else {
            if (isLastBorder) {
                // segment start
                segmentStart = i;
                isLastBorder = false;
            } else {
                continue;
            }
        }
    }

    return segments;
}


