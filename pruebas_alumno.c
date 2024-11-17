#include "pa2m.h"
#include "src/pokemon.h"


void prueba_simple()
{
	pokemon_t *pokemon = pokemon_crear();
	pokemon_insertar_atributos(pokemon,"Pikachu",12,"AZUL","RNES");
	pa2m_afirmar(pokemon != NULL, "pokemon se creo correctamente");
	pa2m_afirmar(strcmp(pokemon_devolver_nombre(pokemon),"Pikachu") == 0, "el nombre es el correcto");
	pokemon_destruir(pokemon);
}

int main()
{
	pa2m_nuevo_grupo("============== ??? ===============");
	prueba_simple();

	return pa2m_mostrar_reporte();
}
