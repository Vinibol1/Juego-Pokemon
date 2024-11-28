#include "pokemon.h"



struct  pokemon
{
        char *nombre;
	int puntos;
	char *color;
	char *movimiento;
        int x;
        int y;
};

struct pokemon* pokemon_crear(){
        return calloc(1,sizeof(pokemon_t));
}

static char *copiar_string(char*string){
        if (string == NULL)
                return NULL;
        
        char *copia_string = malloc((strlen(string) + 1) * sizeof(char));
	if (!copia_string)
		return NULL;
	strcpy(copia_string, string);
        return copia_string;
}

bool pokemon_insertar_atributos(pokemon_t *pokemon, char *nombre,int puntaje, char* color, char* movimientos){
        if (!pokemon || !nombre)
                return false;
        
        
        char *copia_nombre = copiar_string(nombre);
        if (!copia_nombre)
                return false;

        pokemon->nombre = copia_nombre;
        pokemon->puntos = puntaje;

        char *copia_color = copiar_string(color);
        if (!copia_color){
                free(copia_nombre);
                return false;
        }
        pokemon->color = copia_color;

        char *copia_movimientos = copiar_string(movimientos);
        if (!copia_movimientos){
                free(copia_nombre);
                free(copia_color);
                return false;
        }
        pokemon->movimiento = copia_movimientos;

        return true;
}

void pokemon_insertar_posicion(pokemon_t *pokemon,int x,int y){
        if (!pokemon)
                return;
        
        pokemon->x = x;
        pokemon->y = y;
}


char pokemon_devolver_inicial(pokemon_t *pokemon){
        if (!pokemon)
                return 0;

        return pokemon->nombre[0];
}

int pokemon_devolver_posicion_x(pokemon_t *pokemon){
        if (!pokemon)
                return -1;
        return pokemon->x;
}

int pokemon_devolver_posicion_y(pokemon_t *pokemon){
        if (!pokemon)
                return -1;
        return pokemon->y;
}

char *pokemon_devolver_ansi(pokemon_t *pokemon){
        if (!pokemon)
                return NULL;
        if (strcmp(pokemon->color,"VERDE") == 0)
                return ANSI_COLOR_GREEN;
        if (strcmp(pokemon->color,"AMARILLO") == 0 )
                return ANSI_COLOR_YELLOW;
        if (strcmp(pokemon->color,"ROJO") == 0 )
                return ANSI_COLOR_RED;
        if (strcmp(pokemon->color,"MAGENTA") == 0)
                return ANSI_COLOR_MAGENTA;
        if (strcmp(pokemon->color,"AZUL") == 0)
                return ANSI_COLOR_BLUE;
        return ANSI_COLOR_RESET;
        
}


char* pokemon_devolver_nombre(pokemon_t *pokemon){
        if (!pokemon)
                return NULL;
        return pokemon->nombre;
}

char* pokemon_devolver_movimiento(pokemon_t *pokemon){
        if (!pokemon)
                return NULL;
        return pokemon->movimiento;
}

char* pokemon_devolver_color(pokemon_t *pokemon){
        if (!pokemon)
                return NULL;
        return pokemon->color;
}

int pokemon_devolver_puntos(pokemon_t *pokemon){
        if (!pokemon)
                return -1;
        return pokemon->puntos;
}

void pokemon_aumentar_en_1_posicion_y(pokemon_t *pokemon){
        if (!pokemon)
                return;
        pokemon->y++;
        
}

void pokemon_aumentar_en_1_posicion_x(pokemon_t *pokemon){
        if (!pokemon)
                return;
        pokemon->x++;
}

void pokemon_restar_en_1_posicion_y(pokemon_t *pokemon){
        if (!pokemon)
                return;
        pokemon->y--;
}

void pokemon_restar_en_1_posicion_x(pokemon_t *pokemon){
        if (!pokemon)
                return;
        pokemon->x--;
}

void pokemon_destruir_atributos(pokemon_t *pokemon){
        if (!pokemon)
                return;
        free(pokemon->nombre);
        free(pokemon->color);
        free(pokemon->movimiento);
}

void pokemon_destruir(pokemon_t *pokemon){
        if (!pokemon)
                return;
        free(pokemon->nombre);
        free(pokemon->color);
        free(pokemon->movimiento);
        free(pokemon);
}