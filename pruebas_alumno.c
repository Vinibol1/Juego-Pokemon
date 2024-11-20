#include "pa2m.h"
#include "src/pokemon.h"


void prueba_simple()
{
	pokemon_t *pokemon = pokemon_crear();
	pokemon_insertar_atributos(pokemon,"Pikachu",12,"AZUL","RNES");
	pa2m_afirmar(pokemon != NULL, "pokemon se creo correctamente");
	pa2m_afirmar(strcmp(pokemon_devolver_nombre(pokemon),"Pikachu") == 0, "el nombre es el correcto");
	pa2m_afirmar(strcmp(pokemon_devolver_movimiento(pokemon),"RNES") == 0, "el movimiento es el correcto");
	pa2m_afirmar(strcmp(pokemon_devolver_color(pokemon),"AZUL") == 0, "el color es el correcto");
	pa2m_afirmar(pokemon_devolver_puntos(pokemon) == 12, "el puntaje es el correcto");
	pokemon_destruir(pokemon);
}

int main()
{
	pa2m_nuevo_grupo("Pokemon TDA");
	prueba_simple();

	return pa2m_mostrar_reporte();
}
