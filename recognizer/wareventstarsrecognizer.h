#ifndef WAREVENTSTARSRECOGNIZER_H
#define WAREVENTSTARSRECOGNIZER_H

#include "opencv2/core.hpp"
#include "define.h"


class WarEventStarsRecognizer
{
public:
    WarEventStarsRecognizer();

    bool recognize(cv::Mat grayBar, cv::Mat binBar, cv::Rect starsRect, int* stars);

private:

    bool isStarColor(uchar color);

    bool isRectStar(cv::Mat grayBar, cv::Mat binBar, cv::Rect starRect);
};

#endif // WAREVENTSTARSRECOGNIZER_H
