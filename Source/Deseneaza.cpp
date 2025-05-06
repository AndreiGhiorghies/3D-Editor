#include <winbgim.h>
#include "../Headers/Structuri.h"
//#include "../Headers/Functionalitati.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <set>
#include <algorithm>
#include <string.h>
using namespace std; 
Punct2D Transforma2D(const Punct3D &punct);

void AfiseazaText(const Obiect &obj, const int culoare) {
    char ss[200];
    setcolor(culoare);

    sprintf(ss, "RotatieX = %0.2f", obj.rotatieX);
    outtextxy(10, 10, ss);
    sprintf(ss, "RotatieY = %0.2f", obj.rotatieY);
    outtextxy(10, 30, ss);
    sprintf(ss, "RotatieZ = %0.2f", obj.rotatieZ);
    outtextxy(10, 50, ss);
    sprintf(ss, "Zoom = %0.2f", zoom);
    outtextxy(10, 70, ss);
    sprintf(ss, "MarimeX = %0.2f", obj.marimeX);
    outtextxy(10, 90, ss);
    sprintf(ss, "MarimeY = %0.2f", obj.marimeY);
    outtextxy(10, 110, ss);
    sprintf(ss, "MarimeZ = %0.2f", obj.marimeZ);
    outtextxy(10, 130, ss);
    sprintf(ss, "PozitieX = %0.2f", obj.deplasareX);
    outtextxy(10, 150, ss);
    sprintf(ss, "PozitieY = %0.2f", obj.deplasareY);
    outtextxy(10, 170, ss);
    sprintf(ss, "PozitieZ = %0.2f", obj.deplasareZ);
    outtextxy(10, 190, ss);

    sprintf(ss, "X - GALBEN");
    outtextxy(10, 730, ss);
    sprintf(ss, "Y - ROSU");
    outtextxy(10, 750, ss);
    sprintf(ss, "Z - VERDE");
    outtextxy(10, 770, ss);
    setcolor(WHITE);
}
void isHover(const int i) {
    if(i == butonHover)
        setbkcolor(COLOR(70, 70, 70));
    else
        setbkcolor(COLOR(50, 50, 50));
}
void DeseneazaBaraComenzi() {
    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));

    bar(0, 0, lungimeFereastra, inaltimeFereastra / 35);

    char ss[200];
    setcolor(WHITE);
    int inaltime = 4;
    
    isHover(0);
    sprintf(ss, " File ");
    outtextxy(8, inaltime, ss);
    coordonateButoaneHover[0] = {8, inaltime, 8 + textwidth((char*) " File "), inaltime + textheight((char*) " File ")};
    //cout << coordonateInputBox[0][0] << ' ' << coordonateInputBox[0][1] << ' ' << coordonateInputBox[0][2] << ' ' << coordonateInputBox[0][3] << endl;

    isHover(1);
    if(fereastraDreapta == 0) {
        sprintf(ss, " Ascuns ");
        coordonateButoaneHover[1] = {1380, inaltime, 1380 + textwidth((char*) " Ascuns "), inaltime + textheight((char*) " Ascuns ")};
    }
    else if(fereastraDreapta == 1) {
        sprintf(ss, " Puncte ");
        coordonateButoaneHover[1] = {1380, inaltime, 1380 + textwidth((char*) " Puncte "), inaltime + textheight((char*) " Puncte ")};
    }
    else if(fereastraDreapta == 2) {
        sprintf(ss, " Muchii/Fete ");
        coordonateButoaneHover[1] = {1380, inaltime, 1380 + textwidth((char*) " Muchii/Fete "), inaltime + textheight((char*) " Muchii/Fete ")};
    }
    else if(fereastraDreapta == 3) {
        sprintf(ss, " Obiect ");
        coordonateButoaneHover[1] = {1380, inaltime, 1380 + textwidth((char*) " Obiect "), inaltime + textheight((char*) " Obiect ")};
    }
    outtextxy(1380, inaltime, ss);

    isHover(5);
    if(axeObiectSelectat == 0) {
        sprintf(ss, " Ascuns ");
        coordonateButoaneHover[5] = {1470, inaltime, 1470 + textwidth((char*) " Ascuns "), inaltime + textheight((char*) " Ascuns ")};
    }
    else if(axeObiectSelectat == 1) {
        sprintf(ss, " Obiect ");
        coordonateButoaneHover[5] = {1470, inaltime, 1470 + textwidth((char*) " Obiect "), inaltime + textheight((char*) " Obiect ")};
    }
    else if(axeObiectSelectat == 2) {
        sprintf(ss, " Fixat ");
        coordonateButoaneHover[5] = {1470, inaltime, 1470 + textwidth((char*) " Fixat "), inaltime + textheight((char*) " Fixat ")};
    }
    outtextxy(1470, inaltime, ss);
    
    if(fereastraFile) {
        setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
        setbkcolor(COLOR(50, 50, 50));

        bar(0, inaltimeFereastra / 35, 100, inaltimeFereastra / 35 + 130);

        isHover(2);
        coordonateButoaneHover[2] = {0, inaltimeFereastra / 35, 100, inaltimeFereastra / 35 + 25};
        if(butonHover == 2) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[2][0], coordonateButoaneHover[2][1], coordonateButoaneHover[2][2], coordonateButoaneHover[2][3]);
        }
        sprintf(ss, " Deschide ");
        outtextxy(50 - textwidth((char*) " Deschide ") / 2, inaltimeFereastra / 35 + textheight((char*) " Deschide ") / 2 - 3, ss);

        isHover(3);
        coordonateButoaneHover[3] = {0, inaltimeFereastra / 35 + 25, 100, inaltimeFereastra / 35 + 50};
        if(butonHover == 3) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[3][0], coordonateButoaneHover[3][1], coordonateButoaneHover[3][2], coordonateButoaneHover[3][3]);
        }
        sprintf(ss, " Salveaza ");
        outtextxy(50 - textwidth((char*) " Salveaza ") / 2, inaltimeFereastra / 35 + textheight((char*) " Salveaza ") / 2 - 3 + 25, ss);
        
        isHover(6);
        coordonateButoaneHover[6] = {0, inaltimeFereastra / 35 + 50, 100, inaltimeFereastra / 35 + 75};
        if(butonHover == 6) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[6][0], coordonateButoaneHover[6][1], coordonateButoaneHover[6][2], coordonateButoaneHover[6][3]);
        }
        sprintf(ss, " Undo ");
        outtextxy(50 - textwidth((char*) " Undo ") / 2, inaltimeFereastra / 35 + textheight((char*) " Undo ") / 2 - 3 + 50, ss);
        
        isHover(7);
        coordonateButoaneHover[7] = {0, inaltimeFereastra / 35 + 75, 100, inaltimeFereastra / 35 + 100};
        if(butonHover == 7) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[7][0], coordonateButoaneHover[7][1], coordonateButoaneHover[7][2], coordonateButoaneHover[7][3]);
        }
        sprintf(ss, " Redo ");
        outtextxy(50 - textwidth((char*) " Redo ") / 2, inaltimeFereastra / 35 + textheight((char*) " Redo ") / 2 - 3 + 75, ss);

        isHover(4);
        coordonateButoaneHover[4] = {0, inaltimeFereastra / 35 + 100, 100, inaltimeFereastra / 35 + 130};
        if(butonHover == 4) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[4][0], coordonateButoaneHover[4][1], coordonateButoaneHover[4][2], coordonateButoaneHover[4][3]);
        }
        sprintf(ss, " Inchide ");
        outtextxy(50 - textwidth((char*) " Inchide ") / 2, inaltimeFereastra / 35 + textheight((char*) " Inchide ") / 2 - 3 + 100, ss);

        setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
        bar(100 - 5, inaltimeFereastra / 35, 100, inaltimeFereastra / 35 + 130);
        bar(0, inaltimeFereastra / 35 + 125, 100, inaltimeFereastra / 35 + 130);

    }

    isHover(8);
    if(butonGrid == 0) {
        sprintf(ss, " Ascuns ");
        coordonateButoaneHover[8] = {1300, inaltime, 1300 + textwidth((char*) " Ascuns "), inaltime + textheight((char*) " Ascuns ")};
    }
    else if(butonGrid == 2) {
        sprintf(ss, " Dinamic ");
        coordonateButoaneHover[8] = {1300, inaltime, 1300 + textwidth((char*) " Dinamic "), inaltime + textheight((char*) " Dinamic ")};
    }
    else if(butonGrid == 1) {
        sprintf(ss, " Fixat ");
        coordonateButoaneHover[8] = {1300, inaltime, 1300 + textwidth((char*) " Fixat "), inaltime + textheight((char*) " Fixat ")};
    }
    outtextxy(1300, inaltime, ss);

    isHover(9);
    if(local) {
        sprintf(ss, " Local ");
        coordonateButoaneHover[9] = {1230, inaltime, 1230 + textwidth((char*) " Local "), inaltime + textheight((char*) " Local ")};
    }
    else {
        sprintf(ss, " Global ");
        coordonateButoaneHover[9] = {1230, inaltime, 1230 + textwidth((char*) " Global "), inaltime + textheight((char*) " Global ")};
    }
    outtextxy(1230, inaltime, ss);

    isHover(34);
    if(fereastraStanga == 0) {
        sprintf(ss, " Ascuns ");
        coordonateButoaneHover[34] = {70, inaltime, 70 + textwidth((char*) " Ascuns "), inaltime + textheight((char*) " Ascuns ")};
    }
    else if(fereastraStanga == 1) {
        sprintf(ss, " Puncte ");
        coordonateButoaneHover[34] = {70, inaltime, 70 + textwidth((char*) " Puncte "), inaltime + textheight((char*) " Puncte ")};
    }
    else if(fereastraStanga == 2) {
        sprintf(ss, " Fete ");
        coordonateButoaneHover[34] = {70, inaltime, 70 + textwidth((char*) " Fete "), inaltime + textheight((char*) " Fete ")};
    }
    else if(fereastraStanga == 3) {
        sprintf(ss, " Obiecte ");
        coordonateButoaneHover[34] = {70, inaltime, 70 + textwidth((char*) " Obiecte "), inaltime + textheight((char*) " Obiecte ")};
    }
    outtextxy(70, inaltime, ss);

    isHover(35);
    sprintf(ss, " Adauga ");
    outtextxy(165, inaltime, ss);
    coordonateButoaneHover[35] = {165, inaltime, 165 + textwidth((char*) " Adauga "), inaltimeFereastra / 35};

    if(fereastraAdauga) {
        setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
        setbkcolor(COLOR(50, 50, 50));

        bar(140, inaltimeFereastra / 35, 240, inaltimeFereastra / 35 + 155);

        isHover(36);
        coordonateButoaneHover[36] = {140, inaltimeFereastra / 35, 240, inaltimeFereastra / 35 + 25};
        if(butonHover == 36) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[36][0], coordonateButoaneHover[36][1], coordonateButoaneHover[36][2], coordonateButoaneHover[36][3]);
        }
        sprintf(ss, " Empty ");
        outtextxy(190 - textwidth((char*) " Empty ") / 2, inaltimeFereastra / 35 + textheight((char*) " Empty ") / 2 - 3, ss);

        isHover(37);
        coordonateButoaneHover[37] = {140, inaltimeFereastra / 35 + 25, 240, inaltimeFereastra / 35 + 50};
        if(butonHover == 37) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[37][0], coordonateButoaneHover[37][1], coordonateButoaneHover[37][2], coordonateButoaneHover[37][3]);
        }
        sprintf(ss, " Cub ");
        outtextxy(190 - textwidth((char*) " Cub ") / 2, inaltimeFereastra / 35 + textheight((char*) " Cub ") / 2 - 3 + 25, ss);
        
        isHover(38);
        coordonateButoaneHover[38] = {140, inaltimeFereastra / 35 + 50, 240, inaltimeFereastra / 35 + 75};
        if(butonHover == 38) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[38][0], coordonateButoaneHover[38][1], coordonateButoaneHover[38][2], coordonateButoaneHover[38][3]);
        }
        sprintf(ss, " Cilindru ");
        outtextxy(190 - textwidth((char*) " Cilindru ") / 2, inaltimeFereastra / 35 + textheight((char*) " Cilindru ") / 2 - 3 + 50, ss);
        
        isHover(39);
        coordonateButoaneHover[39] = {140, inaltimeFereastra / 35 + 75, 240, inaltimeFereastra / 35 + 100};
        if(butonHover == 39) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[39][0], coordonateButoaneHover[39][1], coordonateButoaneHover[39][2], coordonateButoaneHover[39][3]);
        }
        sprintf(ss, " Con ");
        outtextxy(190 - textwidth((char*) " Con ") / 2, inaltimeFereastra / 35 + textheight((char*) " Con ") / 2 - 3 + 75, ss);

        isHover(40);
        coordonateButoaneHover[40] = {140, inaltimeFereastra / 35 + 100, 240, inaltimeFereastra / 35 + 125};
        if(butonHover == 40) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[40][0], coordonateButoaneHover[40][1], coordonateButoaneHover[40][2], coordonateButoaneHover[40][3]);
        }
        sprintf(ss, " Sfera ");
        outtextxy(190 - textwidth((char*) " Sfera ") / 2, inaltimeFereastra / 35 + textheight((char*) " Sfera ") / 2 - 3 + 100, ss);

        isHover(41);
        coordonateButoaneHover[41] = {140, inaltimeFereastra / 35 + 125, 240, inaltimeFereastra / 35 + 155};
        if(butonHover == 41) {
            setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
            bar(coordonateButoaneHover[41][0], coordonateButoaneHover[41][1], coordonateButoaneHover[41][2], coordonateButoaneHover[41][3]);
        }
        sprintf(ss, " Masca ");
        outtextxy(190 - textwidth((char*) " Masca ") / 2, inaltimeFereastra / 35 + textheight((char*) " Masca ") / 2 - 3 + 125, ss);

        setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
        bar(235, inaltimeFereastra / 35, 240, inaltimeFereastra / 35 + 155);
        bar(140, inaltimeFereastra / 35, 145, inaltimeFereastra / 35 + 155);
        bar(140, inaltimeFereastra / 35 + 150, 240, inaltimeFereastra / 35 + 155);

    }

    setbkcolor(BLACK);
}
void DeseneazaCasutaInput(const Obiect &obj, char* text, int i, int mijlocLatime, int mijlocInaltime, int lungimeInputBox, int inaltimeInputBox, int limitaInaltime) {
    setbkcolor(COLOR(50, 50, 50));
    char ss[10];
    sprintf(ss, text);
    outtextxy(mijlocLatime - lungimeInputBox / 2 - textwidth((char*) text), limitaInaltime + 30, ss);
    setbkcolor(COLOR(70, 70, 70));
    setcolor(COLOR(20, 20, 20));
    rectangle(mijlocLatime - lungimeInputBox / 2, mijlocInaltime - inaltimeInputBox / 2, mijlocLatime + lungimeInputBox / 2, mijlocInaltime + inaltimeInputBox / 2);
    coordonateInputBox[i] = {mijlocLatime - lungimeInputBox / 2 + 1, mijlocInaltime - inaltimeInputBox / 2 + 1, mijlocLatime + lungimeInputBox / 2 - 1, mijlocInaltime + inaltimeInputBox / 2 - 1};
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime - lungimeInputBox / 2 + 1, mijlocInaltime - inaltimeInputBox / 2 + 1, mijlocLatime + lungimeInputBox / 2 - 1, mijlocInaltime + inaltimeInputBox / 2 - 1);
    setcolor(WHITE);
    if(varfSelectat != -1 && i < 9 && !unestePuncte && !eliminaPuncte && selectareInput != i) {
        char text[10] = "";
        int j = 0;
        string s;
        if(i % 3 == 0)
            if(obj.varfuri[varfSelectat].x > -0.01 && obj.varfuri[varfSelectat].x < 0.01)
                s = "0.00";
            else
                s = to_string(obj.varfuri[varfSelectat].x * 10);
        else if(i % 3 == 1)
            if(obj.varfuri[varfSelectat].y > -0.01 && obj.varfuri[varfSelectat].y < 0.01)
                s = "0.00";
            else
                s = to_string(-obj.varfuri[varfSelectat].y * 10);
        else
            if(obj.varfuri[varfSelectat].z > -0.01 && obj.varfuri[varfSelectat].z < 0.01)
                s = "0.00";
            else
                s = to_string(obj.varfuri[varfSelectat].z * 10);
        while(j < s.length() && s[j] != '.') {
            text[j] = s[j];
            j++;
        }
        text[j] = '.';
        text[j + 1] = s[j + 1];
        text[j + 2] = s[j + 2];
        text[j + 3] = '\0';
        sprintf(ss, text);
        outtextxy(mijlocLatime - textwidth(ss) / 2, mijlocInaltime - textheight(ss) / 2, ss);
    }
    else if(i >= 11 && i <= 13 && colorarePuncteSelectate) {
        char text[10] = "";
        int j = 0;
        string s;
        if(i == 11)
            s = to_string(culoarePuncteSelectate[0]);
        else if(i == 12)
            s = to_string(culoarePuncteSelectate[1]);
        else
            s = to_string(culoarePuncteSelectate[2]);
        while(j < s.length()) {
            text[j] = s[j];
            j++;
        }
        text[j] = '\0';
        sprintf(ss, text);
        outtextxy(mijlocLatime - textwidth(ss) / 2, mijlocInaltime - textheight(ss) / 2, ss);
    }
    else if(i >= 22 && i <= 24) {
        char text[10] = "";
        int j = 0;
        string s;
        if(i == 22)
            s = to_string(culoarePuncte[0]);
        else if(i == 23)
            s = to_string(culoarePuncte[1]);
        else
            s = to_string(culoarePuncte[2]);
        while(j < s.length()) {
            text[j] = s[j];
            j++;
        }
        text[j] = '\0';
        sprintf(ss, text);
        outtextxy(mijlocLatime - textwidth(ss) / 2, mijlocInaltime - textheight(ss) / 2, ss);
    }
    else if(i >= 26 && i <= 28) {
        char text[10] = "";
        int j = 0;
        string s;
        if(i == 26)
            s = to_string(culoareMuchii[0]);
        else if(i == 27)
            s = to_string(culoareMuchii[1]);
        else
            s = to_string(culoareMuchii[2]);
        while(j < s.length()) {
            text[j] = s[j];
            j++;
        }
        text[j] = '\0';
        sprintf(ss, text);
        outtextxy(mijlocLatime - textwidth(ss) / 2, mijlocInaltime - textheight(ss) / 2, ss);
    }
    else if(i >= 30 && i <= 32 && fataSelectata != -1) {
        char text[10] = "";
        int j = 0;
        string s;
        if(i == 30)
            s = to_string(obj.culoareFete[fataSelectata][0]);
        else if(i == 31)
            s = to_string(obj.culoareFete[fataSelectata][1]);
        else
            s = to_string(obj.culoareFete[fataSelectata][2]);
        while(j < s.length()) {
            text[j] = s[j];
            j++;
        }
        text[j] = '\0';
        sprintf(ss, text);
        outtextxy(mijlocLatime - textwidth(ss) / 2, mijlocInaltime - textheight(ss) / 2, ss);
    }
    else if(i >= 37 && i <= 39) {
        char text[10] = "";
        int j = 0;
        string s;
        if(i == 37)
            if(obj.centru.x  > -0.01 && obj.centru.x < 0.01)
                s = "0.00";
            else
                s = to_string(obj.centru.x * 10);
        else if(i == 38)
            if(obj.centru.y  > -0.01 && obj.centru.y < 0.01)
                s = "0.00";
            else
                s = to_string(-obj.centru.y * 10);
        else if(obj.centru.z  > -0.01 && obj.centru.z < 0.01)
                s = "0.00";
            else
                s = to_string(obj.centru.z * 10);
        while(j < s.length() && s[j] != '.') {
            text[j] = s[j];
            j++;
        }
        text[j] = '.';
        text[j + 1] = s[j + 1];
        text[j + 2] = s[j + 2];
        text[j + 3] = '\0';
        sprintf(ss, text);
        outtextxy(mijlocLatime - textwidth(ss) / 2, mijlocInaltime - textheight(ss) / 2, ss);
    }
    else if(i >= 41 && i <= 43) {
        char text[10] = "";
        int j = 0;
        string s;
        if(i == 41)
            if(obj.rotatieX > -0.01 && obj.rotatieX < 0.01)
                s = "0.00";
            else if(!max360)
                s = to_string(obj.rotatieX * 180 / 3.14);
            else {
                double unghi;
                double fractie = modf(obj.rotatieX * 180 / 3.14, &unghi);
                unghi = (int) unghi % 360;
                if(unghi < 0)
                    unghi += 360;
                s = to_string(unghi + fractie);
            }
        else if(i == 42)
            if(obj.rotatieY > -0.01 && obj.rotatieY < 0.01)
                s = "0.00";
            else if(!max360)
                s = to_string(obj.rotatieY * 180 / 3.14);
            else {
                double unghi;
                double fractie = modf(obj.rotatieY * 180 / 3.14, &unghi);
                unghi = (int) unghi % 360;
                if(unghi < 0)
                    unghi += 360;
                s = to_string(unghi + fractie);
            }
        else
            if(obj.rotatieZ > -0.01 && obj.rotatieZ < 0.01)
                s = "0.00";
            else if(!max360)
                s = to_string(obj.rotatieZ * 180 / 3.14);
            else {
                double unghi;
                double fractie = modf(obj.rotatieZ * 180 / 3.14, &unghi);
                unghi = (int) unghi % 360;
                if(unghi < 0)
                    unghi += 360;
                s = to_string(unghi + fractie);
            }
        
        while(j < s.length() && s[j] != '.') {
            text[j] = s[j];
            j++;
        }
        text[j] = '.';
        text[j + 1] = s[j + 1];
        text[j + 2] = s[j + 2];
        text[j + 3] = '\0';
        sprintf(ss, text);
        outtextxy(mijlocLatime - textwidth(ss) / 2, mijlocInaltime - textheight(ss) / 2, ss);
    }
    else if(i >= 45 && i <= 47) {
        char text[10] = "";
        int j = 0;
        string s;
        if(i == 45)
            if(obj.marimeX  > -0.01 && obj.marimeX < 0.01)
                s = "0.00";
            else
                s = to_string(obj.marimeX);
        else if(i == 46)
            if(obj.marimeY  > -0.01 && obj.marimeY < 0.01)
                s = "0.00";
            else
                s = to_string(obj.marimeY);
        else
            if(obj.marimeZ  > -0.01 && obj.marimeZ < 0.01)
                s = "0.00";
            else
                s = to_string(obj.marimeZ);
        while(j < s.length() && s[j] != '.') {
            text[j] = s[j];
            j++;
        }
        text[j] = '.';
        text[j + 1] = s[j + 1];
        text[j + 2] = s[j + 2];
        text[j + 3] = '\0';
        sprintf(ss, text);
        outtextxy(mijlocLatime - textwidth(ss) / 2, mijlocInaltime - textheight(ss) / 2, ss);
    }
    else if(i >= 48 && i <= 50) {
        char text[10] = "";
        int j = 0;
        string s;
        if(i == 48)
            s = to_string(coeficientDeplasare);
        else if(i == 49)
            s = to_string(unghiRotatie * 180 / 3.14);
        else
            s = to_string(coeficientMarire);
        while(j < s.length() && s[j] != '.') {
            text[j] = s[j];
            j++;
        }
        text[j] = '.';
        text[j + 1] = s[j + 1];
        text[j + 2] = s[j + 2];
        text[j + 3] = '\0';
        sprintf(ss, text);
        outtextxy(mijlocLatime - textwidth(ss) / 2, mijlocInaltime - textheight(ss) / 2, ss);
    }
}
void DeseneazaBaraLateralaDreaptaPuncte(const Obiect &obj) {
    int lungimeInputBox = 100, inaltimeInputBox = 25;
    char ss[200];

    setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
    int limitaStanga = lungimeFereastra - 250, limitaInaltime = 40;
    bar(limitaStanga, inaltimeFereastra / 35, lungimeFereastra, inaltimeFereastra);

    int mijlocLatime = limitaStanga + (lungimeFereastra - limitaStanga) / 2;
    int mijlocInaltime = limitaInaltime + 3 * inaltimeInputBox / 2;

    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, inaltimeFereastra / 35, limitaStanga + 7, inaltimeFereastra);

    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "PUNCTE:  ");
    outtextxy(mijlocLatime - 40, limitaInaltime, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 30, mijlocInaltime - 40, mijlocLatime + 50, mijlocInaltime - 20);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 31, mijlocInaltime - 39, mijlocLatime + 49, mijlocInaltime - 21);
    coordonateInputBox[21] = {mijlocLatime + 31, mijlocInaltime - 39, mijlocLatime + 49, mijlocInaltime - 21};
    if(deseneazaPuncte) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 40, mijlocInaltime - 30, j);
    }

    mijlocInaltime += 15;
    limitaInaltime += 15;
    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, mijlocInaltime - 15, lungimeFereastra, mijlocInaltime - 20);

    mijlocInaltime += 35;
    limitaInaltime += 35;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "POZITIE");
    outtextxy(mijlocLatime - textwidth((char*) "POZITIE") / 2, limitaInaltime, ss);

    DeseneazaCasutaInput(obj, (char*) "X:  ", 0, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);
    mijlocInaltime += 35;
    limitaInaltime += 35;
    DeseneazaCasutaInput(obj, (char*) "Y:  ", 1, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);
    mijlocInaltime += 35;
    limitaInaltime += 35;
    DeseneazaCasutaInput(obj, (char*) "Z:  ", 2, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);

    mijlocInaltime += 20;
    limitaInaltime += 20;
    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, mijlocInaltime + 15, lungimeFereastra, mijlocInaltime + 10);

    mijlocInaltime += 30;
    limitaInaltime += 30;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "CULOARE PUNCTE");
    outtextxy(mijlocLatime - textwidth((char*) "CULOARE PUNCTE") / 2, limitaInaltime + 35, ss);

    mijlocInaltime += 35;
    limitaInaltime += 35;
    DeseneazaCasutaInput(obj, (char*) "R:  ", 22, mijlocLatime - 67, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
    DeseneazaCasutaInput(obj, (char*) "G:  ", 23, mijlocLatime + 13, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
    DeseneazaCasutaInput(obj, (char*) "B:  ", 24, mijlocLatime + 93, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);

    mijlocInaltime += 35;
    limitaInaltime += 35;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "AXE:  ");
    outtextxy(mijlocLatime - 40, limitaInaltime + 30, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 5, mijlocInaltime - 10, mijlocLatime + 25, mijlocInaltime + 10);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 6, mijlocInaltime - 9, mijlocLatime + 24, mijlocInaltime + 9);
    coordonateInputBox[9] = {mijlocLatime + 6, mijlocInaltime - 9, mijlocLatime + 24, mijlocInaltime + 9};
    if(axePunct) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 15, mijlocInaltime, j);
    }

    mijlocInaltime += 35;
    limitaInaltime += 35;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "CULOARE PCT SELECT:  ");
    outtextxy(mijlocLatime - 95, limitaInaltime + 30, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 70, mijlocInaltime - 10, mijlocLatime + 90, mijlocInaltime + 10);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 71, mijlocInaltime - 9, mijlocLatime + 89, mijlocInaltime + 9);
    coordonateInputBox[10] = {mijlocLatime + 71, mijlocInaltime - 9, mijlocLatime + 89, mijlocInaltime + 9};
    if(colorarePuncteSelectate) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 80, mijlocInaltime, j);
    }

    if(colorarePuncteSelectate) {
        mijlocInaltime += 35;
        limitaInaltime += 35;
        setcolor(WHITE);
        setbkcolor(COLOR(50, 50, 50));
        sprintf(ss, (char*) "CULOARE");
        outtextxy(mijlocLatime - textwidth((char*) "CULOARE") / 2, limitaInaltime + 30, ss);
        mijlocInaltime += 35;
        limitaInaltime += 35;
        DeseneazaCasutaInput(obj, (char*) "R:  ", 11, mijlocLatime - 67, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
        DeseneazaCasutaInput(obj, (char*) "G:  ", 12, mijlocLatime + 13, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
        DeseneazaCasutaInput(obj, (char*) "B:  ", 13, mijlocLatime + 93, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
    }

    mijlocInaltime += 35;
    limitaInaltime += 35;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "SELECTARE MULTIPLA:  ");
    outtextxy(mijlocLatime - 96, limitaInaltime + 30, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 70, mijlocInaltime - 10, mijlocLatime + 90, mijlocInaltime + 10);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 71, mijlocInaltime - 9, mijlocLatime + 89, mijlocInaltime + 9);
    coordonateInputBox[14] = {mijlocLatime + 71, mijlocInaltime - 9, mijlocLatime + 89, mijlocInaltime + 9};
    if(selectarePuncte) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 80, mijlocInaltime, j);
    }

    mijlocInaltime += 15;
    limitaInaltime += 15;
    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, mijlocInaltime + 15, lungimeFereastra, mijlocInaltime + 10);

    mijlocInaltime += 35;
    limitaInaltime += 35;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "UNIRE PUNCTE");
    outtextxy(mijlocLatime - textwidth((char*) "UNIRE PUNCTE") / 2, limitaInaltime + 30, ss);

    mijlocInaltime += 35;
    limitaInaltime += 35;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "UNESTE:  ");
    outtextxy(mijlocLatime - 40, limitaInaltime + 30, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 30, mijlocInaltime - 10, mijlocLatime + 50, mijlocInaltime + 10);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 31, mijlocInaltime - 9, mijlocLatime + 49, mijlocInaltime + 9);
    coordonateInputBox[17] = {mijlocLatime + 31, mijlocInaltime - 9, mijlocLatime + 49, mijlocInaltime + 9};
    if(unestePuncte) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 40, mijlocInaltime, j);
    }

    mijlocInaltime += 35;
    limitaInaltime += 35;
    setcolor(COLOR(90, 90, 90));
    rectangle(mijlocLatime - textwidth((char*) "PRIMUL -> FIECARE") / 2 - 6, mijlocInaltime - textheight((char*) "PRIMUL -> FIECARE") / 2 - 6, mijlocLatime + textwidth((char*) "PRIMUL -> FIECARE") / 2 + 6, mijlocInaltime + textheight((char*) "PRIMUL -> FIECARE") / 2 + 6);
    setfillstyle(SOLID_FILL, COLOR(65, 65, 65));
    bar(mijlocLatime - textwidth((char*) "PRIMUL -> FIECARE") / 2 - 5, mijlocInaltime - textheight((char*) "PRIMUL -> FIECARE") / 2 - 5, mijlocLatime + textwidth((char*) "PRIMUL -> FIECARE") / 2 + 5, mijlocInaltime + textheight((char*) "PRIMUL -> FIECARE") / 2 + 5);
    setcolor(WHITE);
    setbkcolor(COLOR(65, 65, 65));
    sprintf(ss, (char*) "PRIMUL -> FIECARE");
    outtextxy(mijlocLatime - textwidth((char*) "PRIMUL -> FIECARE") / 2, limitaInaltime + 30, ss);
    coordonateInputBox[15] = {mijlocLatime - textwidth((char*) "PRIMUL -> FIECARE") / 2 - 5, mijlocInaltime - textheight((char*) "PRIMUL -> FIECARE") / 2 - 5, mijlocLatime + textwidth((char*) "PRIMUL -> FIECARE") / 2 + 5, mijlocInaltime + textheight((char*) "PRIMUL -> FIECARE") / 2 + 5};

    mijlocInaltime += 45;
    limitaInaltime += 45;
    setcolor(COLOR(90, 90, 90));
    rectangle(mijlocLatime - textwidth((char*) "CONSECUTIVE") / 2 - 6, mijlocInaltime - textheight((char*) "CONSECUTIVE") / 2 - 6, mijlocLatime + textwidth((char*) "CONSECUTIVE") / 2 + 6, mijlocInaltime + textheight((char*) "CONSECUTIVE") / 2 + 6);
    setfillstyle(SOLID_FILL, COLOR(65, 65, 65));
    bar(mijlocLatime - textwidth((char*) "CONSECUTIVE") / 2 - 5, mijlocInaltime - textheight((char*) "CONSECUTIVE") / 2 - 5, mijlocLatime + textwidth((char*) "CONSECUTIVE") / 2 + 5, mijlocInaltime + textheight((char*) "CONSECUTIVE") / 2 + 5);
    setcolor(WHITE);
    setbkcolor(COLOR(65, 65, 65));
    sprintf(ss, (char*) "CONSECUTIVE");
    outtextxy(mijlocLatime - textwidth((char*) "CONSECUTIVE") / 2, limitaInaltime + 30, ss);
    coordonateInputBox[16] = {mijlocLatime - textwidth((char*) "CONSECUTIVE") / 2 - 5, mijlocInaltime - textheight((char*) "CONSECUTIVE") / 2 - 5, mijlocLatime + textwidth((char*) "CONSECUTIVE") / 2 + 5, mijlocInaltime + textheight((char*) "CONSECUTIVE") / 2 + 5};

    mijlocInaltime += 20;
    limitaInaltime += 20;
    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, mijlocInaltime + 15, lungimeFereastra, mijlocInaltime + 10);

    mijlocInaltime += 40;
    limitaInaltime += 40;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "ADAUGA PUNCTE:  ");
    outtextxy(mijlocLatime - 80, limitaInaltime + 30, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 50, mijlocInaltime - 10, mijlocLatime + 70, mijlocInaltime + 10);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 51, mijlocInaltime - 9, mijlocLatime + 69, mijlocInaltime + 9);
    coordonateInputBox[18] = {mijlocLatime + 51, mijlocInaltime - 9, mijlocLatime + 69, mijlocInaltime + 9};
    if(adaugaPunct) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 60, mijlocInaltime, j);
    }

    mijlocInaltime += 40;
    limitaInaltime += 40;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "ELIMINA PUNCTE:  ");
    outtextxy(mijlocLatime - 80, limitaInaltime + 30, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 50, mijlocInaltime - 10, mijlocLatime + 70, mijlocInaltime + 10);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 51, mijlocInaltime - 9, mijlocLatime + 69, mijlocInaltime + 9);
    coordonateInputBox[19] = {mijlocLatime + 51, mijlocInaltime - 9, mijlocLatime + 69, mijlocInaltime + 9};
    if(eliminaPuncte) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 60, mijlocInaltime, j);
    }
}
void DeseneazaBaraLateralaDreaptaMuchii(const Obiect &obj) {
    int lungimeInputBox = 100, inaltimeInputBox = 25;
    char ss[200];

    setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
    int limitaStanga = lungimeFereastra - 250, limitaInaltime = 40;
    bar(limitaStanga, inaltimeFereastra / 35, lungimeFereastra, inaltimeFereastra);

    int mijlocLatime = limitaStanga + (lungimeFereastra - limitaStanga) / 2;
    int mijlocInaltime = limitaInaltime + 3 * inaltimeInputBox / 2;

    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, inaltimeFereastra / 35, limitaStanga + 7, inaltimeFereastra);

    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "MUCHII:  ");
    outtextxy(mijlocLatime - 40, limitaInaltime, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 30, mijlocInaltime - 40, mijlocLatime + 50, mijlocInaltime - 20);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 31, mijlocInaltime - 39, mijlocLatime + 49, mijlocInaltime - 21);
    coordonateInputBox[25] = {mijlocLatime + 31, mijlocInaltime - 39, mijlocLatime + 49, mijlocInaltime - 21};
    if(deseneazaMuchii) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 40, mijlocInaltime - 30, j);
    }

    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "CULOARE MUCHII");
    outtextxy(mijlocLatime - textwidth((char*) "CULOARE MUCHII") / 2, limitaInaltime + 35, ss);

    mijlocInaltime += 40;
    limitaInaltime += 40;
    DeseneazaCasutaInput(obj, (char*) "R:  ", 26, mijlocLatime - 67, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
    DeseneazaCasutaInput(obj, (char*) "G:  ", 27, mijlocLatime + 13, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
    DeseneazaCasutaInput(obj, (char*) "B:  ", 28, mijlocLatime + 93, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
    
    mijlocInaltime += 30;
    limitaInaltime += 30;
    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, mijlocInaltime + 5, lungimeFereastra, mijlocInaltime);

    mijlocInaltime += 55;
    limitaInaltime += 55;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "FETE:  ");
    outtextxy(mijlocLatime - 30, limitaInaltime, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 20, mijlocInaltime - 40, mijlocLatime + 40, mijlocInaltime - 20);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 21, mijlocInaltime - 39, mijlocLatime + 39, mijlocInaltime - 21);
    coordonateInputBox[29] = {mijlocLatime + 21, mijlocInaltime - 39, mijlocLatime + 39, mijlocInaltime - 21};
    if(deseneazaFete) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 30, mijlocInaltime - 30, j);
    }

    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "CULOARE FATA");
    outtextxy(mijlocLatime - textwidth((char*) "CULOARE FATA") / 2, limitaInaltime + 35, ss);

    mijlocInaltime += 40;
    limitaInaltime += 40;
    DeseneazaCasutaInput(obj, (char*) "R:  ", 30, mijlocLatime - 67, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
    DeseneazaCasutaInput(obj, (char*) "G:  ", 31, mijlocLatime + 13, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
    DeseneazaCasutaInput(obj, (char*) "B:  ", 32, mijlocLatime + 93, mijlocInaltime, lungimeInputBox - 50, inaltimeInputBox, limitaInaltime);
     
    mijlocInaltime += 45;
    limitaInaltime += 45;
    setcolor(COLOR(90, 90, 90));
    rectangle(mijlocLatime - textwidth((char*) " COPIE ") / 2 - 6, mijlocInaltime - textheight((char*) " COPIE ") / 2 - 6, mijlocLatime + textwidth((char*) " COPIE ") / 2 + 6, mijlocInaltime + textheight((char*) " COPIE ") / 2 + 6);
    setfillstyle(SOLID_FILL, COLOR(65, 65, 65));
    bar(mijlocLatime - textwidth((char*) " COPIE ") / 2 - 5, mijlocInaltime - textheight((char*) " COPIE ") / 2 - 5, mijlocLatime + textwidth((char*) " COPIE ") / 2 + 5, mijlocInaltime + textheight((char*) " COPIE ") / 2 + 5);
    setcolor(WHITE);
    setbkcolor(COLOR(65, 65, 65));
    sprintf(ss, (char*) " COPIE ");
    outtextxy(mijlocLatime - textwidth((char*) " COPIE ") / 2, limitaInaltime + 30, ss);
    coordonateInputBox[33] = {mijlocLatime - textwidth((char*) " COPIE ") / 2 - 5, mijlocInaltime - textheight((char*) " COPIE ") / 2 - 5, mijlocLatime + textwidth((char*) " COPIE ") / 2 + 5, mijlocInaltime + textheight((char*) " COPIE ") / 2 + 5};
    
    mijlocInaltime += 45;
    limitaInaltime += 45;
    setcolor(COLOR(90, 90, 90));
    rectangle(mijlocLatime - textwidth((char*) " LIPIRE ") / 2 - 6, mijlocInaltime - textheight((char*) " LIPIRE ") / 2 - 6, mijlocLatime + textwidth((char*) " LIPIRE ") / 2 + 6, mijlocInaltime + textheight((char*) " LIPIRE ") / 2 + 6);
    setfillstyle(SOLID_FILL, COLOR(65, 65, 65));
    bar(mijlocLatime - textwidth((char*) " LIPIRE ") / 2 - 5, mijlocInaltime - textheight((char*) " LIPIRE ") / 2 - 5, mijlocLatime + textwidth((char*) " LIPIRE ") / 2 + 5, mijlocInaltime + textheight((char*) " LIPIRE ") / 2 + 5);
    setcolor(WHITE);
    setbkcolor(COLOR(65, 65, 65));
    sprintf(ss, (char*) " LIPIRE ");
    outtextxy(mijlocLatime - textwidth((char*) " LIPIRE ") / 2, limitaInaltime + 30, ss);
    coordonateInputBox[34] = {mijlocLatime - textwidth((char*) " LIPIRE ") / 2 - 5, mijlocInaltime - textheight((char*) " LIPIRE ") / 2 - 5, mijlocLatime + textwidth((char*) " LIPIRE ") / 2 + 5, mijlocInaltime + textheight((char*) " LIPIRE ") / 2 + 5};

    mijlocInaltime += 45;
    limitaInaltime += 45;
    setcolor(COLOR(90, 90, 90));
    rectangle(mijlocLatime - textwidth((char*) "CREARE FATA") / 2 - 6, mijlocInaltime - textheight((char*) "CONSECUTIVE") / 2 - 6, mijlocLatime + textwidth((char*) "CREARE FATA") / 2 + 6, mijlocInaltime + textheight((char*) "CREARE FATA") / 2 + 6);
    setfillstyle(SOLID_FILL, COLOR(65, 65, 65));
    bar(mijlocLatime - textwidth((char*) "CREARE FATA") / 2 - 5, mijlocInaltime - textheight((char*) "CREARE FATA") / 2 - 5, mijlocLatime + textwidth((char*) "CREARE FATA") / 2 + 5, mijlocInaltime + textheight((char*) "CREARE FATA") / 2 + 5);
    setcolor(WHITE);
    setbkcolor(COLOR(65, 65, 65));
    sprintf(ss, (char*) "CREARE FATA");
    outtextxy(mijlocLatime - textwidth((char*) "CREARE FATA") / 2, limitaInaltime + 30, ss);
    coordonateInputBox[20] = {mijlocLatime - textwidth((char*) "CREARE FATA") / 2 - 5, mijlocInaltime - textheight((char*) "CREARE FATA") / 2 - 5, mijlocLatime + textwidth((char*) "CREARE FATA") / 2 + 5, mijlocInaltime + textheight((char*) "CREARE FATA") / 2 + 5};


    mijlocInaltime += 30;
    limitaInaltime += 30;
    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, mijlocInaltime + 5, lungimeFereastra, mijlocInaltime);

    mijlocInaltime += 60;
    limitaInaltime += 60;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "XRAY:  ");
    outtextxy(mijlocLatime - 30, limitaInaltime, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 20, mijlocInaltime - 40, mijlocLatime + 40, mijlocInaltime - 20);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 21, mijlocInaltime - 39, mijlocLatime + 39, mijlocInaltime - 21);
    coordonateInputBox[35] = {mijlocLatime + 21, mijlocInaltime - 39, mijlocLatime + 39, mijlocInaltime - 21};
    if(xRay) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 30, mijlocInaltime - 30, j);
    }
}
void DeseneazaBaraLateralaDreaptaObiect(const Obiect &obj) {
    int lungimeInputBox = 100, inaltimeInputBox = 25;
    char ss[200];

    setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
    int limitaStanga = lungimeFereastra - 250, limitaInaltime = 40;
    bar(limitaStanga, inaltimeFereastra / 35, lungimeFereastra, inaltimeFereastra);

    int mijlocLatime = limitaStanga + (lungimeFereastra - limitaStanga) / 2;
    int mijlocInaltime = limitaInaltime + 3 * inaltimeInputBox / 2;

    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, inaltimeFereastra / 35, limitaStanga + 7, inaltimeFereastra);

    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "ACTIV:  ");
    outtextxy(mijlocLatime - 30, limitaInaltime, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 30, mijlocInaltime - 40, mijlocLatime + 50, mijlocInaltime - 20);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 31, mijlocInaltime - 39, mijlocLatime + 49, mijlocInaltime - 21);
    coordonateInputBox[36] = {mijlocLatime + 31, mijlocInaltime - 39, mijlocLatime + 49, mijlocInaltime - 21};
    if(obj.activ) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 40, mijlocInaltime - 30, j);
    }

    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, mijlocInaltime, lungimeFereastra, mijlocInaltime - 5);

    mijlocInaltime += 50;
    limitaInaltime += 50;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "POZITIE");
    outtextxy(mijlocLatime - textwidth((char*) "POZITIE") / 2, limitaInaltime, ss);

    DeseneazaCasutaInput(obj, (char*) "X:  ", 37, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);
    mijlocInaltime += 35;
    limitaInaltime += 35;
    DeseneazaCasutaInput(obj, (char*) "Y:  ", 38, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);
    mijlocInaltime += 35;
    limitaInaltime += 35;
    DeseneazaCasutaInput(obj, (char*) "Z:  ", 39, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);

    mijlocInaltime += 40;
    limitaInaltime += 40;
    DeseneazaCasutaInput(obj, (char*) "COEFICIENT:  ", 48, mijlocLatime + 50, mijlocInaltime, lungimeInputBox - 30, inaltimeInputBox, limitaInaltime);


    mijlocInaltime += 35;
    limitaInaltime += 35;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "AXE:  ");
    outtextxy(mijlocLatime - 30, limitaInaltime + 35, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 20, mijlocInaltime - 5, mijlocLatime + 40, mijlocInaltime + 15);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 21, mijlocInaltime - 4, mijlocLatime + 39, mijlocInaltime + 14);
    coordonateInputBox[40] = {mijlocLatime + 21, mijlocInaltime - 4, mijlocLatime + 39, mijlocInaltime + 14};
    if(axeMiscareObiect) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 30, mijlocInaltime + 5, j);
    }

    mijlocInaltime += 20;
    limitaInaltime += 20;
    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, mijlocInaltime + 15, lungimeFereastra, mijlocInaltime + 10);

    mijlocInaltime += 65;
    limitaInaltime += 65;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "ROTATIE");
    outtextxy(mijlocLatime - textwidth((char*) "ROTATIE") / 2, limitaInaltime, ss);

    DeseneazaCasutaInput(obj, (char*) "X:  ", 41, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);
    mijlocInaltime += 35;
    limitaInaltime += 35;
    DeseneazaCasutaInput(obj, (char*) "Y:  ", 42, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);
    mijlocInaltime += 35;
    limitaInaltime += 35;
    DeseneazaCasutaInput(obj, (char*) "Z:  ", 43, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);

    mijlocInaltime += 40;
    limitaInaltime += 40;
    DeseneazaCasutaInput(obj, (char*) "COEFICIENT:  ", 49, mijlocLatime + 50, mijlocInaltime, lungimeInputBox - 30, inaltimeInputBox, limitaInaltime);

    mijlocInaltime += 70;
    limitaInaltime += 70;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "MAX 360:  ");
    outtextxy(mijlocLatime - 45, limitaInaltime, ss);
    setcolor(BLACK);
    rectangle(mijlocLatime + 30, mijlocInaltime - 40, mijlocLatime + 50, mijlocInaltime - 20);
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(mijlocLatime + 31, mijlocInaltime - 39, mijlocLatime + 49, mijlocInaltime - 21);
    coordonateInputBox[44] = {mijlocLatime + 31, mijlocInaltime - 39, mijlocLatime + 49, mijlocInaltime - 21};
    if(max360) {
        setcolor(WHITE);
        for(int j = 1; j < 6; j++)
            circle(mijlocLatime + 40, mijlocInaltime - 30, j);
    }

    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaStanga, mijlocInaltime, lungimeFereastra, mijlocInaltime - 5);

    mijlocInaltime += 50;
    limitaInaltime += 50;
    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "MARIME");
    outtextxy(mijlocLatime - textwidth((char*) "MARIME") / 2, limitaInaltime, ss);

    DeseneazaCasutaInput(obj, (char*) "X:  ", 45, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);
    mijlocInaltime += 35;
    limitaInaltime += 35;
    DeseneazaCasutaInput(obj, (char*) "Y:  ", 46, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);
    mijlocInaltime += 35;
    limitaInaltime += 35;
    DeseneazaCasutaInput(obj, (char*) "Z:  ", 47, mijlocLatime, mijlocInaltime, lungimeInputBox, inaltimeInputBox, limitaInaltime);

    mijlocInaltime += 40;
    limitaInaltime += 40;
    DeseneazaCasutaInput(obj, (char*) "COEFICIENT:  ", 50, mijlocLatime + 50, mijlocInaltime, lungimeInputBox - 30, inaltimeInputBox, limitaInaltime);

}
void DeseneazaBaraLateralaDreapta(const Obiect &obj) {
    if(fereastraDreapta == 1)
        DeseneazaBaraLateralaDreaptaPuncte(obj);
    else if(fereastraDreapta == 2)
        DeseneazaBaraLateralaDreaptaMuchii(obj);
    else if(fereastraDreapta == 3)
        DeseneazaBaraLateralaDreaptaObiect(obj);
}

