#ifndef WAREVENTBARRECOGNIZER_H
#define WAREVENTBARRECOGNIZER_H

#include "opencv2/core.hpp"
#include "define.h"

class WarEventBarRecognizer
{
public:
    WarEventBarRecognizer();

    void recognize(cv::Mat bar);

private:
    bool isAttack(cv::Mat bar);

    int scanBarBottomHeight(cv::Mat grayBar);

    void clearBackground(cv::Mat bar, cv::Mat grayBar, cv::OutputArray binBar);

    void clearFlags(cv::Mat& binbar);

    friend class Test;
};

#endif // WAREVENTBARRECOGNIZER_H
