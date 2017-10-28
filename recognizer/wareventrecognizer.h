#ifndef WAREVENTRECOGNIZER_H
#define WAREVENTRECOGNIZER_H

#include "opencv2/core.hpp"
#include "define.h"

class WarEventRecognizer
{
public:
    WarEventRecognizer();

    bool getWarEventData(cv::Mat img);

private:
    void findWarEventBars(cv::Mat img, OUT std::vector<cv::Mat>& bars);

    friend class Test;
};

#endif // WAREVENTRECOGNIZER_H
