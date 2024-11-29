#ifndef __POKEMON_H__
#define __POKEMON_H__
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../extra/ansi.h"

typedef struct pokemon pokemon_t;

pokemon_t *pokemon_crear();

/**
 * inserta los atributos del pokemon, en caso de que el nombre, color o movimiento sean NULL la funcion devolvera false
 * en caso de error devuelvera false
 */
bool pokemon_insertar_atributos(pokemon_t *pokemon,char *nombre,int puntaje, char* color, char* movimientos);


/**
 * inserta posicion x e y del pokemon
 */
void pokemon_insertar_posicion(pokemon_t *pokemon,int x,int y);


/**
 * devuelve la posicion y del pokemon
 * en caso de error devuelve -1
 */
int pokemon_devolver_posicion_y(pokemon_t *pokemon);


/**
 * devuelve la posicion x del pokemon
 * en caso de error devuelve -1 
 */
int pokemon_devolver_posicion_x(pokemon_t *pokemon);

/**
 * devuelve la inicial del nombre del pokemon
 * en caso de error devuelve /0
 */
char pokemon_devolver_inicial(pokemon_t *pokemon);

/**
 * devuelve el nombre del pokemon
 * en caso de error devuelve NULL
 */
char* pokemon_devolver_nombre(pokemon_t *pokemon);

/**
 * devuelve el movimiento que hace el pokemon
 * en caso de error devuelve NULL
 */
char* pokemon_devolver_movimiento(pokemon_t *pokemon);

/**
 * devuelve el color del pokemon
 * en caso de error devuelve NULL
 */
char* pokemon_devolver_color(pokemon_t *pokemon);

/**
 * devuelve los puntos que da el pokemon
 * en caso de error devuelve -1
 */
int pokemon_devolver_puntos(pokemon_t *pokemon);

/**
 * aumenta 1 a la posicion y
 */
void pokemon_aumentar_en_1_posicion_y(pokemon_t *pokemon);

/**
 * aumenta 1 a la posicion x
 */
void pokemon_aumentar_en_1_posicion_x(pokemon_t *pokemon);

/**
 * resta 1 a la posicion y
 */
void pokemon_restar_en_1_posicion_y(pokemon_t *pokemon);

/**
 * resta 1 a la posicion x
 */
void pokemon_restar_en_1_posicion_x(pokemon_t *pokemon);

/**
 * devuelvo el codigo ansi del color del pokemon
 * en caso de no encontrarse devuelve codigo ansi para resetear el color
 */
char * pokemon_devolver_ansi(pokemon_t *pokemon);

/**
 * libera unicamente los atributos del pokemon
 */
void pokemon_destruir_atributos(pokemon_t *pokemon);

/**
 * destruye el pokemon
 */
void pokemon_destruir(pokemon_t *pokemon);

#endif // __POKEMON_H__