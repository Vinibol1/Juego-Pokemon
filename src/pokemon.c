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
        pokemon->x = x;
        pokemon->y = y;
}


char pokemon_devolver_inicial(pokemon_t *pokemon){
        return pokemon->nombre[0];
}

int pokemon_devolver_posicion_x(pokemon_t *pokemon){
        return pokemon->x;
}

int pokemon_devolver_posicion_y(pokemon_t *pokemon){
        return pokemon->y;
}


char* pokemon_devolver_nombre(pokemon_t *pokemon){
        return pokemon->nombre;
}

char* pokemon_devolver_movimiento(pokemon_t *pokemon){
        return pokemon->movimiento;
}

char* pokemon_devolver_color(pokemon_t *pokemon){
        return pokemon->color;
}

int pokemon_devolver_puntos(pokemon_t *pokemon){
        return pokemon->puntos;
}


void pokemon_destruir(pokemon_t *pokemon){
        free(pokemon->nombre);
        free(pokemon->color);
        free(pokemon->movimiento);
        free(pokemon);
}