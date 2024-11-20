#include "extra/engine.h"
#include "extra/ansi.h"
#include "src/diccionario/abb.h"
#include "src/csv.h"
#include "src/pokemon.h"
#include "src/menu.h"
#include "src/jugador.h"
#include <string.h>
#include <time.h>
#include <stdio.h>


#define ERROR -1
#define VALOR_MAXIMO 300
#define MAXIMO_TERRENO_X 20
#define MAXIMO_TERRENO_Y 20
#define COLUMNAS 4




struct menu
{
        void (*mostrar_pokedex)(abb_t *);
        void (*iniciar_juego)(int (*callback)(int, void *), void *data);
        void (*iniciar_juego_con_semilla);
        void (*salir_juego);
};

struct datos {
	jugador_t *jugador;
	abb_t *pokedex;
};

int valor_random_terreno_x(){
	return (rand() % MAXIMO_TERRENO_X);
}

int valor_random_terreno_y(){
	return (rand() % MAXIMO_TERRENO_Y);
}

bool posicion_inicial(void *elemento, void *ctx){
	pokemon_t *pokemon = elemento;
	pokemon_insertar_posicion(pokemon,(int)(valor_random_terreno_x()),(int)(valor_random_terreno_y()));
	return true;
}

bool posicionar_pokemon_en_terreno(void *elemento, void *ctx){
	pokemon_t *pokemon = elemento;
	char (*terreno)[MAXIMO_TERRENO_Y][MAXIMO_TERRENO_X] = ctx;
	(*terreno)[pokemon_devolver_posicion_y(pokemon)][pokemon_devolver_posicion_x(pokemon)] = pokemon_devolver_inicial(pokemon); 
	return true;
}


void inicializar_terreno(char terreno[MAXIMO_TERRENO_Y][MAXIMO_TERRENO_X]){
	for(int i = 0;i < MAXIMO_TERRENO_Y; i++) 
		for (int j = 0; j < MAXIMO_TERRENO_X; j++)
			terreno[i][j] = '-';
}

void imprimir_terreno(void *_datos){
	char terreno[MAXIMO_TERRENO_Y][MAXIMO_TERRENO_X];
	inicializar_terreno(terreno);
	struct datos *datos = _datos;
	abb_t *pokedex = datos->pokedex;
	jugador_t *jugador = datos->jugador;
	abb_iterar_inorden(pokedex,posicionar_pokemon_en_terreno,&terreno);
	terreno[jugador->y][jugador->x] = 'J';
	for (int i = 0; i < MAXIMO_TERRENO_Y; i++)
	{
		for (int j = 0; j < MAXIMO_TERRENO_X; j++)
			printf("| %c ", terreno[i][j]);

		printf("|\n");
	}

}

int comparar_nombre_pokemon(void *_p1, void *_p2)
{
	pokemon_t *p1 = _p1;
	pokemon_t *p2 = _p2;
	return strcmp((char *)pokemon_devolver_nombre(p1), (char *)pokemon_devolver_nombre(p2));
}

void destructor(void *_pokemon)
{
	pokemon_t * pokemon = _pokemon;
	pokemon_destruir(pokemon);
}


bool imprimir_pokemon_por_pantalla(void *elemento, void *ctx){
	pokemon_t *pokemon_actual = elemento; 
	printf("Nombre:%s, Puntos:%i, Color:%s, Movimiento:%s, Posicion_X:%i \n",
	       pokemon_devolver_nombre(pokemon_actual), pokemon_devolver_puntos(pokemon_actual),
	       pokemon_devolver_color(pokemon_actual), pokemon_devolver_movimiento(pokemon_actual), pokemon_devolver_posicion_x(pokemon_actual));
	return true;
}

void mostrar_pokedex(abb_t *pokedex){
	abb_iterar_inorden(pokedex, imprimir_pokemon_por_pantalla, NULL);
}


int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

