#pragma once
#include <vector>
#include <stack>
#include <windows.h>
#include <map>

#define POV 15
#define razaPunct 4
#define dimensiunePuncteFete 4
#define dimensiuneFete 6
#define dimensiune 12

int fataSelectata;

int lungimeFereastra;
int inaltimeFereastra;

double zoom;
double unghiRotatie, coeficientMarire, coeficientDeplasare, coeficientZoom;
bool obiectCentrat;
int varfSelectat, marimeAxeVarfSelectat;

char axaSelectata;
int ultimaPozitieMouseX, ultimaPozitieMouseY, numarareRotatii, numarareDeplasare, numarareMarire;
bool  mouseApasat;
int pagina;

std::vector<int> puncteSelectate;
bool selectarePuncte;

std::vector<int> col;

std::vector<bool> hoverButoane;
int butonHover, butonGrid;
std::vector<std::vector<int>> coordonateInputBox, coordonateButoaneHover;
int selectareInput, fereastraDreapta, axeObiectSelectat, indexStart, fereastraStanga;
bool intratAici;
bool modificaPagina;
bool fereastraFile, fereastraAdauga;
bool deseneazaFete, deseneazaPuncte, xRay, deseneazaMuchii, undoRotatieGrid, nuDesena;

bool axePunct, colorarePuncteSelectate, unestePuncte, adaugaPunct, eliminaPuncte, axeMiscareObiect, max360;
std::vector<int> culoarePuncteSelectate, culoarePuncte, culoareMuchii, copieCuloareFata;

bool inchideEditor;
bool local;

struct Punct2D {
    double x;
    double y;
};

struct Punct3D {
    double x;
    double y;
    double z;
};

struct Undo {
    int alegere;
    int numarRotatii;

    char axa;
    bool local;
    std::vector<std::pair<int, double>> coordonate, indexFete;
    std::vector<std::vector<int>> culoareFete;
    std::vector<std::vector<int>> fete;
    std::vector<int> index;
    Punct3D puncteEliminat;
};

std::stack<Undo> undo;
std::stack<Undo> redo;

Punct2D pozitieMouseUnirePuncte;

struct Grid {
    std::vector<Punct3D> puncte;
    std::vector<std::pair<int, int>> muchii;
}grid2;

struct Obiect {
    int numarVarfuri;
    int numarMuchii;
    int numarFete;
    bool activ;

    std::vector<Punct3D> varfuri;
    std::vector<std::pair<int, int>> muchii;
    std::vector<std::vector<int>> fete, culoareFete;
    std::vector<int> culoareFata, indexFata;

    Punct3D centru, centruPentruAxe;

    Punct3D axaOX, axaOY, axaOZ;

    std::vector<Punct3D> axePunctSelectat, axeObiectStabile;

    double rotatieX, rotatieY, rotatieZ;

    double marimeX, marimeY, marimeZ;

    double deplasareX, deplasareY, deplasareZ;
    
    //first -> axa; second -> valoarea unghiului
    std::stack<std::pair<char, double>> rotatiiAnterioare;
    std::stack<std::pair<char, double>> stackDeplasare, stackMarire;
    std::stack<std::pair<char, double>> stackRedoRotatie, stackRedoDeplasare, stackRedoMarire;
};

Obiect grid;

std::vector<Obiect> obiecte;
std::vector<int> obiecteSelectate;
int obiectSelectat;