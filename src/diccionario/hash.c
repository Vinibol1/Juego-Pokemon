#include "hash.h"
#include "abb.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#define CAPACIDAD_MINIMA 3
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

typedef struct pares {
	char *clave;
	void *valor;
} pares_t;

struct hash {
	abb_t **tabla;
	size_t capacidad;
	size_t cantidad;
};

// creditos
// https://en.wikipedia.org/wiki/Fowler–Noll–Vo_hash_function
static uint64_t hash_key(const char *key)
{
	uint64_t hash = FNV_OFFSET;
	for (const char *p = key; *p; p++) {
		hash ^= (uint64_t)(unsigned char)(*p);
		hash *= FNV_PRIME;
	}
	return hash;
}

static int comparador(void *elemento_, void *elemento_agregado_)
{
	pares_t *elemento = elemento_;
	pares_t *elemento_agregado = elemento_agregado_;
	return strcmp(elemento->clave, elemento_agregado->clave);
}

hash_t *hash_crear(size_t capacidad_inicial)
{
	if (capacidad_inicial < CAPACIDAD_MINIMA)
		capacidad_inicial = CAPACIDAD_MINIMA;

	hash_t *hash = calloc(1, sizeof(hash_t));
	if (!hash)
		return NULL;

	hash->capacidad = capacidad_inicial;
	hash->cantidad = 0;
	hash->tabla = malloc(capacidad_inicial * sizeof(void *));
	if (!hash->tabla) {
		free(hash);
		return NULL;
	}

	for (size_t i = 0; i < capacidad_inicial; i++)
		hash->tabla[i] = abb_crear(comparador);

	return hash;
}

size_t hash_cantidad(hash_t *hash)
{
	if (!hash)
		return 0;
	return hash->cantidad;
}

static double hash_factor_de_carga(hash_t *hash)
{
	return (double)hash->cantidad / (double)hash->capacidad;
}

void *hash_buscar(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return NULL;
	size_t indice = hash_key((const char *)clave) % hash->capacidad;
	pares_t pares;
	pares.clave = clave;
	pares_t *pares2 = (pares_t *)abb_obtener(hash->tabla[indice], &pares);
	if (pares2)
		return pares2->valor;
	return NULL;
}

bool hash_contiene(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return false;
	size_t indice = hash_key((const char *)clave) % hash->capacidad;
	pares_t pares;
	pares.clave = clave;
	if (abb_obtener(hash->tabla[indice], &pares) != NULL)
		return true;
	return false;
}

static bool reposisionar_pares_rehash(void *pares, void *ctx_)
{
	size_t indice = hash_key((const char *)(((pares_t *)pares))->clave) %
			((hash_t *)ctx_)->capacidad;

	bool inserto = abb_insertar(((hash_t *)ctx_)->tabla[indice], pares);
	return inserto;
}

static void rehash(hash_t *hash)
{
	hash_t *nuevo_hash = hash_crear(hash->capacidad * 2);

	for (size_t i = 0; i < hash->capacidad; i++) {
		abb_iterar_inorden(hash->tabla[i], reposisionar_pares_rehash,
				   nuevo_hash);
		abb_destruir(hash->tabla[i]);
	}
	free(hash->tabla);
	hash->tabla = nuevo_hash->tabla;
	hash->capacidad = nuevo_hash->capacidad;
	free(nuevo_hash);
}

bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
{
	if (!hash || !clave)
		return false;
	if (encontrado)
		*encontrado = NULL;
	if (hash_factor_de_carga(hash) > 0.70)
		rehash(hash);
	pares_t *pares = NULL;
	pares_t pares2;
	size_t indice = hash_key((const char *)clave) % hash->capacidad;
	if (hash_contiene(hash, clave)) {
		pares2.clave = clave;
		pares2.valor = valor;

		pares = abb_obtener(hash->tabla[indice], &pares2);
		if (encontrado)
			*encontrado = pares->valor;
		pares->valor = valor;
		return true;
	}

	char *copia_clave = malloc((strlen(clave) + 1) * sizeof(char));
	if (!copia_clave)
		return false;

	strcpy(copia_clave, clave);
	pares = malloc(sizeof(pares_t));
	if (!pares) {
		free(copia_clave);
		return false;
	}
	hash->cantidad += 1;
	pares->clave = copia_clave;
	pares->valor = valor;
	bool inserto = abb_insertar(hash->tabla[indice], pares);
	return inserto;
}

void *hash_quitar(hash_t *hash, char *clave)
{
	if (!hash || !clave)
		return false;
	size_t indice = hash_key((const char *)clave) % hash->capacidad;
	void *encontrado = NULL;
	pares_t pares;
	pares.clave = clave;
	if (!abb_quitar(hash->tabla[indice], &pares, &encontrado)) {
		return NULL;
	}
	hash->cantidad -= 1;
	pares = *(pares_t *)encontrado;
	free(pares.clave);
	free(encontrado);
	return pares.valor;
}

struct iterador {
	bool (*f)(char *, void *, void *);
	void *ctx;
	bool seguir_iteracion;
};

static bool aplicar_funcion_elementos(void *elemento, void *ctx)
{
	pares_t *pares_elem = elemento;
	struct iterador *iterador = ctx;
	iterador->seguir_iteracion = iterador->f(
		pares_elem->clave, pares_elem->valor, iterador->ctx);
	return iterador->seguir_iteracion;
}

size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx)
{
	if (!hash || !f)
		return 0;
	struct iterador iterador;
	iterador.f = f;
	iterador.ctx = ctx;
	iterador.seguir_iteracion = true;
	size_t cantidad = 0;
	int i = 0;
	while (iterador.seguir_iteracion && i < hash->capacidad) {
		cantidad += abb_iterar_inorden(
			hash->tabla[i], aplicar_funcion_elementos, &iterador);
		i++;
	}
	return cantidad;
}

struct destructor {
	void (*destructor)(void *);
};

static bool destruir_claves(void *pares_, void *ctx)
{
	pares_t *pares = pares_;
	free(pares->clave);
	struct destructor *destructor = ctx;
	if (destructor && destructor->destructor)
		destructor->destructor(pares->valor);
	return true;
}

static void destruir_pares(void *pares_)
{
	pares_t *pares = pares_;
	free(pares);
}

void hash_destruir(hash_t *hash)
{
	if (!hash)
		return;

	for (size_t i = 0; i < hash->capacidad; i++) {
		abb_iterar_inorden(hash->tabla[i], destruir_claves, NULL);
		abb_destruir_todo(hash->tabla[i], destruir_pares);
	}
	free(hash->tabla);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (!hash)
		return;

	struct destructor destructor2;
	destructor2.destructor = destructor;
	for (size_t i = 0; i < hash->capacidad; i++) {
		abb_iterar_inorden(hash->tabla[i], destruir_claves,
				   &destructor2);
		abb_destruir_todo(hash->tabla[i], destruir_pares);
	}
	free(hash->tabla);
	free(hash);
}