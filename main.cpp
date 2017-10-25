#include "mainwindow.h"
#include <QApplication>

#include "opencv2/core.hpp"
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Mat ab;

    return a.exec();
}