void isHoverBaraStanga(const int i) {
    if(i == butonHover) {
        setfillstyle(SOLID_FILL, COLOR(80, 80, 80));
        setbkcolor(COLOR(80, 80, 80));
    }
}
void DeseneazaBaraLateralaStangaPuncte(const Obiect &obj) {
    setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
    int limitaDreapta = 140, limitaInaltime = 30;
    int mijlocLatime = 70;
    bar(0, inaltimeFereastra / 35, limitaDreapta, inaltimeFereastra);

    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaDreapta, inaltimeFereastra / 35, limitaDreapta + 7, inaltimeFereastra);

    char ss[20];

    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "TOTAL: ");
    strcat(ss, to_string(obj.varfuri.size()).c_str());
    outtextxy(mijlocLatime - textwidth(ss) / 2, limitaInaltime, ss);

    bool culoare = true;
    for(int i = 1; i <= min(24, (int) obj.varfuri.size()); i++) {
        limitaInaltime += 30;
        if(culoare) {
            setfillstyle(SOLID_FILL, COLOR(60, 60, 60));
            setbkcolor(COLOR(60, 60, 60));
        }
        else {
            setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
            setbkcolor(COLOR(50, 50, 50));
        }
        isHoverBaraStanga(9 + i);
        if(i - 1 + indexStart == varfSelectat || find(puncteSelectate.begin(), puncteSelectate.end(), i - 1 + indexStart) != puncteSelectate.end()) {
            setfillstyle(SOLID_FILL, COLOR(80, 80, 80));
            setbkcolor(COLOR(80, 80, 80));
        }
        bar(0, limitaInaltime - textheight(ss) + 9, limitaDreapta, limitaInaltime + textheight(ss) + 7);
        coordonateButoaneHover[9 + i] = {0, limitaInaltime - textheight(ss) + 9, limitaDreapta, limitaInaltime + textheight(ss) + 7};
        sprintf(ss, (char*) "Punct ");
        strcat(ss, to_string(i + indexStart).c_str());
        outtextxy(mijlocLatime - textwidth(ss) / 2, limitaInaltime, ss);
        culoare = !culoare;
    }
}
void DeseneazaBaraLateralaStangaFete(const Obiect &obj) {
    setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
    int limitaDreapta = 140, limitaInaltime = 30;
    int mijlocLatime = 70;
    bar(0, inaltimeFereastra / 35, limitaDreapta, inaltimeFereastra);

    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaDreapta, inaltimeFereastra / 35, limitaDreapta + 7, inaltimeFereastra);

    char ss[20];

    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "TOTAL: ");
    strcat(ss, to_string(obj.fete.size()).c_str());
    outtextxy(mijlocLatime - textwidth(ss) / 2, limitaInaltime, ss);

    bool culoare = true;
    for(int i = 1; i <= min(24, (int) obj.fete.size()); i++) {
        limitaInaltime += 30;
        if(culoare) {
            setfillstyle(SOLID_FILL, COLOR(60, 60, 60));
            setbkcolor(COLOR(60, 60, 60));
        }
        else {
            setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
            setbkcolor(COLOR(50, 50, 50));
        }
        isHoverBaraStanga(9 + i);
        if(obj.indexFata[i - 1 + indexStart] == fataSelectata) {
            setfillstyle(SOLID_FILL, COLOR(80, 80, 80));
            setbkcolor(COLOR(80, 80, 80));
        }
        bar(0, limitaInaltime - textheight(ss) + 9, limitaDreapta, limitaInaltime + textheight(ss) + 7);
        coordonateButoaneHover[9 + i] = {0, limitaInaltime - textheight(ss) + 9, limitaDreapta, limitaInaltime + textheight(ss) + 7};
        sprintf(ss, (char*) "Fata ");
        strcat(ss, to_string(i + indexStart).c_str());
        outtextxy(mijlocLatime - textwidth(ss) / 2, limitaInaltime, ss);
        culoare = !culoare;
    }
}

