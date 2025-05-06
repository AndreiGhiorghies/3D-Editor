#include <iostream>
#include <vector>
#include <math.h>
#include <windows.h>
#include <algorithm>
#include <set>
#include <graphics.h>
#include <conio.h>
#include <cstring>
#include <ctime>
#include "../Headers/Structuri.h"
#include "../Headers/Deseneaza.h"

using namespace std;

template <typename T>
void clearStack(stack<T> &s) {
    while(!s.empty()) {
        s.pop();
    }
}

void EditareRotatie(Obiect &obj, bool resetareRotatie);
void UndoChange(Obiect &obj, bool skip);
void RedoChange(Obiect &obj, bool skip);

void operator-=(Punct3D &obj1, Punct3D &obj2) {
    obj1.x -= obj2.x;
    obj1.y -= obj2.y;
    obj1.z -= obj2.z;
}

void operator+=(Punct3D &obj1, Punct3D &obj2) {
    obj1.x += obj2.x;
    obj1.y += obj2.y;
    obj1.z += obj2.z;
}

Punct3D operator-(Punct3D obj1, Punct3D obj2) {
    return {obj1.x - obj2.x, obj1.y - obj2.y, obj1.z - obj2.z};
}

Punct2D Transforma2D(const Punct3D &punct) {
    double x = lungimeFereastra / 2 + (punct.x * POV) / (punct.z + POV) * zoom;
    double y = inaltimeFereastra / 2 + (punct.y * POV) / (punct.z + POV) * zoom;
    return {x, y};
}

Punct3D GetMijlocObiect(Obiect &obj) {
    double x = 0, y = 0, z = 0;

    for(int i = 0; i < obj.varfuri.size(); i++) {
        x += obj.varfuri[i].x;
        y += obj.varfuri[i].y;
        z += obj.varfuri[i].z;
    }
    
    return {x / (double) obj.numarVarfuri, y / (double) obj.numarVarfuri, z / (double) obj.numarVarfuri};
}

void CentreazaObiect(Obiect &obj) {
    if(obiectCentrat)
        return;

    for(int i = 0; i < obj.varfuri.size(); i++)
        obj.varfuri[i] -= obj.centru;
    
    obj.axaOX -= obj.centru;
    obj.axaOY -= obj.centru;
    obj.axaOZ -= obj.centru;

    if(varfSelectat != -1)
        for(int i = 0; i < 3; i++)
            obj.axePunctSelectat[i] -= obj.centru;

    obiectCentrat = true;
}

void RefacePozitia(Obiect &obj) {
    if(!obiectCentrat)
        return;

    for(int i = 0; i < obj.numarVarfuri; i++)
        obj.varfuri[i] += obj.centru;
    
    obj.axaOX += obj.centru;
    obj.axaOY += obj.centru;
    obj.axaOZ += obj.centru;

    if(varfSelectat != -1)
        for(int i = 0; i < 3; i++)
            obj.axePunctSelectat[i] += obj.centru;

    obiectCentrat = false;
}

bool VerificareZoom(Obiect &obj) {
    bool ctrl = GetAsyncKeyState(VK_CONTROL) & 0x8000;
    bool plus = GetAsyncKeyState(VK_ADD) & 0x8000;
    bool minus = GetAsyncKeyState(VK_SUBTRACT) & 0x8000;

    if(!ctrl || (plus && minus) || (!plus && !minus))
        return false;
    
    if(plus) {
        if(zoom + coeficientZoom <= 200)
            zoom += coeficientZoom;
    }
    else if(zoom - coeficientZoom > 1)
            zoom -= coeficientZoom;
    
    Deseneaza(obj);

    return true;
}

//-------------------------------------------------- DEPLASARE --------------------------------------------------------------------

void DeplaseazaX(Obiect &obj) {
    obj.deplasareX += coeficientDeplasare;

    for(int i = 0; i < obj.varfuri.size(); i++)
        obj.varfuri[i].x += coeficientDeplasare;

    obj.axaOX.x += coeficientDeplasare;
    obj.axaOY.x += coeficientDeplasare;
    obj.axaOZ.x += coeficientDeplasare;

    if(varfSelectat != -1)
        for(int i = 0; i < 3; i++)
            obj.axePunctSelectat[i].x += coeficientDeplasare;
}

void DeplaseazaY(Obiect &obj) {
    obj.deplasareY += coeficientDeplasare;

    for(int i = 0; i < obj.varfuri.size(); i++)
        obj.varfuri[i].y += coeficientDeplasare;

    obj.axaOX.y += coeficientDeplasare;
    obj.axaOY.y += coeficientDeplasare;
    obj.axaOZ.y += coeficientDeplasare;

    if(varfSelectat != -1)
        for(int i = 0; i < 3; i++)
            obj.axePunctSelectat[i].y += coeficientDeplasare;
}

void DeplaseazaZ(Obiect &obj) {
    obj.deplasareZ += coeficientDeplasare;

    for(int i = 0; i < obj.varfuri.size(); i++)
        obj.varfuri[i].z += coeficientDeplasare;

    obj.axaOX.z += coeficientDeplasare;
    obj.axaOY.z += coeficientDeplasare;
    obj.axaOZ.z += coeficientDeplasare;

    if(varfSelectat != -1)
        for(int i = 0; i < 3; i++)
            obj.axePunctSelectat[i].z += coeficientDeplasare;
}

bool VerificareDeplasare(Obiect &obj) {
    if(!obj.activ)
        return false;
    bool x = GetAsyncKeyState('X') & 0x8000;
    bool y = GetAsyncKeyState('Y') & 0x8000;
    bool z = GetAsyncKeyState('Z') & 0x8000;
    bool up = GetAsyncKeyState(VK_UP) & 0x8000, down = GetAsyncKeyState(VK_DOWN) & 0x8000;
    bool left = GetAsyncKeyState(VK_LEFT) & 0x8000, right = GetAsyncKeyState(VK_RIGHT) & 0x8000;

    if(!(x && left) && !(x && right) && !(y && up) && !(y && down) && !(z && up) && !(z && down)) {
        if(numarareDeplasare != 0) {
            clearStack(redo);
            Undo temp;
            temp.alegere = 2;
            temp.local = local;
            temp.numarRotatii = numarareDeplasare;
            numarareDeplasare = 0;
            undo.push(temp);
        }
        return false;
    }
    
    if(left || (y && up) || (z && down && !y))
        coeficientDeplasare = -coeficientDeplasare;
    
    if(local) 
        EditareRotatie(obj, true);

    if(x && (left || right)) {
        DeplaseazaX(obj);
        obj.stackDeplasare.push({'x', coeficientDeplasare});
        numarareDeplasare++;
    }
    if(y && (up || down)) {
        DeplaseazaY(obj);
        obj.stackDeplasare.push({'y', coeficientDeplasare});
        numarareDeplasare++;
    }
    else if(z && (up || down)) {
        DeplaseazaZ(obj);
        obj.stackDeplasare.push({'z', coeficientDeplasare});
        numarareDeplasare++;
    }

    if(local)
        EditareRotatie(obj, false);

    if(left || (y && up) || (z && down && !y))
        coeficientDeplasare = -coeficientDeplasare;

    obj.centru = GetMijlocObiect(obj);
    obj.centruPentruAxe = obj.centru;

    Deseneaza(obj);

    return true;
}

//---------------------------------------------- SFARSIT DEPLASARE ----------------------------------------------------------------



//-------------------------------------------------- ROTATIE ----------------------------------------------------------------------

void RotestePunctX(Punct3D &punct) {
    double x = punct.x;

    punct.x = cos(unghiRotatie) * x - sin(unghiRotatie) * punct.z;
    punct.z = sin(unghiRotatie) * x + cos(unghiRotatie) * punct.z;
}

void RotestePunctY(Punct3D &punct) {
    double y = punct.y;

    punct.y = cos(-unghiRotatie) * y - sin(-unghiRotatie) * punct.z;
    punct.z = sin(-unghiRotatie) * y + cos(-unghiRotatie) * punct.z;    
}

void RotestePunctZ(Punct3D &punct) {
    double x = punct.x;

    punct.x = cos(unghiRotatie) * x - sin(unghiRotatie) * punct.y;
    punct.y = sin(unghiRotatie) * x + cos(unghiRotatie) * punct.y;
}

void RotesteObiectX(Obiect &obj) {
    obj.rotatieX += unghiRotatie;

    for(int i = 0; i < obj.numarVarfuri; i++)
        RotestePunctX(obj.varfuri[i]);
    
    RotestePunctX(obj.axaOX);
    RotestePunctX(obj.axaOY);
    RotestePunctX(obj.axaOZ);

    for(int i = 1; i < 4; i++)
        RotestePunctX(obj.axeObiectStabile[i]);
    
    if((butonGrid && (GetAsyncKeyState(VK_MENU) & 0x8000)) || undoRotatieGrid) //ALT
        for(Punct3D &i : grid.varfuri)
            RotestePunctX(i);

    for(int i = 0; i < 3; i++)
        RotestePunctX(obj.axePunctSelectat[i]);
}

void RotesteObiectY(Obiect &obj) {
    obj.rotatieY += unghiRotatie;

    for(int i = 0; i < obj.numarVarfuri; i++)
        RotestePunctY(obj.varfuri[i]);
    
    RotestePunctY(obj.axaOX);
    RotestePunctY(obj.axaOY);
    RotestePunctY(obj.axaOZ);

    for(int i = 1; i < 4; i++)
        RotestePunctY(obj.axeObiectStabile[i]);
    
    if((butonGrid == 2 && (GetAsyncKeyState(VK_MENU) & 0x8000)) || undoRotatieGrid) //ALT
        for(Punct3D &i : grid.varfuri)
            RotestePunctY(i);

    for(int i = 0; i < 3; i++)
        RotestePunctY(obj.axePunctSelectat[i]);
}

void RotesteObiectZ(Obiect &obj) {
    obj.rotatieZ += unghiRotatie;

    for(int i = 0; i < obj.numarVarfuri; i++)
        RotestePunctZ(obj.varfuri[i]);
    
    RotestePunctZ(obj.axaOX);
    RotestePunctZ(obj.axaOY);
    RotestePunctZ(obj.axaOZ);
    
    for(int i = 1; i < 4; i++)
        RotestePunctZ(obj.axeObiectStabile[i]);

    for(int i = 0; i < 3; i++)
        RotestePunctZ(obj.axePunctSelectat[i]);
}

void EditareRotatie(Obiect &obj, bool resetareRotatie) {
    CentreazaObiect(obj);

    stack<pair<char, double>> save;
    double saveUnghiRotatie = unghiRotatie;
    int resetare;
    
    if(resetareRotatie)
        resetare = -1;
    else
        resetare = 1;

    while(!obj.rotatiiAnterioare.empty()) {
        char axa = obj.rotatiiAnterioare.top().first;
        unghiRotatie = resetare * obj.rotatiiAnterioare.top().second;

        save.push(obj.rotatiiAnterioare.top());
        obj.rotatiiAnterioare.pop();
        
        if(axa == 'x') {
            RotesteObiectX(obj);
        }
        else if(axa == 'y')
                RotesteObiectY(obj);
        else
            RotesteObiectZ(obj);
    }

    unghiRotatie = saveUnghiRotatie;
    obj.rotatiiAnterioare = save;

    RefacePozitia(obj);
}

bool VerificareRotatie(Obiect &obj) {
    if(!obj.activ)
        return false;
    bool x = GetAsyncKeyState('X') & 0x8000;
    bool y = GetAsyncKeyState('Y') & 0x8000;
    bool z = GetAsyncKeyState('Z') & 0x8000;
    bool up = GetAsyncKeyState(VK_UP), down = GetAsyncKeyState(VK_DOWN);
    bool left = GetAsyncKeyState(VK_LEFT), right = GetAsyncKeyState(VK_RIGHT);
    bool rotireDiagonala = (left && up) || (left && down) || (right && up) || (right && down);
    double saveUnghiRotatie = unghiRotatie;

    if(x || y || (z && !left && !right) || (!left && !right && !up && !down) || (fereastraStanga && mousex() <= 140)) {
       if(numarareRotatii != 0) {
            clearStack(redo);

            Undo temp;
            temp.alegere = 1;
            temp.numarRotatii = numarareRotatii;
            numarareRotatii = 0;

            if(undoRotatieGrid)
                temp.axa = 'g';
            
            undo.push(temp);
       }
        undoRotatieGrid = false;

        return false;
    }

    if(numarareRotatii != 0 && undoRotatieGrid != ((GetAsyncKeyState(VK_MENU) & 0x8000) && butonGrid == 2)) {
        clearStack(redo);

        Undo temp;
        temp.alegere = 1;
        temp.numarRotatii = numarareRotatii;
        numarareRotatii = 0;

        if(undoRotatieGrid)
            temp.axa = 'g';
        
        undo.push(temp);
    }

    undoRotatieGrid = (GetAsyncKeyState(VK_MENU) & 0x8000) && butonGrid == 2;

    int salvareSize = (int) obj.rotatiiAnterioare.size();

    if(rotireDiagonala)
        unghiRotatie /= 1.5;

    if((up || down || left || right))
        CentreazaObiect(obj);

    if(left) {
        unghiRotatie = -unghiRotatie;
        if(z) {
            RotesteObiectZ(obj);

            obj.rotatiiAnterioare.push({'z', unghiRotatie});
        }
        else {
            RotesteObiectX(obj);

            obj.rotatiiAnterioare.push({'x', unghiRotatie});
        }

        unghiRotatie = -unghiRotatie;
    }

    if(right) {
        if(z) {
            RotesteObiectZ(obj);

            obj.rotatiiAnterioare.push({'z', unghiRotatie});
        }
        else {
            RotesteObiectX(obj);

            obj.rotatiiAnterioare.push({'x', unghiRotatie});
        }
    }

    if(up) {
        RotesteObiectY(obj);

        obj.rotatiiAnterioare.push({'y', unghiRotatie});
    }

    if(down) {
        unghiRotatie = -unghiRotatie;

        RotesteObiectY(obj);

        obj.rotatiiAnterioare.push({'y', unghiRotatie});

        unghiRotatie = -unghiRotatie;
    }

    numarareRotatii += (int) obj.rotatiiAnterioare.size() - salvareSize;
    
    if(rotireDiagonala)
        unghiRotatie = saveUnghiRotatie;

    if((up || down || left || right))
        RefacePozitia(obj);

    if(nuDesena) {
        obj.rotatieX = 0;
        obj.rotatieY = 0;
        obj.rotatieZ = 0;
    }
            
    Deseneaza(obj);

    return true;
}

//------------------------------------------------- SFARSIT ROTATIE --------------------------------------------------------------



//----------------------------------------------------- MARIME -------------------------------------------------------------------

void MaresteX(Obiect &obj) {
    obj.marimeX += coeficientMarire;

    for(int i = 0; i < obj.numarVarfuri; i++)
        if(obj.varfuri[i].x < 0)
            obj.varfuri[i].x = min(obj.varfuri[i].x - coeficientMarire, -0.0001);
        else
            obj.varfuri[i].x = max(obj.varfuri[i].x + coeficientMarire, 0.0001);
    
    obj.axaOX.x = max(obj.axaOX.x + coeficientMarire / 2, 0.0001);

    if(varfSelectat != -1)
        for(int i = 0; i < 3; i++)
            obj.axePunctSelectat[i].x = max(obj.axePunctSelectat[i].x + coeficientMarire, 0.0001);
}

void MaresteY(Obiect &obj) {
    obj.marimeY += coeficientMarire;

    for(int i = 0; i < obj.numarVarfuri; i++)
        if(obj.varfuri[i].y < 0)
            obj.varfuri[i].y = min(obj.varfuri[i].y - coeficientMarire, -0.0001);
        else
            obj.varfuri[i].y = max(obj.varfuri[i].y + coeficientMarire, 0.0001);

    obj.axaOY.y = min(obj.axaOY.y - coeficientMarire / 2, -0.0001);

    if(varfSelectat != -1)
        for(int i = 0; i < 3; i++)
            obj.axePunctSelectat[i].y = max(obj.axePunctSelectat[i].y + coeficientMarire, 0.0001);
}

