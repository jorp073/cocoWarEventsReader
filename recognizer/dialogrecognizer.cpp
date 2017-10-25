#include "dialogrecognizer.h"

DialogRecognizer::DialogRecognizer()
{

}

int DialogRecognizer::recognize(cv::Mat img, int pageCount)
{
    auto rect = findCloseButton(img);
    return 0;
}

cv::Rect DialogRecognizer::findCloseButton(cv::Mat img)
{
    return cv::Rect(0, 0, 0, 0);
}

