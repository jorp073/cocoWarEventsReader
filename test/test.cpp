#include "test.h"

#include "recognizer/dialogrecognizer.h"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

#define RESPATH "F:\\workspace\\COCClashHistoryReader\\test\\res\\dialogrecognize\\"

Test::Test(QObject *parent) : QObject(parent)
{

}

void Test::test_index()
{
    DialogRecognizer rec;

    QCOMPARE(rec.recognize(imread(RESPATH "2of4.png", CV_8UC3), 4), 2);
    QCOMPARE(rec.recognize(imread(RESPATH "3of4.png", CV_8UC3), 4), 3);
}
