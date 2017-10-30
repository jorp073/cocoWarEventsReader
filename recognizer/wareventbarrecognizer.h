#ifndef WAREVENTBARRECOGNIZER_H
#define WAREVENTBARRECOGNIZER_H

#include "opencv2/core.hpp"
#include "define.h"

class WarEventBarRecognizer
{
public:
    WarEventBarRecognizer();

    bool recognize(cv::Mat bar);

private:
    bool isAttack(cv::Mat bar);

    int scanBarBottomHeight(cv::Mat grayBar);

    void clearBackground(cv::Mat bar, cv::Mat grayBar, cv::OutputArray binBar);

    bool filterObjects(cv::Mat &binbar, cv::Rect &flagRect1, cv::Rect &flagRect2,
                       cv::Rect &attackArrowRect, cv::Rect &playButtonRect);

    friend class Test;
};

#endif // WAREVENTBARRECOGNIZER_H
