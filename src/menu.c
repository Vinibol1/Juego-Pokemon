#include "menu.h"
#include "pokemon.h"

struct menu
{
        void (*pokedex);
        void (*iniciar_juego);
        void (*iniciar_juego_con_semilla);
        void (*salir_juego)
};
