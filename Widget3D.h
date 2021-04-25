#pragma once
#pragma once

//Includes
#include <QtWidgets/QMainWindow>
#include "ui_DICOM_3Dmouse.h"
#include <QApplication>
#include <QtWidgets>
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <QFileDialog>
#include "utils.h"
#include <QMenuBar>
#include <dcm/dicom_file.h>
#include <dcm/logger.h>
#include <dcm/util.h>
#include "Scene3D.h"
#include <direct.h>
#include "image.h"

//Appel des variables globales externes
extern HWND         hWndMain;
extern INT			Coupe, Min, Max;
extern INT			pTx, pTy, pTz, pRx, pRy, pRz;
extern INT			mode3D;

class Widget3D : public QWidget
{
	Q_OBJECT

private:
	QSlider* slider;

public:
    Widget3D(image* image1, image* image2, image* image3);
	My3DScene* Visualisation3D;
    void closeEvent(QCloseEvent* event);
signals:

public slots:

	void Actu3D();
    void centrage();
	void AIDE3D();
    void SaveAs3D();

public:	
	QGridLayout* Layout;
	QWidget* container;
	QMenuBar* menu;
	QMenu* Fichier;
	QMenu* Affichage;
	QTimer* timer;
};
