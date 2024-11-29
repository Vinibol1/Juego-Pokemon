#include "extra/engine.h"
#include "extra/ansi.h"
#include "src/abb.h"
#include "src/csv.h"
#include "src/jugador.h"
#include "src/pokemon.h"
#include <string.h>
#include <time.h>
#include <stdio.h>



#define ERROR -1
#define MAXIMOS_POKEMONES_EN_TERRENO 7
#define MAXIMO_TERRENO_X 15
#define MAXIMO_TERRENO_Y 32
#define COLUMNAS 4




struct aux {
	int rand;
	int contador;
	pokemon_t *nuevo_pokemon;
};


struct datos {
	char **actual_combo_pokemones;
        char **mejor_combo_pokemones;
	jugador_t *jugador;
	abb_t *pokedex;
	abb_t *pokedex_en_tablero;
};

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

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

bool devolver_pokemon_random(void *_pokemon, void* ctx){
	pokemon_t *pokemon = _pokemon;
	struct aux *contenedor = ctx;
	if (contenedor->contador != contenedor->rand)
	{
		contenedor->contador++;
		return true;
	}
	pokemon_t *nuevo_pokemon = pokemon_crear();
	pokemon_insertar_atributos(nuevo_pokemon,pokemon_devolver_nombre(pokemon),pokemon_devolver_puntos(pokemon),pokemon_devolver_color(pokemon),pokemon_devolver_movimiento(pokemon));
	contenedor->nuevo_pokemon = nuevo_pokemon;
	return false;
}


