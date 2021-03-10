#ifndef IMAGE_H
#define IMAGE_H
#include <QLabel>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>

#include <iostream>


class image:public QLabel
{
    Q_OBJECT
public:
    image(QWidget* parent);
    QPixmap data;
    int startX, startY, endX, endY;
    bool drag=false, line=false;
signals :
    void clicked(QMouseEvent*);
public slots :
    //void RectCoupe(QMouseEvent* event);
protected :
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);
};

#endif // IMAGE_H
