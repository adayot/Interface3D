/**
 * @Authors: DAYOT Amelie / DREAU Vincent
 * @Company: ISEN Yncr?a Ouest
 */

#include "DICOM_3Dmouse.h"
#include "Scene3D.h"
#include "Widget3D.h"

/*--------------------------------------------------------------------------
* Fonctions : closeEvent()
*
* Description : Permet de lib?rer la m?moire lors de la fermeture de la fen?tre
*
* Arguments : event : ?venement de fermeture de la fen?tre
*
* Valeur retourn?e : aucune
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
* Valeur retourn?e : aucune
*--------------------------------------------------------------------------*/
void Widget3D::centrage() {
    Visualisation3D->Recentrer();
}

/*--------------------------------------------------------------------------
* Fonctions : SaveAs3D()
*
* Description : Permet de screenshoter l'interafce de reconstruction 3D
*
* Arguments : aucun
*
* Valeur retourn?e : aucune
*--------------------------------------------------------------------------*/
void Widget3D::SaveAs3D() {
    qApp->beep(); // Signal the screenshoot

    // Prise du screenshoot
    QPixmap pixmap3D = QPixmap::grabWindow(this->winId());

    //Fen?tre d'enregistrement
    QString filePath3D = QFileDialog::getSaveFileName(this, tr("Enregistrer sous"), "../Screenshot3D_1.png", tr("Images (*.png *.xpm *.jpg)"));

    //Sauvegarde de l'image
    pixmap3D.save(filePath3D);

}

/*--------------------------------------------------------------------------
* Fonctions : Actu3D()
*
* Description : Appel de la fonction de mouse3DMove de la classe My3DScene
*
* Arguments : aucun
*
* Valeur retourn?e : aucune
*--------------------------------------------------------------------------*/
void Widget3D::Actu3D() {
   Visualisation3D->mouse3DMove();
}

/*--------------------------------------------------------------------------
* Fonctions : AIDE3D()
*
* Description : Affiche une fen?tre expliquant les diversses actions de la
* souris 3D
*
* Arguments : aucun
*
* Valeur retourn?e : aucune
*--------------------------------------------------------------------------*/
void Widget3D::AIDE3D()
{
    QMessageBox aide;
    QString n = "\n";
    QPixmap image ("C:/Users/vince/Desktop/ISEN 2020-2021/Projet M1/IRM_Souris3D_Final/InfSouris.png");

    aide.setWindowTitle("Aide utilisation logiciel avec la Souris 3D");
    image = image.scaled(QSize(200,200), Qt::IgnoreAspectRatio);
    aide.setIconPixmap(QPixmap(image));

    aide.setText(n+"Centrer la camera et l'objet : Appuyer sur le bouton de gauche de votre souris 3D." +n+n+
                 " Realiser une capture d'ecran : Appuyer sur le bouton de droite de votre souris 3D.");
    aide.exec();
}

/*--------------------------------------------------------------------------
* Fonctions : Widget3D()
*
* Description : Construction de la fen?tre 3D
*
* Arguments : aucun
*
* Valeur retourn?e : aucune
*--------------------------------------------------------------------------*/
Widget3D::Widget3D(image* image1, image* image2, image* image3)
{
    //Initialisation des ?l?ments
    Visualisation3D = new My3DScene(image1, image2, image3);
    Layout = new QGridLayout();
    menu = new QMenuBar();//Initialisation Barre de menus
    Fichier = new QMenu("&Fichier");//Init menu fichier
    Affichage = new QMenu("&Affichage");//Init menu affichage

    //Actions li?es aux diff?rents menus
    Fichier->addAction("Enregistrer sous", this, SLOT(SaveAs3D()));
    Affichage->addAction("Recentrer", this, SLOT(centrage()));

    //Conversion de la Scene3d en QWidget et fixation de sa taille
    QHBoxLayout* container = new QHBoxLayout;
    container->addWidget(Visualisation3D);

    QWidget* w = new QWidget;
    w->setLayout(container);

    //Ajout des composants ? leurs entit?s
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

    //param?tres du widget
    setLayout(Layout);
    setStyleSheet("background-color:white;");
     setMinimumSize(200,200);
    setWindowState(windowState() | Qt::WindowMaximized);//Fenetre en plein ecran

    setWindowIcon(QIcon("icon.png"));
    setWindowTitle("Interface de reconstruction 3D des IRM");//titre fenetre

    show();
}
