/* 
 * File:   util.h
 * Author: JoseDavid
 *
 * Created on 11 de noviembre de 2014, 17:31
 */

#ifndef UTIL_H
#define	UTIL_H
#include <iostream> // cout
#include <cstdlib>  // atoi
#include <sstream> // stringstream
#include <fstream> // fstream
#include <bitset> 
#include <vector>
#include <queue>
#include <algorithm> // Sort

using namespace std;

int mayor(int *v, int tam); // V

int menor(int *v, int tam); // V

int lectura(int **&flujos, int **&distancias, string fichero); // V

int *potencialGRASP(int **v, int &tam); // V

int coste(int *v, int tam, int **distancias, int **flujos);

int factorizacion(int* v, int tam, int **flujos, int** distancias, int r, int s);

int *busquedaLocal(int nCasos, int **flujos, int **distancias, int seed, int *solucionInicial);

int *randomGreedy(int nCasos, int **flujos, int **distancias, int seed);

int *GRASP(int nCasos, int **flujos, int **distancias, int seed);

int *potencial(int **v, int &tam);

int greedy(int **flujos, int **distancias, int *&solGreedy, int nCasos);
#endif	/* UTIL_H */

