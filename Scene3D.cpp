/**
 * @Authors: LE FUR Anthony / LE BERRE Thomas
 * @Company: ISEN Yncr?a Ouest
 * @Email: anthony.le-fur@isen-ouest.yncrea.fr / thomas.le-berre@isen-ouest.yncrea.fr
 * @Last Modified: 02-Juil-2020
 */

#include "Scene3D.h"
#include "DICOM_3Dmouse.h"
#include "Widget3D.h"

/*--------------------------------------------------------------------------
* Fonction : init()
*
* Description : Initialisation des variables globales qui permettent la
* m?morisation des donn?es r??ues par la souris
*
* Arguments : Aucun
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void My3DScene::init() {
    //Navigation selon TX
    precValueTX = new qint16;
    compteurTX = new qint16;
    *precValueTX = 0;
    *compteurTX = 0;

    //Navigation selon TY
    precValueTY = new qint16;
    compteurTY = new qint16;
    *precValueTY = 0;
    *compteurTY = 0;

    //Navigation selon TZ
    precValueTZ = new qint16;
    compteurTZ = new qint16;
    *precValueTZ = 0;
    *compteurTZ = 0;

    //Rotation selon RX
    precValueRX = new qint16;
    compteurRX = new qint16;
    *precValueRX = 0;
    *compteurRX = 0;

    //Rotation selon RY
    precValueRY = new qint16;
    compteurRY = new qint16;
    *precValueRY = 0;
    *compteurRY = 0;

    //Rotation selon RZ
    precValueRZ = new qint16;
    compteurRZ = new qint16;
    *precValueRZ = 0;
    *compteurRZ = 0;
}

/*--------------------------------------------------------------------------
* Fonction : initializeGL()
*
* Description : Initialisation de la fen?tre OpenGL et reconstruction 3D
*
* Arguments : Aucun
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void My3DScene::initializeGL()
{
    int c=0; //Compteur du nombre de voxels affichés
    QVector<unsigned short> pixelVoisin;

    //Appel de la fonction pour la représentation 3D d'une zone sélectionnée
    if ((monImage1->rectangle && monImage2->rectangle) || (monImage1->rectangle && monImage3->rectangle) || (monImage2->rectangle && monImage3->rectangle)){
        //Création d'un QVector<unsigned short>* decoupe, de la même taille que allpixels, on l'initialise rempli de 0
        QVector<unsigned short> decoupe = *allpixels;
        for (int i=0; i<decoupe.size(); i++) decoupe[i]=0;

        //Appel de la fonction decoupe3D() pour avoir un QVector retaillé.
        pixelVoisin = decoupe3D(decoupe); //On met les valeurs de decoupe dans pixelVoisin
        pixelVoisinFunc(&pixelVoisin); //Appel de la fonction pour remplir pixelVoisin
    }

    else {
        //Appel de la fonction pour remplir pixelVoisin
        pixelVoisin = *allpixels; //Copie de allpixels dans pixelVoisin pour ne pas gêner les autres utilisations de allpixels
        pixelVoisinFunc(&pixelVoisin);
    }

    //Mode openGL
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);

    glGenLists(*NbFichiers); //On réserve un nombre de liste égal au nombre de fichiers soit au nombre de coupes

    //Création barre de chargement de la reconstruction 3D
    QProgressDialog Chargement ("Reconstruction 3D", "Cancel", 0, *NbFichiers);//Paramètres de la barre
    Chargement.setWindowTitle("Chargement");
    Chargement.setWindowModality(Qt::WindowModal);
    Chargement.setMinimumDuration(5);//Pas de temps mini de chargement

    glClearColor(0.5,0.5,0.5,0.0); // Gris par défaut
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // On remplit le tampon

    // Paramètres fournis pour la caméra
    glMatrixMode(GL_MODELVIEW);

    //Boucle et appel de la fonction drawCube
    for (int k=0; k < *NbFichiers; k++){ //coupe k
        glNewList(k+1, GL_COMPILE); //Création d'une nouvelle liste. On commence à 1 car la coupe 0 va servir lors des zoom dans la reconstruction 3D pour réafficher la première coupe entièrement
        for (int y=0; y < ligne; y++){
            for (int x=0; x < colonne; x++){
                if ((int)pixelVoisin[k*256*256 + y*256 + x] > 50){ //Seuillage : ignore les pixels trop noir
                    drawCube(pixelVoisin[k*256*256 + y*256 + x], x, y, k);//Appel de la fonction pour dessiner le voxel
                    c++;
                }
            }
        }
        glEndList(); //Fin de la liste
    }

    //Ajout de la valeur max pour fin de chargement
    Chargement.setValue(*NbFichiers);
    cout << "Nombre de voxels affichés : " << c << endl; //Affiche compteur de voxel

    glFlush();
}

/*--------------------------------------------------------------------------
* Fonction : pixelVoisinFunc()
*
* Description : Va rechercher les voxels non visible càd avec 6 voisins visible,
* stocker leur position dans un tableau et à la fin on met les valeurs de pixelVoisin
* aux indices du tableau à 0 pour ne pas les afficher.
*
* Arguments : QVector<unsigned short>* pixelVoisin : poiteur sur pixelVoisin
*
* Valeur retournee : Aucune
*
*--------------------------------------------------------------------------*/
void My3DScene::pixelVoisinFunc(QVector<unsigned short>* pixelVoisin)
{
    vector<int> tab; //Tableau qui va contenir les positions des pixels à ne pas afficher

    //On ne prend pas les bords de l'image pour éviter les "out of range"
    for (int k=1; k < *NbFichiers-1; k++){
        for (int y=1; y < ligne-1; y++){
            for (int x=1; x < colonne-1; x++){
                /*Si pixel de devant et pixel de derrière
                 *et pixel de droite et pixel de gauche
                 *et pixel dessus et pixel dessous peuvent être affichés,
                 *on récupère l'indice du pixel au milieu qu'on affichera pas
                */
                if ((int)(*pixelVoisin)[(k-1)*256*256 + y*256 + x]>45 && (int)(*pixelVoisin)[(k+1)*256*256 + y*256 + x]>45
                   && (int)(*pixelVoisin)[k*256*256 + y*256 + (x+1)]>45 && (int)(*pixelVoisin)[k*256*256 + y*256 + (x-1)]>45
                   && (int)(*pixelVoisin)[k*256*256 + (y-1)*256 + x]>45 && (int)(*pixelVoisin)[k*256*256 + (y+1)*256 + x]>45){

                   tab.push_back(k*256*256 + y*256 + x); //Ajoute l'indice du pixel à ne pas afficher dans tab
                } //fin if
            } //fin for colone
        } //fin for ligne
    } //fin for *NbFichiers

    //On met à 0 les pixels qu'on ne va pas afficher dans pixelVoisin
    for (int i=0; i<(int)tab.size(); i++){
        (*pixelVoisin)[tab[i]] = 0;
    }
}

