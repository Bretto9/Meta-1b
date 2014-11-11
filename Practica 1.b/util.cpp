#include "util.h"

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