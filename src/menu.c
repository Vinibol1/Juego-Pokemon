#include "menu.h"
#include "diccionario/hash.h"
#include "pokemon.h"

void mostrar_pokedex(hash_t *pokedex){
	if (!elemento)
		return false;
	struct pokemon *pokemon_actual = elemento;
	printf("Nombre:%s, Tipo:%c, Fuerza:%i, Destreza:%i, Resistencia:%i\n",
	       pokemon_actual->nombre, pokemon_actual->tipo,
	       pokemon_actual->fuerza, pokemon_actual->destreza,
	       pokemon_actual->resistencia);
	return true;
}
}

struct menu
{
        void (*pokedex);
        void (*iniciar_juego);
        void (*iniciar_juego_con_semilla);
        void (*salir_juego)
};