/*--------------------------------------------------------------------------
* Fonction : decoupe3D()
*
* Description : A partir d'une copie de allpixels dans decoupe initialisée à 0,
* on défini les bornes de l'image avec les coordonées du rectangle dessiné.
* Ensuite on met la valeur de allpixels dans decoupe pour les pixels à afficher
*
* Arguments : QVector<unsigned short> decoupe
*
* Valeur retournee : Aucune
*
*--------------------------------------------------------------------------*/
QVector<unsigned short> My3DScene::decoupe3D(QVector<unsigned short> decoupe)
{
    if (monImage1->rectangle && monImage2->rectangle){ //1ere et 2eme images
        for (int k=((monImage2->startX)*(*NbFichiers))/156; k < ((monImage2->endX)*(*NbFichiers))/156; k++){
            for (int y=monImage1->startY; y < monImage1->endY; y++){
                for (int x=monImage1->startX; x < monImage1->endX; x++){
                    decoupe[k*256*256 + y*256 + x]=(*allpixels)[k*256*256 + y*256 + x]; //On met dans decoupe la valeur du pixel de allpixels à la position actuelle
                }//fin for x
            }//fin for y
        }//fin for k
    }
    else if (monImage1->rectangle && monImage3->rectangle){
        for (int k=((monImage3->startX)*(*NbFichiers))/156; k < ((monImage3->endX)*(*NbFichiers))/156; k++){
            for (int y=monImage1->startY; y < monImage1->endY; y++){
                for (int x=monImage1->startX; x < monImage1->endX; x++){
                    decoupe[k*256*256 + y*256 + x]=(*allpixels)[k*256*256 + y*256 + x];
                }//fin for x
            }//fin for y
        }//fin for k

    }
    else if (monImage2->rectangle && monImage3->rectangle){
        for (int k=((monImage2->startX)*(*NbFichiers))/156; k < ((monImage2->endX)*(*NbFichiers))/156; k++){
            for (int y=monImage3->startY; y < monImage3->endY; y++){
                for (int x=monImage3->startX; x < monImage3->endX; x++){
                    decoupe[k*256*256 + y*256 + x]=(*allpixels)[k*256*256 + y*256 + x];
                }//fin for x
            }//fin for y
        }//fin for k
    }

    //On retourne les valeurs de decoupe
    return decoupe;

}

