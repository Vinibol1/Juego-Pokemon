#ifndef __POKEMON_H__
#define __POKEMON_H__
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct pokemon pokemon_t;

struct pokemon* pokemon_crear();

bool pokemon_insertar_atributos(pokemon_t *pokemon,char *nombre,int puntaje, char* color, char* movimientos);

char* pokemon_devolver_nombre(pokemon_t *pokemon);

char* pokemon_devolver_movimiento(pokemon_t *pokemon);

char* pokemon_devolver_color(pokemon_t *pokemon);

int pokemon_devolver_puntos(pokemon_t *pokemon);

void pokemon_destruir(pokemon_t *pokemon);

#endif // __POKEMON_H__