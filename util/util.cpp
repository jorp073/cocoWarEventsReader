#include "util.h"
#include "opencv2/core.hpp"

using namespace cv;


void Util::maxDistIdx(const std::vector<double>& values, int *index)
{
    int count = values.size();
    std::vector<double> distances;
    for (int i=0; i<count; i++) {
        double sum = 0;
        for (int j=0; j<count; j++) {
            double dist = values[i] - values[j];
            sum += dist*dist;
        }

        distances.push_back(sum);
    }

    double maxVal = 0;
    int maxIdx;
    minMaxIdx(Mat(distances), 0, &maxVal, 0, &maxIdx);

    *index = maxIdx;
}
