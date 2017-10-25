#ifndef DIALOGRECOGNIZER_H
#define DIALOGRECOGNIZER_H

#include "opencv2/core.hpp"

class DialogRecognizer
{
public:
    DialogRecognizer();

    // return index of page
    int recognize(cv::Mat img, int pageCount);

private:
    cv::Rect findCloseButton(cv::Mat img);
};

#endif // DIALOGRECOGNIZER_H
