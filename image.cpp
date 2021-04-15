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
            rectangle = true;  //Utilisé dans scene3D.cpp
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
    if (event->button() == Qt::MidButton) {
        isZoom = !isZoom;
    }
}

void image::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && (drag || line))
    {
        endX = event->pos().x();
        endY = event->pos().y();
        repaint();

        if (line){
            //Calcul de la mesure et affichage sous le curseur
            float mesure = (sqrt((endX-startX)*(endX-startX) + (endY-startY)*(endY-startY))) ;
            switch (zoom)
            {
            case 1 :
                mesure *= 1;
                break;
            case 2 :
                mesure *= 0.8;
                break;
            case 3 :
                mesure *= 0.6;
                break;
            case 4 :
                mesure *= 0.4;
                break;
            case 5 :
                mesure *= 0.2;
                break;
            case 6 :
                mesure *= 0.1;
                break;
            default :
                mesure *= 0.1;
                break;
            }
            QString cursor = QString::number(round(mesure*100)/100);
            QString toolTipText = cursor  + "mm";
            QToolTip::showText(QCursor::pos(), toolTipText);
        }
    }
}

void image::wheelEvent(QWheelEvent* event)
{
    if(isZoom){
        zoom += event->delta()/120;
        if(zoom==0) zoom=1; //Empeche de dézoomer + que l'image originale
        emit scrolled(event);
    }
}

void image::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && (drag || line))
    {
        endX = event->pos().x();
        endY = event->pos().y();
        drag = false;

        if (line){
            //Calcul de la mesure et affichage sous le curseur
            float mesure = (sqrt((endX-startX)*(endX-startX) + (endY-startY)*(endY-startY)));
            switch (zoom)
            {
            case 1 :
                mesure *= 1;
                break;
            case 2 :
                mesure *= 0.8;
                break;
            case 3 :
                mesure *= 0.6;
                break;
            case 4 :
                mesure *= 0.4;
                break;
            case 5 :
                mesure *= 0.2;
                break;
            case 6 :
                mesure *= 0.1;
                break;
            default :
                mesure *= 0.1;
                break;
            }
            QString cursor = QString::number(round(mesure*100)/100);
            QString toolTipText = cursor  + "mm";
            QToolTip::showText(QCursor::pos(), toolTipText);
            line = false;
        }
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
        painter.drawLine(startX, startY, endX, endY);
    }
}


