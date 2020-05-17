#include "CImage.h"
#include <cstdlib>
#include <cmath>

using namespace std;

CImage::CImage(FILE *f) {
    file = f;
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
//----------------------Ву--------------------------

void CImage::WuLine(CLine line) {
    bool swapped = abs(line.end_.y_ - line.start_.y_) > abs(line.end_.x_ - line.start_.x_);
    if (swapped) {
        swap(line.start_.x_, line.start_.y_);
        swap(line.end_.x_, line.end_.y_);
    }
    if (line.start_.x_ > line.end_.x_) {
        swap(line.start_, line.end_);
    }
    double dx = line.end_.x_ - line.start_.x_;
    double dy = line.end_.y_ - line.start_.y_;
    double gradient = dy / dx;
    double y = line.start_.y_ + gradient * (round(line.start_.x_) - line.start_.x_);
    int x = (int) round(line.start_.x_);
    while (x <= (int) round(line.end_.x_)) {
        double b = y - (int) y;
        point(swapped, {x, (int) y}, 1.0 - b, line);
        point(swapped, {x, (int) y + 1}, b, line);
        y += gradient;
        x++;
    }
}

bool CImage::inRec(int x, int y, CLine AB, CLine BC, CLine CD, CLine DA) {
    if (y <= AB.start_.y_ + x * (AB.end_.y_ - AB.start_.y_) / (AB.end_.x_ - AB.start_.x_) -
             AB.start_.x_ * (AB.end_.y_ - AB.start_.y_) / (AB.end_.x_ - AB.start_.x_)) {
        return false;
    }
    if (y >= BC.start_.y_ + x * (BC.end_.y_ - BC.start_.y_) / (BC.end_.x_ - BC.start_.x_) -
             BC.start_.x_ * (BC.end_.y_ - BC.start_.y_) / (BC.end_.x_ - BC.start_.x_)) {
        return false;
    }
    if (y >= CD.start_.y_ + x * (CD.end_.y_ - CD.start_.y_) / (CD.end_.x_ - CD.start_.x_) -
             CD.start_.x_ * (CD.end_.y_ - CD.start_.y_) / (CD.end_.x_ - CD.start_.x_)) {
        return false;
    }
    if (y <= DA.start_.y_ + x * (DA.end_.y_ - DA.start_.y_) / (DA.end_.x_ - DA.start_.x_) -
             DA.start_.x_ * (DA.end_.y_ - DA.start_.y_) / (DA.end_.x_ - DA.start_.x_)) {
        return false;
    }
    return true;
}

void CImage::point(bool sw, IPoint p, double brightness, CLine line) {
    double b = round(gammaCorrection(pix[p.x * width + p.y], line) * (1.0 - brightness) + line.brightness * brightness);
    if (sw) {
        swap(p.x, p.y);
    }
    pix[p.y * width + p.x] = (int) round(
            Decorrection(line.brightness / 255.0 * b, line));
}

void CImage::drawLine(CLine line, SInput input) {
    if (line.thickness < 0) {
        throw CExpension("Thickness is < 0", file);
    }
    if (line.thickness == 1.0) {
        WuLine(line);
    } else {
        double a = line.end_.x_ - line.start_.x_ == 0 ? M_PI / 2 : M_PI / 2 - abs(atan(
                (line.end_.y_ - line.start_.y_) / (line.end_.x_ - line.start_.x_)));
        double l = line.thickness / 2.0;
        SPoint A;
        SPoint B;
        SPoint C;
        SPoint D;
        A.x_ = line.start_.x_ + l * sin(a);
        A.y_ = line.start_.y_ + l * cos(a);
        B.x_ = line.start_.x_ - l * sin(a);
        B.y_ = line.start_.y_ - l * cos(a);
        C.x_ = line.end_.x_ - l * sin(a);
        C.y_ = line.end_.y_ - l * cos(a);
        D.x_ = line.end_.x_ + l * sin(a);
        D.y_ = line.end_.y_ + l * cos(a);
        if ((line.end_.y_ - line.start_.y_) / (line.end_.x_ - line.start_.x_) < 0) {
            swap(A, D);
            swap(B, C);
        }
        SInput ABinf = input;
        SInput BCinf = input;
        SInput CDinf = input;
        SInput DAinf = input;
        ABinf.start = A;
        ABinf.end = B;
        BCinf.start = B;
        BCinf.end = C;
        CDinf.start = C;
        CDinf.end = D;
        DAinf.start = D;
        DAinf.end = A;
        CLine AB(ABinf);
        CLine BC(BCinf);
        CLine CD(CDinf);
        CLine DA(DAinf);
        WuLine(AB);
        WuLine(BC);
        WuLine(CD);
        WuLine(DA);
    }
}
