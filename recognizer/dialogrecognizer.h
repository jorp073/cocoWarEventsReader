#ifndef DIALOGRECOGNIZER_H
#define DIALOGRECOGNIZER_H

#include "opencv2/core.hpp"
#include "define.h"

class DialogRecognizer
{
public:
    DialogRecognizer();

    // return index of page
    bool recognizePageIndex(cv::Mat img, int pageCount, OUT int* index);

private:
    bool findCloseButton(cv::Mat img, OUT cv::Rect& rect);
    bool _findCloseButtonRect(cv::Mat mat, OUT cv::Rect& rect);

    int getTabBgColor(cv::Mat img);
};

#endif // DIALOGRECOGNIZER_H
