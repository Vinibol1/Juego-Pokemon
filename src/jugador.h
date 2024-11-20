#ifndef __JUGADOR_H__
#define __JUGADOR_H__
#include <stdlib.h>

typedef struct jugador {
	int x;
	int y;
	int iteraciones;
	int puntaje;
}jugador_t;


jugador_t * jugador_crear();

#endif // __JUGADOR_H__