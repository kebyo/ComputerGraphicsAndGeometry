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
    CImage(FILE *f, CLine line);

    ~CImage();;

    friend void getAnsw(const char *fileName, CImage &output);

    void WuLine(CLine line_);

    void drawLine(CLine line);

private:
    FILE *file;
    int version;
    int width;
    int height;
    int max_val;
    int size;
    unsigned char *pix;

    void point(IPoint p, CLine line);

    void putPoint(bool sw, SPoint p, CLine line);

    void point(IPoint p, double brightness, CLine line);

    double Decorrection(double value, CLine line);

    void WuPoint(bool sw, SPoint p, double brigthness, CLine line);

    double gammaCorrection(double value, CLine line);
};


#endif //GRAPH_CIMAGE_H
