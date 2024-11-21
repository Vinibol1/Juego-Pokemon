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
        void (*salir_juego);
};

void atrapar_pokemon(void * pokemon, void *jugador) {
	if (pokemon_devolver_posicion_x(pokemon) == jugador_devolver_posicion_x(jugador) && pokemon_devolver_posicion_y(pokemon) == jugador_devolver_posicion_y(jugador))
	{
		/* code */
	}
	
}

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
	
	terreno[jugador_devolver_posicion_y(jugador)][jugador_devolver_posicion_x(jugador)] = 'J';
	for (int i = 0; i < MAXIMO_TERRENO_Y; i++)
	{
		for (int j = 0; j < MAXIMO_TERRENO_X; j++)
			printf("| " " %c " , terreno[i][j]);

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
	printf("Nombre:%s, Puntos:%i, Color:%s, Movimiento:%s \n",
	       pokemon_devolver_nombre(pokemon_actual), pokemon_devolver_puntos(pokemon_actual),
	       pokemon_devolver_color(pokemon_actual), pokemon_devolver_movimiento(pokemon_actual));
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

bool movimientos_pokemones(void *_pokemon,void *_entrada_jugador){
	pokemon_t *pokemon = _pokemon;
	int *entrada_jugador = _entrada_jugador;
	char *movimiento = pokemon_devolver_movimiento(pokemon);
	int numero_random = rand() % 4;
	while (*movimiento != 0)
	{
		switch (*movimiento)
		{
			case 'N':
				pokemon_restar_en_1_posicion_y(pokemon);
				break;
			case 'S':
				pokemon_aumentar_en_1_posicion_y(pokemon);
				break;
			case 'E':
				pokemon_aumentar_en_1_posicion_x(pokemon);
				break;
			case 'O':
				pokemon_restar_en_1_posicion_x(pokemon);
				break;
			case 'J':
				if (*entrada_jugador == TECLA_DERECHA)
					pokemon_aumentar_en_1_posicion_x(pokemon);
				else if (*entrada_jugador == TECLA_IZQUIERDA)
					pokemon_restar_en_1_posicion_x(pokemon);
				else if (*entrada_jugador == TECLA_ARRIBA)
					pokemon_restar_en_1_posicion_y(pokemon);
				else if (*entrada_jugador == TECLA_ABAJO)
					pokemon_aumentar_en_1_posicion_y(pokemon);
				break;
			case 'I':
				if (*entrada_jugador == TECLA_DERECHA)
					pokemon_restar_en_1_posicion_x(pokemon);
				else if (*entrada_jugador == TECLA_IZQUIERDA)
					pokemon_aumentar_en_1_posicion_x(pokemon);
				else if (*entrada_jugador == TECLA_ARRIBA)
					pokemon_aumentar_en_1_posicion_y(pokemon);
				else if (*entrada_jugador == TECLA_ABAJO)
					pokemon_restar_en_1_posicion_y(pokemon);
				break;
			case 'R':
				if (numero_random == 0)
					pokemon_restar_en_1_posicion_x(pokemon);
				else if (numero_random == 1)
					pokemon_aumentar_en_1_posicion_x(pokemon);
				else if (numero_random == 2)
					pokemon_aumentar_en_1_posicion_y(pokemon);
				else if (numero_random == 3)
					pokemon_restar_en_1_posicion_y(pokemon);
			default:
				break;
		}
		movimiento++;
		
	}
	pokemon_insertar_posicion(pokemon,min(MAXIMO_TERRENO_X - 1, max(0, pokemon_devolver_posicion_x(pokemon))),min(MAXIMO_TERRENO_Y - 1, max(0, pokemon_devolver_posicion_y(pokemon))));
	return true;
}


int logica(int entrada, void *_datos)
{
	struct datos *datos = _datos;
	abb_t *pokedex = datos->pokedex;
	jugador_t *jugador = datos->jugador;

	borrar_pantalla();
	system("clear");
	if (entrada == TECLA_DERECHA){
		jugador_aumentar_en_1__posicion_x(jugador);
		abb_iterar_inorden(pokedex,movimientos_pokemones,&entrada);
	}
	else if (entrada == TECLA_IZQUIERDA){
		jugador_restar_en_1_posicion_x(jugador);
		abb_iterar_inorden(pokedex,movimientos_pokemones,&entrada);
	}
	else if (entrada == TECLA_ARRIBA){
		jugador_restar_en_1_posicion_y(jugador);
		abb_iterar_inorden(pokedex,movimientos_pokemones,&entrada);
	}
	else if (entrada == TECLA_ABAJO){
		jugador_aumentar_en_1_posicion_y(jugador);
		abb_iterar_inorden(pokedex,movimientos_pokemones,&entrada);
	}

	jugador_insertar_posicion_x(jugador,min(MAXIMO_TERRENO_X - 1, max(0, jugador_devolver_posicion_x(jugador))));
	jugador_insertar_posicion_y(jugador,min(MAXIMO_TERRENO_Y - 1, max(0, jugador_devolver_posicion_y(jugador))));

	jugador_insertar_iteraciones(jugador,jugador_devolver_iteraciones(jugador) + 1);

	printf("Utilizar " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
	       "⬆⬇⬅➡" ANSI_COLOR_RESET " para moverse\n");

	printf("Presionar " ANSI_COLOR_RED ANSI_COLOR_BOLD "Q" ANSI_COLOR_RESET
	       " para salir\n");
	
	printf("Iteraciones: %d Tiempo: %d\n", jugador_devolver_iteraciones(jugador),
	       jugador_devolver_iteraciones(jugador) / 5);
	imprimir_terreno(datos);

	printf("\n");
	esconder_cursor();

	return entrada == 'q' || entrada == 'Q';
};

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


struct aux {
	abb_t *pokedex_7_pokemones;
	int rand;
	int contador;
};

bool pokemones_randoms_de_pokedex(void *_pokemon,void *ctx){
	pokemon_t *pokemon = _pokemon;
	struct aux *contenedor = ctx;
	if (contenedor->contador != contenedor->rand)
	{
		contenedor->contador++;
		return true;
	}
	pokemon_t *nuevo_pokemon = pokemon_crear();
	pokemon_insertar_atributos(nuevo_pokemon,pokemon_devolver_nombre(pokemon),pokemon_devolver_puntos(pokemon),pokemon_devolver_color(pokemon),pokemon_devolver_movimiento(pokemon));
	abb_insertar(contenedor->pokedex_7_pokemones,nuevo_pokemon);
	return false;
}

abb_t *crear_pokemones_elegidos(abb_t *pokedex){
	abb_t *pokedex_con_7_pokemones = abb_crear(comparar_nombre_pokemon);
	struct aux contenedor;
	contenedor.pokedex_7_pokemones = pokedex_con_7_pokemones;
	for (size_t i = 0; i < 7; i++)
	{
		contenedor.contador = 0;
		contenedor.rand = rand() % abb_cantidad(pokedex);
		abb_iterar_inorden(pokedex,pokemones_randoms_de_pokedex,&contenedor);
	}
	return pokedex_con_7_pokemones;
}

int main(int argc, char *argv[])
{
	srand((unsigned)time(NULL));
	jugador_t *jugador = jugador_crear();
	struct datos datos;
	datos.jugador = jugador;
	abb_t *pokedex = crear_pokedex(argv[1]);
	menu_t menu;
	abb_t *pokedex_con_7_pokemones = NULL;
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
				pokedex_con_7_pokemones = crear_pokemones_elegidos(pokedex);
				datos.pokedex = pokedex_con_7_pokemones;
				abb_iterar_inorden(pokedex_con_7_pokemones,posicion_inicial,NULL);
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
				pokedex_con_7_pokemones = crear_pokemones_elegidos(pokedex);
				datos.pokedex = pokedex_con_7_pokemones;
				abb_iterar_inorden(pokedex_con_7_pokemones,posicion_inicial,NULL);
				menu.iniciar_juego(logica,&datos);
				es_correcto = true;
				break;
			case 'Q':
				mostrar_cursor();
				abb_destruir_todo(pokedex,destructor);

				return 0;
			default:
				printf("Numero no valido ingresado, por favor ingrese 1 o 2\n");
				break;
		}
	}

	mostrar_cursor();
	jugador_destruir(jugador);
	abb_destruir_todo(pokedex,destructor);
	abb_destruir_todo(pokedex_con_7_pokemones,destructor);

	return 0;
}