void DeseneazaBaraLateralaStangaObiecte() {
    setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
    int limitaDreapta = 140, limitaInaltime = 30;
    int mijlocLatime = 70;
    bar(0, inaltimeFereastra / 35, limitaDreapta, inaltimeFereastra);

    setfillstyle(SOLID_FILL, COLOR(40, 40, 40));
    bar(limitaDreapta, inaltimeFereastra / 35, limitaDreapta + 7, inaltimeFereastra);

    char ss[20];

    setcolor(WHITE);
    setbkcolor(COLOR(50, 50, 50));
    sprintf(ss, (char*) "TOTAL: ");
    strcat(ss, to_string(obiecte.size()).c_str());
    outtextxy(mijlocLatime - textwidth(ss) / 2, limitaInaltime, ss);

    bool culoare = true;
    for(int i = 1; i <= min(24, (int) obiecte.size()); i++) {
        limitaInaltime += 30;
        if(culoare) {
            setfillstyle(SOLID_FILL, COLOR(60, 60, 60));
            setbkcolor(COLOR(60, 60, 60));
        }
        else {
            setfillstyle(SOLID_FILL, COLOR(50, 50, 50));
            setbkcolor(COLOR(50, 50, 50));
        }
        isHoverBaraStanga(9 + i);
        if(find(obiecteSelectate.begin(), obiecteSelectate.end(), i + indexStart - 1) != obiecteSelectate.end()) {
            setfillstyle(SOLID_FILL, COLOR(80, 80, 80));
            setbkcolor(COLOR(80, 80, 80));
        }
        bar(0, limitaInaltime - textheight(ss) + 9, limitaDreapta, limitaInaltime + textheight(ss) + 7);
        coordonateButoaneHover[9 + i] = {0, limitaInaltime - textheight(ss) + 9, limitaDreapta, limitaInaltime + textheight(ss) + 7};
        sprintf(ss, (char*) "Obiect ");
        strcat(ss, to_string(i + indexStart).c_str());
        if(!obiecte[i - 1 + indexStart].activ)
            setcolor(DARKGRAY);
        outtextxy(mijlocLatime - textwidth(ss) / 2, limitaInaltime, ss);
        if(!obiecte[i - 1 + indexStart].activ)
            setcolor(WHITE);
        culoare = !culoare;
    }
}