/*--------------------------------------------------------------------------
* Fonction : drawCube(unsigned short val)
*
* Description :
*
* Arguments :
*
* Valeur retournee :
*
*--------------------------------------------------------------------------*/
void My3DScene::drawCube(unsigned short val, int x, int y, int z)
{
    float Val = ((float)val/255.0);
    // On dessine des quadrilatères reliés entre eux grâce à GL_QUADS
    glBegin(GL_QUADS);

        //Gauche
        glColor4f(Val, Val, Val, 1.0f);
            glNormal3d(-1.0,0.0,0.0);
            glVertex3d( x-1, y+1, z+1);
            glVertex3d( x-1, y+1, z-1);
            glVertex3d( x-1, y-1, z-1);
            glVertex3d( x-1, y-1, z+1);
        //Au-dessus
        glColor4f(Val, Val, Val, 1.0f);
            glNormal3d(0.0,1.0,0.0);
            glVertex3d( x-1, y+1, z+1);
            glVertex3d( x+1, y+1, z+1);
            glVertex3d( x+1, y+1, z-1);
            glVertex3d( x-1, y+1, z-1);
        //En-dessous
        glColor4f(Val, Val, Val, 1.0f);
            glNormal3d(0.0,-1.0,0.0);
            glVertex3d( x-1, y-1, z+1);
            glVertex3d( x-1, y-1, z-1);
            glVertex3d( x+1, y-1, z-1);
            glVertex3d( x+1, y-1, z+1);
        //Droite
        glColor4f(Val, Val, Val, 1.0f);
            glNormal3d(1.0,0.0,0.0);
            glVertex3d( x+1, y+1, z+1);
            glVertex3d( x+1, y-1, z+1);
            glVertex3d( x+1, y-1, z-1);
            glVertex3d( x+1, y+1, z-1);
        //Derrière
        glColor4f(Val, Val, Val, 1.0f);
            glNormal3d(0.0,0.0,-1.0);
            glVertex3d( x+1, y+1, z-1);
            glVertex3d( x+1, y-1, z-1);
            glVertex3d( x-1, y-1, z-1);
            glVertex3d( x-1, y+1, z-1);
        //Face
        glColor4f(Val, Val, Val, 1.0f);
            glNormal3d(0.0,0.0,1.0);
            glVertex3d(x-1, y+1, z+1);
            glVertex3d(x-1, y-1, z+1);
            glVertex3d( x+1, y-1, z+1);
            glVertex3d( x+1, y+1, z+1);
    glEnd();
}

