/* 
 * File:   main.cpp
 * Author: JoseDavid
 *
 * Created on 11 de noviembre de 2014, 17:14
 */

#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <fstream>
#include <bitset>         // std::bitset
#include <time.h>
#include <vector>
#include <stdlib.h>
using namespace std;

/**
 * Rellena la matriz de flujos y distancias a partir de un fichero dado
 * @param flujos Matriz de flujos a llenar
 * @param distancias Matriz de distancias a llenar
 * @param fichero Fichero del que leer
 * @return nCasos numero de filas y columnas que tiene las matrices
 */
int lectura(int **&flujos, int **&distancias, string fichero) {

    string temp;
    ifstream flujo(fichero.c_str());
    int nCasos = 0;
    if (flujo.is_open()) {
        if (flujo.good()) {
            getline(flujo, temp);
            nCasos = atoi(temp.c_str());
            flujos = new int*[nCasos];
            for (int i = 0; i < nCasos; i++) {
                flujos[i] = new int[nCasos];
            }

            distancias = new int*[nCasos];
            for (int i = 0; i < nCasos; i++) {
                distancias[i] = new int[nCasos];
            }

            int tempi = 0;
            int tempj = 0;
            int numero;
            bool fin = false;
            while (getline(flujo, temp) && !fin) {
                if (temp.length() > 0) {
                    stringstream lectura(temp);
                    while (lectura >> numero) {
                        flujos[tempi][tempj] = numero;
                        tempj++;
                        if (tempj == nCasos) {
                            tempj = 0;
                            tempi++;
                        }
                    }
                }
                if (tempi >= nCasos) {
                    fin = true;
                }
            }
            tempi = 0;
            tempj = 0;
            fin = false;
            while (getline(flujo, temp) && !fin) {
                if (temp.length() > 0) {
                    stringstream lectura(temp);
                    while (lectura >> numero) {
                        distancias[tempi][tempj] = numero;
                        tempj++;
                        if (tempj == nCasos) {
                            tempj = 0;
                            tempi++;
                        }
                    }
                }
                if (tempi >= nCasos) {
                    fin = true;
                }
            }
            return nCasos;
        } else {
            cout << "Error opening file";
            return -1;
        }
    }
}

/**
 * Calcula el vector de potenciales
 * @param v Matriz a partir de la cual calcular los potenciales
 * @param tam Numero de casos a evaluar
 * @return Vector de potenciales
 */
int *potencial(int **v, int &tam) {
    int *pot = new int[tam];

    for (int i = 0; i < tam; i++) {
        pot[i] = 0;
        for (int j = 0; j < tam; j++) {
            pot[i] += v[j][i];
        }
    }
    return pot;
}

/**
 * Dado un vector de enteros, devuelve la posicion del mayor de sus elementos
 * @param v Vector en el que buscar
 * @param tam Tamaño del vector
 * @return Posicion del mayor dato
 */
int mayor(int *v, int tam) {
    if (tam <= 0) {
        return -1;
    } else if (tam == 1) {
        return v[0];
    } else if (tam > 1) {
        int m = 0;
        for (int i = 1; i < tam; i++) {
            if (v[i] > v[m]) {
                m = i;
            }
        }
        return m;
    }
}

/**
 * Dado un vector de enteros, devuelve la posicion del menor de sus elementos
 * @param v Vector en el que buscar
 * @param tam Tamaño del vector
 * @return Posicion del menor dato
 */
int menor(int *v, int tam) {
    if (tam <= 0) {
        return -1;
    } else if (tam == 1) {
        return v[0];
    } else if (tam > 1) {
        int m = 0;
        for (int i = 1; i < tam; i++) {
            if (v[i] < v[m]) {
                m = i;
            }
        }
        return m;
    }
}

/**
 * Dado un vector de solucion, calcula el coste de esa solucion
 * 
 * @param v Vector solucion
 * @param tam Tamaño del vector
 * @param distancias Matriz de distancias de donde obtener los valores para el coste
 * @param flujos Matriz de flujos de donde obtener los valores para el coste
 * @return coste total de la solucion
 */
int coste(int *v, int tam, int **distancias, int **flujos) {
    int costo = 0;
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            costo += (flujos[i][j])*(distancias[v[i]][v[j]]);
        }
    }
    return costo;
}

/**
 * Desarrollo del algoritmo voraz para encontrar una solucion
 * @param flujos Matriz de flujos
 * @param distancias Matriz de distancias
 * @param solGreedy Vector a rellenar con una solucion
 * @param nCasos Tamaño del problema
 * @return Coste de la solucion
 */
int greedy(int **flujos, int **distancias, int *&solGreedy, int nCasos) {
    int *potFlujos = potencial(flujos, nCasos);
    int *potDistancias = potencial(distancias, nCasos);
    solGreedy = new int[nCasos];
    int cont = nCasos;
    int cost;
    while (cont > 0) {
        int may = mayor(potFlujos, nCasos);
        int men = menor(potDistancias, nCasos);
        if (may != -1 && men != -1) {
            potFlujos[may] = -1;
            potDistancias[men] = 999999;
            solGreedy[may] = men;
            cont--;
        } else {
            break;
        }
    }

    int costo;
    if (cont == 0) {
        costo = coste(solGreedy, nCasos, flujos, distancias);
        return costo;
    } else {
        return -1;
        cost = coste(solGreedy, nCasos, distancias, flujos);
        return cost;

    }
}

