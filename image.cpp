#include "image.h"

image::image(QWidget* parent)
    : QLabel(parent)
{


}

void image::mousePressEvent(QMouseEvent* event)
{
    emit clicked(event);

    if(Qt::ControlModifier == QApplication::keyboardModifiers()){ // Vérifie que ctrl est appuyé
        //Ensuite on récupère la position du curseur
        if (event->button() == Qt::LeftButton) {
            startX = event->pos().x();
            startY = event->pos().y();
            drag = true;
        }
    }
    if(Qt::ShiftModifier == QApplication::keyboardModifiers()){ // Vérifie que shift est appuyé
        //Ensuite on récupère la position du curseur
        if (event->button() == Qt::LeftButton) {
            startX = event->pos().x();
            startY = event->pos().y();
            line = true;
        }
    }
}

void image::mouseMoveEvent(QMouseEvent* event)
{
    endX=startX;
    endY=startY;
    if ((event->buttons() & Qt::LeftButton) && (drag || line))
    {
        endX = event->pos().x();
        endY = event->pos().y();
        repaint();
    }
}

void image::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && (drag || line))
    {
        endX = event->pos().x();
        endY = event->pos().y();
        drag = false;
        line = false;
    }
}

void image::paintEvent(QPaintEvent* event){


    QPainter painter(this);
    painter.drawPixmap(0,0,data);
    if (drag){
        painter.setPen(Qt::green);
        painter.drawRect(startX, startY, endX-startX, endY-startY);
    }
    if (line){
        painter.setPen(Qt::red);
        painter.drawLine(startX, startY, endX-startX, endY-startY);
        std::cout << "longueur : " << sqrt((endX-startX)*(endX-startX) + (endY-startY)*(endY-startY))/37 << std::endl;
    }




}



