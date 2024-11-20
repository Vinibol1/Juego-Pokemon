#ifndef __MENU_H__
#define __MENU_H__
#include "diccionario/abb.h"

typedef struct menu menu_t;

menu_t *menu_crear();

void mostrar_pokedex(abb_t *pokedex);

void iniciar_juego();

void iniciar_juego_con_semilla();

void salir_juego();

#endif // __MENU_H__