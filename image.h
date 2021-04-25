#ifndef IMAGE_H
#define IMAGE_H
#include <QLabel>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QToolTip>
#include <QGraphicsView>

#include <iostream>

using namespace std;

class image:public QLabel
{
    Q_OBJECT
public:
    image(QWidget* parent);
    QPixmap data;
    int startX, startY, endX, endY;
    int zoom=1;
    bool drag=false, rectangle=false, line=false, isZoom=false;

signals :
    void clicked(QMouseEvent*);
    void scrolled(QWheelEvent*);

protected :
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent* event);
};

#endif // IMAGE_H
