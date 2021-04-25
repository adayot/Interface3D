/**
 * @Authors: DAYOT Amelie / DREAU Vincent
 * @Company: ISEN Yncr�a Ouest
 */

#include "Read_data_3Dmouse.h"
#include "DICOM_3Dmouse.h"
#include "Scene3D.h"

/*--------------------------------------------------------------------------
* Fonction : main()
*
* Description : programme principal d'execution de l'application
*
* Arguments : argc, argv[] : valeurs du syst�me
*
* Valeur retourn�e : aucune
*--------------------------------------------------------------------------*/
int main(int argc, char* argv[])
{
    //Cr�ation d'une application
	QApplication a(argc, argv);
	Interface w;
	w.show();
    w.setStyleSheet("color: rgb(230,230,230);"
        "background-color:rgb(30,30,30);"
        "selection-color: rgb(30,30,30);"
        "selection-background-color:rgb(230,230,230) ;");

    //Initialisation de la souris 3D
    Init3DMouse();
    

    return a.exec();
}
