#ifndef __JUGADOR_H__
#define __JUGADOR_H__
#include <stdlib.h>
#include <stdbool.h>

typedef struct jugador jugador_t;

jugador_t * jugador_crear();

/**
 * inserta posicion x
 */
void jugador_insertar_posicion_x(jugador_t *jugador,int x);

/**
 * inserta posicion y
 */
void jugador_insertar_posicion_y(jugador_t *jugador,int y);

/**
 * inserta el multiplicador deseado
 */
void jugador_insertar_multiplicador(jugador_t *jugador,int multiplicado);


/**
 * suma el puntaje del jugador
 */
void jugador_insertar_puntaje(jugador_t *jugador,int puntaje);


/**
 * inserta la ultima inicial del pokemon capturado
 */
void jugador_insertar_ultima_inicial(jugador_t *jugador,char inicial);

/**
 * inserta la ultimo pokemon  capturado
 */
void jugador_insertar_ultimo_pokemon(jugador_t *jugador,char *nombre_pokemon);



/**
 * inserta el ultimo color de pokemon capturado
 */
void jugador_insertar_ultimo_color(jugador_t *jugador,char *color);


/**
 * inserta iteraciones al jugador
 */
void jugador_insertar_iteraciones(jugador_t *jugador,int iteraciones);


/**
 * inserta un pokemon al combo, si perdes el combo y conseguis uno mas grande se reemplaza
 * en caso de error devuelve false 
 */
bool jugador_insertar_pokemon_combo(jugador_t *jugador,char *pokemon);

/**
 * devuelve la posicion y del jugador
 * en caso de error devuelve -1s
 */
int jugador_devolver_posicion_y(jugador_t *jugador);


/**
 * devuelve el combo de los pokemones capturados 
 * en caso de error devuelve NULL
 */
char **jugador_devolver_pokemon_combo(jugador_t *jugador);


/**
 * devuelve posicion de x
 * en caso de error devuelve -1
 */
int jugador_devolver_posicion_x(jugador_t *jugador);


/**
 * devuelve el total del puntaje obtenido 
 * en caso de error da -1 
 */
int jugador_devolver_puntaje(jugador_t *jugador);


/**
 * devuelve el multiplicador actual del jugador.
 * si da error devuelve -1
 */
int jugador_devolver_multiplicador(jugador_t *jugador);


/**
 * devuelve las iteraciones del jugador
 * si da error devuelve -1 
 */
int jugador_devolver_iteraciones(jugador_t *jugador);


/**
 * devuelve ultima inicial del pokemon capturado
 * si da error devuelve /0
 */
char jugador_devolver_ultima_inicial(jugador_t *jugador);

/**
 * devuelve el nombre del ultimo pokemon capturado
 * si da error devuelve NULL
 */
char *jugador_devolver_ultimo_pokemon(jugador_t *jugador);

/**
 * devuelve el multiplicador maximo del jugador.
 * devuelve -1 si da error
 */
int jugador_devolver_multiplicador_maximo(jugador_t *jugador);



/**
 * devuelve el ultimo color del pokemon capturado
 * si no tiene devuelve NULL
 */
char *jugador_devolver_ultimo_color(jugador_t *jugador);


/**
 * aumenta 1 posición y.
 */
void jugador_aumentar_en_1_posicion_y(jugador_t *jugador);


/**
 * aumenta 1 posición x.
 */
void jugador_aumentar_en_1_posicion_x(jugador_t *jugador);

/**
 * resta 1 posición y.
 */
void jugador_restar_en_1_posicion_y(jugador_t *jugador);

/**
 * resta 1 a posición x.
 */
void jugador_restar_en_1_posicion_x(jugador_t *jugador);


/**
 * Destruye el jugador.
 */
void jugador_destruir(jugador_t *jugador);


#endif // __JUGADOR_H__