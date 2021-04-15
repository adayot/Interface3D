#pragma once

// Includes
#include <GL/glut.h>
#include <GL/gl.h>
#include <iostream>
#include <QApplication>
#include <QGLWidget>
#include <QVector3D>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPoint>
#include <si.h>
#include <siapp.h>
#include <opencv2/opencv.hpp>
#include "image.h"
#include <vector>
// Namespaces.
using namespace std;
using namespace cv;

//Variables globales externes
//extern INT			Coupe, Min, Max;
extern INT			pTx, pTy, pTz, pRx, pRy, pRz;
extern INT			colonne;
extern INT			ligne;
extern qint16*		NbFichiers;
extern qint16*		NbCouleurs;
//extern 	qint16*		coupe;
extern QVector<unsigned short>* allpixels;
extern FLOAT		EcartCoupe;
extern FLOAT        EcartPixel;



//Class My3DScene
class My3DScene : public QGLWidget
{
private:
	// Attributes.
	GLfloat rotationX;
	GLfloat rotationY;
	GLfloat rotationZ;
	GLfloat zoomZ;
	GLfloat TranslationX;
	GLfloat TranslationY;
	QPoint lastPosition;

	// Private methods.
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	void mousePressEvent(QMouseEvent* event);
	void mouseMoveEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent * e);

	//Variables globales
	qint16* precValueTX;
	qint16* compteurTX;
	qint16* precValueTY;
	qint16* compteurTY;
	qint16* precValueTZ;
	qint16* compteurTZ;
	qint16* precValueRX;
	qint16* compteurRX;
	qint16* precValueRY;
	qint16* compteurRY;
	qint16* precValueRZ;
	qint16* compteurRZ;
	QVector<Vec3b>* ImgVec;

	
public slots:
	void Recentrer();
    void SaveAs();
    void mouse3DMove();
public:
    My3DScene(image* image1, image* image2, image* image3);
    void init();
    void pixelVoisinFunc(QVector<unsigned short>* pixelVoisin);
    QVector<unsigned short> decoupe3D(QVector<unsigned short> decoupe);
    void drawCube(unsigned short val, int x, int y, int z);

    image* monImage1;
    image* monImage2;
    image* monImage3;

    bool inside = false;
    int zoom = 0;
};
