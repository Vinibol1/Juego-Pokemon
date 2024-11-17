#include "extra/engine.h"
#include "extra/ansi.h"
#include "src/diccionario/hash.h"
#include "src/csv.h"
#include <string.h>
#include <stdio.h>

#define ERROR -1
#define VALOR_MAXIMO 300
#define COLUMNAS 4

struct pokemon {
	char *nombre;
	int puntos;
	char *color;
	char *movimiento;
};



struct jugador {
	int x;
	int y;
	int iteraciones;
	int puntaje;
};

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

int logica(int entrada, void *datos)
{
	struct jugador *jugador = datos;
	borrar_pantalla();

	if (entrada == TECLA_DERECHA)
		jugador->x++;
	else if (entrada == TECLA_IZQUIERDA)
		jugador->x--;
	else if (entrada == TECLA_ARRIBA)
		jugador->y--;
	else if (entrada == TECLA_ABAJO)
		jugador->y++;

	jugador->x = min(32, max(0, jugador->x));
	jugador->y = min(15, max(0, jugador->y));

	jugador->iteraciones++;

	printf("Utilizar " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
	       "⬆⬇⬅➡" ANSI_COLOR_RESET " para moverse\n");

	printf("Presionar " ANSI_COLOR_RED ANSI_COLOR_BOLD "Q" ANSI_COLOR_RESET
	       " para salir\n");

	printf("Iteraciones: %d Tiempo: %d\n", jugador->iteraciones,
	       jugador->iteraciones / 5);

	for (int i = 0; i < jugador->y; i++)
		printf("\n");

	for (int i = 0; i < jugador->x; i++)
		printf(" ");

	printf(ANSI_COLOR_MAGENTA ANSI_COLOR_BOLD "Ω" ANSI_COLOR_RESET);

	printf("\n");
	esconder_cursor();

	return entrada == 'q' || entrada == 'Q';
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

void crear_pokedex(const char* nombre_archivo){
	struct archivo_csv *archivo = abrir_archivo_csv(nombre_archivo, ',');
	if (!archivo) {
		cerrar_archivo_csv(archivo);
		return ERROR;
	}

	struct pokemon *pokemon;

	hash_t *pokedex = hash_crear(10);
	if (!pokedex) {
		cerrar_archivo_csv(archivo);
		return ERROR;
	}
	bool (*funciones[COLUMNAS])(const char *, void *) = {
		crear_string_nuevo, leer_int, crear_string_nuevo, crear_string_nuevo  
	};
	char *nombre = NULL;
	int puntos;
	char *color = NULL;
	char *movimiento = NULL;
	void *punteros[COLUMNAS] = { &nombre, &puntos, &color, &movimiento
				     };
	while (leer_linea_csv(archivo, COLUMNAS, funciones, punteros) ==
	       COLUMNAS) {
		pokemon = malloc(sizeof(struct pokemon));
		if (!pokemon) {
			hash_destruir(pokedex);
			cerrar_archivo_csv(archivo);
			return ERROR;
		}
		pokemon->nombre = nombre;
		pokemon->tipo = tipo;
		pokemon->fuerza = fuerza;
		pokemon->destreza = destreza;
		pokemon->resistencia = resistencia;
		if (!hash_insertar(pokedex, pokemon->nombre, pokemon, NULL)) {
			hash_destruir(pokedex);
			cerrar_archivo_csv(archivo);
			return ERROR;
		}
	}
	cerrar_archivo_csv(archivo);
	hash_destruir_todo(pokedex, destruir_pokemon);
}

int main()
{
	struct jugador jugador = { 0 };

	game_loop(logica, &jugador);

	mostrar_cursor();

	return 0;
}
