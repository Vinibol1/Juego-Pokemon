#include "csv.h"
#include "split.h"
#include <stdio.h>
#include <string.h>

struct archivo_csv {
	FILE *abierto;
	char separador;
};

struct archivo_csv *abrir_archivo_csv(const char *nombre_archivo,
				      char separador)
{
	if (nombre_archivo == NULL)
		return NULL;

	struct archivo_csv *archivo = malloc(sizeof(struct archivo_csv));
	if (archivo == NULL) {
		free(archivo);
		return NULL;
	}
	archivo->abierto = fopen(nombre_archivo, "r");
	if (archivo->abierto == NULL) {
		free(archivo);
		return NULL;
	}
	archivo->separador = separador;
	return archivo;
}
/**
 * la funcion necesita que haya la misma cantidad de funciones que
 * de columnas que se quieran leer
 * esta devuelve la cantidad de columnas leídas correctamente.
 */
size_t leer_linea_csv(struct archivo_csv *archivo, size_t columnas,
		      bool (*funciones[])(const char *, void *), void *ctx[])
{
	if (archivo == NULL || columnas == 0)
		return 0;
	struct Partes *archivo_divido = NULL;
	size_t cantidad_caracteres = 300;
	size_t tamaño = 0;
	char *texto = malloc(cantidad_caracteres);
	if (texto == NULL) {
		free(texto);
		return 0;
	}

	while (fgets(texto + tamaño, (int)(cantidad_caracteres - tamaño),
		     archivo->abierto) != NULL) {
		tamaño += strlen(texto + tamaño);

		if (texto[tamaño - 1] == '\n')
			break;

		cantidad_caracteres *= 2;
		char *nuevo_texto = realloc(texto, cantidad_caracteres);
		if (nuevo_texto == NULL) {
			free(nuevo_texto);
			free(texto);
			return 0;
		}
		texto = nuevo_texto;
	}

	if (feof(archivo->abierto)) {
		free(texto);
		return 0;
	}

	archivo_divido = dividir_string(texto, archivo->separador);
	if (!archivo_divido) {
		free(texto);
		return 0;
	}

	size_t contador = 0;
	for (size_t i = 0; i < columnas; i++)
		if (funciones[i] == NULL)
			i = columnas;
		else if (funciones[i](archivo_divido->string[i], ctx[i]))
			contador++;
	free(texto);
	liberar_partes(archivo_divido);
	return contador;
}

void cerrar_archivo_csv(struct archivo_csv *archivo)
{
	if (archivo == NULL)
		return;
	fclose(archivo->abierto);
	free(archivo);
}
