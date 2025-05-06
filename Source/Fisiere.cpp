#include <fstream>
#include "../Headers/Structuri.h"
#include <winbgim.h>

int CitesteObiect(const std::string &adresaFisier) {
    std::ifstream f(adresaFisier);
    int nrObiecte;

    f >> nrObiecte;

    for(int nr = 0; nr < nrObiecte; nr++) {
        Obiect obj;

        f >> obj.numarVarfuri >> obj.numarMuchii >> obj.numarFete;

        for(int i = 0; i < obj.numarVarfuri; i++) {
            double x, y, z;
            f >> x >> y >> z;

            obj.varfuri.push_back({x, y, z});
        }

        for(int i = 0; i < obj.numarMuchii; i++) {
            int a, b;
            f >> a >> b;

            obj.muchii.push_back({a, b});
        }

        for(int i = 0; i < obj.numarFete; i++) {
            int dim, x;
            f >> dim;
            obj.fete.push_back({});
            for(int j = 0; j < dim; j++) {
                f >> x;
                obj.fete[i].push_back(x);
            }

            obj.indexFata.push_back(i);
        }

        f >> obj.rotatieX >> obj.rotatieY >> obj.rotatieZ;
        f >> obj.marimeX >> obj.marimeY >> obj.marimeZ;
        f >> obj.deplasareX >> obj.deplasareY >> obj.deplasareZ;

        char axa;
        f >> axa;

        while(axa != '#') {
            double unghi;

            f >> unghi >> axa;

            obj.rotatiiAnterioare.push({axa, unghi});
        }

        if(obj.rotatiiAnterioare.size() > 0)
            obj.rotatiiAnterioare.pop();

        obj.activ = true;

        obiecte.push_back(obj);
    }

    obiectSelectat = (int) obiecte.size() - 1;
    obiecteSelectate.push_back(obiectSelectat);

    f.close();

    return nrObiecte;
}