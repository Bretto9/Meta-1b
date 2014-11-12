

#include "util.h"
using namespace std;

int main(int argc, char** argv) {
    int **flujos, **distancias;
    int costo;

    string fichero = "dat/tai50a.dat";
    string ficheros[20] = {"dat/els19.dat", "dat/chr20a.dat", "dat/chr25a.dat", "dat/nug25.dat",
        "dat/bur26a.dat", "dat/bur26b.dat", "dat/tai30a.dat", "dat/tai30b.dat",
        "dat/esc32a.dat", "dat/kra32.dat", "dat/tai35a.dat", "dat/tai35b.dat",
        "dat/tho40.dat", "dat/tai40a.dat", "dat/sko42.dat", "dat/sko49.dat",
        "dat/tai50a.dat", "dat/tai50b.dat", "dat/tai60a.dat", "dat/lipa90a.dat"};
    //    cout << "Usando semilla: " << seed << endl;

    int seed = 123456;
//    cout << "Introduzca semilla" << endl;
//    cin >> seed;
//    cout << "Usando semilla " << seed << endl;
//    for (int i = 0; i < 20; i++) {
//        fichero = ficheros[i];
//
//        cout << "Leyendo fichero... " << fichero << endl;
//        int nCasos = lectura(flujos, distancias, fichero);
//        cout << nCasos << endl;
//
//        //        // LOCAL
//        int *solLocal = busquedaLocal(nCasos, flujos, distancias, seed);
//        cout << solLocal << endl;
//        costo = coste(solLocal, nCasos, distancias, flujos);
//        cout << "Coste del algoritmo LOCAL para el fichero( " << i + 1 << " ) " << fichero << " es:" << costo << endl;
//
//        for (int j = 0; j < nCasos; j++) {
//            delete[] flujos[j];
//            delete[] distancias[j];
//        }
//        delete[] flujos;
//        delete[] distancias;
//        delete[] solLocal;
//    }
    
    
    int nCasos = lectura(flujos, distancias, fichero);
    int *solGrasp = GRASP(nCasos, flujos, distancias, seed);
    costo = coste(solGrasp, nCasos, distancias, flujos);
    cout << "Coste del algoritmo GRASP para el fichero( " << 1 << " ) " << fichero << " es:" << costo << endl;
    
    cout << endl;
    return 0;
}

