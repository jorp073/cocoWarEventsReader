#ifndef WAREVENTRECOGNIZER_H
#define WAREVENTRECOGNIZER_H

#include "opencv2/core.hpp"
#include "define.h"

class WarEventRecognizer
{
public:
    WarEventRecognizer();

    bool getData(cv::Mat img);

private:
    void findBars(cv::Mat img, OUT std::vector<cv::Mat>& bars);

    bool isAttack(cv::Mat bar);

    bool parseBar(cv::Mat bar);

    friend class Test;
};

#endif // WAREVENTRECOGNIZER_H