/*--------------------------------------------------------------------------
* Fonction : resizeGL()
*
* Description : Gestion de la zone de visualisation 3D
*
* Arguments : Aucun
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void My3DScene::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = (GLfloat)width / height;
    glFrustum(-x, x, -1.0, 1.0, 10, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}

/*--------------------------------------------------------------------------
* Fonction : paintGL()
*
* Description : Translation et rotation de la reconstruction 3D
*
* Arguments : Aucun
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void My3DScene::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(TranslationX, TranslationY, zoomZ - 2000.0);
    //Rotation de la représentation en focntion de la souris
    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);
    glRotatef(180, -1.0, 0, 0); //Retourne l'image et regard vers la gauche
    glTranslatef((-colonne/2), (-ligne/2), -*NbFichiers / 2); //centrage

    //Affichage de la première coupe entièrement (varie suivant le zoom dans les coupes)
    glNewList(0, GL_COMPILE);
    for (int y=0; y < ligne; y++){
        for (int x=0; x < colonne; x++){
            if ((int)(*allpixels)[zoom*256*256 + y*256 + x] > 50){ //Seuillage : ignore les pixels trop noir
                drawCube((*allpixels)[zoom*256*256 + y*256 + x], x, y, zoom); //Appel de la fonction pour dessiner le voxel
            }
        }
    }
    glEndList();

    //Affichage de toutes les coupes suivantes
    for (int i=zoom; i<*NbFichiers; i++){
        glCallList(i);
    }
}

/*--------------------------------------------------------------------------
* Fonction : keyPressEvent()
*
* Description : Cette méthode est appelée à chaque relâchement d'une touche clavier
*
* Arguments : event : ?v?nement du clavier
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void My3DScene::keyPressEvent(QKeyEvent* e)
{
   //L'argument e contient entre autres le nom de la touche relâchée
   if(e->key() == 82) //Si la touche R est appuyée, 82 est le code pour la tocuhe R
   {
       Recentrer(); //Appel fonction Recentrer()
   }

   if(e->key() == 88) //Si la touche R est appuyée, 88 est le code pour la tocuhe R
   {
       SaveAs();
   }
}

/*--------------------------------------------------------------------------
* Fonction : mousePressEvent()
*
* Description : R?cup?ration de la derni?re position de la souris classique
*
* Arguments : event : ?v?nement de la souris classique
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void My3DScene::mousePressEvent(QMouseEvent* event)
{
    lastPosition = event->pos();
}

/*--------------------------------------------------------------------------
* Fonction : mouseMoveEvent()
*
* Description : Contr?le de la sc?ne avec la souris classique
*
* Arguments : event : ?v?nement de la souris classique
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void My3DScene::mouseMoveEvent(QMouseEvent* event)
{

    GLfloat dx = (GLfloat)(event->x() - lastPosition.x()) / width();
    GLfloat dy = (GLfloat)(event->y() - lastPosition.y()) / height();

    if (event->buttons() & Qt::LeftButton)
    {
        TranslationY -= 500 * dy;
        TranslationX += 500 * dx;
        updateGL();
    }
    else if (event->buttons() & Qt::RightButton)
    {
        rotationX += 180 * dy;
        rotationY += 180 * dx;
        updateGL();
    }

    lastPosition = event->pos();
}

void My3DScene::wheelEvent(QWheelEvent* event) {

    if(Qt::ShiftModifier == QApplication::keyboardModifiers()){ // Vérifie que shift est appuyé -> pour rentrer dans les coupes de la reconstruction
        inside = true; //Pour ne pas faire de zoom de l'image
        zoom += event->delta()/120; //ajoute +1 ou -1 à la valeur de zoom
        if (zoom==0) zoom=1; //Empeche de dézoomer plus que l'image originale
        if (zoom>*NbFichiers) zoom=*NbFichiers; //Empeche de zoom plus loin que la dernière coupe
    }
    if (!inside) zoomZ+= (GLfloat)event->delta();
    updateGL();

}

/*--------------------------------------------------------------------------
* Fonction : mouse3DMove()
*
* Description : Controle de la sc?ne avec la souris 3D
*
* Arguments : Aucun
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void My3DScene::mouse3DMove()
{
    //Condition si souris en mode interface 2D
    if (mode3D == 0)
        return;

    //Passage des valeurs globales en locales
    int h = *compteurTX;
    int l = *compteurTY;
    int m = *compteurTZ;
    int i = *compteurRX;
    int j = *compteurRY;
    int k = *compteurRZ;



    if ((-pTx > 5)|| (-pTx < -5) || (-pTy > 5) || (-pTy < -5) || (-pTz > 5) || (-pTz < -5) || (-pRx > 5) || (-pRx < -5) || (pRy > 5) || (pRy < -5)|| (pRz > 5) || (pRz < -5))
        updateGL();
    else
        return;

    TranslationX = 10*h;

    //--------------------------TX------------------------------ -
    //Conditions de sensibilit? de la souris 3D
    if ((-pTx > 5) && (-pTx >= *precValueTX) && (-pTx < 20)) {
        h = h - 1;
    }
    else if ((-pTx >= 20) && (-pTx >= *precValueTX) && (-pTx < 600)) {
        h = h - 3;
    }

    else if ((-pTx < -5) && (-pTx <= *precValueTX) && (-pTx > -20)) {
        h = h + 1;
    }
    else if ((-pTx <= -20) && (-pTx <= *precValueTX) && (-pTx > -600)) {
        h = h + 3;
    }

    //M?morisation de la valeur du NumImageTx
    *compteurTX = h;

    //M?morisation de la valeur -pTx de la souris 3D
    *precValueTX = -pTx;

    zoomZ = 10 * l;

    //--------------------------TY-------------------------------
    //Conditions de sensibilit? de la souris 3D
    if ((-pTy > 5) && (-pTy >= *precValueTY) && (-pTy < 20)) {
        l = l - 1;
    }
    else if ((-pTy >= 20) && (-pTy >= *precValueTY) && (-pTy < 600)) {
        l = l - 3;
    }

    else if ((-pTy < -5) && (-pTy <= *precValueTY) && (-pTy > -20)) {
        l = l + 1;
    }
    else if ((-pTy <= -20) && (-pTy <= *precValueTY) && (-pTy > -600)) {
        l = l + 3;
    }

    //M?morisation de la valeur du NumImageTx
    *compteurTY = l;

    //M?morisation de la valeur pTy de la souris 3D
    *precValueTY = -pTy;

    TranslationY = 10* m;

    //--------------------------TZ-------------------------------
    //Conditions de sensibilit? de la souris 3D
    if ((-pTz > 5) && (-pTz >= *precValueTZ) && (-pTz < 20)) {
        m = m - 1;
    }
    else if ((-pTz >= 20) && (-pTz >= *precValueTZ) && (-pTz < 600)) {
        m = m - 3;
    }

    else if ((-pTz < -5) && (-pTz <= *precValueTZ) && (-pTz > -20)) {
        m = m + 1;
    }
    else if ((-pTz <= -20) && (-pTz <= *precValueTZ) && (-pTz > -600)) {
        m = m + 3;
    }

    //M?morisation de la valeur du NumImageTx
    *compteurTZ = m;

    //M?morisation de la valeur pTz de la souris 3D
    *precValueTZ = -pTz;

    //--------------------------RX-------------------------------
    //Conditions pour avoir une valeur d'angle coh?rente
    if ((i > -360) && (i < 360)) {
        rotationX=i;

        //Conditions de sensibilit? de la souris 3D
        if ((-pRx > 5) && (-pRx >= *precValueRX) && (-pRx < 20)) {
            i = i - 1;
        }
        else if ((-pRx >= 20) && (-pRx >= *precValueRX) && (-pRx < 600)) {
            i = i - 3;
        }

        else if ((-pRx < -5) && (-pRx <= *precValueRX) && (-pRx > -20)) {
            i = i + 1;
        }
        else if ((-pRx <= -20) && (-pRx <= *precValueRX) && (-pRx > -600)) {
            i = i + 3;
        }
    }

    else if (i <= -360) {
        i = 0;
    }
    else if (i >= 360) {
        i = 0;
    }

    //M?morisation de la valeur du NumImageTx
    *compteurRX = i;

    //M?morisation de la valeur -pRx de la souris 3D
    *precValueRX = -pRx;

    //--------------------------RY-------------------------------
    //Conditions pour avoir une valeur d'angle coh?rente
    if ((j > -360) && (j < 360)) {
        rotationY=j;

        //Conditions de sensibilit? de la souris 3D
        if ((pRy > 5) && (pRy >= *precValueRY) && (pRy < 20)) {
            j = j - 1;
        }
        else if ((pRy >= 20) && (pRy >= *precValueRY) && (pRy < 600)) {
            j = j - 3;
        }

        else if ((pRy < -5) && (pRy <= *precValueRY) && (pRy > -20)) {
            j = j + 1;
        }
        else if ((pRy <= -20) && (pRy <= *precValueRY) && (pRy > -600)) {
            j = j + 3;
        }
    }

    else if (j <= -360) {
        j = 0;
    }
    else if (j >= 360) {
        j = 0;
    }

    //M?morisation de la valeur du NumImageTx
    *compteurRY = j;

    //M?morisation de la valeur pRy de la souris 3D
    *precValueRY = pRy;


    //--------------------------RZ-------------------------------
    //Conditions pour avoir une valeur d'angle coh?rente
    if ((k > -360) && (k < 360)) {
        rotationZ=k;

        //Conditions de sensibilit? de la souris 3D
        if ((pRz > 5) && (pRz >= *precValueRZ) && (pRz < 20)) {
            k = k - 1;
        }
        else if ((pRz >= 20) && (pRz >= *precValueRZ) && (pRz < 600)) {
            k = k - 3;
        }

        else if ((pRz < -5) && (pRz <= *precValueRZ) && (pRz > -20)) {
            k = k + 1;
        }
        else if ((pRz <= -20) && (pRz <= *precValueRZ) && (pRz > -600)) {
            k = k + 3;
        }
    }

    else if (k <= -360) {
        k = 0;
    }
    else if (k >= 360) {
        k = 0;
    }


    //M?morisation de la valeur du NumImageTx
    *compteurRZ = k;

    //M?morisation de la valeur pRz de la souris 3D
    *precValueRZ = pRz;



}

/*--------------------------------------------------------------------------
* Fonction : Recentrer()
*
* Description : Recentrer l'objet 3D
*
* Arguments : Aucun
*
* Valeur retourn?e : Aucune
*--------------------------------------------------------------------------*/
void My3DScene::Recentrer()
{
    TranslationX = 0;
    TranslationY = 0;
    zoomZ = 0;
    updateGL();
    *compteurTX = 0;
    *compteurTY = 0;
    *compteurTZ = 0;
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
void My3DScene::SaveAs()
{
    qApp->beep(); //signal de screenshot

    //Prise du screenshot
    QPixmap pixmap = QPixmap::grabWindow(this->winId());
    //Fenetre d'enregristrement
    QString filePath = QFileDialog::getSaveFileName(this, tr("Enregristrer sous"),"../Screenshot_1.png", tr("Images (*.png *.xpm *.jpg"));
    //Sauvegarde de l'image
    pixmap.save(filePath);
}

//------------------------------------------------------------------------------
//--- Constructors -------------------------------------------------------------
//------------------------------------------------------------------------------
My3DScene::My3DScene(image* image1, image* image2, image* image3)
{
    setFocusPolicy(Qt::StrongFocus);
    init();
    setFormat(QGLFormat(QGL::DoubleBuffer | QGL::DepthBuffer));
    this->TranslationX = 0;
    this->TranslationY = 0;
    this->zoomZ = 0;
    this->rotationX = 0;
    this->rotationY = 0;
    this->rotationZ = 0;
    this->monImage1=image1;
    this->monImage2=image2;
    this->monImage3=image3;

}




