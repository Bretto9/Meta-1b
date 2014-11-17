
#include <time.h>
#include <windows.h>

#include "util.h"
using namespace std;

int main(int argc, char** argv) {
    LARGE_INTEGER frequency; // ticks per second
    LARGE_INTEGER t1, t2; // ticks
    double elapsedTime;

    // get ticks per second
    QueryPerformanceFrequency(&frequency);


    int **flujos = 0;
    int **distancias = 0;
    int costo;
    int nCasos = 0;
    int *solGreedy;
    string fichero = "dat/els19.dat";
    string ficheros[20] = {"dat/els19.dat", "dat/chr20a.dat", "dat/chr25a.dat", "dat/nug25.dat",
        "dat/bur26a.dat", "dat/bur26b.dat", "dat/tai30a.dat", "dat/tai30b.dat",
        "dat/esc32a.dat", "dat/kra32.dat", "dat/tai35a.dat", "dat/tai35b.dat",
        "dat/tho40.dat", "dat/tai40a.dat", "dat/sko42.dat", "dat/sko49.dat",
        "dat/tai50a.dat", "dat/tai50b.dat", "dat/tai60a.dat", "dat/lipa90a.dat"};
    cout << "NombreFichero: CosteGreedy CosteGrasp CosteILS" << endl;
    int seed = 1413978675;
    int cont = 0;
    cout << "Usando semilla: " << seed << endl;
    for (int i = 0; i < 20; i++) {
        fichero = ficheros[i];
        QueryPerformanceCounter(&t1);
        nCasos = lectura(flujos, distancias, fichero);
        QueryPerformanceCounter(&t2);
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
        cout << fichero << " " << elapsedTime << " ";
        
        QueryPerformanceCounter(&t1);
        costo = greedy(flujos, distancias, solGreedy, nCasos);
        QueryPerformanceCounter(&t2);
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
        cout << costo << " " << elapsedTime << " ";

        QueryPerformanceCounter(&t1);
        int *solGrasp = GRASP(nCasos, flujos, distancias, seed);
        costo = coste(solGrasp, nCasos, distancias, flujos);
        QueryPerformanceCounter(&t2);
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
        cout << costo << " " << elapsedTime << " ";

        QueryPerformanceCounter(&t1);
        int *solILS = ils(nCasos, flujos, distancias, seed);
        costo = coste(solILS, nCasos, distancias, flujos);
        QueryPerformanceCounter(&t2);
        elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
        cout << costo << " " << elapsedTime << " " << endl;

        delete[] solGreedy;
        delete[] solGrasp;
        delete[] solILS;
    }
    cont++;
    //cout << endl << endl;
    //}
    cout << endl;
    return 0;
}

