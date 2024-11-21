#include "jugador.h"

struct jugador {
	int x;
	int y;
	int iteraciones;
	int puntaje;
	int multiplicador;
}; 

jugador_t * jugador_crear(){
        return calloc(1,sizeof(jugador_t));
}

void jugador_insertar_posicion_x(jugador_t *jugador,int x){
        if (!jugador)
                return;
        jugador->x = x;
}

void jugador_insertar_posicion_y(jugador_t *jugador,int y){
        if (!jugador)
                return;
        jugador->y = y;
}


void jugador_insertar_multiplicador(jugador_t *jugador,int multiplicado){
        if (!jugador)
                return;
        jugador->multiplicador = multiplicado;
}

void jugador_insertar_puntaje(jugador_t *jugador,int puntaje){
        if (!jugador)
                return;
        jugador->puntaje = puntaje;
}

void jugador_insertar_iteraciones(jugador_t *jugador,int iteraciones){
        if (!jugador)
                return;
        jugador->iteraciones = iteraciones;
}


int jugador_devolver_posicion_y(jugador_t *jugador){
        if (!jugador)
                return -1;
        return jugador->y;
        
}

int jugador_devolver_posicion_x(jugador_t *jugador){
        if (!jugador)
                return -1;
        return jugador->x;
        
}

int jugador_devolver_puntaje(jugador_t *jugador){
        if (!jugador)
                return -1;
        return jugador->puntaje;
        
}

int jugador_devolver_multiplicador(jugador_t *jugador){
        if (!jugador)
                return -1;
        return jugador->multiplicador;
}

int jugador_devolver_iteraciones(jugador_t *jugador){
        if (!jugador)
                return -1;
        return jugador->iteraciones;
}


void jugador_aumentar_en_1_posicion_y(jugador_t *jugador){
        if (!jugador)
                return; 
        jugador->y++;
}

void jugador_aumentar_en_1__posicion_x(jugador_t *jugador){
        if (!jugador)
                return;
        jugador->x++;
}

void jugador_restar_en_1_posicion_y(jugador_t *jugador){
        if (!jugador)
                return;
        jugador->y--;
}

void jugador_restar_en_1_posicion_x(jugador_t *jugador){
        if (!jugador)
                return;
        jugador->x--;
}


void jugador_destruir(jugador_t *jugador){
        free(jugador);
}
