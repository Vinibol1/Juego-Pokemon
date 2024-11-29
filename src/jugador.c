#include "jugador.h"
#include <string.h>

struct combo_pokemones{
        char **actual_combo_pokemones;
        int tamanio_actual;
        char **mejor_combo_pokemones;
        int tamanio_mejor;
};

struct jugador {
	int x;
	int y;
	int iteraciones;
	int puntaje;
	int multiplicador;
        int multiplicador_maximo;
        char ultima_inicial_capturada;
        char *ultimo_color_capturado;
        char *ultimo_pokemon_capturado;
        struct combo_pokemones *combo_pokemones;
}; 

jugador_t * jugador_crear(){
        jugador_t *jugador = calloc(1,sizeof(jugador_t));
        jugador->multiplicador = 1;
        jugador->multiplicador_maximo = 1;
        jugador->combo_pokemones = calloc(1,sizeof(struct combo_pokemones));
        jugador->combo_pokemones->actual_combo_pokemones = calloc(1,sizeof(char *));
        jugador->combo_pokemones->mejor_combo_pokemones = calloc(1,sizeof(char *));
        return jugador;
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


void jugador_insertar_multiplicador(jugador_t *jugador,int multiplicador){
        if (!jugador)
                return;
        jugador->multiplicador = multiplicador;
        if (jugador->multiplicador > jugador->multiplicador_maximo)
                jugador->multiplicador_maximo = jugador->multiplicador;       
}

void jugador_insertar_puntaje(jugador_t *jugador,int puntaje){
        if (!jugador)
                return;
        jugador->puntaje += puntaje;
}

void jugador_insertar_ultimo_pokemon(jugador_t *jugador,char *nombre_pokemon){
        if (!jugador || !nombre_pokemon)
                return ;
        if (jugador->ultimo_pokemon_capturado)
                free(jugador->ultimo_pokemon_capturado);
        
        
        char *copia_string = malloc((strlen(nombre_pokemon) + 1) * sizeof(char));
	if (!copia_string)
		return ;
	strcpy(copia_string, nombre_pokemon);
        jugador->ultimo_pokemon_capturado = copia_string;
}

void jugador_insertar_iteraciones(jugador_t *jugador,int iteraciones){
        if (!jugador)
                return;
        jugador->iteraciones = iteraciones;
}

void jugador_insertar_ultima_inicial(jugador_t *jugador,char inicial){
        if (!jugador)
                return ;
        jugador->ultima_inicial_capturada = inicial;
}

void jugador_insertar_ultimo_color(jugador_t *jugador,char *color){
        if (!jugador || !color)
                return ;
        if (jugador->ultimo_color_capturado)
                free(jugador->ultimo_color_capturado);
        
        
        char *copia_string = malloc((strlen(color) + 1) * sizeof(char));
	if (!copia_string)
		return ;
	strcpy(copia_string, color);
        jugador->ultimo_color_capturado = copia_string;
}

bool jugador_insertar_pokemon_combo(jugador_t *jugador,char *pokemon){
        if (!jugador || !pokemon)
                return false;
        char *copia_string = malloc((strlen(pokemon) + 1) * sizeof(char));
	if (!copia_string)
		return false;
	strcpy(copia_string, pokemon);

        if (*jugador->combo_pokemones->actual_combo_pokemones && (jugador->multiplicador == 1))
        {
                for (int i = 0; jugador->combo_pokemones->actual_combo_pokemones[i]; i++) {
                        free(jugador->combo_pokemones->actual_combo_pokemones[i]);
                }
                jugador->combo_pokemones->tamanio_actual = 0;
                
        }
        char **aux_actual = realloc(jugador->combo_pokemones->actual_combo_pokemones,sizeof(char *)*(size_t)(jugador->multiplicador + 1));
        if (!aux_actual)
        {
                free(copia_string);
                return false;
        }
        aux_actual[jugador->multiplicador] = NULL;
        aux_actual[jugador->multiplicador - 1] = copia_string;
        if (jugador->multiplicador >= jugador->multiplicador_maximo)
        {
                char **aux_mejor = realloc(jugador->combo_pokemones->mejor_combo_pokemones,sizeof(char *)*(size_t)(jugador->multiplicador+1));
                if (!aux_mejor){
                        free(aux_actual);
                        free(copia_string);
                        return false;
                }
                aux_mejor[jugador->multiplicador] = NULL;
                if (*aux_mejor && strcmp(*aux_actual,*aux_mejor) != 0)
                {
                        for (int i = 0; aux_actual[i]; i++)
                        {
                                char *copia_string2 = malloc((strlen(aux_actual[i]) + 1) * sizeof(char));
                                if (!copia_string2){
                                        free(copia_string);
                                        free(aux_actual);
                                        free(aux_mejor);
                                        return false;
                                }
                                strcpy(copia_string2, aux_actual[i]);
                                free(aux_mejor[i]);
                                aux_mejor[i] = copia_string2;
                        }
                }
                else {
                        char *copia_string2 = malloc((strlen(pokemon) + 1) * sizeof(char));
                        if (!copia_string2){
                                free(copia_string);
                                free(aux_actual);
                                free(aux_mejor);
                                return false;
                        }
                        strcpy(copia_string2, pokemon);
                        aux_mejor[jugador->multiplicador - 1] = copia_string2;
                }
                jugador->combo_pokemones->mejor_combo_pokemones = aux_mejor;
                jugador->combo_pokemones->tamanio_mejor += 1;
        } 
        jugador->combo_pokemones->actual_combo_pokemones = aux_actual;
        jugador->combo_pokemones->tamanio_actual += 1;
        return true;
}

char **jugador_devolver_pokemon_combo(jugador_t *jugador){
         if (!jugador)
                return NULL;
        return jugador->combo_pokemones->mejor_combo_pokemones;
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


int jugador_devolver_multiplicador_maximo(jugador_t *jugador){
        if (!jugador)
                return -1;
        return jugador->multiplicador_maximo;
        
}
char *jugador_devolver_ultimo_pokemon(jugador_t *jugador){
        if (!jugador)
                return NULL;
        return jugador->ultimo_pokemon_capturado;
        
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


char jugador_devolver_ultima_inicial(jugador_t *jugador){
        if (!jugador)
                return '\0';
        return jugador->ultima_inicial_capturada;
        
}

char *jugador_devolver_ultimo_color(jugador_t *jugador){
        if (!jugador)
                return NULL;
        return jugador->ultimo_color_capturado;
}

void jugador_aumentar_en_1_posicion_y(jugador_t *jugador){
        if (!jugador)
                return; 
        jugador->y++;
}

void jugador_aumentar_en_1_posicion_x(jugador_t *jugador){
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
        for (int i = 0; jugador->combo_pokemones->actual_combo_pokemones[i]; i++) {
                        free(jugador->combo_pokemones->actual_combo_pokemones[i]);
                }
        free(jugador->combo_pokemones->actual_combo_pokemones);
        for (int i = 0; jugador->combo_pokemones->mejor_combo_pokemones[i]; i++) {
                free(jugador->combo_pokemones->mejor_combo_pokemones[i]);
        }
        free(jugador->combo_pokemones->mejor_combo_pokemones);
        free(jugador->combo_pokemones);  
        free(jugador->ultimo_color_capturado);
        free(jugador->ultimo_pokemon_capturado);
        free(jugador);
}
