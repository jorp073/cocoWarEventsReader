#ifndef WAREVENTSTARSRECOGNIZER_H
#define WAREVENTSTARSRECOGNIZER_H

#include "opencv2/core.hpp"
#include "define.h"


class WarEventStarsRecognizer
{
public:
    WarEventStarsRecognizer();

    bool recognize(cv::Mat grayImg, int* stars);

private:

    bool isStarColor(uchar color);
};

#endif // WAREVENTSTARSRECOGNIZER_H