int logica(int entrada, void *_datos)
{
	struct datos *datos = _datos;
	abb_t *pokedex = datos->pokedex;
	jugador_t *jugador = datos->jugador;

	borrar_pantalla();
	system("clear");
	if (entrada == TECLA_DERECHA)
		jugador->x++;
	else if (entrada == TECLA_IZQUIERDA)
		jugador->x--;
	else if (entrada == TECLA_ARRIBA)
		jugador->y--;
	else if (entrada == TECLA_ABAJO)
		jugador->y++;

	jugador->x = min(MAXIMO_TERRENO_X - 1, max(0, jugador->x));
	jugador->y = min(MAXIMO_TERRENO_Y - 1, max(0, jugador->y));

	jugador->iteraciones++;

	printf("Utilizar " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
	       "⬆⬇⬅➡" ANSI_COLOR_RESET " para moverse\n");

	printf("Presionar " ANSI_COLOR_RED ANSI_COLOR_BOLD "Q" ANSI_COLOR_RESET
	       " para salir\n");
	
	printf("Iteraciones: %d Tiempo: %d\n", jugador->iteraciones,
	       jugador->iteraciones / 5);
	imprimir_terreno(datos);

	printf("\n");
	esconder_cursor();

	return entrada == 'q' || entrada == 'Q';
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

abb_t *crear_pokedex(const char* nombre_archivo){
	struct archivo_csv *archivo = abrir_archivo_csv(nombre_archivo, ',');
	if (!archivo) {
		cerrar_archivo_csv(archivo);
		return NULL;
	}

	pokemon_t *pokemon = NULL;

	abb_t *pokedex = abb_crear(comparar_nombre_pokemon);
	if (!pokedex) {
		cerrar_archivo_csv(archivo);
		return NULL;
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
		pokemon = pokemon_crear();
		if (!pokemon) {
			abb_destruir(pokedex);
			cerrar_archivo_csv(archivo);
			return NULL;
		}
		pokemon_insertar_atributos(pokemon,nombre,puntos,color,movimiento);
		if (!abb_insertar(pokedex, pokemon)) {
			abb_destruir(pokedex);
			pokemon_destruir(pokemon);
			cerrar_archivo_csv(archivo);
			return NULL;
		}
		free(nombre);
		free(movimiento);
		free(color);
	}
	cerrar_archivo_csv(archivo);
	return pokedex;
}


int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	jugador_t *jugador = jugador_crear();
	struct datos datos;
	datos.jugador = jugador;
	abb_t *pokedex = crear_pokedex(argv[1]);
	menu_t menu;
	datos.pokedex = pokedex;
	menu.mostrar_pokedex = mostrar_pokedex; 
	menu.iniciar_juego = game_loop;


	bool es_correcto = false;
	while (!es_correcto) {
		char opcion;
		int semilla;
		printf("Escribe el numero de la opción que quieras\nP: Muestra el pokedex cargado. Se deben mostrar los pokemon por orden alfabético\n- J: Jugar. Inicia el juego.\n- S: Jugar con semilla. Inicia el juego pero pidiendo un número para utilizar como semilla.\n- Q: Salir. Sale del juego.");
		if (!scanf("%c", &opcion)) {
			printf("No se pudo leer el numero correctamente");
			return ERROR;
		}
		switch (opcion) {
			case 'P':
				menu.mostrar_pokedex(pokedex);
				es_correcto = true;
				break;
			case 'J':
				abb_iterar_inorden(pokedex,posicion_inicial,NULL);
				menu.iniciar_juego(logica,&datos);
				es_correcto = true;
				break;
			case 'S':
				printf("ingresa el numero de la semilla: ");
				if (!scanf("%i", &semilla)) {
					printf("No se pudo leer el numero correctamente");
					break;
				}
				srand((unsigned int)semilla);
				abb_iterar_inorden(pokedex,posicion_inicial,NULL);
				menu.iniciar_juego(logica,&datos);
				es_correcto = true;
				break;
			default:
				printf("Numero no valido ingresado, por favor ingrese 1 o 2\n");
				break;
		}
	}

	mostrar_cursor();
	abb_destruir_todo(pokedex,destructor);

	return 0;
}
