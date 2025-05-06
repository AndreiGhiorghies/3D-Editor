#include <iostream>
#include "Headers/Structuri.h"
#include "Headers/Fisiere.h"
#include "Headers/Deseneaza.h"
#include "Headers/Functionalitati.h"
#include <winbgim.h>
#include <windows.h>

using namespace std;

void InitializareFereastra() {
    RECT workArea;

    SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

    lungimeFereastra = workArea.right - workArea.left;
    inaltimeFereastra = workArea.bottom - workArea.top;
 
    initwindow(lungimeFereastra, inaltimeFereastra, "Editor 3D", workArea.left, workArea.top);

    HWND hwnd = GetForegroundWindow();

    SetWindowLong(hwnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);
    ShowWindow(hwnd, SW_MAXIMIZE);

    setviewport(0, 0, lungimeFereastra - 1, inaltimeFereastra - 1, 1);
}

int main() {
    InitializareFereastra();
 
    coordonateInputBox = vector<vector<int>>(51, {0, 0, 0, 0});
    coordonateButoaneHover = vector<vector<int>>(42, {0, 0, 0, 0}); // pana la 10 butoane obisnuite; 10-33 pt puncte
    copieCuloareFata = vector<int>(3, 100);

    culoarePuncteSelectate = {0, 255, 0};
    culoarePuncte = {255, 255, 255};
    culoareMuchii = {255, 255, 255};

    deseneazaFete = true;
    deseneazaPuncte = false;
    deseneazaMuchii = false;
    fereastraAdauga = false;
    obiectCentrat = false;
    modificaPagina = true;
    axePunct = false;
    local = true;
    xRay = false;

    fereastraDreapta = 0;
    fereastraStanga = 0;
    butonGrid = 0;
    axeObiectSelectat = 0;

    varfSelectat = -1;
    fataSelectata = -1;
    butonHover = -1;

    zoom = 100;
    unghiRotatie = 2 / 180.0 * 3.14;
    coeficientMarire = 0.02;
    coeficientDeplasare = 0.05;
    coeficientZoom = 0.5;

    int latime = 6, lungime = 10;
    for(int i = 0; i <= lungime; i++)
        for(int j = 0; j <= latime; j++) {
            grid.varfuri.push_back({(double) (i - lungime / 2) * 3, 3, (double) (j - 10) * 3});

            if(j < latime)
                grid.muchii.push_back({i * (latime + 1) + j, i * (latime + 1) + j + 1});
            
            if(i < lungime)
                grid.muchii.push_back({(i + 1) * (latime + 1) + j, i * (latime + 1) + j});
        }
    
    grid.numarVarfuri = (int) grid.varfuri.size();
    grid.centru = GetMijlocObiect(grid);
    
    for(int i = 0; i < grid.varfuri.size(); i++) {
        grid.varfuri[i].x -= grid.centru.x;
        grid.varfuri[i].z -= grid.centru.z;
    }

    Obiect obj = IncarcaObiect("Obiecte/cub.txt");

    Deseneaza(obj);
    Deseneaza(obj);

    bool a;

    while(!(GetAsyncKeyState(VK_ESCAPE) & 0x8000) && !inchideEditor) {
        if(obiectSelectat == -1) {
            VerificaHoverButoane(obj);
            a = VerificaApasareVarf(obj);

            nuDesena = true;
            a = VerificareZoom(obj);
            if(butonGrid && (GetAsyncKeyState(VK_MENU) & 0x8000))
                a = VerificareRotatie(obj);
            nuDesena = false;

            if(ismouseclick(WM_LBUTTONDOWN))
                clearmouseclick(WM_LBUTTONDOWN);
            if(!a)
                delay(5);
            
            continue;
        }
        else {
            VerificaApasareButon(obiecte[obiectSelectat]);

            a = VerificaApasareVarf(obiecte[obiectSelectat]);
            a = VerificareZoom(obiecte[obiectSelectat]);  
        }

        VerificaScrool(obiecte[obiectSelectat]);

        if (GetAsyncKeyState('E') & 0x8000) {
            if(fataSelectata == -1)
               fataSelectata = 0;
            else 
                SelecteazaFata(obiecte[obiectSelectat], obiecte[obiectSelectat].indexFata[fataSelectata] - 1);
        }
        else if (GetAsyncKeyState('Q') & 0x8000) {
                if(fataSelectata == -1)
                    fataSelectata = 0;
                else 
                    SelecteazaFata(obiecte[obiectSelectat], obiecte[obiectSelectat].indexFata[fataSelectata] + 1);
            }
        else if (GetAsyncKeyState('W') & 0x8000) {
                fataSelectata = -1;

                Deseneaza(obj);
                delay(200);
            }

        a = VerificareRotatie(obiecte[obiectSelectat]);

        UndoChange(obiecte[obiectSelectat], false);

        RedoChange(obiecte[obiectSelectat], false);

        a = VerificareMarire(obiecte[obiectSelectat]);

        a = VerificareDeplasare(obiecte[obiectSelectat]);

        a = VerificareZoom(obiecte[obiectSelectat]);
        
        CrearePoligon(obiecte[obiectSelectat]);
            
        AdaugaPunct(obiecte[obiectSelectat]);
            
        VerificaHoverButoane(obiecte[obiectSelectat]);
    }

    return 0;
}