bool atrapar_pokemon(void * _pokemon, void *_datos){
	struct datos *datos = _datos; 
	pokemon_t *pokemon = _pokemon;
	jugador_t *jugador = datos->jugador;
	if (pokemon_devolver_posicion_x(pokemon) == jugador_devolver_posicion_x(jugador) && pokemon_devolver_posicion_y(pokemon) == jugador_devolver_posicion_y(jugador))
	{
		jugador_insertar_puntaje(jugador,pokemon_devolver_puntos(pokemon)*jugador_devolver_multiplicador(jugador));
		
		if (jugador_devolver_ultimo_color(jugador) != NULL && jugador_devolver_ultimo_color(jugador) != NULL && (jugador_devolver_ultima_inicial(jugador) == pokemon_devolver_inicial(pokemon) || strcmp(jugador_devolver_ultimo_color(jugador) ,pokemon_devolver_color(pokemon)) == 0))
			jugador_insertar_multiplicador(jugador,jugador_devolver_multiplicador(jugador) + 1);
			
		else{
			jugador_insertar_multiplicador(jugador,1);
		}
		jugador_insertar_ultimo_color(jugador,pokemon_devolver_color(pokemon));
		jugador_insertar_ultima_inicial(jugador,pokemon_devolver_inicial(pokemon));
		jugador_insertar_ultimo_pokemon(jugador,pokemon_devolver_nombre(pokemon));
		jugador_insertar_pokemon_combo(jugador,pokemon_devolver_nombre(pokemon));
		
		
		struct aux contenedor;
		contenedor.contador = 0;
		contenedor.rand = rand() % (int)abb_cantidad(datos->pokedex);
		abb_iterar_inorden(datos->pokedex,devolver_pokemon_random,&contenedor);
		pokemon_destruir_atributos(pokemon);
		pokemon_insertar_atributos(pokemon,pokemon_devolver_nombre(contenedor.nuevo_pokemon),pokemon_devolver_puntos(contenedor.nuevo_pokemon),pokemon_devolver_color(contenedor.nuevo_pokemon),pokemon_devolver_movimiento(contenedor.nuevo_pokemon));
		pokemon_destruir(contenedor.nuevo_pokemon);
		posicion_inicial(pokemon,NULL);

	}
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

struct posicion_pokemon{
	int x;
	int y;
	char *ansi;
};

bool hallar_color(void *_pokemon, void *ctx){
	pokemon_t *pokemon = _pokemon;
	struct posicion_pokemon *posicion = ctx;
	if (pokemon_devolver_posicion_x(pokemon) == posicion->x && pokemon_devolver_posicion_y(pokemon) == posicion->y)
		posicion->ansi = pokemon_devolver_ansi(pokemon);
	return true;
}


void imprimir_terreno(void *_datos){
	char terreno[MAXIMO_TERRENO_Y][MAXIMO_TERRENO_X];
	inicializar_terreno(terreno);
	struct datos *datos = _datos;
	struct posicion_pokemon posicion_pokemon;
	abb_t *pokedex = datos->pokedex_en_tablero;
	jugador_t *jugador = datos->jugador;
	
	abb_iterar_inorden(pokedex,posicionar_pokemon_en_terreno,&terreno);
	
	terreno[jugador_devolver_posicion_y(jugador)][jugador_devolver_posicion_x(jugador)] = 'J';
	for (int i = 0; i < MAXIMO_TERRENO_Y; i++)
	{
		for (int j = 0; j < MAXIMO_TERRENO_X; j++){
			posicion_pokemon.x = j;
			posicion_pokemon.y = i;
			posicion_pokemon.ansi = ANSI_COLOR_WHITE;
			abb_iterar_inorden(pokedex,hallar_color,&posicion_pokemon);
			printf("| " "%s" ANSI_COLOR_BOLD " %c " ANSI_COLOR_RESET, posicion_pokemon.ansi , terreno[i][j]);
		}
		printf("|\n");
	}
}


void finalizar_juego(struct datos *datos){
	jugador_t *jugador = datos->jugador;
	printf("El juego a finalizado!!, tu puntaje fue: %i tu modificador maximo fue: %i\n",jugador_devolver_puntaje(jugador), jugador_devolver_multiplicador_maximo(jugador));
	printf("y tu combo fue: ");
	char **combo = jugador_devolver_pokemon_combo(jugador);
	while(*combo != NULL)
	{
		printf("%s ",*combo);
		combo++; 
	}

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



int comparar_nombre_pokemon(void *_p1, void *_p2)
{
	pokemon_t *p1 = _p1;
	pokemon_t *p2 = _p2;
	return strcmp((char *)pokemon_devolver_nombre(p1), (char *)pokemon_devolver_nombre(p2));
}


abb_t *crear_pokemones_elegidos(struct datos *datos){
	abb_t *pokedex_en_juego = abb_crear(comparar_nombre_pokemon);
	struct aux contenedor;
	for (size_t i = 0; i < MAXIMOS_POKEMONES_EN_TERRENO; i++)
	{
		contenedor.contador = 0;
		contenedor.rand = rand() % (int)abb_cantidad(datos->pokedex);
		abb_iterar_inorden(datos->pokedex,devolver_pokemon_random,&contenedor);
		abb_insertar(pokedex_en_juego,contenedor.nuevo_pokemon);
	}
	

	return pokedex_en_juego;
}



void destructor(void *_pokemon)
{
	pokemon_t * pokemon = _pokemon;
	pokemon_destruir(pokemon);
}



int logica(int entrada, void *_datos)
{
	struct datos *datos = _datos;
	abb_t *pokedex = datos->pokedex_en_tablero;
	jugador_t *jugador = datos->jugador;
	
	borrar_pantalla();
	
	if (entrada == TECLA_DERECHA){
		jugador_aumentar_en_1_posicion_x(jugador);
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

	abb_iterar_inorden(pokedex,atrapar_pokemon,datos);

	printf("Utilizar " ANSI_COLOR_CYAN ANSI_COLOR_BOLD
	       "⬆⬇⬅➡" ANSI_COLOR_RESET " para moverse\n");

	printf("Presionar " ANSI_COLOR_RED ANSI_COLOR_BOLD "Q" ANSI_COLOR_RESET
	       " para salir\n");
	
	printf("Iteraciones: %d Tiempo: %d Multiplicador: %i \npuntaje: %i   ultimo pokemon capturado: %s   color ultimo pokemon: %s\n", jugador_devolver_iteraciones(jugador),
	       jugador_devolver_iteraciones(jugador) / 5, jugador_devolver_multiplicador(jugador), jugador_devolver_puntaje(jugador), jugador_devolver_ultimo_pokemon(jugador), jugador_devolver_ultimo_color(jugador));

	imprimir_terreno(datos);

	printf("\n");
	esconder_cursor();
	if (entrada == 'q' || entrada == 'Q' || (jugador_devolver_iteraciones(jugador) / 5) == 60)
	{
		finalizar_juego(datos);
		return 'q';
	}
	return 0;
}



void iniciar_juego(struct datos* datos){
	abb_t *pokedex_en_tablero = crear_pokemones_elegidos(datos);
	datos->pokedex_en_tablero = pokedex_en_tablero;
	abb_iterar_inorden(pokedex_en_tablero,posicion_inicial,NULL);
	game_loop(logica,datos);
	abb_destruir_todo(pokedex_en_tablero,destructor);
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

char transformar_caracter_en_mayuscula(char caracter){
	switch (caracter)
		{
		case 'p':
			return 'P';
		case 'q':
			return 'Q';
		case 's':
			return 'S';
		case 'j':
			return 'J';
		}
	return caracter;
}

void menu(struct datos* datos){
	bool es_correcto = false;
	while (!es_correcto) {
		int semilla;
		printf("Escribe el numero de la opción que quieras\n- P: Muestra el pokedex cargado. \n- J: Jugar. \n- S: Jugar con semilla. \n- Q: Salir. ");
		char caracter = (char)getchar();
		caracter = transformar_caracter_en_mayuscula(caracter);
		switch (caracter) {
			case 'P':
				mostrar_pokedex(datos->pokedex);
				es_correcto = true;
				break;
			case 'J':
				srand((unsigned)time(NULL));
				iniciar_juego(datos);
				es_correcto = true;
				break;
			case 'S':
				printf("ingresa el numero de la semilla: ");
				if (!scanf("%i", &semilla)) {
					printf("No se pudo leer el numero correctamente");
					break;
				}
				srand((unsigned int)semilla);
				iniciar_juego(datos);
				es_correcto = true;
				break;
			case 'Q':
				es_correcto = true;
			default:
				printf("Letra no valida ingresada, por favor ingrese P, J, S o Q\n");
				break;
		}
	}
}

int main(int argc, char *argv[])
{
	jugador_t *jugador = jugador_crear();
	struct datos datos;
	datos.jugador = jugador;
	abb_t *pokedex = crear_pokedex(argv[1]);
	datos.pokedex = pokedex;
	
	menu(&datos);

	mostrar_cursor();
	jugador_destruir(jugador);
	abb_destruir_todo(pokedex,destructor);

	return 0;
}
