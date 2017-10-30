#ifndef WAREVENTBARRECOGNIZER_H
#define WAREVENTBARRECOGNIZER_H

#include "opencv2/core.hpp"
#include "define.h"
#include <map>

class WarEventBarRecognizer
{
public:
    WarEventBarRecognizer();

    bool recognize(cv::Mat bar);

private:
    bool isAttack(cv::Mat bar);

    int scanBarBottomHeight(cv::Mat grayBar);

    void clearBackground(cv::Mat bar, cv::Mat grayBar, cv::OutputArray binBar);

    bool getSegments(cv::Mat &binBar, std::map<int, cv::Rect> &objectRects);

    friend class Test;
};

#endif // WAREVENTBARRECOGNIZER_H
