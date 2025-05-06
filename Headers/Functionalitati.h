#pragma once
#include "../Source/Functionalitati.cpp"
#include "Structuri.h"

using namespace std;

Punct2D Transforma2D(const Punct3D &punct);
Punct3D GetMijlocObiect(const Obiect &obj);
bool VerificareRotatie(Obiect &obj);
void CentreazaObiect(Obiect &obj);
void RefacePozitia(Obiect &obj);
void EditareRotatie(Obiect &obj, bool resetareRotatie);
bool VerificareMarire(Obiect &obj);
bool VerificareDeplasare(Obiect &obj);
bool VerificaApasareVarf(Obiect &obj);
bool VerificaMousePeAxa(Obiect &obj);
bool VerificaMiscareMousePeAxa(Obiect &obj);
void AdaugaPunct(Obiect &obj);
void CrearePoligon(Obiect &obj);
void VerificaHoverButoane(const Obiect &obj);
void EditareInputBox(Obiect &obj);
void VerificaApasareButon( Obiect &obj);
Obiect IncarcaObiect(const string adresa);
void CentreazaObiectul(Obiect &obj);
void RefacePozitia(Obiect &obj);

void SelecteazaFata(Obiect &obj, int fataIndex);
void UndoChange(Obiect &obj, bool skip);
void RedoChange(Obiect &obj, bool skip);
void VerificaScrool(Obiect &obj);