void MaresteZ(Obiect &obj) {
    obj.marimeZ += coeficientMarire;

    for(int i = 0; i < obj.numarVarfuri; i++)
        if(obj.varfuri[i].z < 0)
            obj.varfuri[i].z = min(obj.varfuri[i].z - coeficientMarire, -0.0001);
        else
            obj.varfuri[i].z = max(obj.varfuri[i].z + coeficientMarire, 0.0001);

    obj.axaOZ.z = max(obj.axaOZ.z + coeficientMarire / 2, 0.0001);

    if(varfSelectat != -1)
        for(int i = 0; i < 3; i++)
            obj.axePunctSelectat[i].z = max(obj.axePunctSelectat[i].z + coeficientMarire, 0.0001);
}

bool VerificareMarire(Obiect &obj) {
    if(!obj.activ)
        return false;
    
    bool x = GetAsyncKeyState('X') & 0x8000;
    bool y = GetAsyncKeyState('Y') & 0x8000;
    bool z = GetAsyncKeyState('Z') & 0x8000;
    bool plus = GetAsyncKeyState(VK_ADD) & 0x8000;
    bool minus = GetAsyncKeyState(VK_SUBTRACT) & 0x8000;

    if((minus && plus) || !(x || y || z) || !(minus || plus)) {
        if(numarareMarire != 0) {
            clearStack(redo);

            Undo temp;
            temp.alegere = 3;
            temp.numarRotatii = numarareMarire;
            numarareMarire = 0;

            undo.push(temp);
        }

        return false;
    }
    if(minus && ((x && obj.marimeX <= coeficientMarire) || (y && obj.marimeY <= coeficientMarire) || (z && obj.marimeZ <= coeficientMarire))) {
        if(numarareMarire != 0) {
            clearStack(redo);

            Undo temp;
            temp.alegere = 3;
            temp.numarRotatii = numarareMarire;
            numarareMarire = 0;

            undo.push(temp);
        }

        return false;
    }
    
    EditareRotatie(obj, true);

    CentreazaObiect(obj);

    if(minus)
        coeficientMarire = -coeficientMarire;

    if(x) {
        MaresteX(obj);
        obj.stackMarire.push({'x', coeficientMarire});
        numarareMarire++;
    }
    if(y) {
        MaresteY(obj);
        obj.stackMarire.push({'y', coeficientMarire});
        numarareMarire++;
    }
    if(z) {
        MaresteZ(obj); 
        obj.stackMarire.push({'z', coeficientMarire});
        numarareMarire++;
    }

    RefacePozitia(obj);

    if(varfSelectat != -1) {
        for(int j = 0; j < 3; j++) {
                obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
            }

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].z += 0.5;
    }

    EditareRotatie(obj, false);

    if(minus)
        coeficientMarire = -coeficientMarire;

    obj.centru = GetMijlocObiect(obj);
    obj.centruPentruAxe = obj.centru;
    
    Deseneaza(obj);

    return true; 
}

//------------------------------------------------ SFARSIT MARIME --------------------------------------------------------------

bool VerificaMiscareMousePeAxa(Obiect &obj, int alegere) {
    bool rotatieLocala = local;

    Undo temp;
    temp.alegere = 4;
    temp.axa = axaSelectata;
   
    if(alegere == 0) {
        if(selectarePuncte) {
            for(const int &i : puncteSelectate) {
                temp.coordonate.push_back({i, obj.varfuri[i].x});
                temp.coordonate.push_back({i, obj.varfuri[i].y});
                temp.coordonate.push_back({i, obj.varfuri[i].z});
            }
        }
        else {
            temp.coordonate.push_back({varfSelectat, obj.varfuri[varfSelectat].x});
            temp.coordonate.push_back({varfSelectat, obj.varfuri[varfSelectat].y});
            temp.coordonate.push_back({varfSelectat, obj.varfuri[varfSelectat].z});
        }
        
        for(int i = 0; i < 3; i++) {
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].x});
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].y});
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].z});
        }
    }
    else if(alegere == 1) {
        for(int i = 0; i < obj.varfuri.size(); i++) {
            temp.coordonate.push_back({i, obj.varfuri[i].x});
            temp.coordonate.push_back({i, obj.varfuri[i].y});
            temp.coordonate.push_back({i, obj.varfuri[i].z});
        }

        for(int i = 0; i < 3; i++) {
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].x});
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].y});
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].z});
        }
    }

    clearStack(redo);
    undo.push(temp);
    
    while (!ismouseclick(WM_LBUTTONUP)) {
        int mouseX = mousex();
        int mouseY = mousey();

        if(ultimaPozitieMouseX == mouseX && ultimaPozitieMouseY == mouseY)  
            continue;
            
        if(alegere == 0) {
            if(local)
                EditareRotatie(obj, true);
            
            if(selectarePuncte) {
                for(int i = 0; i < puncteSelectate.size(); i++) {
                    if (axaSelectata == 'x')
                        obj.varfuri[puncteSelectate[i]].x += (mouseX - ultimaPozitieMouseX) / (double) (zoom - POV);
                    else if (axaSelectata == 'y')
                        obj.varfuri[puncteSelectate[i]].y += (mouseY - ultimaPozitieMouseY) / (double) (zoom - POV);
                    else if (axaSelectata == 'z')
                        obj.varfuri[puncteSelectate[i]].z += (mouseX - ultimaPozitieMouseX) / (double) (zoom - POV);
                }

                if(axaSelectata == 'x')
                    for(int i = 0; i < 3; i++)
                        obj.axePunctSelectat[i].x += (mouseX - ultimaPozitieMouseX) / (double) (zoom - POV);
                else if(axaSelectata == 'y')
                    for(int i = 0; i < 3; i++)
                        obj.axePunctSelectat[i].y += (mouseY - ultimaPozitieMouseY) / (double) (zoom - POV);
                else
                    for(int i = 0; i < 3; i++)
                        obj.axePunctSelectat[i].z += (mouseX - ultimaPozitieMouseX) / (double) (zoom - POV);
            }
            else {
                if(axaSelectata == 'x') {
                    obj.varfuri[varfSelectat].x += (mouseX - ultimaPozitieMouseX) / (double) (zoom - POV);

                    for(int i = 0; i < 3; i++)
                        obj.axePunctSelectat[i].x += (mouseX - ultimaPozitieMouseX) / (double) (zoom - POV);
                } else if (axaSelectata == 'y') {
                    obj.varfuri[varfSelectat].y += (mouseY - ultimaPozitieMouseY) / (double) (zoom - POV);

                    for(int i = 0; i < 3; i++)
                        obj.axePunctSelectat[i].y += (mouseY - ultimaPozitieMouseY) / (double) (zoom - POV);     
                } else if (axaSelectata == 'z') {
                    obj.varfuri[varfSelectat].z += (mouseX - ultimaPozitieMouseX) / (double) (zoom - POV);

                    for(int i = 0; i < 3; i++)
                        obj.axePunctSelectat[i].z += (mouseX - ultimaPozitieMouseX) / (double) (zoom - POV);
                }
            }

            if(local)
                EditareRotatie(obj, false);
        }
        else if(alegere == 1) {
            int saveCoeficient = coeficientDeplasare;
            coeficientDeplasare = (mouseX - ultimaPozitieMouseX) / (double) (zoom - POV);

            if(local)
                EditareRotatie(obj, true);

            if(axaSelectata == 'x')
                DeplaseazaX(obj);
            else if(axaSelectata == 'y')
                DeplaseazaY(obj);
            else if(axaSelectata == 'z')
                DeplaseazaZ(obj);
                
            if(local)
                EditareRotatie(obj, false);
                
            coeficientDeplasare = saveCoeficient;
        }

        obj.centru = GetMijlocObiect(obj);

        EditareRotatie(obj, true);

        obj.axaOX = obj.centru;
        obj.axaOY = obj.centru;
        obj.axaOZ = obj.centru;

        obj.axaOX.x += 0.5;
        obj.axaOY.y -= 0.5;
        obj.axaOZ.z += 0.5;

        EditareRotatie(obj, false);

        Deseneaza(obj);

        ultimaPozitieMouseX = mouseX;
        ultimaPozitieMouseY = mouseY;
    }

    axaSelectata = '\0';

    return true;
}

bool esteAproapeDeLinie(Punct2D start, Punct2D end, int px, int py, int toleranta) {
    float numarator = abs((end.x - start.x) * (start.y - py) - (start.x - px) * (end.y - start.y));
    float numitor = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));

    float distanta = numarator / numitor;

    return distanta <= toleranta;
}

bool VerificaMousePeAxa(Obiect &obj, int mouseX, int mouseY) {
    mouseApasat = false;
    ultimaPozitieMouseX = mouseX;
    ultimaPozitieMouseY = mouseY;

    int toleranta = 3, alegere = 0, varf = varfSelectat;
    Punct2D varfSelectat2D = Transforma2D(obj.varfuri[varfSelectat]);
    Punct2D centruObiect = Transforma2D(obj.centru);
    Punct2D varf2D;

    if(varf != -1 && puncteSelectate.size() > 0)
        varf = puncteSelectate[puncteSelectate.size() - 1];
    
    if(varf != -1)
        varf2D = Transforma2D(obj.varfuri[varf]);
    
    if(!local) {
        if(varf != -1 && esteAproapeDeLinie(varf2D, Transforma2D({obj.varfuri[varf].x + 0.5, obj.varfuri[varf].y, obj.varfuri[varf].z}), mouseX, mouseY, toleranta))
            axaSelectata = 'x';
        else if(varf != -1 && esteAproapeDeLinie(varf2D, Transforma2D({obj.varfuri[varf].x, obj.varfuri[varf].y - 0.5, obj.varfuri[varf].z}), mouseX, mouseY, toleranta))
            axaSelectata = 'y';
        else if(varf != -1 && esteAproapeDeLinie(varf2D, Transforma2D({obj.varfuri[varf].x + 0.25, obj.varfuri[varf].y - 0.25, obj.varfuri[varf].z}), mouseX, mouseY, toleranta))
            axaSelectata = 'z';
        else if(esteAproapeDeLinie(centruObiect, Transforma2D({obj.centru.x + 0.5, obj.centru.y, obj.centru.z}), mouseX, mouseY, toleranta)) {
            axaSelectata = 'x';
            alegere = 1;
        }
        else if(esteAproapeDeLinie(centruObiect, Transforma2D({obj.centru.x, obj.centru.y - 0.5, obj.centru.z}), mouseX, mouseY, toleranta)) {
            axaSelectata = 'y';
            alegere = 1;
        }
        else if(esteAproapeDeLinie(centruObiect, Transforma2D({obj.centru.x + 0.25, obj.centru.y - 0.25, obj.centru.z}), mouseX, mouseY, toleranta)) {
            axaSelectata = 'z';
            alegere = 1;
        }
        else
            return false;
    }
    else {
        if(esteAproapeDeLinie(varfSelectat2D, Transforma2D(obj.axePunctSelectat[0]), mouseX, mouseY, toleranta))
            axaSelectata = 'x';
        else if(esteAproapeDeLinie(varfSelectat2D, Transforma2D(obj.axePunctSelectat[1]), mouseX, mouseY, toleranta))
            axaSelectata = 'y';
        else if(esteAproapeDeLinie(varfSelectat2D, Transforma2D(obj.axePunctSelectat[2]), mouseX, mouseY, toleranta))
            axaSelectata = 'z';
        else if(esteAproapeDeLinie(centruObiect, Transforma2D(obj.axaOX), mouseX, mouseY, toleranta)) {
            axaSelectata = 'x';
            alegere = 1;
        }
        else if(esteAproapeDeLinie(centruObiect, Transforma2D(obj.axaOY), mouseX, mouseY, toleranta)) {
            axaSelectata = 'y';
            alegere = 1;
        }
        else if(esteAproapeDeLinie(centruObiect, Transforma2D(obj.axaOZ), mouseX, mouseY, toleranta)) {
            axaSelectata = 'z';
            alegere = 1;
        }
        else
            return false;
    }

    bool temp = VerificaMiscareMousePeAxa(obj, alegere);

    return true;
}

bool estePunctApropiat(const Punct2D &punct, const Punct2D &mouse) {
    return sqrt(pow(mouse.x - punct.x, 2) + pow(mouse.y - punct.y, 2)) <= razaPunct * 2;
}

void UnestePuncte(Obiect &obj) {
    while (!ismouseclick(WM_LBUTTONUP)) {
        int mouseX = mousex();
        int mouseY = mousey();

        if(ultimaPozitieMouseX == mouseX && ultimaPozitieMouseY == mouseY)  
            continue;
        
        pozitieMouseUnirePuncte = {(double) mouseX, (double) mouseY};

        for(int i = 0; i < obj.numarVarfuri; i++)
            if(estePunctApropiat(Transforma2D(obj.varfuri[i]), pozitieMouseUnirePuncte))
                pozitieMouseUnirePuncte = Transforma2D(obj.varfuri[i]);
        
        Deseneaza(obj);

        ultimaPozitieMouseX = mouseX;
        ultimaPozitieMouseY = mouseY;
    }

    for(int i = 0; i < obj.numarVarfuri; i++)
        if(estePunctApropiat(Transforma2D(obj.varfuri[i]), pozitieMouseUnirePuncte)) {
            bool ok = true;
            for(int j = 0; j < obj.numarMuchii; j++)
                if((i == obj.muchii[j].first && varfSelectat == obj.muchii[j].second) || (i == obj.muchii[j].second && varfSelectat == obj.muchii[j].first)) {
                    ok = false;
                    break;
                }
            if(ok) {
                obj.muchii.push_back({varfSelectat, i});
                obj.numarMuchii++;

                clearStack(redo);

                Undo temp;
                temp.alegere = 6;
                temp.numarRotatii = 1;

                undo.push(temp);

                break;
            }
        }
    
    varfSelectat = -1;
        
    Deseneaza(obj);

    clearmouseclick(WM_LBUTTONUP);
}

void EliminaPunct(Obiect &obj) {
    Undo temp;
    temp.alegere = 7;
    temp.numarRotatii = varfSelectat;
    temp.puncteEliminat = obj.varfuri[varfSelectat];

    obj.varfuri.erase(obj.varfuri.begin() + varfSelectat);
    obj.numarVarfuri--;
    int nrEliminariFete = 0;

    int ok;
    for(int i = 0; i < obj.fete.size(); i++){
        ok = 0;
        for(int j : obj.fete[i])
            if(j == varfSelectat) {
                for(int k = 0; k < obj.indexFata.size(); k++)
                    if(obj.indexFata[k] == i) {
                        temp.indexFete.insert(temp.indexFete.begin(), {k, i});
                        obj.indexFata.erase(obj.indexFata.begin() + k);

                        k--;
                    }
                    else if(obj.indexFata[k] > i)
                            obj.indexFata[k]--;

                temp.fete.push_back(obj.fete[i]);
                obj.fete.erase(obj.fete.begin() + i);
                temp.index.push_back(i + nrEliminariFete);
                temp.culoareFete.push_back(obj.culoareFete[i]);
                obj.culoareFete.erase(obj.culoareFete.begin() + i);

                i--;
                nrEliminariFete++;
                obj.numarFete--;
                ok = 1;

                break;
            }
            if(!ok)
                for(int &j:obj.fete[i])
                     if(j > varfSelectat)
                        j--;
    }

    nrEliminariFete = 0;

    for(int i = 0; i < obj.muchii.size(); i++)
        if(obj.muchii[i].first == varfSelectat || obj.muchii[i].second == varfSelectat) {
            obj.muchii.erase(obj.muchii.begin() + i);
            temp.coordonate.push_back({obj.muchii[i].first, obj.muchii[i].second});
            temp.index.push_back(i + nrEliminariFete);

            nrEliminariFete++;
            obj.numarMuchii--;
            i--;
        }
        else { 
            if(obj.muchii[i].first > varfSelectat)
                obj.muchii[i].first--;
            if(obj.muchii[i].second > varfSelectat)
                obj.muchii[i].second--;
        }

    clearStack(redo);
    undo.push(temp);

    obj.centru = GetMijlocObiect(obj);
    EditareRotatie(obj, true);
    obj.axaOX = obj.centru;
    obj.axaOY = obj.centru;
    obj.axaOZ = obj.centru;
    obj.axaOX.x += 0.5;
    obj.axaOY.y -= 0.5;
    obj.axaOZ.z += 0.5;
    EditareRotatie(obj, false);
    
    Deseneaza(obj);
}