void DeseneazaBaraLateralaStanga(const Obiect &obj) {
    if(fereastraStanga == 1)
        DeseneazaBaraLateralaStangaPuncte(obj);
    else if(fereastraStanga == 2) 
        DeseneazaBaraLateralaStangaFete(obj);
    else if(fereastraStanga == 3)
        DeseneazaBaraLateralaStangaObiecte();
}

void SorteazaFetele(Obiect &obj) {
    if(obj.fete.size() == 0)
        return;
    vector<double> Zmedie((int) obj.fete.size());

    for (int i = 0; i < obj.fete.size(); i++) {
        double sumaZ = 0;

        for (const int &j : obj.fete[i])
            sumaZ += obj.varfuri[j].z;
        
        Zmedie[i] = (double) sumaZ / (double) obj.fete.size();
    }
    
    bool ok;

    do {
        ok = true;

        for(int i = 0; i < obj.fete.size() - 1; i++)
            if(Zmedie[i] < Zmedie[i + 1]) {
                swap(Zmedie[i], Zmedie[i + 1]);
                swap(obj.fete[i], obj.fete[i + 1]);
                swap(obj.culoareFete[i], obj.culoareFete[i + 1]);
                int a, b;
                for(int j = 0; j < obj.indexFata.size(); j++)
                    if(obj.indexFata[j] == i)
                        a = j;
                    else if(obj.indexFata[j] == i + 1)
                            b = j;
                
                swap(obj.indexFata[a], obj.indexFata[b]);
                ok = false;
                
                if(fataSelectata == i)
                    fataSelectata = i + 1;
                else if(fataSelectata == i + 1)
                    fataSelectata = i;
            }
    }while(!ok);
}

