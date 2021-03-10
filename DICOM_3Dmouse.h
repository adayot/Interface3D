#pragma once

//Includes
#include <QtWidgets>
#include "ui_DICOM_3Dmouse.h"
#include <QApplication>
#include <QtWidgets>
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <QFileDialog>
#include "utils.h"
#include <QMenuBar>
#include <dcm/dicom_file.h>
#include <dcm/logger.h>
#include <dcm/util.h>
#include <dcm/defs.h>
#include "Scene3D.h"
#include <direct.h>
#include <QPaintEvent>
#include <QRubberBand>
#include "image.h"

//Appel des variables globales externes
extern HWND         hWndMain;
extern INT			Coupe, Min, Max;
extern INT			pTx, pTy, pTz, pRx, pRy, pRz;
extern INT			Intensite, prevInt, OnOffSouris3D;
extern INT			clicD, clicG;

class Interface : public QWidget
{
    Q_OBJECT

public:
    Interface();
    void mousePressEvent(QMouseEvent* e);
    void closeEvent(QCloseEvent* event);
    QVector<unsigned short>* ALLPixelsFunc(vector<unsigned short>* pixels, QVector<unsigned short>* allpixels);
    //QVector<Vec3b>* VectorImages(QVector<unsigned short>* all, QVector<Vec3b>* ImgVec, int NbFichier);


signals:
    void clic(QMouseEvent* e);

public slots:
    //------------------Interface 2D-------------------------
    void ouvrirFichiers();
    void GestionImages(int v);
    void GestionImagesLignes(int v);
    void GestionImagesColonnes(int v);
    void DoubleClics();
    void ClicGauche();
    void SaveAs();
    void UtiliserSouris3D();
    void AffichageOriginal();
    void AffichageJet();
    void AffichageBone();
    void AffichageCivids();
    void AffichageTurbo();
    void AffichageHot();
    void AffichageParula();
    void AffichageTwilightShifted();
    void UtiliserCurseurIntensite();
    void ChangementIntensite(int k);
    void ActionSlider1(int k);
    void ActionSlider2(int k);
    void ActionSlider3(int k);
    void ActionSpin1(int k);
    void ActionSpin2(int k);
    void ActionSpin3(int k);
    void displayTags();
    void Action3DMouseTx();
    void Action3DMouseTy();
    void Action3DMouseTz();
    void Action3DMouseIntensite();
    void MajClicCoupe1(QMouseEvent* e);
    void MajClicCoupe2(QMouseEvent* e);
    void MajClicCoupe3(QMouseEvent* e);
    void InfoCoupes();


    void AIDE();
    void fenetreActive();

    //------------------Interface 3D-------------------------
    void Affichage3D();

private:
    //------------------Interface 2D-------------------------
    image* imageLabel1;
    image* imageLabel2;
    image* imageLabel3;
    QString* pathFolder;
    QString* pathFolderSave;
    QGridLayout* layout;
    QSlider* slider1;
    QSlider* slider2;
    QSlider* slider3;
    QSlider* sliderIntensite;
    QMenu* Affichage;
    QMenuBar* menu;
    QMenu* file;
    QMenu* Info;
    QMenu* Outils;
    QStringList* Listechemin;
    QSpinBox* SpinBox1;
    QSpinBox* SpinBox2;
    QSpinBox* SpinBox3;
    qint16* IntensiteMaxInitCoupe1;
    qint16* IntensiteVariableCoupe1;
    qint16* IntensiteMaxInitCoupe2;
    qint16* IntensiteVariableCoupe2;
    qint16* IntensiteMaxInitCoupe3;
    qint16* IntensiteVariableCoupe3;
    qint16* visible;
    qint16* lastTxValue;
    qint16* NumImageTx;
    qint16* lastTyValue;
    qint16* NumImageTy;
    qint16* lastTzValue;
    qint16* NumImageTz;
    qint16* lastRyValue;
    qint16* variationIntensite;
    qint16* souris3D;
    qint16* MenuSouris3D;
    qint16* OnOffIntensite;
    qint16* MenuIntensite;
    qint16* Mode;
    qint16* coupe;
    qint16* IntensiteOnOff;

};