void normalizeCoordinates(vector<Punct3D>& points, double minRange = -2.0f, double maxRange = 2.0f) {
    if(points.size() == 0)
        return;
    
    double minX = points[0].x, maxX = points[0].x;
    double minY = points[0].y, maxY = points[0].y;
    double minZ = points[0].z, maxZ = points[0].z;

    for (const auto& point : points) {
        minX = std::min(minX, point.x);
        maxX = std::max(maxX, point.x);
        minY = std::min(minY, point.y);
        maxY = std::max(maxY, point.y);
        minZ = std::min(minZ, point.z);
        maxZ = std::max(maxZ, point.z);
    }

    double rangeX = maxX - minX;
    double rangeY = maxY - minY;
    double rangeZ = maxZ - minZ;

    double maxRangeOriginal = std::max({rangeX, rangeY, rangeZ});

    double scale = (maxRange - minRange) / maxRangeOriginal;

    for (auto& point : points) {
        point.x = minRange + (point.x - minX) * scale;
        point.y = minRange + (point.y - minY) * scale;
        point.z = minRange + (point.z - minZ) * scale;
    }
}

string GetAdresaFisier() {
    OPENFILENAME ofn;
    char filePath[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = ".obj\0*.obj\0.txt\0*.txt\0Toate Fisierele\0*.*\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
        return string(filePath);
    else
        return "";
}

Obiect IncarcaObiect(const string adresa) {
    int nrObiecte = CitesteObiect(adresa);

    varfSelectat = -1;
    fataSelectata = -1;
    selectareInput = -1;
    puncteSelectate.clear();

    for(int i = (int) obiecte.size() - nrObiecte; i < obiecte.size(); i++) {
        if(obiecte[i].varfuri.size() > 30)
            normalizeCoordinates(obiecte[i].varfuri);
        else
            normalizeCoordinates(obiecte[i].varfuri, -1, 1);
        
        obiecte[i].axePunctSelectat = vector<Punct3D>(3, {0, 0, 0});
        obiecte[i].centru = GetMijlocObiect(obiecte[i]);
        obiecte[i].centruPentruAxe = obiecte[i].centru;
        
        obiecte[i].axeObiectStabile = {{0, 0, 0}, {0.30, 0, 0}, {0, -0.30, 0}, {0, 0, 0.30}};

        EditareRotatie(obiecte[i], true);

        obiecte[i].axaOX = {obiecte[i].centru.x + 0.5, obiecte[i].centru.y, obiecte[i].centru.z};
        obiecte[i].axaOY = {obiecte[i].centru.x, obiecte[i].centru.y - 0.5, obiecte[i].centru.z};
        obiecte[i].axaOZ = {obiecte[i].centru.x, obiecte[i].centru.y, obiecte[i].centru.z + 0.5};

        EditareRotatie(obiecte[i], false);

        int k = 100;
        for(int j = 0; j < obiecte[i].fete.size(); j++) {
            obiecte[i].culoareFete.push_back({k, k, k});
            k += 7;
            if(k > 200)
                k = 100;
        }

        sort(obiecte[i].fete.begin(), obiecte[i].fete.end(), [&](const vector<int> &a, const vector<int> &b) {
            double sumaZa = 0, sumaZb = 0;

            for (const int &j : a)
                sumaZa += obiecte[i].varfuri[j].z;

            for (const int &j : b)
                sumaZb += obiecte[i].varfuri[j].z;

            return sumaZa / a.size() > sumaZb / b.size();
        });
    }

    return obiecte[obiectSelectat];
}

void SalveazaObiect(Obiect &obj);

Obiect IncarcaObiect_Obj(const string adresa) {
    Obiect obj;
    obj.numarFete = 0;
    obj.numarMuchii = 0;
    obj.numarVarfuri = 0;

    ifstream f(adresa);

    string linie;
    int nr = 0;

    while(getline(f, linie)) {
        istringstream linieStream(linie);
        string tip;

        linieStream >> tip;

        if(tip == "v") {
            double x, y, z;

            linieStream >> x >> y >> z;

            obj.varfuri.push_back({x, y, z});
            obj.numarVarfuri++;
        }
        else if(tip == "f") {
            obj.fete.push_back({});
            int n = obj.fete.size() - 1;
            string indice;

            while(linieStream >> indice) {
                int indiceVarf = stoi(indice) - 1;
                obj.fete[n].push_back(indiceVarf);
            }

            obj.indexFata.push_back(nr++);
            obj.numarFete++;
        }
    }

    if(obj.varfuri.size() > 30)
        normalizeCoordinates(obj.varfuri);
    else
        normalizeCoordinates(obj.varfuri, -1, 1);

    obj.rotatieX = 0;
    obj.rotatieY = 0;
    obj.rotatieZ = 0;
    obj.marimeX = 1;
    obj.marimeY = 1;
    obj.marimeZ = 1;
    obj.deplasareX = 0;
    obj.deplasareY = 0;
    obj.deplasareZ = 0;

    f.close();

    varfSelectat = -1;
    fataSelectata = -1;
    puncteSelectate.clear();
    obj.axePunctSelectat = vector<Punct3D>(3, {0, 0, 0});
    selectareInput = -1;
    obj.centru = GetMijlocObiect(obj);
    obj.centruPentruAxe = obj.centru;

    obj.axaOX = {obj.centru.x + 0.5, obj.centru.y, obj.centru.z};
    obj.axaOY = {obj.centru.x, obj.centru.y - 0.5, obj.centru.z};
    obj.axaOZ = {obj.centru.x, obj.centru.y, obj.centru.z + 0.5};

    int k = 100;
    for(int i = 0; i < obj.fete.size(); i++) {
        obj.culoareFete.push_back({k, k, k});
        k += 7;
        if(k > 200)
            k = 100;
    }

    sort(obj.fete.begin(), obj.fete.end(), [&](const vector<int> &a, const vector<int> &b) {
        double sumaZa = 0, sumaZb = 0;

        for (const int &j : a)
            sumaZa += obj.varfuri[j].z;

        for (const int &j : b)
            sumaZb += obj.varfuri[j].z;

        return sumaZa / a.size() > sumaZb / b.size();
    });


    obj.axeObiectStabile = {{0, 0, 0}, {0.30, 0, 0}, {0, -0.30, 0}, {0, 0, 0.30}};


    return obj;
}

string SalveazaFisier() {
    OPENFILENAME ofn;
    char filePath[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = ".txt\0*.txt\0.obj\0*.obj\0Toate Fisierele\0*.*\0";
    ofn.lpstrFile = filePath;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "txt";

    if (GetSaveFileName(&ofn))
        return std::string(filePath);
    else
        return "";
}

void SalveazaObiect_Obj(Obiect &obj2, const string &adresa) {
    ofstream f(adresa);

    int plus = 0;

    for(const auto &obj : obiecteSelectate)
        for(auto i : obiecte[obj].varfuri)
            f << "v " << i.x << ' ' << i.y << ' ' << i.z << endl;

    for(const auto &obj : obiecteSelectate) {
        for(auto i : obiecte[obj].fete) {
            f << "f ";
            for(auto j : i)
                f << j + 1 + plus << ' ';
            f << endl;
        }
        plus += (int) obiecte[obj].fete.size() + 2;
    }

    f.close();
}

void SalveazaObiect(Obiect &obj, const string &adresa) {
    ofstream f(adresa);

    f << (int) obiecteSelectate.size() << endl;

    for(const auto &i : obiecteSelectate) {
        f << (int) obiecte[i].varfuri.size() << ' ' << (int) obiecte[i].muchii.size() << ' ' << (int) obiecte[i].fete.size() << endl;

        for(int j = 0; j < obiecte[i].varfuri.size(); j++)
            f << obiecte[i].varfuri[j].x << ' ' << obiecte[i].varfuri[j].y << ' ' << obiecte[i].varfuri[j].z << endl;
        
        for(int j = 0; j < obiecte[i].muchii.size(); j++)
            f << obiecte[i].muchii[j].first << ' ' << obiecte[i].muchii[j].second << endl;
        
        for(int j = 0; j < obiecte[i].fete.size(); j++) {
            f << (int) obiecte[i].fete[j].size() << ' ';
            for(int k = 0; k < obiecte[i].fete[j].size(); k++)
                f << obiecte[i].fete[j][k] << ' ';
            f << endl;
        }

        f << obiecte[i].rotatieX << ' ' << obiecte[i].rotatieY << ' ' << obiecte[i].rotatieZ << endl;
        f << obiecte[i].marimeX << ' ' << obiecte[i].marimeY << ' ' << obiecte[i].marimeZ << endl;
        f << obiecte[i].deplasareX << ' ' << obiecte[i].deplasareY << ' ' << obiecte[i].deplasareZ << endl;

        stack<pair<char, double>> st;
        while(!obiecte[i].rotatiiAnterioare.empty()) {
            st.push(obiecte[i].rotatiiAnterioare.top());
            obiecte[i].rotatiiAnterioare.pop();
        }

        while(!st.empty()) {
            f << st.top().first << ' ' << st.top().second << endl;
            obiecte[i].rotatiiAnterioare.push(st.top());
            st.pop();
        }

        f << endl << '#' << endl << endl;
    }

    f.close();
}

bool VerificaApasareVarf(Obiect &obj) {
    if(!obj.activ)
        return false;
    
    if(ismouseclick(WM_LBUTTONUP)) {
        mouseApasat = false;

        clearmouseclick(WM_LBUTTONDOWN);
        clearmouseclick(WM_LBUTTONUP);
    }

    int x = mousex(), y = mousey();

    if(fereastraFile) {
        if(((butonHover < 2 || butonHover > 4) && butonHover != 6 && butonHover != 7) && !(x >= coordonateButoaneHover[0][0] && y >= coordonateButoaneHover[0][1] && x <= coordonateButoaneHover[0][2] && coordonateButoaneHover[0][3])) {
            fereastraFile = false;
            Deseneaza(obj);
        }
        
        else if(ismouseclick(WM_LBUTTONDOWN)) {
            if(butonHover == 2) {
                string adresa = GetAdresaFisier();

                if(adresa != "") {
                    Obiect obj2;

                    if(adresa.substr(adresa.size() - 4) == ".obj")
                        obj2 = IncarcaObiect_Obj(adresa);
                    else
                        obj2 = IncarcaObiect(adresa);
                    
                    obj2.activ = true;
                    clearStack(undo);
                    clearStack(redo);
                    clearStack(obj.stackDeplasare);
                    clearStack(obj.stackMarire);
                    clearStack(obj.stackRedoRotatie);
                    clearStack(obj.stackRedoMarire);
                    clearStack(obj.stackRedoDeplasare);
                    
                    if(adresa.substr(adresa.size() - 4) == ".obj")
                        obiecte.push_back(obj2);
                    
                    obiectSelectat = (int) obiecte.size() - 1;
                    obiecteSelectate.clear();
                    obiecteSelectate.push_back(obiectSelectat);

                    Deseneaza(obj);
                }
            }
            else if(butonHover == 4)
                inchideEditor = true;
            else if(butonHover == 3) {
                string adresa = SalveazaFisier();

                if(adresa != "")
                    if(adresa.substr(adresa.size() - 4) == ".obj")
                        SalveazaObiect_Obj(obj, adresa);
                    else
                        SalveazaObiect(obj, adresa);
            }
            else if(butonHover == 6)
                UndoChange(obj, true);
            else if(butonHover == 7)
                RedoChange(obj, true);

            clearmouseclick(WM_LBUTTONDOWN);
        }
        return true;
    }

    if(fereastraAdauga) {
        if(!(butonHover >= 35 && butonHover <= 41)) {
            fereastraAdauga = false;
            Deseneaza(obj);
        }
        else if(ismouseclick(WM_LBUTTONDOWN)) {
            string adresa = "";

            if(butonHover == 37)
                adresa = "Obiecte_Default/cub.obj";
            else if(butonHover == 38)
                    adresa = "Obiecte_Default/cilindru.obj";
            else if(butonHover == 39)
                    adresa = "Obiecte_Default/_on.obj";
            else if(butonHover == 40)
                    adresa = "Obiecte_Default/sfera.obj";
            else if(butonHover == 41)
                    adresa = "Obiecte_Default/masca.obj";
            
            if(adresa != "") {    
                Obiect obj2;

                obj2 = IncarcaObiect_Obj(adresa);
                obj2.activ = true;

                clearStack(undo);
                clearStack(redo);
                clearStack(obj.stackDeplasare);
                clearStack(obj.stackMarire);
                clearStack(obj.stackRedoRotatie);
                clearStack(obj.stackRedoMarire);
                clearStack(obj.stackRedoDeplasare);

                obiecte.push_back(obj2);
                obiectSelectat = (int) obiecte.size() - 1;
                obiecteSelectate.clear();
                obiecteSelectate.push_back(obiectSelectat);
            }
            else {
                Obiect obj2;

                obj2.axeObiectStabile = {{0, 0, 0}, {0.30, 0, 0}, {0, -0.30, 0}, {0, 0, 0.30}};
                varfSelectat = -1;
                fataSelectata = -1;
                puncteSelectate.clear();
                obj2.axePunctSelectat = vector<Punct3D>(3, {0, 0, 0});
                selectareInput = -1;
                obj2.centru = {0, 0, 0};
                obj2.centruPentruAxe = obj2.centru;

                obj2.axaOX = {obj2.centru.x + 0.5, obj2.centru.y, obj2.centru.z};
                obj2.axaOY = {obj2.centru.x, obj2.centru.y - 0.5, obj2.centru.z};
                obj2.axaOZ = {obj2.centru.x, obj2.centru.y, obj2.centru.z + 0.5};

                obj2.rotatieX = 0;
                obj2.rotatieY = 0;
                obj2.rotatieZ = 0;
                obj2.marimeX = 1;
                obj2.marimeY = 1;
                obj2.marimeZ = 1;
                obj2.deplasareX = 0;
                obj2.deplasareY = 0;
                obj2.deplasareZ = 0;
                obj2.activ = true;

                obj2.numarVarfuri = 0;
                obj2.numarFete = 0;
                obj2.numarMuchii = 0;

                obiecte.push_back(obj2);
                obiecteSelectate.clear();
                obiectSelectat = (int) obiecte.size() - 1;
                obiecteSelectate.push_back(obiectSelectat);
            }

            Deseneaza(obj);
            clearmouseclick(WM_LBUTTONDOWN);
        }

        return true;
    }

    if(obiectSelectat == -1)
        return false;

    if(varfSelectat != -1 && (GetAsyncKeyState(VK_DELETE) & 0x8000)) {
        EliminaPunct(obj);

        varfSelectat = -1;
        fataSelectata = -1;

        Deseneaza(obj);
        delay(150);

        return true;
    }

    if(!ismouseclick(WM_LBUTTONDOWN) || !intratAici || mouseApasat || (GetAsyncKeyState(VK_LCONTROL) & 0x8000) || adaugaPunct || selectareInput != -1 || (fereastraDreapta && mousex() >= lungimeFereastra - 250) || mousey() <= inaltimeFereastra / 35 || mousex() <= 147)
        return false;
    
    for(int i = 0; i < coordonateInputBox.size(); i++)
        if((x >= coordonateInputBox[i][0] && y >= coordonateInputBox[i][1] && x <= coordonateInputBox[i][2] && y <= coordonateInputBox[i][3]))
            return false;
    
    mouseApasat = true;
    Punct2D mouse = {(double) mousex(), (double) mousey()};
    
    if((varfSelectat != -1 && axePunct) || axeMiscareObiect) {
        clearmouseclick(WM_LBUTTONDOWN);

        if(VerificaMousePeAxa(obj, mouse.x, mouse.y))
            return true;
    }

    int saveVarfSelectat = varfSelectat;
    varfSelectat = -1;
    
    for(int i = 0; i < obj.numarVarfuri; i++)
        if(estePunctApropiat(Transforma2D(obj.varfuri[i]), mouse) && i != saveVarfSelectat) {
            varfSelectat = i;
            
            if(!unestePuncte) {
                EditareRotatie(obj, true);
            
                for(int j = 0; j < 3; j++)
                    obj.axePunctSelectat[j] = obj.varfuri[i];

                obj.axePunctSelectat[0].x += 0.5;
                obj.axePunctSelectat[1].y -= 0.5;
                obj.axePunctSelectat[2].z += 0.5;

                EditareRotatie(obj, false);
            }

            varfSelectat = i;
        }
    
    if(varfSelectat == -1) {
        fataSelectata = -1;

        Deseneaza(obj);
    }

    if(!deseneazaPuncte && !axeMiscareObiect) {
        varfSelectat = -1;
        puncteSelectate.clear();

        Deseneaza(obj);

        return false;
    }
    
    if(!deseneazaPuncte) {
        varfSelectat = -1;
        puncteSelectate.clear();

        Deseneaza(obj);

        return false;
    }
    
    if(unestePuncte) {
        clearmouseclick(WM_LBUTTONDOWN);

        ultimaPozitieMouseX = x;
        ultimaPozitieMouseY = y;
        UnestePuncte(obj);
        varfSelectat = -1;

        return true;
    }

    if(eliminaPuncte) {
        clearmouseclick(WM_LBUTTONDOWN);

        EliminaPunct(obj);
        varfSelectat = -1;

        return true;
    }

    if(saveVarfSelectat != varfSelectat) {
        if(selectarePuncte)
            puncteSelectate.push_back(varfSelectat);
        
        Deseneaza(obj);
    }

    if(varfSelectat != saveVarfSelectat) {
        puncteSelectate.clear();

        Deseneaza(obj);
    }
        
    return true;
}

bool EsteVizibila(Punct3D puncte[], int fata[]) {
    Punct3D vec1 = {puncte[fata[1]].x - puncte[fata[0]].x,
                    puncte[fata[1]].y - puncte[fata[0]].y,
                    puncte[fata[1]].z - puncte[fata[0]].z};
    
    Punct3D vec2 = {puncte[fata[2]].x - puncte[fata[0]].x,
                    puncte[fata[2]].y - puncte[fata[0]].y,
                    puncte[fata[2]].z - puncte[fata[0]].z};

    Punct3D normala = {vec1.y * vec2.z - vec1.z * vec2.y,
                       vec1.z * vec2.x - vec1.x * vec2.z,
                       vec1.x * vec2.y - vec1.y * vec2.x};
    
    return normala.z < 0;
}

void AdaugaPunct(Obiect &obj) {
    if(!deseneazaPuncte || !ismouseclick(WM_LBUTTONDOWN) || !((GetAsyncKeyState(VK_LCONTROL) & 0x8000) || adaugaPunct) || (fereastraDreapta && mousex() >= lungimeFereastra - 250) || mousey() <= inaltimeFereastra / 35 || (fereastraStanga && mousex() <= 140))
        return;
    
    obj.varfuri.push_back({((double) mousex() - lungimeFereastra / 2) / zoom, ((double) mousey() - inaltimeFereastra / 2) / zoom, 0});
    obj.numarVarfuri++;

    Undo temp;
    clearStack(redo);
    temp.alegere = 5;
    undo.push(temp);

    obj.centru = GetMijlocObiect(obj);
    EditareRotatie(obj, true);
    obj.axaOX = obj.centru;
    obj.axaOY = obj.centru;
    obj.axaOZ = obj.centru;
    obj.axaOX.x += 0.5;
    obj.axaOY.y -= 0.5;
    obj.axaOZ.z += 0.5;
    EditareRotatie(obj, false);

    Deseneaza(obj);

    clearmouseclick(WM_LBUTTONDOWN);
}

void CrearePoligon(Obiect &obj) {
    if(!selectarePuncte || !deseneazaPuncte)
        return;
    
    if(ismouseclick(WM_LBUTTONDOWN)) {
        int x = mousex();
        if((fereastraDreapta && x >= lungimeFereastra - 250) || mousey() <= inaltimeFereastra / 35)
            return;
        bool ok = false;
        for(int i = 0; i < obj.numarVarfuri; i++)
            if(estePunctApropiat(Transforma2D(obj.varfuri[i]), {(double) mousex(), (double) mousey()})) {
                ok = true;
                if(find(puncteSelectate.begin(), puncteSelectate.end(), i) == puncteSelectate.end())
                    puncteSelectate.push_back(i);
                else
                    puncteSelectate.erase(find(puncteSelectate.begin(), puncteSelectate.end(), i));
                
                if(puncteSelectate.size() > 0) {
                    varfSelectat = puncteSelectate[puncteSelectate.size() - 1];

                    EditareRotatie(obj, true);

                    for(int j = 0; j < 3; j++)
                        obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];

                    obj.axePunctSelectat[0].x += 0.5;
                    obj.axePunctSelectat[1].y -= 0.5;
                    obj.axePunctSelectat[2].z += 0.5;

                    EditareRotatie(obj, false);
                }
                else
                    varfSelectat = -1;

                Deseneaza(obj);
            }
        
        if(ok)
            clearmouseclick(WM_LBUTTONDOWN);    
    }
}

void VerificaApasareButonBaraSus(Obiect &obj) {
    if(!ismouseclick(WM_LBUTTONDOWN))
        return;

    fereastraDreapta = (fereastraDreapta + 1) % 4;

    Deseneaza(obj);

    clearmouseclick(WM_LBUTTONDOWN);
}

void VerificaHoverButoane(Obiect &obj) {
    int x = mousex(), y = mousey();
    //getmouseclick(WM_MOUSEMOVE, mouseX, mouseY);

    //cout << mouseX << ' ' << mouseY << endl;
    //int ultimaValoare = hoverButoane[0];
    int ultimaValoare = butonHover;
    butonHover = -1;

    for(int i = 0; i < coordonateButoaneHover.size(); i++)
        if(x >= coordonateButoaneHover[i][0] && y >= coordonateButoaneHover[i][1] && x <= coordonateButoaneHover[i][2] && y <= coordonateButoaneHover[i][3]) {
            if(!fereastraFile && !(i >= 2 && i <= 4) && i != 6 && i != 7) {
                butonHover = i;
                break;
            }
            else if(fereastraFile) {
                butonHover = i;
                break;
            }
        }
    
    /* for(int i = 10; i < min(34, (int) obj.varfuri.size()); i++)
        if(x >= coordonateButoaneHover[i][0] && y >= coordonateButoaneHover[i][1] && x <= coordonateButoaneHover[i][2] && y <= coordonateButoaneHover[i][3]) {
            butonHover = i;
            break;
        } */
    
    if((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('A') & 0x8000) && fereastraStanga == 3) {
        obiecteSelectate.clear();
        for(int i = 0; i < obiecte.size(); i++)
            obiecteSelectate.push_back(i);
        obiectSelectat = (int) obiecte.size() - 1;

        clearStack(undo);
                        clearStack(redo);
                        clearStack(obj.stackDeplasare);
                        clearStack(obj.stackMarire);
                        clearStack(obj.stackRedoRotatie);
                        clearStack(obj.stackRedoMarire);
                        clearStack(obj.stackRedoDeplasare);

        Deseneaza(obj);
        return;
    }
    else if((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState(VK_DELETE) & 0x8000) && obiecte.size() > 0) {
        sort(obiecteSelectate.begin(), obiecteSelectate.end(), greater<int>());
        for(const int &i : obiecteSelectate)
            obiecte.erase(obiecte.begin() + i);
        obiecteSelectate.clear();
        obiectSelectat = (int) obiecte.size() - 1;
        obiecteSelectate.push_back(obiectSelectat);
        varfSelectat = -1;
        fataSelectata = -1;
        clearStack(undo);
                        clearStack(redo);
                        clearStack(obj.stackDeplasare);
                        clearStack(obj.stackMarire);
                        clearStack(obj.stackRedoRotatie);
                        clearStack(obj.stackRedoMarire);
                        clearStack(obj.stackRedoDeplasare);

        Deseneaza(obj);

        delay(200);

        return;
    }
    else if((GetAsyncKeyState('O') & 0x8000) && fereastraStanga == 3) {
        Obiect obj2;
        obj2.axeObiectStabile = {{0, 0, 0}, {0.30, 0, 0}, {0, -0.30, 0}, {0, 0, 0.30}};
        varfSelectat = -1;
    fataSelectata = -1;
    puncteSelectate.clear();
    obj2.axePunctSelectat = vector<Punct3D>(3, {0, 0, 0});
    selectareInput = -1;
    obj2.centru = {0, 0, 0};
    obj2.centruPentruAxe = obj2.centru;

    obj2.axaOX = {obj2.centru.x + 0.5, obj2.centru.y, obj2.centru.z};
    obj2.axaOY = {obj2.centru.x, obj2.centru.y - 0.5, obj2.centru.z};
    obj2.axaOZ = {obj2.centru.x, obj2.centru.y, obj2.centru.z + 0.5};

    obj2.rotatieX = 0;
    obj2.rotatieY = 0;
    obj2.rotatieZ = 0;
    obj2.marimeX = 1;
    obj2.marimeY = 1;
    obj2.marimeZ = 1;
    obj2.deplasareX = 0;
    obj2.deplasareY = 0;
    obj2.deplasareZ = 0;
    obj2.activ = true;

    obj2.numarVarfuri = 0;
    obj2.numarFete = 0;
    obj2.numarMuchii = 0;

        obiecte.push_back(obj2);
        obiecteSelectate.clear();
        obiectSelectat = (int) obiecte.size() - 1;
        obiecteSelectate.push_back(obiectSelectat);

        clearStack(undo);
                        clearStack(redo);
                        clearStack(obj.stackDeplasare);
                        clearStack(obj.stackMarire);
                        clearStack(obj.stackRedoRotatie);
                        clearStack(obj.stackRedoMarire);
                        clearStack(obj.stackRedoDeplasare);

        Deseneaza(obj2);

        delay(200);

        return;
    }
    
    if(ultimaValoare != butonHover) {
        Deseneaza(obj);

        return;
    }
    if(butonHover == 1)
        VerificaApasareButonBaraSus(obj);
    if(butonHover == 0) {
        if(!ismouseclick(WM_LBUTTONDOWN))
            return;
        
        fereastraFile = true;

        Deseneaza(obj);

        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(butonHover == 5) {
        if(!ismouseclick(WM_LBUTTONDOWN))
            return;
        
        axeObiectSelectat = (axeObiectSelectat + 1) % 3;

        Deseneaza(obj);

        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(butonHover == 8) {
        if(!ismouseclick(WM_LBUTTONDOWN))
            return;
        
        butonGrid = (butonGrid + 1) % 3;

        if(butonGrid == 1) {
            grid.varfuri.clear();
            grid.muchii.clear();
            int latime = 12, lungime = 12;
    for(int i = 0; i <= lungime; i++) {
        bool adauga = false;
        for(int j = 0; j <= latime; j++) {
            grid.varfuri.push_back({(double) (i - lungime / 2) * 1.5, 3, (double) (j - 10) * 1.5});
            if(adauga && i % 2 && i < lungime)
                col.push_back(grid.varfuri.size() - 1);
            if(j < latime)
                grid.muchii.push_back({i * (latime + 1) + j, i * (latime + 1) + j + 1});
            if(i < lungime)
                grid.muchii.push_back({(i + 1) * (latime + 1) + j, i * (latime + 1) + j});
            if(i > 0 && i < lungime)
            adauga = !adauga;
        }
    }
 /*        for(int i = 0; i <= lungime; i++)
            for(int j = 0; j <= latime; j++) {
                grid.varfuri.push_back({(double) (i - lungime / 2) * 3 + 5, 3, (double) (j - 10) * 3});
                grid.muchii.push_back({ (int) grid.varfuri.size() - 1, (int) grid.varfuri.size() - 2});
            } */
    grid.numarVarfuri = (int) grid.varfuri.size();
    grid.centru = GetMijlocObiect(grid);
    
    for(int i = 0; i < grid.varfuri.size(); i++) {
        grid.varfuri[i].x -= grid.centru.x;
        grid.varfuri[i].z -= grid.centru.z;
    }
        }

        Deseneaza(obj);

        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(butonHover == 9) {
        if(!ismouseclick(WM_LBUTTONDOWN))
            return;
        
        local = !local;

        Deseneaza(obj);

        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(butonHover > 9 && butonHover < 34 && fereastraStanga) {
        if(!ismouseclick(WM_LBUTTONDOWN))
            return;
        int index = butonHover - 10 + indexStart;
        if(fereastraStanga == 1) {
            if(index > obj.varfuri.size() - 1) {
                clearmouseclick(WM_LBUTTONDOWN);
                return;
            }
        if(index == varfSelectat) {
            puncteSelectate.pop_back();
            if(puncteSelectate.size() > 0)
                varfSelectat = puncteSelectate[puncteSelectate.size() - 1];
            else
                varfSelectat = -1;
        }
        else if((GetAsyncKeyState(VK_CONTROL) & 0x8000) && find(puncteSelectate.begin(), puncteSelectate.end(), index) != puncteSelectate.end()) {
            for(int i = 0; i < puncteSelectate.size(); i++)
                if(puncteSelectate[i] == index) {
                    puncteSelectate.erase(puncteSelectate.begin() + i);
                    break;
                }
        }
        else if(index < obj.varfuri.size()) {
            if(GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                selectarePuncte = true;
                if(varfSelectat != -1)
                    puncteSelectate.push_back(index);
                varfSelectat = index;
            }
            else {
                puncteSelectate.clear();
                varfSelectat = index;
                puncteSelectate.push_back(index);
            }
        }
        
        if(varfSelectat != -1) {
            EditareRotatie(obj, true);
            for(int j = 0; j < 3; j++) {
                obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
            }

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].z += 0.5;

            EditareRotatie(obj, false);
        }
        }
        else if(fereastraStanga == 2) {
            if(index > obj.fete.size() - 1) {
                clearmouseclick(WM_LBUTTONDOWN);
                return;
            }
            if(obj.indexFata[index] == fataSelectata)
                fataSelectata = -1;
            else
                fataSelectata = obj.indexFata[index];
        }
        else if(fereastraStanga == 3) {
            if(index > obiecte.size() - 1) {
                clearmouseclick(WM_LBUTTONDOWN);
                return;
            }
            clearStack(undo);
                        clearStack(redo);
                        clearStack(obj.stackDeplasare);
                        clearStack(obj.stackMarire);
                        clearStack(obj.stackRedoRotatie);
                        clearStack(obj.stackRedoMarire);
                        clearStack(obj.stackRedoDeplasare);
            if(index == obiectSelectat) {
            obiecteSelectate.pop_back();
            if(obiecteSelectate.size() > 0)
                obiectSelectat = obiecteSelectate[obiecteSelectate.size() - 1];
            else
                obiectSelectat = -1;
        }
        else if((GetAsyncKeyState(VK_CONTROL) & 0x8000) && find(obiecteSelectate.begin(), obiecteSelectate.end(), index) != obiecteSelectate.end()) {
            for(int i = 0; i < obiecteSelectate.size(); i++)
                if(obiecteSelectate[i] == index) {
                    obiecteSelectate.erase(obiecteSelectate.begin() + i);
                    break;
                }
        }
        else if(index < obiecte.size()) {
            if(GetAsyncKeyState(VK_CONTROL) & 0x8000) {
                selectarePuncte = true;
                if(obiectSelectat != -1)
                    obiecteSelectate.push_back(index);
                obiectSelectat = index;
            }
            else {
                obiecteSelectate.clear();
                obiectSelectat = index;
                obiecteSelectate.push_back(index);
            }
        }

            /* if(index == obiectSelectat)
                obiectSelectat = -1;
            else
                obiectSelectat = index; */
            
            varfSelectat = -1;
            fataSelectata = -1;
        }
        
        Deseneaza(obj);

        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(butonHover == 34) {
        if(!ismouseclick(WM_LBUTTONDOWN))
            return;
        
        fereastraStanga = (fereastraStanga + 1) % 4;

        if(fereastraStanga == 1 && indexStart >= (int) obj.varfuri.size() - 24)
            indexStart = 0;
        else if(fereastraStanga == 2 && indexStart >= (int) obj.fete.size() - 24)
                indexStart = 0;
        else if(fereastraStanga == 3 && indexStart >= (int) obiecte.size() - 24)
                indexStart = 0;

        Deseneaza(obj);

        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(butonHover == 35) {
        if(!ismouseclick(WM_LBUTTONDOWN))
            return;
        
        fereastraAdauga = !fereastraAdauga;

        Deseneaza(obj);

        clearmouseclick(WM_LBUTTONDOWN);
    }
} 

void displayText(char* text, int x1, int y1, int x2, int y2) {
    // Șterge întreaga zonă a casetei
    setfillstyle(SOLID_FILL, COLOR(70, 70, 70));
    bar(x1 + 1, y1 + 1, x2 - 1, y2 - 1);

    // Setează fundalul și culoarea textului
    setbkcolor(COLOR(70, 70, 70)); // Fundal negru
    setcolor(WHITE);   // Text alb

    // Calculare poziții centrate
    int boxWidth = x2 - x1; // Lățimea casetei
    int boxHeight = y2 - y1; // Înălțimea casetei

    int textWidth = textwidth(text); // Lățimea textului
    int textHeight = textheight((char*) "A"); // Înălțimea textului (oricare caracter)

    int startX = x1 + (boxWidth - textWidth) / 2; // Poziția X pentru text centrat
    int startY = y1 + (boxHeight - textHeight) / 2; // Poziția Y pentru text centrat

    // Afișează textul centrat
    outtextxy(startX, startY, text);
}

// Funcție pentru afișarea cursorului licărind
void displayCursor(int cursorPos,  char* text, int x1, int y1, int x2, int y2, bool showCursor) {
    // Calculare poziții centrate
    int boxWidth = x2 - x1; // Lățimea casetei
    int boxHeight = y2 - y1; // Înălțimea casetei

    int textWidth = textwidth(text); // Lățimea textului
    int textHeight = textheight((char*) "A"); // Înălțimea textului

    int startX = x1 + (boxWidth - textWidth) / 2; // Poziția X pentru text centrat
    int startY = y1 + (boxHeight - textHeight) / 2; // Poziția Y pentru text centrat

    // Calculare poziție cursor
    int cursorX = startX + textwidth(text) - textwidth(text + cursorPos);
    int cursorY = startY;

    // Desenează sau ascunde cursorul
    if (showCursor) {
        line(cursorX, cursorY, cursorX, cursorY + textHeight);
    }
    else {
        setcolor(COLOR(70, 70, 70));
        line(cursorX, cursorY, cursorX, cursorY + textHeight);
        setcolor(WHITE);
    }
}

bool isValidCharacter(char &ch, const char* input, int cursorPos) {
    ch = '#';
    for(int i = 0; i < 10; i++)
        if((GetAsyncKeyState('0' + i) & 0x8000)) {
            ch = '0' + i;
            break;
        }
    
    if((selectareInput >= 11 && selectareInput <= 13) || (selectareInput >= 22 && selectareInput <= 24) || (selectareInput >= 26 && selectareInput <= 28) || (selectareInput >= 30 && selectareInput <= 32)) {
        if(ch < '0' || ch > '9')
            return false;
        string s = string(input);
        s.insert(cursorPos, string(1, ch));
        return stoi(s) <= 255;
    }

    // Permite cifrele
    if (ch >= '0' && ch <= '9') {
        return true;
    }
    // Permite un singur caracter '-' doar ca prim caracter
    if ((GetAsyncKeyState(VK_OEM_MINUS) & 0x8000) && cursorPos == 0 && strchr(input, '-') == nullptr) {
        ch = '-';
        return true;
    }
    // Permite un singur caracter '.'
    if ((GetAsyncKeyState(VK_OEM_PERIOD) & 0x8000) && strchr(input, '.') == nullptr) {
        ch = '.';
        return true;
    }
    // Respinge orice alt caracter
    return false;
}


// Funcție pentru gestionarea textului editabil
void getInputWithCursor(char input[20], int maxLength, int x1, int y1, int x2, int y2, Obiect &obj, int selectie) {
    int cursorPos = strlen(input);

    clearmouseclick(WM_LBUTTONDOWN);

    bool showCursor = true;
    clock_t lastToggleTime = clock();
    double ultimaLocatie = obj.varfuri[varfSelectat].x;
    modificaPagina = false;

    if(selectie == 0) {
        Undo temp;
        temp.alegere = 4;
        if(selectarePuncte) {
            for(const int &i : puncteSelectate) {
                //if(selectareInput == 0)
                    temp.coordonate.push_back({i, obj.varfuri[i].x});
                //else if(selectareInput == 1)
                    temp.coordonate.push_back({i, obj.varfuri[i].y});
                //else
                    temp.coordonate.push_back({i, obj.varfuri[i].z});
            }
        }
        else {
            //if(selectareInput == 0) {
                temp.coordonate.push_back({varfSelectat, obj.varfuri[varfSelectat].x});
            //}
            //else if(selectareInput == 1)
                temp.coordonate.push_back({varfSelectat, obj.varfuri[varfSelectat].y});
           // else
                temp.coordonate.push_back({varfSelectat, obj.varfuri[varfSelectat].z});
        }
        
        if(selectareInput == 0)
            temp.axa = 'x';
        else if(selectareInput == 1)
            temp.axa = 'y';
        else
            temp.axa = 'z';
        
        for(int i = 0; i < 3; i++) {
        temp.coordonate.push_back({-1, obj.axePunctSelectat[i].x});
        temp.coordonate.push_back({-1, obj.axePunctSelectat[i].y});
        temp.coordonate.push_back({-1, obj.axePunctSelectat[i].z});
        }
        
        clearStack(redo);
        undo.push(temp);
    }
    else if(selectie == 4) {
        Undo temp;
        temp.alegere = 9;
        temp.index = vector<int>(3);
        temp.fete.push_back(obj.fete[fataSelectata]);
        temp.index[0] = obj.culoareFete[fataSelectata][0];
        temp.index[1] = obj.culoareFete[fataSelectata][1];
        temp.index[2] = obj.culoareFete[fataSelectata][2];
        clearStack(redo);
        undo.push(temp);
    }

    int salvareDimensiuneRotatii = obj.rotatiiAnterioare.size(), salvareDimensiuneDeplasari = obj.stackDeplasare.size(), salvareDimensiuneMarimi = obj.stackMarire.size();

    displayCursor(cursorPos, input, x1, y1, x2, y2, true);

    while (true) {
        if ((clock() - lastToggleTime) > 500) {
            showCursor = !showCursor;
            lastToggleTime = clock();
            displayCursor(cursorPos, input, x1, y1, x2, y2, showCursor);
        }

            if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
                delay(100);
                break;
            }
            if(ismouseclick(WM_LBUTTONDOWN)) {
                int x = mousex(), y = mousey();
                if(!(x >= x1 && y >= y1 && x <= x2 && y <= y2)) {
                    clearmouseclick(WM_LBUTTONDOWN);
                    break;
                }
                clearmouseclick(WM_LBUTTONDOWN);
            }
            // Backspace
            else if ((GetAsyncKeyState(VK_BACK) & 0x8000) && cursorPos > 0) {
                memmove(input + cursorPos - 1, input + cursorPos, strlen(input) - cursorPos + 1);
                cursorPos--;

                string s = string(input);
                if(s.length() == 0)
                    s = "0";
                else if(s.length() <= 1 && (s[0] == '-' || s[0] == '.'))
                    s = "0";
                else if(s.length() == 2 && (s == ".-" || s == "-."))
                    s = "0";
                else if(s.length() >= 2 && s[0] == '-' && s[1] == '.')
                    s.insert(1, "0");
                
                if(selectie == 0) {
                    //cout << s << endl;
                    if(puncteSelectate.size() > 0) {
                        for(int i = 0; i < puncteSelectate.size(); i++) {
                            if(selectareInput % 3 == 0)
                                obj.varfuri[puncteSelectate[i]].x = stod(s) / 10;
                            else if(selectareInput % 3 == 1)
                                obj.varfuri[puncteSelectate[i]].y = -stod(s) / 10;
                            else
                                obj.varfuri[puncteSelectate[i]].z = stod(s) / 10;
                        }
                    }
                    else if(varfSelectat != -1) {
                        if(selectareInput % 3 == 0) {
                            obj.varfuri[varfSelectat].x = stod(s) / 10;
                        }
                        else if(selectareInput % 3 == 1)
                            obj.varfuri[varfSelectat].y = -stod(s) / 10;
                        else
                            obj.varfuri[varfSelectat].z = stod(s) / 10;
                    }
                    obj.centru = GetMijlocObiect(obj);
                    EditareRotatie(obj, true);

                    for(int j = 0; j < 3; j++)
                        obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];

                    obj.axePunctSelectat[0].x += 0.5;
                    obj.axePunctSelectat[1].y -= 0.5;
                    obj.axePunctSelectat[2].z += 0.5;
                    
                    obj.axaOX = obj.centru;
                    obj.axaOY = obj.centru;
                    obj.axaOZ = obj.centru;
                    obj.axaOX.x += 0.5;
                    obj.axaOY.y -= 0.5;
                    obj.axaOZ.z += 0.5;
                    EditareRotatie(obj, false);
                }
                else if(selectie == 1) {
                    if(selectareInput == 11)
                        culoarePuncteSelectate[0] = stoi(s);
                    else if(selectareInput == 12)
                        culoarePuncteSelectate[1] = stoi(s);
                    else
                        culoarePuncteSelectate[2] = stoi(s);
                }
                else if(selectie == 2) {
                    if(selectareInput == 22)
                        culoarePuncte[0] = stoi(s);
                    else if(selectareInput == 23)
                        culoarePuncte[1] = stoi(s);
                    else
                        culoarePuncte[2] = stoi(s);
                }
                else if(selectie == 3) {
                    if(selectareInput == 26)
                        culoareMuchii[0] = stoi(s);
                    else if(selectareInput == 27)
                        culoareMuchii[1] = stoi(s);
                    else
                        culoareMuchii[2] = stoi(s);
                }
                else if(selectie == 4) {
                    if(selectareInput == 30)
                        obj.culoareFete[fataSelectata][0] = stoi(s);
                    else if(selectareInput == 31)
                        obj.culoareFete[fataSelectata][1] = stoi(s);
                    else
                        obj.culoareFete[fataSelectata][2] = stoi(s);
                }
                else if(selectie == 5) {
                    if(local)
                        EditareRotatie(obj, true);
                    Punct3D centruNou = obj.centru;
                    if(selectareInput == 37)
                        centruNou.x = stod(s) / 10;
                    else if(selectareInput == 38)
                        centruNou.y = -stod(s) / 10;
                    else
                        centruNou.z = stod(s) / 10;
                    
                    for(auto &j : obj.varfuri) {
                        j.x += centruNou.x - obj.centru.x;
                        j.y += centruNou.y - obj.centru.y;
                        j.z += centruNou.z - obj.centru.z;
                    }

                    if(selectareInput == 37)
                        obj.stackDeplasare.push({'x', centruNou.x - obj.centru.x});
                    else if(selectareInput == 38)
                        obj.stackDeplasare.push({'y', centruNou.y - obj.centru.y});
                    else
                        obj.stackDeplasare.push({'z', centruNou.z - obj.centru.z});

                    obj.centru = centruNou;

                    if(varfSelectat != -1) {
                        for(int j = 0; j < 3; j++)
                            obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];

                        obj.axePunctSelectat[0].x += 0.5;
                        obj.axePunctSelectat[1].y -= 0.5;
                        obj.axePunctSelectat[2].z += 0.5;
                    }

                    if(!local)
                        EditareRotatie(obj, true);
                        obj.axaOX = obj.centru;
                        obj.axaOY = obj.centru;
                        obj.axaOZ = obj.centru;
                        obj.axaOX.x += 0.5;
                        obj.axaOY.y -= 0.5;
                        obj.axaOZ.z += 0.5;
                        EditareRotatie(obj, false);
                }
                else if(selectie == 6) {
                    double saveUnghiRotatie = unghiRotatie;
                    CentreazaObiect(obj);
                    if(selectareInput == 41) {
                        unghiRotatie = stod(s) * 3.14 / 180.0 - obj.rotatieX;
                        RotesteObiectX(obj);
                        obj.rotatiiAnterioare.push({'x', unghiRotatie});
                    }
                    else if(selectareInput == 42) {
                        unghiRotatie = stod(s) * 3.14 / 180.0 - obj.rotatieY;
                        RotesteObiectY(obj);
                        obj.rotatiiAnterioare.push({'y', unghiRotatie});
                    }
                    else {
                        unghiRotatie = stod(s) * 3.14 / 180.0 - obj.rotatieZ;
                        RotesteObiectZ(obj);
                        obj.rotatiiAnterioare.push({'z', unghiRotatie});
                    }
                    
                    unghiRotatie = saveUnghiRotatie;

                    RefacePozitia(obj);
                }
                else if(selectie == 7) {
                    EditareRotatie(obj, true);
                    double saveCoeficientMarime = coeficientMarire;
                    if(selectareInput == 45) {
                        coeficientMarire = stod(s) - obj.marimeX;
                        MaresteX(obj);
                        obj.stackMarire.push({'x', coeficientMarire});
                    }
                    else if(selectareInput == 46) {
                        coeficientMarire = stod(s) - obj.marimeY;
                        MaresteY(obj);
                        obj.stackMarire.push({'y', coeficientMarire});
                    }
                    else {
                        coeficientMarire = stod(s) - obj.marimeZ;
                        MaresteZ(obj);
                        obj.stackMarire.push({'z', coeficientMarire});
                    }
                    coeficientMarire = saveCoeficientMarime;
                    EditareRotatie(obj, false);
                }
                else if(selectie == 8) {
                    if(selectareInput == 48)
                        coeficientDeplasare = stod(s);
                    else if(selectareInput == 49)
                        unghiRotatie = stod(s) / 180 * 3.14;
                    else
                        coeficientMarire = stod(s);
                }
                
                Deseneaza(obj);
                displayText(input, x1, y1, x2, y2);

                displayCursor(cursorPos, input, x1, y1, x2, y2, true);
                showCursor = false;
                delay(100);
            }
            else if((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
                if((GetAsyncKeyState(VK_LEFT) & 0x8000) && cursorPos > 0) // Săgeată la stânga
                    cursorPos--;
                else if((GetAsyncKeyState(VK_RIGHT) & 0x8000) && cursorPos < strlen(input)) // Săgeată la dreapta
                    cursorPos++;
                
                displayText(input, x1, y1, x2, y2);

                displayCursor(cursorPos, input, x1, y1, x2, y2, true);
                showCursor = false;
                delay(100);
            }
            // Adaugă caractere
            else { 
                char ch;
                if(isValidCharacter(ch, input, cursorPos) && strlen(input) < maxLength - 1 + (input[0] == '-')) {
                    memmove(input + cursorPos + 1, input + cursorPos, strlen(input) - cursorPos + 1);
                    input[cursorPos] = ch;
                    cursorPos++;

                    string s = string(input);
                    if(s.length() == 0)
                        s = "0";
                    else if(s.length() == 1 && (s[0] == '-' || s[0] == '.'))
                        s = "0";
                    else if(s.length() == 2 && (s == ".-" || s == "-."))
                        s = "0";
                    else if(s.length() >= 2 && s[0] == '-' && s[1] == '.')
                        s.insert(1, "0");

                    if(selectie == 0) {
                        if(puncteSelectate.size() > 0) {
                        for(int i = 0; i < puncteSelectate.size(); i++) {
                            if(selectareInput % 3 == 0)
                                obj.varfuri[puncteSelectate[i]].x = stod(s) / 10;
                            else if(selectareInput % 3 == 1)
                                obj.varfuri[puncteSelectate[i]].y = -stod(s) / 10;
                            else
                                obj.varfuri[puncteSelectate[i]].z = stod(s) / 10;
                        }
                    }
                    else if(varfSelectat != -1) {
                        if(selectareInput % 3 == 0) {
                            obj.varfuri[varfSelectat].x = stod(s) / 10;
                        }
                        else if(selectareInput % 3 == 1)
                            obj.varfuri[varfSelectat].y = -stod(s) / 10;
                        else
                            obj.varfuri[varfSelectat].z = stod(s) / 10;
                    }

                        obj.centru = GetMijlocObiect(obj);
                            EditareRotatie(obj, true);
                        for(int j = 0; j < 3; j++)
                            obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];

                        obj.axePunctSelectat[0].x += 0.5;
                        obj.axePunctSelectat[1].y -= 0.5;
                        obj.axePunctSelectat[2].z += 0.5;

                        obj.axaOX = obj.centru;
                        obj.axaOY = obj.centru;
                        obj.axaOZ = obj.centru;
                        obj.axaOX.x += 0.5;
                        obj.axaOY.y -= 0.5;
                        obj.axaOZ.z += 0.5;
                        EditareRotatie(obj, false);
                    }
                    else if(selectie == 1) {
                        if(selectareInput == 11)
                            culoarePuncteSelectate[0] = stoi(s);
                        else if(selectareInput == 12)
                            culoarePuncteSelectate[1] = stoi(s);
                        else
                            culoarePuncteSelectate[2] = stoi(s);
                    }
                    else if(selectie == 2) {
                    if(selectareInput == 22)
                        culoarePuncte[0] = stoi(s);
                    else if(selectareInput == 23)
                        culoarePuncte[1] = stoi(s);
                    else
                        culoarePuncte[2] = stoi(s);
                }
                else if(selectie == 3) {
                    if(selectareInput == 26)
                        culoareMuchii[0] = stoi(s);
                    else if(selectareInput == 27)
                        culoareMuchii[1] = stoi(s);
                    else
                        culoareMuchii[2] = stoi(s);
                }
                else if(selectie == 4) {
                    if(selectareInput == 30)
                        obj.culoareFete[fataSelectata][0] = stoi(s);
                    else if(selectareInput == 31)
                        obj.culoareFete[fataSelectata][1] = stoi(s);
                    else
                        obj.culoareFete[fataSelectata][2] = stoi(s);
                }
                else if(selectie == 5) {
                    if(local)
                        EditareRotatie(obj, true);
                    Punct3D centruNou = obj.centru;
                    if(selectareInput == 37)
                        centruNou.x = stod(s) / 10;
                    else if(selectareInput == 38)
                        centruNou.y = -stod(s) / 10;
                    else
                        centruNou.z = stod(s) / 10;
                    
                    for(auto &j : obj.varfuri) {
                        j.x += centruNou.x - obj.centru.x;
                        j.y += centruNou.y - obj.centru.y;
                        j.z += centruNou.z - obj.centru.z;
                    }

                    if(selectareInput == 37)
                        obj.stackDeplasare.push({'x', centruNou.x - obj.centru.x});
                    else if(selectareInput == 38)
                        obj.stackDeplasare.push({'y', centruNou.y - obj.centru.y});
                    else
                        obj.stackDeplasare.push({'z', centruNou.z - obj.centru.z});

                    obj.centru = centruNou;

                    if(varfSelectat != -1) {
                        for(int j = 0; j < 3; j++)
                            obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];

                        obj.axePunctSelectat[0].x += 0.5;
                        obj.axePunctSelectat[1].y -= 0.5;
                        obj.axePunctSelectat[2].z += 0.5;
                    }

                    if(!local)
                        EditareRotatie(obj, true);
                        obj.axaOX = obj.centru;
                        obj.axaOY = obj.centru;
                        obj.axaOZ = obj.centru;
                        obj.axaOX.x += 0.5;
                        obj.axaOY.y -= 0.5;
                        obj.axaOZ.z += 0.5;
                        EditareRotatie(obj, false);
                }
                else if(selectie == 6) {
                    double saveUnghiRotatie = unghiRotatie;
                    CentreazaObiect(obj);
                    if(selectareInput == 41) {
                        unghiRotatie = stod(s) * 3.14 / 180.0 - obj.rotatieX;
                        RotesteObiectX(obj);
                        obj.rotatiiAnterioare.push({'x', unghiRotatie});
                    }
                    else if(selectareInput == 42) {
                        unghiRotatie = stod(s) * 3.14 / 180.0 - obj.rotatieY;
                        RotesteObiectY(obj);
                        obj.rotatiiAnterioare.push({'y', unghiRotatie});
                    }
                    else {
                        unghiRotatie = stod(s) * 3.14 / 180.0 - obj.rotatieZ;
                        RotesteObiectZ(obj);
                        obj.rotatiiAnterioare.push({'z', unghiRotatie});
                    }
                    
                    unghiRotatie = saveUnghiRotatie;

                    RefacePozitia(obj);
                }
                else if(selectie == 7) {
                    EditareRotatie(obj, true);
                    double saveCoeficientMarime = coeficientMarire;
                    if(selectareInput == 45) {
                        coeficientMarire = stod(s) - obj.marimeX;
                        MaresteX(obj);
                        obj.stackMarire.push({'x', coeficientMarire});
                    }
                    else if(selectareInput == 46) {
                        coeficientMarire = stod(s) - obj.marimeY;
                        MaresteY(obj);
                        obj.stackMarire.push({'y', coeficientMarire});
                    }
                    else {
                        coeficientMarire = stod(s) - obj.marimeZ;
                        MaresteZ(obj);
                        obj.stackMarire.push({'z', coeficientMarire});
                    }
                    coeficientMarire = saveCoeficientMarime;
                    EditareRotatie(obj, false);
                }
                else if(selectie == 8) {
                    if(selectareInput == 48)
                        coeficientDeplasare = stod(s);
                    else if(selectareInput == 49)
                        unghiRotatie = stod(s) / 180 * 3.14;
                    else
                        coeficientMarire = stod(s);
                }

                Deseneaza(obj);
                displayText(input, x1, y1, x2, y2);

                displayCursor(cursorPos, input, x1, y1, x2, y2, true);
                showCursor = false;
                delay(100);
            }
            
        }

        delay(10);
    }

    if(salvareDimensiuneRotatii != (int) obj.rotatiiAnterioare.size()) {
        Undo temp;
        temp.alegere = 1;
        temp.numarRotatii = (int) obj.rotatiiAnterioare.size() - salvareDimensiuneRotatii;
        clearStack(redo);
        undo.push(temp);
    }
    else if(salvareDimensiuneDeplasari != (int) obj.stackDeplasare.size()) {
        Undo temp;
        temp.alegere = 2;
        temp.numarRotatii = (int) obj.stackDeplasare.size() - salvareDimensiuneDeplasari;
        clearStack(redo);
        undo.push(temp);
    }
    else if(salvareDimensiuneMarimi != (int) obj.stackMarire.size()) {
        Undo temp;
        temp.alegere = 3;
        temp.numarRotatii = (int) obj.stackMarire.size() - salvareDimensiuneMarimi;
        clearStack(redo);
        undo.push(temp);
    }
    else if(selectie == 4 && undo.top().index.size() == 3 && undo.top().index[0] == obj.culoareFete[fataSelectata][0] && undo.top().index[1] == obj.culoareFete[fataSelectata][1] && undo.top().index[2] == obj.culoareFete[fataSelectata][2]) {
        undo.pop();
    }
    else if(selectie == 0) {
        int n = undo.top().coordonate.size() - 1;
        if(selectareInput == 0 && obj.varfuri[varfSelectat].x == undo.top().coordonate[n].second)
            undo.pop();
        else if(selectareInput == 1 && obj.varfuri[varfSelectat].y == undo.top().coordonate[n].second)
            undo.pop();
        else if(selectareInput == 2 && obj.varfuri[varfSelectat].z == undo.top().coordonate[n].second)
            undo.pop();
    }

    modificaPagina = true;
    selectareInput = -1;
    setactivepage(pagina);

    Deseneaza(obj);

    setvisualpage(pagina);

    displayCursor(cursorPos, input, x1, y1, x2, y2, false);
}

void EditareInputBox(Obiect &obj, int selectie) {
    if(selectareInput == -1)
        return;

    char text[20] = "";
    int i = 0;
    /* if(obj.varfuri[varfSelectat].x < 0)
        text[i++] = '-'; */
    //int nr = (int) obj.varfuri[varfSelectat].x;
    string s;
    if(selectie == 0) {
        if(selectareInput % 3 == 0)
            s = to_string(obj.varfuri[varfSelectat].x * 10);
        else if(selectareInput % 3 == 1)
            s = to_string(-obj.varfuri[varfSelectat].y * 10);
        else
            s = to_string(obj.varfuri[varfSelectat].z * 10);
    }
    else if(selectie == 1) {
        if(selectareInput == 11)
            s = to_string(culoarePuncteSelectate[0]);
        else if(selectareInput == 12)
            s = to_string(culoarePuncteSelectate[1]);
        else
            s = to_string(culoarePuncteSelectate[2]);
    }
    else if(selectie == 2) {
         if(selectareInput == 22)
            s = to_string(culoarePuncte[0]);
        else if(selectareInput == 23)
            s = to_string(culoarePuncte[1]);
        else
            s = to_string(culoarePuncte[2]);
    }
    else if(selectie == 3) {
         if(selectareInput == 26)
            s = to_string(culoareMuchii[0]);
        else if(selectareInput == 27)
            s = to_string(culoareMuchii[1]);
        else
            s = to_string(culoareMuchii[2]);
    }
    else if(selectie == 4) {
        if(selectareInput == 30)
            s = to_string(obj.culoareFete[fataSelectata][0]);
        else if(selectareInput == 31)
            s = to_string(obj.culoareFete[fataSelectata][1]);
        else
            s = to_string(obj.culoareFete[fataSelectata][2]);
    }
    else if(selectie == 5) {
        if(selectareInput == 37)
            s = to_string(obj.centru.x * 10);
        else if(selectareInput == 38)
            if(obj.centru.y != 0)
                s = to_string(-obj.centru.y * 10);
            else
                s = "0.00";
        else
            s = to_string(obj.centru.z * 10);
    }
    else if(selectie == 6) {
        if(selectareInput == 41)
            if(obj.rotatieX == 0)
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
        else if(selectareInput == 42)
            if(obj.rotatieY == 0)
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
            if(obj.rotatieZ == 0)
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
    }
    else if(selectie == 7) {
        if(selectareInput == 45)
            s = to_string(obj.marimeX);
        else if(selectareInput == 46)
            s = to_string(obj.marimeY);
        else
            s = to_string(obj.marimeZ);
    }
    else if(selectie == 8) {
        if(selectareInput == 48)
            s = to_string(coeficientDeplasare);
        else if(selectareInput == 49)
            s = to_string(unghiRotatie * 180 / 3.14);
        else
            s = to_string(coeficientMarire);
    }
    
    //cout << s << " ; ";
    while(i < s.length() && s[i] != '.') {
        text[i] = s[i];
        i++;
    }
    /* nr = (int) (obj.varfuri[varfSelectat].x * 100) % 100;
    while(nr) {
        text[i++] = nr % 10 + '0';
        nr /= 10;
    } */
   if(selectie == 0 || selectie == 5 || selectie == 6 || selectie == 7 || selectie == 8) {
    text[i] = '.';
    text[i + 1] = s[i + 1];
    text[i + 2] = s[i + 2];
    text[i + 3] = '\0';
   }
   else// if(selectie == 1)
        text[i] = '\0';
    
    //cout << "Text = " << text << ' ' << i << ' ' << strlen(text) << endl;

    //cout << "da ";
    //pagina=1-pagina;
                setactivepage(1-pagina); 
                setvisualpage(1-pagina);

    displayText(text, coordonateInputBox[selectareInput][0], coordonateInputBox[selectareInput][1], coordonateInputBox[selectareInput][2], coordonateInputBox[selectareInput][3]);
    getInputWithCursor(text, 8, coordonateInputBox[selectareInput][0], coordonateInputBox[selectareInput][1], coordonateInputBox[selectareInput][2], coordonateInputBox[selectareInput][3], obj, selectie);
/* setactivepage(pagina);
setvisualpage(1-pagina); */
//Deseneaza(obj);
}

void VerificaApasareButon(Obiect &obj) {
    intratAici = true;
    if(!ismouseclick(WM_LBUTTONDOWN))
        return;
    int x = mousex(), y = mousey();

    selectareInput = -1;

    for(int i = 0; i < 25 && fereastraDreapta == 1; i++)
        if(i != 20 && x >= coordonateInputBox[i][0] && y >= coordonateInputBox[i][1] && x <= coordonateInputBox[i][2] && y <= coordonateInputBox[i][3]) {
            selectareInput = i;
        }

    for(int i = 25; i < 36 && fereastraDreapta == 2; i++)
        if(x >= coordonateInputBox[i][0] && y >= coordonateInputBox[i][1] && x <= coordonateInputBox[i][2] && y <= coordonateInputBox[i][3]) {
            selectareInput = i;
        }
    
    if(fereastraDreapta == 2 && x >= coordonateInputBox[20][0] && y >= coordonateInputBox[20][1] && x <= coordonateInputBox[20][2] && y <= coordonateInputBox[20][3])
        selectareInput = 20;

    for(int i = 36; i < coordonateInputBox.size() && fereastraDreapta == 3; i++)
        if(x >= coordonateInputBox[i][0] && y >= coordonateInputBox[i][1] && x <= coordonateInputBox[i][2] && y <= coordonateInputBox[i][3]) {
            selectareInput = i;
        }
    
    //selectie: 0 -> pozitie; 1 -> culoare(RGB)
    if(selectareInput != -1 && varfSelectat != -1) {
        if(selectareInput < 9)
            EditareInputBox(obj, 0);
    }

    if(selectareInput == 9) {
        axePunct = !axePunct;
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(selectareInput == 10) {
        colorarePuncteSelectate = !colorarePuncteSelectate;
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(colorarePuncteSelectate && selectareInput >= 11 && selectareInput <= 13) {
        EditareInputBox(obj, 1);
    }
    else if(selectareInput == 14) {
        selectarePuncte = !selectarePuncte;
        unestePuncte = false;
        adaugaPunct = false;
        eliminaPuncte = false;
        if(!selectarePuncte) {
            puncteSelectate.clear();
            varfSelectat = -1;
        }
        else if(varfSelectat != -1)
                puncteSelectate.push_back(varfSelectat);
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(selectareInput == 15) {
        clearmouseclick(WM_LBUTTONDOWN);
        setactivepage(1-pagina);
        setcolor(COLOR(60, 60, 60));
        rectangle(coordonateInputBox[15][0] - 1, coordonateInputBox[15][1] - 1, coordonateInputBox[15][2] + 1, coordonateInputBox[15][3] + 1);

        Undo temp;
        temp.alegere = 6;
        temp.numarRotatii = 0;
        for(int i = 1; i < puncteSelectate.size(); i++) {
            bool ok = true;
            for(int j = 0; j < obj.numarMuchii; j++)
                if((obj.muchii[j].first == puncteSelectate[0] && obj.muchii[j].second == puncteSelectate[i]) || (obj.muchii[j].first == puncteSelectate[i] && obj.muchii[j].second == puncteSelectate[0])) {
                    ok = false;
                    break;
                }
            if(ok) {
                obj.muchii.push_back({puncteSelectate[0], puncteSelectate[i]});
                obj.numarMuchii++;
                temp.numarRotatii++;
            }
        }

        if(temp.numarRotatii > 0) {
            clearStack(redo);
            undo.push(temp);
        }

        delay(150);
        modificaPagina = false;
        Deseneaza(obj);
        modificaPagina = true;
        setactivepage(pagina);
    }
    else if(selectareInput == 16) { //uneste puncte consecutive
        clearmouseclick(WM_LBUTTONDOWN);
        setactivepage(1-pagina);
        setcolor(COLOR(60, 60, 60));
        rectangle(coordonateInputBox[16][0] - 1, coordonateInputBox[16][1] - 1, coordonateInputBox[16][2] + 1, coordonateInputBox[16][3] + 1);

        Undo temp;
        temp.alegere = 6;
        temp.numarRotatii = 0;
        for(int i = 0; i < (int) puncteSelectate.size() - 1; i++) {
            bool ok = true;
            for(int j = 0; j < obj.numarMuchii; j++)
                if((obj.muchii[j].first == puncteSelectate[i] && obj.muchii[j].second == puncteSelectate[i + 1]) || (obj.muchii[j].first == puncteSelectate[i + 1] && obj.muchii[j].second == puncteSelectate[i])) {
                    ok = false;
                    break;
                }
            if(ok) {
                obj.muchii.push_back({puncteSelectate[i], puncteSelectate[i + 1]});
                obj.numarMuchii++;
                temp.numarRotatii++;
            }
        }

        if(temp.numarRotatii > 0) {
            clearStack(redo);
            undo.push(temp);
        }

        delay(150);
        modificaPagina = false;
        Deseneaza(obj);
        modificaPagina = true;
        setactivepage(pagina);
    }
    else if(selectareInput == 17) {
        unestePuncte = !unestePuncte;
        selectarePuncte = false;
        eliminaPuncte = false;
        adaugaPunct = false;
        varfSelectat = -1;
        puncteSelectate.clear();
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(selectareInput == 18) {
        adaugaPunct = !adaugaPunct;
        eliminaPuncte = false;
        unestePuncte = false;
        selectarePuncte = false;
        varfSelectat = -1;
        puncteSelectate.clear();
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(selectareInput == 19) {
        eliminaPuncte = !eliminaPuncte;
        adaugaPunct = false;
        unestePuncte = false;
        selectarePuncte = false;
        varfSelectat = -1;
        puncteSelectate.clear();
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
    }
    else if(selectareInput == 20) {
        clearmouseclick(WM_LBUTTONDOWN);
        setactivepage(1-pagina);
        setcolor(COLOR(60, 60, 60));
        rectangle(coordonateInputBox[16][0] - 1, coordonateInputBox[16][1] - 1, coordonateInputBox[16][2] + 1, coordonateInputBox[16][3] + 1);

        if(puncteSelectate.size() >= 3) {
            obj.fete.push_back({});
            int n = obj.fete.size() - 1;
            for(auto i : puncteSelectate)
                obj.fete[n].push_back(i);
            obj.numarFete++;

            for(int i = 0; i < puncteSelectate.size(); i++) {
                vector<pair<int, int>>::iterator it1 = find_if(obj.muchii.begin(), obj.muchii.end(), [&](const pair<int, int> &p) {
                    return (p.first == puncteSelectate[i] && p.second == puncteSelectate[(i + 1) % static_cast<int>(puncteSelectate.size())])
                    || (p.second == puncteSelectate[i] && p.first == puncteSelectate[(i + 1) % static_cast<int>(puncteSelectate.size())]);
                });

                if(it1 != obj.muchii.end()) {
                    obj.muchii.erase(obj.muchii.begin() + i);
                    continue;
                }
            }
        
            obj.culoareFete.push_back({100, 100, 100});
            obj.indexFata.push_back((int) obj.indexFata.size());
            Undo temp;
            temp.alegere = 8;
            temp.fete.push_back(obj.fete[obj.fete.size() - 1]);
            clearStack(redo);
            undo.push(temp);
        }
        modificaPagina = false;
        Deseneaza(obj);
        modificaPagina = true;
        setactivepage(pagina);
        delay(150);
        
    }
    else if(selectareInput == 21) {
        clearmouseclick(WM_LBUTTONDOWN);
        deseneazaPuncte = !deseneazaPuncte;
        varfSelectat = -1;
        puncteSelectate.clear();
        Deseneaza(obj);
        delay(150);
    }
    else if(selectareInput >= 22 && selectareInput <= 24)
        EditareInputBox(obj, 2);
    else if(selectareInput == 25) {
        deseneazaMuchii = !deseneazaMuchii;
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
        delay(150);
    }
    else if(selectareInput >= 26 && selectareInput <= 28)
        EditareInputBox(obj, 3);
    else if(selectareInput == 29) {
        deseneazaFete = !deseneazaFete;
        xRay = false;
        if(!deseneazaFete)
            fataSelectata = -1;
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
        delay(150);
    }
    else if(selectareInput >= 30 && selectareInput <= 32 && fataSelectata != -1) {
        EditareInputBox(obj, 4);
    }
    else if(selectareInput == 33 && fataSelectata != -1) {
        clearmouseclick(WM_LBUTTONDOWN);
        setactivepage(1-pagina);
        setcolor(COLOR(60, 60, 60));
        rectangle(coordonateInputBox[33][0] - 1, coordonateInputBox[33][1] - 1, coordonateInputBox[33][2] + 1, coordonateInputBox[33][3] + 1);

        copieCuloareFata = obj.culoareFete[fataSelectata];

        modificaPagina = false;
        Deseneaza(obj);
        modificaPagina = true;
        setactivepage(pagina);
        delay(150);
    }
    else if(selectareInput == 34 && fataSelectata != -1) {
        clearmouseclick(WM_LBUTTONDOWN);
        setactivepage(1-pagina);
        setcolor(COLOR(60, 60, 60));
        rectangle(coordonateInputBox[33][0] - 1, coordonateInputBox[33][1] - 1, coordonateInputBox[33][2] + 1, coordonateInputBox[33][3] + 1);

        if(obj.culoareFete[fataSelectata] != copieCuloareFata) {
            Undo temp;
            temp.alegere = 9;
            temp.index = vector<int>(3);
            temp.fete.push_back(obj.fete[fataSelectata]);
            temp.index[0] = obj.culoareFete[fataSelectata][0];
            temp.index[1] = obj.culoareFete[fataSelectata][1];
            temp.index[2] = obj.culoareFete[fataSelectata][2];
            clearStack(redo);
            undo.push(temp);
        }

        obj.culoareFete[fataSelectata] = copieCuloareFata;

        modificaPagina = false;
        Deseneaza(obj);
        modificaPagina = true;
        setactivepage(pagina);
        delay(150);
    }
    else if(selectareInput == 35) {
        xRay = !xRay;
        deseneazaFete = false;
        if(xRay)
            deseneazaMuchii = true;
        fataSelectata = -1;
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
        delay(150);
    }
    else if(selectareInput == 36) {
        for(const int &i : obiecteSelectate)
            obiecte[i].activ = !obiecte[i].activ;
        //obj.activ = !obj.activ;
        fataSelectata = -1;
        varfSelectat = -1;
        puncteSelectate.clear();
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
        delay(150);
    }
    else if(selectareInput >= 37 && selectareInput <= 39) {
        EditareInputBox(obj, 5);
    }
    else if(selectareInput == 40) {
        axeMiscareObiect = !axeMiscareObiect;
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
        delay(150);
    }
    else if(selectareInput >= 41 && selectareInput <= 43) {
        EditareInputBox(obj, 6);
    }
    else if(selectareInput == 44) {
        max360 = !max360;
        Deseneaza(obj);
        clearmouseclick(WM_LBUTTONDOWN);
        delay(150);
    }
    else if(selectareInput >= 45 && selectareInput <= 47)
        EditareInputBox(obj, 7);
    else if(selectareInput >= 48 && selectareInput <= 50)
        EditareInputBox(obj, 8);

    if((fereastraDreapta && mousex() >= lungimeFereastra - 250) || mousey() <= inaltimeFereastra / 35)
        clearmouseclick(WM_LBUTTONDOWN);
}  

void SelecteazaFata(Obiect &obj, int fataIndex) {
    if(!deseneazaFete || obj.fete.size() == 0)
        return;
    if (fataIndex >= 0 && fataIndex < (int) obj.fete.size()) {
        for(int i = 0; i < obj.indexFata.size(); i++)
            if(obj.indexFata[i] == fataIndex) {
                fataSelectata = i;
                break;
            }
    }
    else
        if (fataIndex < 0) {
        for(int i = 0; i < obj.indexFata.size(); i++)
            if(obj.indexFata[i] == obj.indexFata.size() - 1) {
                fataSelectata = i;
                break;
            }
        }
        else {
        for(int i = 0; i < obj.indexFata.size(); i++)
            if(obj.indexFata[i] == 0) {
                fataSelectata = i;
                break;
            }
        }
    Deseneaza(obj);
    delay(200);
    
}

void SchimbaCuloareaFata(Obiect &obj, int culoare2) {
    if(fataSelectata == -1)
        return;

    obj.culoareFata[fataSelectata] = culoare2 * 30;
    
    Deseneaza(obj);
    delay(200);
}

void UndoChange(Obiect &obj, bool skip) {
    if(((!GetAsyncKeyState(VK_CONTROL) || !GetAsyncKeyState('Z')) && !skip) || undo.empty())
        return;

    int alegere = undo.top().alegere, saveLocal = local;
    local = false;
    if(alegere == 1) {
        int nrRotatii = undo.top().numarRotatii;
        double saveUnghiRotatie = unghiRotatie;
        redo.push(undo.top());
        CentreazaObiect(obj);
        while(nrRotatii--) {
            unghiRotatie = -obj.rotatiiAnterioare.top().second;
            obj.stackRedoRotatie.push(obj.rotatiiAnterioare.top());
            if(undo.top().axa == 'g')
                undoRotatieGrid = true;
            if(obj.rotatiiAnterioare.top().first == 'x')
                RotesteObiectX(obj);
            else if(obj.rotatiiAnterioare.top().first == 'y')
                RotesteObiectY(obj);
            else
                RotesteObiectZ(obj);
            obj.rotatiiAnterioare.pop();
            undoRotatieGrid = false;
        }
        unghiRotatie = saveUnghiRotatie;

        RefacePozitia(obj);

        /* if(varfSelectat != -1) {
                for(int j = 0; j < 3; j++) {
                    obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
                //obj.axePunctSelectat[j].z = (zoom * obj.axePunctSelectat[j].x * POV) / (obj.axePunctSelectat[j].x - lungimeFereastra / 2)
                }
            

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].x += 0.25;
            obj.axePunctSelectat[2].y -= 0.25;
            } */
    }
    else if(alegere == 2) {
        int nrDeplasari = undo.top().numarRotatii;
        //cout << nrDeplasari << ' ' << obj.stackDeplasare.size() << ' ' << undo.size() << ' ' << numarareDeplasare << endl;
        double saveCoeficientDeplasare = coeficientDeplasare;
        redo.push(undo.top());
        if(undo.top().local) 
            EditareRotatie(obj, true);
        while(nrDeplasari--) {
            coeficientDeplasare = -obj.stackDeplasare.top().second;
            obj.stackRedoDeplasare.push(obj.stackDeplasare.top());
            if(obj.stackDeplasare.top().first == 'x')
                DeplaseazaX(obj);
            else if(obj.stackDeplasare.top().first == 'y')
                DeplaseazaY(obj);
            else
                DeplaseazaZ(obj);
            obj.stackDeplasare.pop();
        }
        if(undo.top().local) 
            EditareRotatie(obj, false);
        coeficientDeplasare = saveCoeficientDeplasare;
        obj.centru = GetMijlocObiect(obj);
        obj.centruPentruAxe = obj.centru;

        /* if(varfSelectat != -1) {
                for(int j = 0; j < 3; j++) {
                    obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
                //obj.axePunctSelectat[j].z = (zoom * obj.axePunctSelectat[j].x * POV) / (obj.axePunctSelectat[j].x - lungimeFereastra / 2)
                }
            

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].x += 0.25;
            obj.axePunctSelectat[2].y -= 0.25;
            } */
    }
    else if(alegere == 3) {
        int nrMariri = undo.top().numarRotatii;
        double saveCoeficientMarire = coeficientMarire;
        redo.push(undo.top());
        EditareRotatie(obj, true);

    CentreazaObiect(obj);
        while(nrMariri--) {
            coeficientMarire = -obj.stackMarire.top().second;
            obj.stackRedoMarire.push(obj.stackMarire.top());
            if(obj.stackMarire.top().first == 'x')
                MaresteX(obj);
            else if(obj.stackMarire.top().first == 'y')
                MaresteY(obj);
            else
                MaresteZ(obj);
            obj.stackMarire.pop();
        }
        coeficientMarire = saveCoeficientMarire;
        
        EditareRotatie(obj, false);
    /* if(varfSelectat != -1) {
        for(int j = 0; j < 3; j++) {
                obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
            }

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].x += 0.25;
            obj.axePunctSelectat[2].y -= 0.25;
    } */

    obj.centru = GetMijlocObiect(obj);
    obj.centruPentruAxe = obj.centru;
    }
    else if(alegere == 4) {
        char axa = undo.top().axa;
        Undo temp;
        temp.alegere = 4;
        temp.axa = axa;
        cout << undo.top().coordonate.size() << endl;
        for(int i = 0; i < (int) undo.top().coordonate.size() - 9; i++) {
            temp.coordonate.push_back({undo.top().coordonate[i].first, obj.varfuri[undo.top().coordonate[i].first].x});
            obj.varfuri[undo.top().coordonate[i].first].x = undo.top().coordonate[i].second;
            i++;
            temp.coordonate.push_back({undo.top().coordonate[i].first, obj.varfuri[undo.top().coordonate[i].first].y});
            obj.varfuri[undo.top().coordonate[i].first].y = undo.top().coordonate[i].second;
            i++;
            temp.coordonate.push_back({undo.top().coordonate[i].first, obj.varfuri[undo.top().coordonate[i].first].z});
            obj.varfuri[undo.top().coordonate[i].first].z = undo.top().coordonate[i].second;
        }

        for(int i = 0; i < 3; i++) {
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].x});
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].y});
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].z});
        }

        for(int i = 0; i < 9; i += 3) {
            obj.axePunctSelectat[i / 3] = {undo.top().coordonate[undo.top().coordonate.size() - 9 + i].second, 
            undo.top().coordonate[undo.top().coordonate.size() - 8 + i].second, undo.top().coordonate[undo.top().coordonate.size() - 7 + i].second};
        }
        /* for(const pair<int, double> &i : undo.top().coordonate)
            if(axa == 'x') {
                temp.coordonate.push_back({i.first, obj.varfuri[i.first].x});
                obj.varfuri[i.first].x = i.second;
            }
            else if(axa == 'y') {
                temp.coordonate.push_back({i.first, obj.varfuri[i.first].y});
                obj.varfuri[i.first].y = i.second;
            }
            else {
                temp.coordonate.push_back({i.first, obj.varfuri[i.first].z});
                obj.varfuri[i.first].z = i.second;
            } */
        redo.push(temp);
        
        /* if(varfSelectat != -1) {
                for(int j = 0; j < 3; j++) {
                    obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
                //obj.axePunctSelectat[j].z = (zoom * obj.axePunctSelectat[j].x * POV) / (obj.axePunctSelectat[j].x - lungimeFereastra / 2)
                }
            

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].z += 0.5;
            } */

            obj.centru = GetMijlocObiect(obj);
            EditareRotatie(obj, true);
            obj.axaOX = obj.centru;
            obj.axaOY = obj.centru;
            obj.axaOZ = obj.centru;
            obj.axaOX.x += 0.5;
            obj.axaOY.y -= 0.5;
            obj.axaOZ.z += 0.5;
            EditareRotatie(obj, false);
    }
    else if(alegere == 5) {
        Undo temp;
        temp.alegere = 5;
        temp.puncteEliminat = obj.varfuri[obj.varfuri.size() - 1];
        redo.push(temp);
        obj.varfuri.pop_back();
        if(obj.varfuri.size() > 0) {
        obj.centru = GetMijlocObiect(obj);
            EditareRotatie(obj, true);
            obj.axaOX = obj.centru;
            obj.axaOY = obj.centru;
            obj.axaOZ = obj.centru;
            obj.axaOX.x += 0.5;
            obj.axaOY.y -= 0.5;
            obj.axaOZ.z += 0.5;
            EditareRotatie(obj, false);
        }
    }
    else if(alegere == 6) {
        int nr = undo.top().numarRotatii;
        Undo temp;
        temp.alegere = 6;
        temp.numarRotatii = nr;
        while(nr--) {
            temp.coordonate.push_back({obj.muchii[obj.muchii.size() - 1].first, obj.muchii[obj.muchii.size() - 1].second});
            obj.muchii.pop_back();
            obj.numarMuchii--;
        }
        redo.push(temp);
    }
    else if(alegere == 7) {
        int index = undo.top().numarRotatii;
        int k = 0;
        Undo temp;
        temp.alegere = 7;
        temp.numarRotatii = index;
        redo.push(temp);
        obj.varfuri.insert(obj.varfuri.begin() + index, undo.top().puncteEliminat);
        obj.numarVarfuri++;
        for(auto &i : obj.fete)
            for(int &j : i)
                if(j >= index)
                    j++;
        for(pair<int, int> &i : obj.muchii) {
            if(i.first >= index)
                i.first++;
            if(i.second >= index)
                i.second++;
        }
        for(const auto &i : undo.top().fete) {
            obj.fete.insert(obj.fete.begin() + undo.top().index[k], i);
            obj.culoareFete.insert(obj.culoareFete.begin() + undo.top().index[k], undo.top().culoareFete[k]);
            obj.numarFete++;
            k++;
        }
        for(const auto &i : undo.top().coordonate) {
            obj.muchii.insert(obj.muchii.begin() + undo.top().index[k++], {i.first, i.second});
            obj.numarMuchii++;
        }

        for(const auto &i : undo.top().indexFete) {
            for(auto &j : obj.indexFata)
                if(j >= i.second)
                    j++;
            obj.indexFata.insert(obj.indexFata.begin() + i.first, i.second);
        }
    }
    else if(alegere == 8) {
        for(int i = 0; i < obj.fete.size(); i++)
            if(obj.fete[i] == undo.top().fete[0]) {
                Undo temp;
                temp.alegere = 8;
                temp.fete.push_back(obj.fete[i]);
                temp.culoareFete.push_back(obj.culoareFete[i]);
                redo.push(temp);
                obj.fete.erase(obj.fete.begin() + i);
                obj.culoareFete.erase(obj.culoareFete.begin() + i);
                break;
            }
        fataSelectata = -1;
        for(int i = 0; i < obj.indexFata.size(); i++)
            if((int) obj.indexFata.size() - 1 == obj.indexFata[i]) {
                obj.indexFata.erase(obj.indexFata.begin() + i);
                break;
            }
        obj.numarFete--;
    }
    else if(alegere == 9) {
        for(int i = 0; i < obj.fete.size(); i++)
            if(obj.fete[i] == undo.top().fete[0]) {
                Undo temp;
                temp.alegere = 9;
                temp.fete.push_back(obj.fete[i]);
                temp.index = {obj.culoareFete[i][0], obj.culoareFete[i][1], obj.culoareFete[i][2]};
                redo.push(temp);
                obj.culoareFete[i] = {undo.top().index[0], undo.top().index[1], undo.top().index[2]};
                break;
            }
    }

    undo.pop();

    local = saveLocal;

    Deseneaza(obj);

    delay(200);
}

