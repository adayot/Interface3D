#ifndef IMAGE_H
#define IMAGE_H
#include <windows.h>
#include <QObject>
#include <QLabel>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QToolTip>
#include <QWheelEvent>
#include <iomanip>      // std::setprecision
#include <opencv2/core/mat.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>

using namespace cv;
using namespace std;


class image:public QLabel
{
    Q_OBJECT

public:


    image(QWidget* parent);
    QPixmap data;
    QImage dest;
    int startX, startY, endX, endY;
    bool drag=false, line=false;
    bool isZoom=false;
    bool rectangle=false;
    int zoom=1, zoomRows=256, zoomCols=256;

signals :
    void clicked(QMouseEvent*);
    void scrolled(QWheelEvent*);

protected :
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
    void wheelEvent(QWheelEvent *event);


};

#endif // IMAGE_H