int nrObiect;

void DeseneazaObiect(Obiect &obj)
{
    //AfiseazaText(obj, YELLOW);
    if(!obj.activ || obj.varfuri.size() == 0)
        return;

    if(!xRay)
        SorteazaFetele(obj);

    vector<Punct2D> proiectii2D(static_cast<int>(obj.varfuri.size()));

    for (int i = 0; i < obj.varfuri.size(); i++)
        proiectii2D[i] = Transforma2D(obj.varfuri[i]);

    vector<bool> vectorFrecventa(obj.numarVarfuri, 0);
    vector<bool> vectorFrecventaMuchii(static_cast<int>(obj.muchii.size()), 0);

    vector<int> pct;
    for(int i = 0; i < obj.varfuri.size(); i++) {
        obj.varfuri[i].x -= obj.centru.x;
        obj.varfuri[i].y -= obj.centru.y;
        obj.varfuri[i].z -= obj.centru.z;
    }

    for(int i = 0; i < obj.varfuri.size(); i++)
        if(obj.varfuri[i].x < 0)
            pct.push_back(i);
    for(int i = 0; i < obj.numarVarfuri; i++) {
        obj.varfuri[i].x += obj.centru.x;
        obj.varfuri[i].y += obj.centru.y;
        obj.varfuri[i].z += obj.centru.z;
    }

    for(int i = 0; i < obj.fete.size(); i++) {
        if(i == fataSelectata && nrObiect == obiectSelectat)
            if(obj.culoareFete[i][0] <= 10 && obj.culoareFete[i][1] <= 10 && obj.culoareFete[i][2] <= 10)
                setfillstyle(XHATCH_FILL, WHITE);
            else
                setfillstyle(XHATCH_FILL, COLOR(obj.culoareFete[i][0], obj.culoareFete[i][1], obj.culoareFete[i][2]));
        else
            if(deseneazaFete)
                setfillstyle(SOLID_FILL, COLOR(obj.culoareFete[i][0], obj.culoareFete[i][1], obj.culoareFete[i][2]));
            else
                setfillstyle(SOLID_FILL, BLACK);

        vector<int> puncte(2 * static_cast<int>(obj.fete[i].size()));

        if(!xRay)
            for(int j = 0; j < obj.fete[i].size(); j++) {
                puncte[2 * j] = proiectii2D[obj.fete[i][j]].x;
                puncte[2 * j + 1] = proiectii2D[obj.fete[i][j]].y;
            }
        
        if(deseneazaMuchii)
            setcolor(COLOR(culoareMuchii[0], culoareMuchii[1], culoareMuchii[2]));
        else
            if(deseneazaFete)
                setcolor(COLOR(obj.culoareFete[i][0],obj.culoareFete[i][1], obj.culoareFete[i][2]));
            else
                setcolor(BLACK);

        if(!xRay)
            fillpoly(static_cast<int>(obj.fete[i].size()), puncte.data());
        else {
            for(int j = 0; j < obj.fete[i].size(); j++) {
                line(proiectii2D[obj.fete[i][j]].x, proiectii2D[obj.fete[i][j]].y, proiectii2D[obj.fete[i][(j + 1) % obj.fete[i].size()]].x, proiectii2D[obj.fete[i][(j + 1) % obj.fete[i].size()]].y);
            }
        }

        /* for(int j = 0; j < obj.muchii.size(); j++) {
            if(vectorFrecventaMuchii[j])
                continue;
            bool ok1 = false, ok2 = false;
            for(auto i : obj.fete[i]) {
                if(i == obj.muchii[j].first)
                    ok1 = true;
                if(i == obj.muchii[j].second)
                    ok2 = true;
            }
            if(ok1 && ok2) {
                cout << obj.muchii[i].first << " " << obj.muchii[i].second << obj.muchii.size() << endl;
                for(auto &punct : obj.fete[i]) {
                    cout << punct << " ";
                }
                cout << endl;
                Punct2D punctulA = Transforma2D(obj.varfuri[obj.muchii[j].first]);
                Punct2D punctulB = Transforma2D(obj.varfuri[obj.muchii[j].second]);
                
                line(punctulA.x, punctulA.y, punctulB.x, punctulB.y);

                vectorFrecventaMuchii[j] = true;
            }
        } */
        
        if(deseneazaPuncte) {
            for(int j = 0; j < obj.fete[i].size(); j++) {
                if(selectarePuncte && colorarePuncteSelectate && find(puncteSelectate.begin(), puncteSelectate.end(), obj.fete[i][j]) != puncteSelectate.end() && nrObiect == obiectSelectat)
                    setcolor(COLOR(culoarePuncteSelectate[0], culoarePuncteSelectate[1], culoarePuncteSelectate[2]));
                else if(varfSelectat == obj.fete[i][j] && colorarePuncteSelectate && !unestePuncte  && nrObiect == obiectSelectat)
                    setcolor(COLOR(culoarePuncteSelectate[0], culoarePuncteSelectate[1], culoarePuncteSelectate[2]));
                else
                    setcolor(COLOR(culoarePuncte[0], culoarePuncte[1], culoarePuncte[2]));
                
                /* if(find(pct.begin(), pct.end(), obj.fete[i][j]) != pct.end())
                    setcolor(COLOR(75, 0, 130));
                else
                    setcolor(WHITE); */
                for(int k = 1; k <= (double) razaPunct - 100 / zoom + 1; k++)
                    circle(proiectii2D[obj.fete[i][j]].x, proiectii2D[obj.fete[i][j]].y, k);

                vectorFrecventa[obj.fete[i][j]] = true;
            }
        }
   }

    if(deseneazaPuncte) {
        for(int i = 0; i < vectorFrecventa.size(); i++)
            if(!vectorFrecventa[i] && deseneazaPuncte) {
                if(selectarePuncte && nrObiect == obiectSelectat && colorarePuncteSelectate && find(puncteSelectate.begin(), puncteSelectate.end(), i) != puncteSelectate.end())
                    setcolor(COLOR(culoarePuncteSelectate[0], culoarePuncteSelectate[1], culoarePuncteSelectate[2]));
                else if(varfSelectat == i && colorarePuncteSelectate && !unestePuncte && nrObiect == obiectSelectat)
                    setcolor(COLOR(culoarePuncteSelectate[0], culoarePuncteSelectate[1], culoarePuncteSelectate[2]));
                else
                    setcolor(COLOR(culoarePuncte[0], culoarePuncte[1], culoarePuncte[2]));
                if(find(pct.begin(), pct.end(), i) != pct.end())
                    setcolor(RED);
                else
                    setcolor(WHITE);
                for(int j = 1; j <= razaPunct; j++)
                    circle(proiectii2D[i].x, proiectii2D[i].y, j);
            }
    }

    if(deseneazaMuchii) {
        setcolor(COLOR(culoareMuchii[0], culoareMuchii[1], culoareMuchii[2]));
        for(const pair<int, int> &muchie : obj.muchii) {
            Punct2D punctulA = Transforma2D(obj.varfuri[muchie.first]);
            Punct2D punctulB = Transforma2D(obj.varfuri[muchie.second]);

            line(proiectii2D[muchie.first].x, proiectii2D[muchie.first].y, proiectii2D[muchie.second].x, proiectii2D[muchie.second].y);
        }
    }

    //Deseneaza laturile
    /* for (int i = 0; i < obj.numarMuchii; i++)
    {
        Punct2D punctulA = Transforma2D(obj.varfuri[obj.muchii[i].first]);
        Punct2D punctulB = Transforma2D(obj.varfuri[obj.muchii[i].second]); */
        //if(puncteVizibile.find(obj.muchii[i].first) == puncteVizibile.end()  || puncteVizibile.find(obj.muchii[i].second) == puncteVizibile.end() ) 
        /* setcolor(DARKGRAY); 
    else
        setcolor(WHITE); */
        //{
            /* if(!ascundeFete)
                line(punctulA.x, punctulA.y, punctulB.x, punctulB.y); */
        //} 
        //else
            //line(punctulA.x, punctulA.y, punctulB.x, punctulB.y);
    //}

    setcolor(WHITE);

    /* int copie[100];
    for(int i = 0; i < 100; i++)
        copie[i] = Colorare.culoare[i];
    int copiefataSelectata=fataSelectata;
    vector<vector<int>> Sortat = SorteazaFetele(obj);
    int k = 0;
    vector<bool> vectorFrecventa(obj.numarVarfuri, 0);
    for (const auto &fata : Sortat) {

        if(k==fataSelectata)
        {
            if(Colorare.culoare[k]==BLACK)
                setfillstyle(XHATCH_FILL, WHITE);
            else
                setfillstyle(XHATCH_FILL, Colorare.culoare[k]);
        }
        else
            setfillstyle(SOLID_FILL, Colorare.culoare[k]);

        int numarPuncte = fata.size();
        int puncte[2 * numarPuncte + 2];

        for(int i = 0; i < numarPuncte; i++) {
            Punct2D punct = Transforma2D(obj.varfuri[fata[i]]);
            puncte[2 * i] = punct.x - 1;
            puncte[2 * i + 1] = punct.y - 1;

            if(deseneazaPuncte) {
                if(selectarePuncte && colorarePuncteSelectate && find(puncteSelectate.begin(), puncteSelectate.end(), fata[i]) != puncteSelectate.end())
                    setcolor(COLOR(culoarePuncteSelectate[0], culoarePuncteSelectate[1], culoarePuncteSelectate[2]));
                else if(varfSelectat == fata[i] && colorarePuncteSelectate && !unestePuncte)
                    setcolor(COLOR(culoarePuncteSelectate[0], culoarePuncteSelectate[1], culoarePuncteSelectate[2]));
                else
                    setcolor(WHITE);
                for(int j = 1; j <= razaPunct; j++)
                    circle(punct.x, punct.y, j);
            }

            vectorFrecventa[fata[i]] = true;
        }
        k++;

        setcolor(WHITE);

        fillpoly(numarPuncte, puncte);
    }

    for(int i = 0; i < vectorFrecventa.size(); i++)
        if(!vectorFrecventa[i] && deseneazaPuncte) {
            Punct2D punct = Transforma2D(obj.varfuri[i]);
            if(selectarePuncte && colorarePuncteSelectate && find(puncteSelectate.begin(), puncteSelectate.end(), i) != puncteSelectate.end())
                setcolor(COLOR(culoarePuncteSelectate[0], culoarePuncteSelectate[1], culoarePuncteSelectate[2]));
            else if(varfSelectat == i && colorarePuncteSelectate && !unestePuncte)
                    setcolor(COLOR(culoarePuncteSelectate[0], culoarePuncteSelectate[1], culoarePuncteSelectate[2]));
            else
                setcolor(WHITE);
            for(int j = 1; j <= razaPunct; j++)
                circle(punct.x, punct.y, j);
        }
    
    for(int i = 0; i < 100; i++)
        Colorare.culoare[i] = copie[i];
    fataSelectata = copiefataSelectata; */
}

