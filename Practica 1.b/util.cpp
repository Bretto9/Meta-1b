#include "util.h"
#define l 10 // Numero de mejores candidatos a generar
#define alfa 0.3
#define repeticionesGRASP 25
#define numIteraciones 25

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
int *potencialGRASP(int **v, int &tam) {
    int *pot = new int[tam];

    for (int i = 0; i < tam; i++) {
        pot[i] = 0;
        for (int j = 0; j < tam; j++) {
            pot[i] += (v[j][i] + v[i][j]);
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
 * Dado un vector de solucion, calcula el coste de esa solucion
 * 
 * @param v Vector solucion
 * @param tam Tamaño del vector
 * @param distancias Matriz de distancias de donde obtener los valores para el coste
 * @param flujos Matriz de flujos de donde obtener los valores para el coste
 * @return coste total de la solucion
 */
int costeGRASP(int *v, int tam, int **distancias, int **flujos, int i, int k) {
    int costo = 0;
    for (int j = 0; j < tam; j++) {
        if (v[j] != -1) {
            costo += flujos[i][j] * distancias[k][v[j]];
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
 *  * Algoritmo para la busqueda local de una solucion
 * 
 * @param nCasos Tamaño del problema
 * @param flujos Matriz de flujos
 * @param distancias Matriz de distancias
 * @param seed Semilla a utilizar
 * @param solucionInicial Solucion a partir de la cual mejorar (no tiene la solucion inicial aleatoria de la anterior implementacion)
 * @return Vector solucion
 */
int *busquedaLocal(int nCasos, int **flujos, int **distancias, int seed, int *solucionInicial) {
    int* solucionActual = solucionInicial;
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

struct comparadorCandidatos {

    bool operator()(const pair<int, pair<int, int> >& cand1, const pair<int, pair<int, int> >& cand2) const {
        return cand1.first < cand2.first;
    }
} comparador;

int *randomGreedy(int nCasos, int **flujos, int **distancias, int seed) {
    // Definicion de datos previa
    int *potFlujos = potencialGRASP(flujos, nCasos);
    int *potDistancias = potencialGRASP(distancias, nCasos);
    int *solInicial = new int[nCasos];
    int nAsignaciones = 0;
    srand(seed);
    for (int i = 0; i < nCasos; i++) {
        solInicial[i] = -1;
    }
    // ETAPA 1
    // Definicion de los umbrales
    int mayorFlujo = mayor(potFlujos, nCasos);
    float umbralFlujo = potFlujos[mayorFlujo] - alfa * (potFlujos[mayorFlujo] - potFlujos[menor(potFlujos, nCasos)]);

    int menorDistancia = menor(potDistancias, nCasos);
    float umbralDistancia = potDistancias[menorDistancia] - alfa * (potDistancias[mayor(potDistancias, nCasos)] - potDistancias[menorDistancia]);

    // Creacion de las LRC
    vector<int> LRCU;
    vector<int> LRCL;
    vector < pair<bool, bool> > asignado;

    // Mantenimiento de lista de asignados
    pair<bool, bool> p;
    p.first = false;
    p.second = false;
    asignado.resize(nCasos, p);

    // Creacion de la primera LRC
    for (int i = 0; i < nCasos; i++) {
        if (potFlujos[i] >= umbralFlujo) {
            LRCU.push_back(i);
        }
        if (potDistancias[i] >= umbralDistancia) {
            LRCL.push_back(i);
        }
    }
    //    cout << "Umbral Distancia: " << umbralDistancia << endl;
    //    cout << "Distancias: ";
    //    for(int i= 0; i< nCasos;i++){
    //        cout << potDistancias[i] << " ";
    //    }

    // Seleccion de la primera Asignacion
    int randomFlujoA = rand() % LRCU.size();
    int randomDistanciaA = rand() % LRCL.size();


    solInicial[randomFlujoA] = randomDistanciaA;
    asignado.at(randomFlujoA).first = true;
    asignado.at(randomDistanciaA).second = true;
    nAsignaciones = 1;

    // Seleccion de la segunda Asignacion
    while (nAsignaciones == 1) {
        int randomFlujoB = rand() % LRCU.size();
        int randomDistanciaB = rand() % LRCL.size();
        if (randomFlujoA != randomFlujoB && randomDistanciaA != randomDistanciaB) {
            solInicial[randomFlujoB] = randomDistanciaB;
            asignado.at(randomFlujoB).first = true;
            asignado.at(randomDistanciaB).second = true;
            nAsignaciones++;
        }
    }


    // ETAPA 2
    vector<pair<int, pair<int, int> > > LRC;
    while (nAsignaciones < nCasos) {
        LRC.clear();
        // Almacenamiento de todas las posibles asignaciones
        for (int i = 0; i < nCasos; i++) {
            if (!asignado.at(i).first) {
                for (int j = 0; j < nCasos; j++) {
                    if (!asignado.at(j).second) {
                        int Cij = costeGRASP(solInicial, nCasos, distancias, flujos, i, j);
                        LRC.push_back(pair<int, pair<int, int> >(Cij, pair<int, int>(i, j)));
                    }
                }
            }
        }

        // Ordenacion por coste
        sort(LRC.begin(), LRC.end(), comparador);
        float umbral = LRC.back().first + alfa * (LRC.front().first - LRC.back().first);
        int pos = 0;
        if (LRC.size() > 1) {
            while (pos < LRC.size() - 1 && LRC.at(pos).first <= umbral) {
                pos++;
            }
            if (pos > 0) {
                pos -= 1;
            }
        }

        // Realizacion de asignacion
        solInicial[LRC[pos].second.first] = LRC[pos].second.second;
        asignado[LRC[pos].second.first].first = true;
        asignado[LRC[pos].second.second].second = true;
        nAsignaciones++;
    }

    return solInicial;
}

int *GRASP(int nCasos, int **flujos, int **distancias, int seed) {
    int *solFinal = new int[nCasos];
    int *solucion;
    int *solucionPrima;
    int cont = 1;

    while (cont < repeticionesGRASP) {
        solucion = randomGreedy(nCasos, flujos, distancias, seed);
        solucionPrima = busquedaLocal(nCasos, flujos, distancias, seed, solucion);
        if (cont == 1) {
            for (int i = 0; i < nCasos; i++) {
                solFinal[i] = solucionPrima[i];
            }
        } else if (coste(solucionPrima, nCasos, distancias, flujos) < coste(solFinal, nCasos, distancias, flujos)) {
            for (int i = 0; i < nCasos; i++) {
                solFinal[i] = solucionPrima[i];
            }
        }
        delete[] solucion;
        delete[] solucionPrima;
        cont++;
    }
    return solFinal;
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
 * 
 * @param solucion vector solucion actual
 * @param nCasos tamaño del problema 
 * @param seed semilla
 * @return 
 */
int* mutar(int* solucion, int nCasos, int seed) {
    int tamSublista = nCasos / 4;
    int srand(seed);
    int aleatorio;
    int posIni = rand() % nCasos;
    int* usados = new int[tamSublista];

    int temp = posIni;
    for (int i = 0; i < tamSublista; i++) {
        usados[i] = solucion[temp];
        solucion[temp] = -1;
        temp++;
        if (temp >= nCasos) {
            temp = 0;
        }
    }
    int temp2 = posIni;
    for (int i = 0; i < tamSublista; i++) {
        do {
            aleatorio = rand() % tamSublista;
        } while (usados[aleatorio] == -1);

        solucion[temp2] = usados[aleatorio];
        temp2++;
        if (temp2 >= nCasos) {
            temp2 = 0;
        }
        usados[aleatorio] = -1;
    }
    return solucion;

}

/**
 * 
 * @param nCasos tamaño del problema
 * @param flujos matriz de flujos
 * @param distancias matriz distancias
 * @param seed semilla
 * @return 
 */
int* ils(int nCasos, int **flujos, int **distancias, int seed) {
    int* soluInicial = solInicial(nCasos, seed);
    int* solActual = busquedaLocal(nCasos, flujos, distancias, seed, soluInicial);
    int* solMutada = new int[nCasos];
    int *solLocal = new int[nCasos];
    int costeLocal;
    int costeAct = coste(solActual, nCasos, distancias, flujos);

    for (int i = 0; i < numIteraciones; i++) {
        solMutada = mutar(solActual, nCasos, seed);
        solLocal = busquedaLocal(nCasos, flujos, distancias, seed, solMutada);
        costeLocal = coste(solLocal, nCasos, distancias, flujos);
        if (costeLocal < costeAct) {
            solActual = solLocal;
            costeAct = costeLocal;
        }
    }
    return solActual;
}
