#include "CImage.h"
#include <cstdlib>
#include <cmath>

using namespace std;

CImage::CImage(FILE *f, CLine line) {
    if (fscanf(f, "P%i%i%i%i\n", &this->version, &this->width, &this->height, &max_val) != 4) {
        throw CExpension("Wrong amount data in file", f);
    }
    size = width * height;
    pix = new unsigned char[size];
    fread(pix, sizeof(unsigned char), size, f);
    fclose(f);
}

CImage::~CImage() {
    delete[] this->pix;
}


double CImage::Decorrection(double value, CLine line) {
    value /= 255.0;
    if (line.gamma_ == -1) {
        double a = 0.055;
        if (value <= 0.0031308) {
            return 255 * value * 12.92;
        }
        return 255 * (1.0 + a) * pow(value, (double) 1 / 2.4) - a;
    }
    return 255 * pow(value, (double) 1 / line.gamma_);
}

double CImage::gammaCorrection(double value, CLine line) {
    value /= 255.0;
    if (line.gamma_ == -1) {
        double a = 0.055;
        if (value <= 0.04045) {
            return 255 * value / 12.92;
        }
        return 255 * pow((value + a) / (1.0 + a), 2.4);
    }
    return 255 * pow(value, line.gamma_);
}

//void CImage::WuPoint(bool sw, SPoint p, double brigthness, CLine line) {
//    double b = round((double) pix[(int) p.x_ * width + (int) p.y_] * (1.0 - brigthness) + line.brightness * brigthness);
//    int x = sw ? (int) p.y_ : (int) p.x_;
//    int y = sw ? (int) p.x_ : (int) p.y_;
//    pix[y * width + x] = round(gammaCorrection(Decorrection(b, line), line));
//}


//void CImage::putPoint(bool sw, SPoint p, CLine line) {
//    p.x_ = floor(p.x_);
//    p.y_ = floor(p.y_);
//    int x = sw ? (int) p.y_ : (int) p.x_;
//    int y = sw ? (int) p.x_ : (int) p.y_;
//    for (int i = 0; i < line.thickness - 1; i++) {
//        pix[y * width + x + i] = line.brightness;
//    }
//}

//-----------------------Ву--------------------------

//void CImage::WuLine(CLine line_) {
//    bool swapped = fabs(line_.end_.y_ - line_.start_.y_) > fabs(line_.end_.x_ - line_.start_.x_);
//    if (swapped) {
//        swap(line_.start_.x_, line_.start_.y_);
//        swap(line_.end_.x_, line_.end_.y_);
//    }
//    if (line_.start_.x_ > line_.end_.x_) {
//        swap(line_.start_, line_.end_);
//    }
//    double dx = line_.end_.x_ - line_.start_.x_;
//    double dy = line_.end_.y_ - line_.start_.y_;
//    double gradient = dy / dx;
//    double y = line_.start_.y_ + gradient;
//    double x = (int) line_.start_.x_;
//    while (x <= line_.end_.x_) {
//        double brightness = y - (int) y;
//        WuPoint(swapped, {x, y}, 1.0 - brightness, line_);
//        WuPoint(swapped, {x, y - 1.0}, brightness, line_);
//        y += gradient;
//        x++;
//    }
//}


void CImage::point(IPoint p, CLine line) {
    for (int i = 0; i < line.thickness - 2; i++) {
        pix[p.x * width + p.y + i] = line.brightness;
    }
}

void CImage::point(IPoint p, double brightness, CLine line) {
    double b = round(gammaCorrection(pix[p.x * width + p.y], line) * (1.0 - brightness) + line.brightness * brightness);
    pix[p.x * width + p.y] = (int) round(
            Decorrection(line.brightness / 255.0 * b, line));
}

void CImage::drawLine(CLine line) {
    bool swapped = abs(line.end_.y_ - line.start_.y_) > abs(line.end_.x_ - line.start_.x_);
    if (swapped) {
        swap(line.start_.x_, line.start_.y_);
        swap(line.end_.x_, line.end_.y_);
    }
    double dx = line.end_.x_ - line.start_.x_;
    double dy = line.end_.y_ - line.start_.y_;
    double gradient = dy / dx;
    double y = line.start_.y_ + gradient * (round(line.start_.x_) - line.start_.x_);
    int x = (int) round(line.start_.x_);
    while (x <= (int) round(line.end_.x_)) {
        double b = y - (int) y;
        point({x, (int) y}, 1.0 - b, line);
        point({x, (int) y + 1}, b, line);
        y += gradient;
        x++;
    }
}