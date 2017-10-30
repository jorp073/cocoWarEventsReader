#ifndef SAMPLE_H
#define SAMPLE_H

#include "opencv2/core.hpp"

class Sample
{
public:
    Sample();
    static Sample& instance();

    double compareFlagContour(std::vector<cv::Point> contour);

private:
    std::vector<cv::Point> _flagContour;
    void initFlagContour();
};

#endif // SAMPLE_H
