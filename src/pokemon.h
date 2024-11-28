#ifndef __POKEMON_H__
#define __POKEMON_H__
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../extra/ansi.h"

typedef struct pokemon pokemon_t;

pokemon_t *pokemon_crear();

bool pokemon_insertar_atributos(pokemon_t *pokemon,char *nombre,int puntaje, char* color, char* movimientos);

void pokemon_insertar_posicion(pokemon_t *pokemon,int x,int y);

int pokemon_devolver_posicion_y(pokemon_t *pokemon);

int pokemon_devolver_posicion_x(pokemon_t *pokemon);

char pokemon_devolver_inicial(pokemon_t *pokemon);

char* pokemon_devolver_nombre(pokemon_t *pokemon);

char* pokemon_devolver_movimiento(pokemon_t *pokemon);

char* pokemon_devolver_color(pokemon_t *pokemon);

int pokemon_devolver_puntos(pokemon_t *pokemon);

void pokemon_aumentar_en_1_posicion_y(pokemon_t *pokemon);

void pokemon_aumentar_en_1_posicion_x(pokemon_t *pokemon);

void pokemon_restar_en_1_posicion_y(pokemon_t *pokemon);

void pokemon_restar_en_1_posicion_x(pokemon_t *pokemon);

char * pokemon_devolver_ansi(pokemon_t *pokemon);

void pokemon_destruir_atributos(pokemon_t *pokemon);

void pokemon_destruir(pokemon_t *pokemon);

#endif // __POKEMON_H__