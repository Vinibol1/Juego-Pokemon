#ifndef __JUGADOR_H__
#define __JUGADOR_H__
#include <stdlib.h>

typedef struct jugador jugador_t;

jugador_t * jugador_crear();

void jugador_insertar_posicion_x(jugador_t *jugador,int x);

void jugador_insertar_posicion_y(jugador_t *jugador,int y);

void jugador_insertar_multiplicador(jugador_t *jugador,int multiplicado);

void jugador_insertar_puntaje(jugador_t *jugador,int puntaje);

void jugador_insertar_iteraciones(jugador_t *jugador,int iteraciones);

int jugador_devolver_posicion_y(jugador_t *jugador);

int jugador_devolver_posicion_x(jugador_t *jugador);

int jugador_devolver_puntaje(jugador_t *jugador);

int jugador_devolver_multiplicador(jugador_t *jugador);

int jugador_devolver_iteraciones(jugador_t *jugador);

void jugador_aumentar_en_1_posicion_y(jugador_t *jugador);

void jugador_aumentar_en_1__posicion_x(jugador_t *jugador);

void jugador_restar_en_1_posicion_y(jugador_t *jugador);

void jugador_restar_en_1_posicion_x(jugador_t *jugador);



void jugador_destruir(jugador_t *jugador);


#endif // __JUGADOR_H__