void DeseneazaAxePunct(const Obiect &obj) {
    Punct2D centruObiect = Transforma2D(obj.centru);
   Punct2D centruPtAxe = Transforma2D(obj.axeObiectStabile[0]);
   int modificax = 450, modificay = 320;
   if(fereastraDreapta == 0)
        modificax = 700;

    /* if(axeObiectSelectat == 2) {
        circle(centruPtAxe.x + modificax, centruPtAxe.y - modificay, 45);
        setfillstyle(SOLID_FILL, COLOR(37, 150, 190));
        floodfill(centruPtAxe.x + modificax, centruPtAxe.y - modificay, WHITE);
    } */

    setcolor(YELLOW);
    Punct2D axaOx;
    if((!axeMiscareObiect && axeObiectSelectat == 1) || (axeMiscareObiect && local)) {
        axaOx = Transforma2D(obj.axaOX);
        line(centruObiect.x, centruObiect.y, axaOx.x, axaOx.y);
    }
    if(axeObiectSelectat == 2) {
        double saveZoom = zoom;
        zoom = 100;
        axaOx = Transforma2D(obj.axeObiectStabile[1]);
        zoom = saveZoom;
        line(centruPtAxe.x + modificax, centruPtAxe.y - modificay, axaOx.x + modificax, axaOx.y - modificay);    
    }
    if(axeMiscareObiect && !local) {
        axaOx = Transforma2D({obj.centru.x + 0.5, obj.centru.y, obj.centru.z});
        line(centruObiect.x, centruObiect.y, axaOx.x, axaOx.y);
    }

    setcolor(RED);
    Punct2D axaOy;
    if((!axeMiscareObiect && axeObiectSelectat == 1) || (axeMiscareObiect && local)) {
        axaOy = Transforma2D(obj.axaOY);
        line(centruObiect.x, centruObiect.y, axaOy.x, axaOy.y);
    }
    if(axeObiectSelectat == 2) {
        double saveZoom = zoom;
        zoom = 100;
        axaOy = Transforma2D(obj.axeObiectStabile[2]);
        zoom = saveZoom;
        line(centruPtAxe.x + modificax, centruPtAxe.y - modificay, axaOy.x + modificax, axaOy.y - modificay);    
    }
    if(axeMiscareObiect && !local) {
        axaOy = Transforma2D({obj.centru.x, obj.centru.y - 0.5, obj.centru.z});
        line(centruObiect.x, centruObiect.y, axaOy.x, axaOy.y);
    }

    setcolor(GREEN);
    Punct2D axaOz;
    if((!axeMiscareObiect && axeObiectSelectat == 1) || (axeMiscareObiect && local)) {
        axaOz = Transforma2D(obj.axaOZ);
        line(centruObiect.x, centruObiect.y, axaOz.x, axaOz.y);
    }
    if(axeObiectSelectat == 2) {
        double saveZoom = zoom;
        zoom = 100;
        axaOz = Transforma2D(obj.axeObiectStabile[3]);
        zoom = saveZoom;
        line(centruPtAxe.x + modificax, centruPtAxe.y - modificay, axaOz.x + modificax, axaOz.y - modificay);    
    }
    if(axeMiscareObiect && !local) {
        axaOz = Transforma2D({obj.centru.x + 0.25, obj.centru.y - 0.25, obj.centru.z});
        line(centruObiect.x, centruObiect.y, axaOz.x, axaOz.y);
    }

    setcolor(WHITE);

    if(varfSelectat != -1 && axePunct && !unestePuncte && !eliminaPuncte) {
        Punct2D varfSelectat2D = Transforma2D(obj.varfuri[varfSelectat]);

        setcolor(YELLOW);
        Punct2D axaOx2;
        if(local)
            axaOx2 = Transforma2D(obj.axePunctSelectat[0]);
        else
            axaOx2 = Transforma2D({obj.varfuri[varfSelectat].x + 0.5, obj.varfuri[varfSelectat].y, obj.varfuri[varfSelectat].z});
        /* double ratieX = (obj.varfuri[varfSelectat].x - obj.centru.x) / 2;
        double ratieY = (obj.varfuri[varfSelectat].y - obj.centru.y) / 2;
        double ratieZ = (obj.varfuri[varfSelectat].z - obj.centru.z) / 2;
        Punct2D axaOx2 = Transforma2D({obj.varfuri[varfSelectat].x + ratieX, obj.varfuri[varfSelectat].y + ratieY, obj.varfuri[varfSelectat].z + ratieZ});
         */line(varfSelectat2D.x, varfSelectat2D.y, axaOx2.x, axaOx2.y);

        setcolor(RED);
        if(local)
        axaOy = Transforma2D(obj.axePunctSelectat[1]);
    else
        axaOy = Transforma2D({obj.varfuri[varfSelectat].x, obj.varfuri[varfSelectat].y - 0.5, obj.varfuri[varfSelectat].z});
        line(varfSelectat2D.x, varfSelectat2D.y, axaOy.x, axaOy.y);

        setcolor(GREEN);
        if(local)
        axaOz = Transforma2D(obj.axePunctSelectat[2]);
    else 
        axaOz = Transforma2D({obj.varfuri[varfSelectat].x + 0.25, obj.varfuri[varfSelectat].y - 0.25, obj.varfuri[varfSelectat].z});
        line(varfSelectat2D.x, varfSelectat2D.y, axaOz.x, axaOz.y);

        setcolor(WHITE);
    }

    if(unestePuncte && varfSelectat != -1)
        line(pozitieMouseUnirePuncte.x, pozitieMouseUnirePuncte.y, Transforma2D(obj.varfuri[varfSelectat]).x, Transforma2D(obj.varfuri[varfSelectat]).y);
}