void RedoChange(Obiect &obj, bool skip) {
    if(((!GetAsyncKeyState(VK_CONTROL) || !GetAsyncKeyState('Y')) && !skip) || redo.empty())
        return;
    
    int alegere = redo.top().alegere, saveLocal = local;
    local = false;

    if(alegere == 1) {
        int nrRotatii = redo.top().numarRotatii;
        double saveUnghiRotatie = unghiRotatie;
        CentreazaObiect(obj);
        undo.push(redo.top());
        while(nrRotatii--) {
            char axa = obj.stackRedoRotatie.top().first;
            unghiRotatie = obj.stackRedoRotatie.top().second;
            if(redo.top().axa == 'g')
                undoRotatieGrid = true;
            obj.rotatiiAnterioare.push(obj.stackRedoRotatie.top());
            if(axa == 'x')
                RotesteObiectX(obj);
            else if(axa == 'y')
                RotesteObiectY(obj);
            else
                RotesteObiectZ(obj);
            
            obj.stackRedoRotatie.pop();
            undoRotatieGrid = false;
        }
        unghiRotatie = saveUnghiRotatie;
        RefacePozitia(obj);

        /* if(varfSelectat != -1) {
                for(int j = 0; j < 3; j++) {
                    obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
                //obj.axePunctSelectat[j].z = (zoom * obj.axePunctSelectat[j].x * POV) / (obj.axePunctSelectat[j].x - lungimeFereastra / 2)
                }
            

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].x += 0.25;
            obj.axePunctSelectat[2].y -= 0.25;
            } */
    }
    else if(alegere == 2) {
        int nrDeplasari = redo.top().numarRotatii;
        double saveCoeficientDeplasare = coeficientDeplasare;
        undo.push(redo.top());
        if(redo.top().local) 
            EditareRotatie(obj, true);
        while(nrDeplasari--) {
            coeficientDeplasare = obj.stackRedoDeplasare.top().second;
            obj.stackDeplasare.push(obj.stackRedoDeplasare.top());
            if(obj.stackDeplasare.top().first == 'x')
                DeplaseazaX(obj);
            else if(obj.stackDeplasare.top().first == 'y')
                DeplaseazaY(obj);
            else
                DeplaseazaZ(obj);
            obj.stackRedoDeplasare.pop();
        }
        if(redo.top().local) 
            EditareRotatie(obj, false);
        coeficientDeplasare = saveCoeficientDeplasare;
        obj.centru = GetMijlocObiect(obj);
        obj.centruPentruAxe = obj.centru;

        /* if(varfSelectat != -1) {
                for(int j = 0; j < 3; j++) {
                    obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
                //obj.axePunctSelectat[j].z = (zoom * obj.axePunctSelectat[j].x * POV) / (obj.axePunctSelectat[j].x - lungimeFereastra / 2)
                }
            

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].x += 0.25;
            obj.axePunctSelectat[2].y -= 0.25;
            } */
    }
    else if(alegere == 3) {
        int nrMariri = redo.top().numarRotatii;
        double saveCoeficientMarire = coeficientMarire;
        undo.push(redo.top());
        EditareRotatie(obj, true);

    CentreazaObiect(obj);
        while(nrMariri--) {
            coeficientMarire = obj.stackRedoMarire.top().second;
            obj.stackMarire.push(obj.stackRedoMarire.top());
            if(obj.stackMarire.top().first == 'x')
                MaresteX(obj);
            else if(obj.stackMarire.top().first == 'y')
                MaresteY(obj);
            else
                MaresteZ(obj);
            obj.stackRedoMarire.pop();
        }
        coeficientMarire = saveCoeficientMarire;
        
        EditareRotatie(obj, false);
    /* if(varfSelectat != -1) {
        for(int j = 0; j < 3; j++) {
                obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
            }

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].x += 0.25;
            obj.axePunctSelectat[2].y -= 0.25;
    } */

    obj.centru = GetMijlocObiect(obj);
    obj.centruPentruAxe = obj.centru;
    }
    else if(alegere == 4) {
        char axa = redo.top().axa;
        Undo temp;
        temp.alegere = 4;
        temp.axa = axa;
        //temp.coordonate = redo.top().coordonate;
        for(int i = 0; i < redo.top().coordonate.size() - 9; i++) {
            temp.coordonate.push_back({redo.top().coordonate[i].first, obj.varfuri[redo.top().coordonate[i].first].x});
            obj.varfuri[redo.top().coordonate[i].first].x = redo.top().coordonate[i].second;
            i++;
            temp.coordonate.push_back({redo.top().coordonate[i].first, obj.varfuri[redo.top().coordonate[i].first].y});
            obj.varfuri[redo.top().coordonate[i].first].y = redo.top().coordonate[i].second;
            i++;
            temp.coordonate.push_back({redo.top().coordonate[i].first, obj.varfuri[redo.top().coordonate[i].first].z});
            obj.varfuri[redo.top().coordonate[i].first].z = redo.top().coordonate[i].second;
        }
        /* for(const pair<int, double> &i : redo.top().coordonate)
            if(axa == 'x') {
                temp.coordonate.push_back({i.first, obj.varfuri[i.first].x});
                obj.varfuri[i.first].x = i.second;
            }
            else if(axa == 'y') {
                temp.coordonate.push_back({i.first, obj.varfuri[i.first].y});
                obj.varfuri[i.first].y = i.second;
            }
            else {
                temp.coordonate.push_back({i.first, obj.varfuri[i.first].z});
                obj.varfuri[i.first].z = i.second;
            } */
        for(int i = 0; i < 3; i++) {
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].x});
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].y});
            temp.coordonate.push_back({-1, obj.axePunctSelectat[i].z});
        }
        for(int i = 0; i < 9; i += 3) {
            obj.axePunctSelectat[i / 3] = {redo.top().coordonate[redo.top().coordonate.size() - 9 + i].second, 
            redo.top().coordonate[redo.top().coordonate.size() - 8 + i].second, redo.top().coordonate[redo.top().coordonate.size() - 7 + i].second};
        }
        undo.push(temp);
        
        /* if(varfSelectat != -1) {
                for(int j = 0; j < 3; j++) {
                    obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
                //obj.axePunctSelectat[j].z = (zoom * obj.axePunctSelectat[j].x * POV) / (obj.axePunctSelectat[j].x - lungimeFereastra / 2)
                }
            

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].x += 0.25;
            obj.axePunctSelectat[2].y -= 0.25;
            } */

            obj.centru = GetMijlocObiect(obj);
                EditareRotatie(obj, true);
            obj.axaOX = obj.centru;
            obj.axaOY = obj.centru;
            obj.axaOZ = obj.centru;
            obj.axaOX.x += 0.5;
            obj.axaOY.y -= 0.5;
            obj.axaOZ.z += 0.5;
            EditareRotatie(obj, false);
    }
    else if(alegere == 5) {
        Undo temp;
        temp.alegere = 5;
        undo.push(temp);
        obj.varfuri.push_back(redo.top().puncteEliminat);
        obj.centru = GetMijlocObiect(obj);
            EditareRotatie(obj, true);
            obj.axaOX = obj.centru;
            obj.axaOY = obj.centru;
            obj.axaOZ = obj.centru;
            obj.axaOX.x += 0.5;
            obj.axaOY.y -= 0.5;
            obj.axaOZ.z += 0.5;
            EditareRotatie(obj, false);
    }
    else if(alegere == 6) {
        int nr = redo.top().numarRotatii;
        Undo temp;
        temp.alegere = 6;
        temp.numarRotatii = nr;
        int k = 0;
        while(nr--) {
            obj.muchii.push_back({redo.top().coordonate[k].first, redo.top().coordonate[k++].second});
            obj.numarMuchii++;
        }
        undo.push(temp);
    }
    else if(alegere == 7) {
        int index = redo.top().numarRotatii;
        modificaPagina = false;
        int saveVarf = varfSelectat;
        varfSelectat = index;
        EliminaPunct(obj);
        varfSelectat = saveVarf;
        modificaPagina = true;
    }
    else if(alegere == 8) {
        Undo temp;
        temp.alegere = 8;
        temp.fete.push_back(redo.top().fete[0]);
        undo.push(temp);
        obj.fete.push_back(redo.top().fete[0]);
        obj.culoareFete.push_back(redo.top().culoareFete[0]);
        obj.indexFata.push_back((int) obj.indexFata.size());
    }
    else if(alegere == 9) {
        for(int i = 0; i < obj.fete.size(); i++)
            if(obj.fete[i] == redo.top().fete[0]) {
                Undo temp;
                temp.alegere = 9;
                temp.fete.push_back(obj.fete[i]);
                temp.index = {obj.culoareFete[i][0], obj.culoareFete[i][1], obj.culoareFete[i][2]};
                undo.push(temp);
                obj.culoareFete[i] = {redo.top().index[0], redo.top().index[1], redo.top().index[2]};
                break;
            }
    }

    redo.pop();

    local = saveLocal;

    Deseneaza(obj);
    delay(200);

}

