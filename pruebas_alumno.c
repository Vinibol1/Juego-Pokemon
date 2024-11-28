#include "pa2m.h"
#include "src/pokemon.h"
#include "string.h"
#include "src/jugador.h"


void prueba_pokemon_atributos()
{
	pokemon_t *pokemon = pokemon_crear();
	pokemon_insertar_atributos(pokemon,"Pikachu",12,"AMARILLO","RNES");
	pa2m_afirmar(pokemon != NULL, "pokemon se creo correctamente");
	pa2m_afirmar(strcmp(pokemon_devolver_nombre(pokemon),"Pikachu") == 0, "el nombre es el correcto");
	pa2m_afirmar(strcmp(pokemon_devolver_movimiento(pokemon),"RNES") == 0, "el movimiento es el correcto");
	pa2m_afirmar(strcmp(pokemon_devolver_color(pokemon),"AMARILLO") == 0, "el color es el correcto");
	pa2m_afirmar(pokemon_devolver_puntos(pokemon) == 12, "el puntaje es el correcto");
	pokemon_destruir(pokemon);
}

void prueba_jugador()
{
	pokemon_t *pokemon = pokemon_crear();
	pokemon_insertar_atributos(pokemon,"Pikachu",12, "AMARILLO","RNES");
	pokemon_t *pokemon2 = pokemon_crear();
	pokemon_insertar_atributos(pokemon2,"Pichu",15,"AMARILLO","RNES");
	pokemon_t *pokemon3 = pokemon_crear();
	pokemon_insertar_atributos(pokemon3,"Charmander",12,"ROJO","RNES");
	pokemon_t *pokemon4 = pokemon_crear();
	pokemon_insertar_atributos(pokemon4,"Charizard",20,"ROJO","RNES");
	jugador_t * jugador = jugador_crear();
	jugador_insertar_ultima_inicial(jugador,pokemon_devolver_inicial(pokemon));
	pa2m_afirmar(jugador_devolver_ultima_inicial(jugador) == 'P', "La primera inicial P es igual a %c ", jugador_devolver_ultima_inicial(jugador));
	pa2m_afirmar(jugador_devolver_multiplicador(jugador) == 1, "El multiplicador se inserto corretamente : 1 es igual a %i ", jugador_devolver_multiplicador(jugador));
	jugador_insertar_pokemon_combo(jugador,pokemon_devolver_nombre(pokemon));
	char **combo = jugador_devolver_pokemon_combo(jugador);
	pa2m_afirmar(strcmp(*combo,pokemon_devolver_nombre(pokemon)) == 0, "el primer pokemon del combo tiene que ser Pikachu y es %s ", *combo);
	jugador_insertar_multiplicador(jugador,jugador_devolver_multiplicador(jugador) + 1);
	jugador_insertar_ultima_inicial(jugador,pokemon_devolver_inicial(pokemon2));
	pa2m_afirmar(jugador_devolver_ultima_inicial(jugador) == 'P', "La primera inicial P es igual a %c ", jugador_devolver_ultima_inicial(jugador));
	pa2m_afirmar(jugador_devolver_multiplicador(jugador) == 2, "El multiplicador se inserto corretamente : 2 es igual a %i ", jugador_devolver_multiplicador(jugador));
	jugador_insertar_pokemon_combo(jugador,pokemon_devolver_nombre(pokemon2));

	combo = jugador_devolver_pokemon_combo(jugador);
	pa2m_afirmar(strcmp(*combo,pokemon_devolver_nombre(pokemon)) == 0, "el primer pokemon del combo tiene que ser pikachu y es %s ", *combo);
	combo++;
	pa2m_afirmar(strcmp(*combo,pokemon_devolver_nombre(pokemon2)) == 0, "el segundo pokemon del combo tiene que ser pichu y es %s ", *combo);


	jugador_insertar_multiplicador(jugador,1);
	jugador_insertar_ultima_inicial(jugador,pokemon_devolver_inicial(pokemon3));
	pa2m_afirmar(jugador_devolver_ultima_inicial(jugador) == 'C', "La primera inicial P es igual a %c ", jugador_devolver_ultima_inicial(jugador));
	pa2m_afirmar(jugador_devolver_multiplicador(jugador) == 1, "iserto un pokemon que reincia el multiplicador, el multiplicador se inserto corretamente : 1 es igual a %i ", jugador_devolver_multiplicador(jugador));
	jugador_insertar_pokemon_combo(jugador,pokemon_devolver_nombre(pokemon3));

	combo = jugador_devolver_pokemon_combo(jugador);
	pa2m_afirmar(strcmp(*combo,pokemon_devolver_nombre(pokemon)) == 0, " primer pokemon del combo tiene que ser pikachu y es %s ", *combo);
	combo++;
	pa2m_afirmar(strcmp(*combo,pokemon_devolver_nombre(pokemon2)) == 0, "el segundo pokemon del combo tiene que ser pichu y es %s ", *combo);
	combo++;
	pa2m_afirmar(*combo == NULL, "el tercer pokemon del combo tiene que ser NULL y es %s ", *combo);

	jugador_insertar_multiplicador(jugador,2);
	jugador_insertar_ultima_inicial(jugador,pokemon_devolver_inicial(pokemon4));
	pa2m_afirmar(jugador_devolver_ultima_inicial(jugador) == 'C', "La primera inicial P es igual a %c ", jugador_devolver_ultima_inicial(jugador));
	pa2m_afirmar(jugador_devolver_multiplicador(jugador) == 2, "iserto un pokemon que cambia al combo nuevo, multiplicador : 2 es igual a %i ", jugador_devolver_multiplicador(jugador));
	jugador_insertar_pokemon_combo(jugador,pokemon_devolver_nombre(pokemon4));

	combo = jugador_devolver_pokemon_combo(jugador);
	pa2m_afirmar(strcmp(*combo,pokemon_devolver_nombre(pokemon3)) == 0, " primer pokemon del combo tiene que ser charmander y es %s ", *combo);
	combo++;
	pa2m_afirmar(strcmp(*combo,pokemon_devolver_nombre(pokemon4)) == 0, "el segundo pokemon del combo tiene que ser charizard y es %s ", *combo);
	combo++;
	pa2m_afirmar(*combo == NULL, "el tercer	 pokemon del combo tiene que ser NULL y es %s ", *combo);

	jugador_destruir(jugador);
	pokemon_destruir(pokemon);
	pokemon_destruir(pokemon2);
	pokemon_destruir(pokemon3);
	pokemon_destruir(pokemon4);
}

int main()
{
	pa2m_nuevo_grupo("Pokemon TDA");
	prueba_pokemon_atributos();
	pa2m_nuevo_grupo("Jugador TDA");
	prueba_jugador();

	return pa2m_mostrar_reporte();
}
