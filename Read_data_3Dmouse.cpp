/**
 * @Authors: DAYOT Amelie / DREAU Vincent
 * @Company: ISEN Yncr?a Ouest
 */

#include "Read_data_3Dmouse.h"

//D?finition des variables globales
SiHdl        devHdl;
HWND         hWndMain;
INT          pTx, pTy, pTz, pRx, pRy, pRz;
INT          Intensite=0,prevInt=0;
INT          OnOffSouris3D=0,prevOnOffSouris3D=0;

/*--------------------------------------------------------------------------
* Fonction : Init3DMouse()
*
* Description : Appel SbInit et v?rifie si l'initialisation de la souris 3D
* c'est bien pass?
*
* Arguments : aucun
*
* Valeur retourn?e : aucune
*--------------------------------------------------------------------------*/
void Init3DMouse() {
    int res;

    // Initialisation de la souris 3D
    res = SbInit();

    //Conditions de v?rification d'erreur
    if (res < 1) //Si erreur
    {
        QMessageBox error;
        error.setIcon(QMessageBox::Warning);
        error.setWindowIcon(QIcon("icon.png"));
        error.setText("Desole - Aucun peripherique 3Dconnexion pris en charge disponible"); //Ajout ? la boite QMessageBox
        error.exec(); //Affichage boite de dialogue

    }
    else { //Si pas d'erreur
        //Appel de la boucle
        DispatchLoopNT();
    }
}

/*--------------------------------------------------------------------------
* Fonction : SbInit()
*
* Description : Initialisation de la souris 3D
*
* Arguments : aucun
*
* Valeur retourn?e : res : r?sultat de SiOpen, =0 si erreur =1 si ?a marche
*--------------------------------------------------------------------------*/
int SbInit()
{
    int res; //r?sultat de SiOpen, ? retourner
    SiOpenDataEx oData; //Donn?es ind?pendantes du syst?me d'exploitation

    //Initialisation de la biblioth?que d'entr?es SpaceWare
    if (SiInitialize() == SPW_DLL_LOAD_ERROR)
    {
        //Message d'erreur
        QMessageBox error;
        error.setIcon(QMessageBox::Warning);
        error.setWindowIcon(QIcon("icon.png"));
        error.setText("Erreur : Impossible de charger la librairie SiAppDll"); //Ajout ? la boite QMessageBox
        error.exec(); //Affichage boite de dialogue
    }

    //Initialisation des donn?es sp?cifiques ? la platfomre Windows
    SiOpenWinInitEx(&oData, hWndMain);

    //Dit au pilote que nous voulons recevoir des V3DCMD au lieu de V3DKeys
    //SiOpenWinAddHintBoolEnum(&oData, SI_HINT_USESV3DCMDS, SPW_TRUE);

    //Dit au driver que nous avons besoin d'une version minimale du driver de 17.5.5.
    //Cela pourrait ?tre utilis? pour indiquer au driver qu'il doit ?tre mis ? niveau avant de pouvoir ex?cuter correctement cette application.
    SiOpenWinAddHintStringEnum(&oData, SI_HINT_DRIVERVERSION, (WCHAR*)"17.5.5");

    //Ouverture des donn?es
    if ((devHdl = SiOpenEx(L"Logiciel de navigation 3D dans les images IRM", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData)) == NULL)
    {
        SiTerminate();  //Appel? pour fermer la biblioth?que d'entr?e SpaceWare

        //Impossible d'ouvrir l'appareil
        res = 0;
        return res;

        //Message d'erreur
        QMessageBox error;
        error.setIcon(QMessageBox::Warning);
        error.setWindowIcon(QIcon("icon.png"));
        error.setText("Erreur : Probl?me avec l'initialisation de la souris 3D"); //Ajout ? la boite QMessageBox
        error.exec(); //Affichage boite de dialogue
    }
    else
    {
        SiDeviceName devName;
        SiGetDeviceName(devHdl, &devName);

        //Appareil ouvert avec succ?s
        res = 1;
        return res;
    }



}

/*--------------------------------------------------------------------------
* Fonction : DispatchLoopNT()
*
* Description : Contient la boucle de message principale qui v?rifie
* constamment les ?v?nements de souris 3D et les g?re de mani?re appropri?e
*
* Arguments : aucun
*
* Valeur retourn?e :  msg.wparam : ?v?nement pass? ? la fen?tre
*--------------------------------------------------------------------------*/
int DispatchLoopNT()
{
    MSG            msg; //Message entrant ? ?valuer
    BOOL           handled; //Le message est-il encore g?r? ?
    SiSpwEvent     Event; //?v?nement SpaceWare
    SiGetEventData EData; //Donn?es d'?v?nement SpaceWare

    //Initialisation du handled
    handled = SPW_FALSE;

    //Boucle de message
    while (GetMessage(&msg, NULL, 0, 0))
    {
        handled = SPW_FALSE;

        //Init des donn?es sp?cifiques ? la plateforme Windows pour un appel ? SiGetEvent
        SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);

        //V?rification si le msg ?tait un ?v?nement de souris 3D et le traiter
        if (SiGetEvent(devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_IS_EVENT)
        {
            if (Event.type == SI_MOTION_EVENT)
            {
                //Processus ?v?nement de mouvements de la souris 3D
                SbMotionEvent(&Event);
            }

            //?v?nement de souris 3D g?r?
            handled = SPW_TRUE;
        }


        //Si c'est pas un ?v?nement de souris 3D, laissez Windows le g?rer
        if (handled == SPW_FALSE)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return((int)msg.wParam);
}


/*--------------------------------------------------------------------------
* Fonction : SbMotionEvent()
*
* Description : R?cup?ration des informations de mouvement et stokage dans
* des variables globales
*
* Arguments : *pEvent : Contient des donn?es d'un ?v?nement de souris 3D
*
* Valeur retourn?e :  aucune
*--------------------------------------------------------------------------*/
void SbMotionEvent(SiSpwEvent* pEvent)
{
    pTx = pEvent->u.spwData.mData[SI_TX];
    pTy = pEvent->u.spwData.mData[SI_TY];
    pTz = pEvent->u.spwData.mData[SI_TZ];
    pRx = pEvent->u.spwData.mData[SI_RX];
    pRy = pEvent->u.spwData.mData[SI_RY];
    pRz = pEvent->u.spwData.mData[SI_RZ];
}
