#include "split.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * Devuelve un Struct Partes con la informacion de un string nulo
 */
struct Partes *string_nulo(struct Partes *partes)
{
	partes->cantidad = 0;
	partes->string = NULL;
	return partes;
}

/**
 * Devuelve un Struct Partes con la informacion de un string vacio
 */
struct Partes *string_vacio(struct Partes *partes, char **str_dividido)
{
	int cantidad = 0;
	str_dividido[0] = "";
	cantidad++;
	partes->cantidad = (size_t)cantidad;
	partes->string = str_dividido;
	return partes;
}
/**
 * Recibe un string y un separador y devuelve un struct con la informacion de los strings separados.
 */
struct Partes *dividir_string(const char *string, char separador)
{
	int contador = 0;
	int cantidad = 0;
	int cantidad_sep = 0;
	struct Partes *partes = (struct Partes *)malloc(sizeof(struct Partes));
	if (!partes) {
		free(partes);
		return NULL;
	}

	if (string == NULL)
		return string_nulo(partes);

	for (int i = 0; i <= strlen(string); i++)
		if (string[i] == separador || string[i] == 0)
			cantidad_sep++;

	char **str_dividido =
		(char **)malloc(sizeof(char *) * ((size_t)cantidad_sep));
	if (!str_dividido) {
		free(partes);
		free(str_dividido);
		return NULL;
	}

	if (string[0] == 0)
		return string_vacio(partes, str_dividido);

	for (int i = 0; i <= strlen(string); i++)
		if (string[i] == separador || string[i] == 0) {
			if (i == 0 || string[i - 1] == separador)
				str_dividido[i] = "";
			else {
				int j;
				str_dividido[cantidad] =
					malloc(sizeof(char) *
					       (size_t)(i - contador + 1));
				if (!str_dividido[cantidad]) {
					free(partes);
					free(str_dividido[cantidad]);
					free(str_dividido);
					return NULL;
				}

				for (j = 0; j < (i - contador); j++)
					str_dividido[cantidad][j] =
						string[j + contador];

				str_dividido[cantidad][j] = 0;
			}
			contador = i + 1;
			cantidad += 1;
		}
	partes->string = str_dividido;
	partes->cantidad = (size_t)cantidad;
	return partes;
}

/**
 * Libera TODA la memoria utilizada por el struct Partes.
 */
void liberar_partes(struct Partes *partes)
{
	if (partes->cantidad > 0 && partes->string[0][0] != 0)
		for (size_t i = 0; i < partes->cantidad; i++)
			free(partes->string[i]);

	free(partes->string);
	free(partes);
}
