#include "menu.h";

void emezar_juego(){
        #include <stdlib.h>

#define ERROR -1
#define VALOR_MAXIMO 300
#define COLUMNAS 5

struct pokemon {
	char *nombre;
	char tipo;
	int fuerza;
	int destreza;
	int resistencia;
};

int comparar_nombre_pokemon(void *_p1, void *_p2)
{
	struct pokemon *p1 = _p1;
	struct pokemon *p2 = _p2;
	return strcmp(p1->nombre, p2->nombre);
}

struct pokemon *encontrar_pokemon_buscado(hash_t *pokedex,
					  struct pokemon *pokemon)
{
	char pokemon_buscado[VALOR_MAXIMO];
	printf("escribí el nombre del pokemon que vas a buscar: ");
	if (scanf("%s", pokemon_buscado) == EOF)
		return NULL;
	pokemon->nombre = pokemon_buscado;
	pokemon = hash_buscar(pokedex, pokemon->nombre);
	if (!pokemon)
		return NULL;

	printf("Nombre:%s, Tipo:%c, Fuerza:%i, Destreza:%i, Resistencia:%i\n",
	       pokemon->nombre, pokemon->tipo, pokemon->fuerza,
	       pokemon->destreza, pokemon->resistencia);
	return pokemon;
}

bool imprimir_pokemones_por_pantalla(char *clave, void *elemento, void *ctx)
{
	if (!elemento)
		return false;
	struct pokemon *pokemon_actual = elemento;
	printf("Nombre:%s, Tipo:%c, Fuerza:%i, Destreza:%i, Resistencia:%i\n",
	       pokemon_actual->nombre, pokemon_actual->tipo,
	       pokemon_actual->fuerza, pokemon_actual->destreza,
	       pokemon_actual->resistencia);
	return true;
}

void destruir_pokemon(void *pokemon)
{
	struct pokemon *pokemon_actual = pokemon;
	free(pokemon_actual->nombre);
	free(pokemon_actual);
}

bool leer_int(const char *str, void *ctx)
{
	return sscanf(str, "%d", (int *)ctx) == 1;
}

bool crear_string_nuevo(const char *str, void *ctx)
{
	char *nuevo = malloc(strlen(str) + 1);
	if (nuevo == NULL)
		return false;
	strcpy(nuevo, str);
	*(char **)ctx = nuevo;
	return true;
}

bool leer_caracter(const char *str, void *ctx)
{
	*(char *)ctx = *(char *)str;
	return true;
}

int main(int argc, char *argv[])
{
	
}

}