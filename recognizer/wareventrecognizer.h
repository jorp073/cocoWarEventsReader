#ifndef WAREVENTRECOGNIZER_H
#define WAREVENTRECOGNIZER_H

#include "opencv2/core.hpp"
#include "define.h"

class WarEventRecognizer
{
public:
    WarEventRecognizer();

    bool recognize(cv::Mat dialog);

private:
    void findBars(cv::Mat img, OUT std::vector<cv::Mat>& bars);

    friend class Test;
};

#endif // WAREVENTRECOGNIZER_H
