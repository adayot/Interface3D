/**
 * @Authors: DAYOT Amelie / DREAU Vincent
 * @Company: ISEN Yncréa Ouest
 */

#include "DICOM_3Dmouse.h"
#include "Scene3D.h"
#include "Widget3D.h"


/*--------------------------------------------------------------------------
* Fonctions : closeEvent()
*
* Description : Permet de libérer la mémoire lors de la fermeture de la fenêtre
*
* Arguments : event : évenement de fermeture de la fenêtre
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Widget3D::closeEvent(QCloseEvent* event)
{

    this->deleteLater();
    event->accept();

}

/*--------------------------------------------------------------------------
* Fonctions : centrage()
*
* Description : Appel de la fonction de recentrage de l'objet 3D dans la 
* classe My3DScene
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Widget3D::centrage() {
    Visualisation3D->Recentrer();
}

/*--------------------------------------------------------------------------
* Fonctions : Actu3D()
*
* Description : Appel de la fonction de mouse3DMove de la classe My3DScene
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Widget3D::Actu3D() {
   Visualisation3D->mouse3DMove();
}

/*--------------------------------------------------------------------------
* Fonctions : AIDE3D()
*
* Description : Affiche une fenêtre expliquant les diversses actions de la
* souris 3D
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
void Widget3D::AIDE3D()
{
    QMessageBox aide;
    aide.setWindowTitle("Aide utilisation logiciel avec Souris 3D");

    QPixmap image ("C:/ImagesProjet/InfSouris.png");
    QPixmap image1;
    image1 = image.scaled(QSize(200,200), Qt::IgnoreAspectRatio);
    aide.setIconPixmap(QPixmap(image1));

    QString n = "\n";
    //aide.setIcon(QMessageBox::Information);
    aide.setText("Centrer la camera et l'objet :" + n +
        "   -> Presser sur le bouton de gauche de votre souris 3D." + n +
        " Realiser une capture d'ecran :" + n +
        "   -> Presser simultanement des deux boutons lateraux de votre souris 3D." + n +
        " Bloquer l'objet en position centrale : " + n +
        "   -> Presser sur le bouton de droite de votre souris 3D");
    aide.exec();
}

/*--------------------------------------------------------------------------
* Fonction : SaveAs()
*
* Description : Fait un screenshot
*
* Arguments : Aucun
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void Widget3D::SaveAs3D()
{
    qApp->beep(); //signal de screenshot

    //Prise du screenshot
    QPixmap pixmap = QPixmap::grabWindow(this->winId());
    //Fenetre d'enregristrement
    QString filePath = QFileDialog::getSaveFileName(this, tr("Enregristrer sous"),"../Screenshot_1.png", tr("Images (*.png *.xpm *.jpg"));
    //Sauvegarde de l'image
    pixmap.save(filePath);
}

/*--------------------------------------------------------------------------
* Fonctions : Widget3D()
*
* Description : Construction de la fenêtre 3D
*
* Arguments : aucun
*
* Valeur retournée : aucune
*--------------------------------------------------------------------------*/
Widget3D::Widget3D(image* image1, image* image2, image* image3)
{
    //Initialisation des éléments
    Visualisation3D = new My3DScene(image1, image2, image3);
    Layout = new QGridLayout();
    menu = new QMenuBar();//Initialisation Barre de menus
    Fichier = new QMenu("&Fichier");//Init menu fichier
    Affichage = new QMenu("&Affichage");//Init menu affichage


    //Actions liées aux différents menus
    Affichage->addAction("Recentrer", this, SLOT(centrage()));  
    Fichier->addAction("Enregistrer sous",this, SLOT(SaveAs3D()));


    //layout = new QGridLayout;//Init layout

    //Conversion de la Scene3d en QWidget et fixation de sa taille
    QHBoxLayout* container = new QHBoxLayout;
    container->addWidget(Visualisation3D);

    QWidget* w = new QWidget;
    w->setLayout(container);

    //Ajout des composants à leurs entités
    menu->addMenu(Fichier);
    menu->addMenu(Affichage);    
    menu->addAction("Aide", this, SLOT(AIDE3D()));
    Fichier->setStyleSheet("color: white;"
        "background-color:rgb(30,30,30);"
        "selection-color: black;"
        "border-radius: 0px;"
        "selection-background-color: rgb(230,230,230);");
    Affichage->setStyleSheet("color: white;"
        "background-color:rgb(30,30,30);"
        "selection-color: black;"
        "border-radius: 0px;"
        "selection-background-color: rgb(230,230,230);");
    menu->setStyleSheet("color: black;"
        "background-color:rgb(230,230,230);"
        "selection-color: black;"
        "border-radius: 0px;"
        "selection-background-color:rgb(30,30,30) ;");
    Layout->setMenuBar(menu);
    Layout->addWidget(w);

    //Appel des fonctions toutes les 10ms
    timer = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(Actu3D()));

    timer->start(10);

    //paramètres du widget
    setLayout(Layout);
    setStyleSheet("background-color:white;");
    setMinimumSize(200, 200);
    setWindowState(windowState() | Qt::WindowMaximized);//Fenetre en plein ecran

    //setWindowIcon(QIcon("icon.png"));
    setWindowTitle("Interface de reconstruction 3D des IRM");//titre fenetre

    show();
}


