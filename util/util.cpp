#include "util.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <QDebug>
#include <iostream>

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

uchar Util::getMainBgGrayColor(Mat ucharGray)
{
    // 获取灰度直方图
    int histSize[] = {256};
    float hranges[] = { 0, 255};
    const float* ranges[] = { hranges };
    MatND hist;
    int channels[] = {0};

    calcHist( &ucharGray, 1, channels, Mat(), // do not use mask
             hist, 1, histSize, ranges);

    //waitKey(0);

    double maxVal=0;
    int maxIdx=-1;
    minMaxIdx(hist, 0, &maxVal, 0, &maxIdx);

    qDebug() << "maxIdx=" << maxIdx << ", maxVal=" << maxVal;
    return maxIdx;
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

Mat Util::getHoriProjectionBgColor(Mat gray)
{
    int channels = gray.channels();
    Q_ASSERT(channels == 1);
    int nRows = gray.rows;
    int nCols = gray.cols;

    Mat ret(Size(2, nRows), CV_8UC1);

    uchar *p;
    for( int i = 0; i < nRows; ++i )
    {
        int count[256] = {0};
        int maxIdx = -1, maxIdx2 = -1;
        int maxVal = -1, maxVal2 = -1;
        p = gray.ptr<uchar>(i);
        for(int j=0; j < nCols; ++j )
        {
            uchar c = p[j];
            int val = ++count[c];
            if (val > maxVal)
            {
                if (c != maxIdx)
                {
                    maxVal2 = maxVal;
                    maxIdx2 = maxIdx;
                    maxIdx = c;
                }
                maxVal = val;
            }
            else if (val > maxVal2)
            {
                maxVal2 = val;
                maxIdx2 = c;
            }
        }
        ret.ptr<uchar>(i)[0] = maxIdx;
        ret.ptr<uchar>(i)[1] = maxIdx2;
        qDebug() << "row=" << i << " c=" << maxIdx << " " << maxIdx2;
    }

    return ret;
}

void Util::removeHoriProjectionBg(Mat gray, Mat bgColor)
{
    int channels = gray.channels();
    Q_ASSERT(channels == 1);
    int nRows = gray.rows;
    int nCols = gray.cols;

    uchar *p;
    for( int i = 0; i < nRows; ++i )
    {
        p = gray.ptr<uchar>(i);
        uchar bg = bgColor.ptr<uchar>(i)[0];
        uchar bg2 = bgColor.ptr<uchar>(i)[1];
        for(int j=0; j < nCols; ++j )
        {
            uchar c = p[j];
            p[j] = c == bg || c == bg2 ? 0 : 255;
        }
    }
}

void Util::getCellRectWithoutBorder(const Mat &bin, float horiBorderMinRatio, float vertBorderMinRatio, float cellMinWidthRatio, float cellMinHeightRatio, std::vector<std::vector<Rect> > &cellRects)
{
    // find table border
    Mat horiProjection, vertProjection;
    horizonBinProjection(bin, horiProjection);
    verticalBinProjection(bin, vertProjection);

    //imshow("horiBorder", horiProjection);
    //imshow("vertBorder", vertProjection);

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

// http://blog.csdn.net/tianzhaixing2013/article/details/53037474
void Util::rotateClockWise90(Mat &src)
{
    if (src.empty()) return;
    transpose(src, src);
    //0: 沿X轴翻转； >0: 沿Y轴翻转； <0: 沿X轴和Y轴翻转
    flip(src, src, 1);
}


