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

using namespace std;

int lectura(int **&flujos, int **&distancias, string fichero);

int *potencial(int **v, int &tam); 

int coste(int *v, int tam, int **distancias, int **flujos);

int* solInicial(int tam, int seed);

int factorizacion(int* v, int tam, int **flujos, int** distancias, int r, int s);

int *busquedaLocal(int nCasos, int **flujos, int **distancias, int seed);


#endif	/* UTIL_H */

