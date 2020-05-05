#ifndef GRAPH_CIMAGE_H
#define GRAPH_CIMAGE_H

#include "CExpension.h"
#include <iostream>
#include "CLine.h"

struct IPoint {
    int x;
    int y;
};

class CImage {
public:
    CImage(FILE *f);

    ~CImage();


    friend void getAnsw(const char *fileName, CImage &output);

    void WuLine(CLine line);

    void drawLine(CLine line, SInput input);

private:
    FILE *file;
    int version;
    int width;
    int height;
    int max_val;
    int size;
    unsigned char *pix;

    bool inRec(int x, int y, CLine AB, CLine BC, CLine CD, CLine DA);

    void point(bool sw, IPoint p, double brightness, CLine line);

    double Decorrection(double value, CLine line);

    double gammaCorrection(double value, CLine line);
};


#endif //GRAPH_CIMAGE_H