void Deseneaza(Obiect &obj) {
    nrObiect = 0;

    if(fereastraDreapta == 0)
        cleardevice();
    else {
        setfillstyle(SOLID_FILL, BLACK);
        bar(0, inaltimeFereastra / 35, lungimeFereastra - 250, inaltimeFereastra);
    }

    if(butonGrid) {
        vector<Punct2D> proiectii2DGrid;
        for(const Punct3D &i : grid.varfuri)
            proiectii2DGrid.push_back(Transforma2D(i)); 
        setcolor(DARKGRAY);
        int km = 100;
        bool a = true, lv = 0;
        for(int i = 0; i < grid.muchii.size(); i++) {
            /*if(find(col.begin(), col.end(), grid.muchii[i].first) != col.end() || find(col.begin(), col.end(), grid.muchii[i].second) != col.end())
                setcolor(blue);
            else
                setcolor(white);*/
                /* if(i - lv == 24) {
                    lv = i;
                    a = !a;
                }
            if(i % 2 == a)
                setcolor(BLUE);
            else
            setcolor(WHITE);
            /* setcolor(COLOR(km - 50, km, km + 50));
            km += 10;
            if(km > 200)
                km = 100; */
            line(proiectii2DGrid[grid.muchii[i].first].x, proiectii2DGrid[grid.muchii[i].first].y, proiectii2DGrid[grid.muchii[i].second].x, proiectii2DGrid[grid.muchii[i].second].y);
        }
    }

    for(Obiect &obj2 : obiecte) {
        DeseneazaObiect(obj2);
        nrObiect++;
    }
    
    if(obiectSelectat != -1 && obiecte[obiectSelectat].activ)
        DeseneazaAxePunct(obiecte[obiectSelectat]);

    DeseneazaBaraLateralaDreapta(obiecte[obiectSelectat]);
    DeseneazaBaraLateralaStanga(obiecte[obiectSelectat]);
    DeseneazaBaraComenzi(); 
    if(modificaPagina) {
        //cout << "da";
        pagina=1-pagina;
                setactivepage(pagina); 
                setvisualpage(1-pagina);

    //if(refaceBaraDreapta)
    /* DeseneazaBaraLateralaDreapta(obj);*/
    
    }

    if((deseneazaPuncte && obj.fete.size() > 9)) {}
    else if(butonGrid) {
        delay(2);
    }
    else if(varfSelectat != -1)
            delay(3);
        else if(obj.varfuri.size() < 400)
        delay(4);
}