/**
 * Calcula una solucion aleatoria
 * @param tam Tamaño del vector solucion
 * @return Vector solucion, generado aleatoriamente
 */
int* solInicial(int tam, int seed) {
    srand(seed);
    int aleatorio;
    int *solucionInicial = new int[tam];
    bool* usado = new bool[tam];
    for (int i = 0; i < tam; i++) {
        usado[i] = false;
    }

    for (int i = 0; i < tam; i++) {
        do {
            aleatorio = rand() % tam;
        } while (usado[aleatorio] == true);

        usado[aleatorio] = true;
        solucionInicial[i] = aleatorio;

    }
    return solucionInicial;
}

/**
 * Devuelve la diferencia de coste existente entre una solucion, 
 *  y la misma con dos posiciones intercambiadas
 * 
 * @param v  vector solucion inicial
 * @param tam Tamaño del vector
 * @param flujos Matriz de flujos
 * @param distancias Matriz de distancias
 * @param r posicion 1 a cambiar
 * @param s posicion 2 a cambiar
 * @return Variacion con respecto al coste original: Positivo si empeora, Negativo si mejora
 */
int factorizacion(int* v, int tam, int **flujos, int** distancias, int r, int s) {

    int fact = 0;
    for (int k = 0; k < tam; k++) {
        if (k != r && k != s) {
            fact += flujos[r][k]*(distancias[v[s]][v[k]] - distancias[v[r]][v[k]]) +
                    flujos[s][k]*(distancias[v[r]][v[k]] - distancias[v[s]][v[k]]) +
                    flujos[k][r]*(distancias[v[k]][v[s]] - distancias[v[k]][v[r]]) +
                    flujos[k][s]*(distancias[v[k]][v[r]] - distancias[v[k]][v[s]]);
        }
    }

    return fact;
}

/**
 * Algoritmo para la busqueda local de la solucion
 * 
 * @param nCasos Tamaño del problema
 * @param flujos Matriz de flujos
 * @param distancias Matriz de distancias
 * @return Vector solucion
 */
int *busquedaLocal(int nCasos, int **flujos, int **distancias, int seed) {
    int* solucionActual = solInicial(nCasos, seed);
    int costo = coste(solucionActual, nCasos, distancias, flujos);
    bitset<100> dlb(0);
    bool mejora = false;
    for (int i = nCasos; i < 100; i++) {
        dlb.flip(i);
    }
    while (!dlb.all()) {
        for (int i = 0; i < nCasos; i++) {
            if (!dlb.test(i)) {
                mejora = false;
                for (int j = 0; j < nCasos; j++) {
                    if (!dlb.test(j)) {
                        int variacion = factorizacion(solucionActual, nCasos, flujos, distancias, i, j);
                        if (variacion < 0) {
                            costo += variacion;
                            int tmp = solucionActual[j];
                            solucionActual[j] = solucionActual[i];
                            solucionActual[i] = tmp;
                            dlb.reset(i);
                            dlb.reset(j);
                            mejora = true;
                        }
                    }
                }
                if (!mejora) {
                    dlb.set(i);
                }
            }
        }
    }
    return solucionActual;
}

int main(int argc, char** argv) {

    int **flujos, **distancias;
    string fichero = "dat/els19.dat";
    string ficheros[20] = {"dat/els19.dat", "dat/chr20a.dat", "dat/chr25a.dat", "dat/nug25.dat",
        "dat/bur26a.dat", "dat/bur26b.dat", "dat/tai30a.dat", "dat/tai30b.dat",
        "dat/esc32a.dat", "dat/kra32.dat", "dat/tai35a.dat", "dat/tai35b.dat",
        "dat/tho40.dat", "dat/tai40a.dat", "dat/sko42.dat", "dat/sko49.dat",
        "dat/tai50a.dat", "dat/tai50b.dat", "dat/tai60a.dat", "dat/lipa90a.dat"};
    //    cout << "Usando semilla: " << seed << endl;

    int seed;
    //    cout << "Introduzca semilla" << endl;
    //    cin >> seed;
    seed = 1413978675;
    cout << "Usando semilla " << seed << endl;
    int costo;
    for (int i = 0; i < 20; i++) {
        fichero = ficheros[i];

        //        cout << "Leyendo fichero... " << fichero << endl;
        int nCasos = lectura(flujos, distancias, fichero);

        //         GREEDY
        int *solGreedy;
        int costo = greedy(flujos, distancias, solGreedy, nCasos);
        cout << "Coste del algoritmo GREEDY para el fichero( " << i + 1 << " ) " << fichero << " es:" << costo << endl;

        // LOCAL
        int *solLocal = busquedaLocal(nCasos, flujos, distancias, seed);
        costo = coste(solLocal, nCasos, distancias, flujos);
        cout << "Coste del algoritmo LOCAL para el fichero( " << i + 1 << " ) " << fichero << " es:" << costo << endl;
    }
    return 0;
}