void VerificaScrool(Obiect &obj) {
    if((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('A') & 0x8000) && fereastraStanga == 1) {
        puncteSelectate.clear();
        for(int i = 0; i < obj.varfuri.size(); i++)
            puncteSelectate.push_back(i);
        varfSelectat = (int) obj.varfuri.size() - 1;
        selectarePuncte = true;
        EditareRotatie(obj, true);
            for(int j = 0; j < 3; j++) {
                obj.axePunctSelectat[j] = obj.varfuri[varfSelectat];
            }

            obj.axePunctSelectat[0].x += 0.5;
            obj.axePunctSelectat[1].y -= 0.5;
            obj.axePunctSelectat[2].z += 0.5;

            EditareRotatie(obj, false);
        Deseneaza(obj);
        return;
    }

    if((GetAsyncKeyState(VK_UP) & 0x8000) == (GetAsyncKeyState(VK_DOWN) & 0x8000) || !fereastraStanga)
        return;
    int x = mousex(), y = mousey();
    if(y < inaltimeFereastra / 35 || x > 140)
        return;
    
    if((GetAsyncKeyState(VK_UP) & 0x8000) && indexStart - 1 >= 0)
        indexStart--;
    else if((GetAsyncKeyState(VK_DOWN) & 0x8000)) {
            if(fereastraStanga == 1 && indexStart + 1 <= (int) obj.varfuri.size() - 24)
                indexStart++;
            else if(fereastraStanga == 2 && indexStart + 1 <= (int) obj.fete.size() - 24)
                    indexStart++;
            else if(fereastraStanga == 3 && indexStart + 1 <= (int) obiecte.size() - 24)
                    indexStart++;
    }
    
    Deseneaza(obj);

    delay(2);
}

