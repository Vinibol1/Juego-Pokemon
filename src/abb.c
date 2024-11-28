#include "abb.h"
#include "abb_estructura_privada.h"
#include "stdio.h"

#define Nodo_raiz 'N'
#define Nodo_izq 'I'
#define Nodo_der 'D'

abb_t *abb_crear(int (*comparador)(void *, void *))
{
	if (!comparador)
		return NULL;

	abb_t *abb = calloc(1, sizeof(abb_t));
	if (!abb)
		return NULL;

	abb->comparador = comparador;
	return abb;
}

size_t abb_cantidad(abb_t *abb)
{
	if (!abb)
		return 0;

	return abb->nodos;
}

/**
 * asigna en que posición insertar el nodo, la posición se pasa con un caracte
 'N' raiz 'I' hijo izquiero 'D' hijo derecho
 + retorna true si se pudo asignar
*/
bool asignar_nodo_abb(abb_t *abb, char nodo, void *elemento)
{
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (!nuevo_nodo)
		return false;

	nuevo_nodo->elemento = elemento;
	switch (nodo) {
	case Nodo_raiz:
		abb->raiz = nuevo_nodo;
		break;
	case Nodo_izq:
		abb->raiz->izq = nuevo_nodo;
		break;
	case Nodo_der:
		abb->raiz->der = nuevo_nodo;
		break;
	}
	return true;
}

bool abb_insertar(abb_t *abb, void *elemento)
{
	if (!abb)
		return false;
	abb->nodos += 1;
	if (abb->raiz == NULL)
		return asignar_nodo_abb(abb, Nodo_raiz, elemento);

	abb_t abb_temp = *abb;

	int comparador = abb->comparador(elemento, abb->raiz->elemento);
	if (comparador > 0) {
		if (!abb->raiz->der)
			return asignar_nodo_abb(abb, Nodo_der, elemento);

		abb_temp.raiz = abb->raiz->der;
		return abb_insertar(&abb_temp, elemento);

	} else if (comparador < 0) {
		if (!abb->raiz->izq)
			return asignar_nodo_abb(abb, Nodo_izq, elemento);

		abb_temp.raiz = abb->raiz->izq;
		return abb_insertar(&abb_temp, elemento);

	} else {
		nodo_t *nodo_aux = abb->raiz->izq;
		asignar_nodo_abb(abb, 'I', elemento);
		abb->raiz->izq->izq = nodo_aux;
		return true;
	}
}

nodo_t *abb_quitar_1_hijo(nodo_t *raiz)
{
	nodo_t *raiz_aux = raiz;
	raiz_aux = raiz->der;
	if (!raiz_aux)
		raiz_aux = raiz->izq;
	free(raiz);
	raiz = raiz_aux;
	return raiz;
}
/**
 *Quita el predecesor buscado del arbol. Si lo encuentra y encontrado no es
 *NULL, almacena el puntero.
 */
nodo_t *abb_quitar_predecesor(nodo_t *raiz, void **predecesor)
{
	if (!raiz->der) {
		if (!raiz->elemento) {
			return abb_quitar_1_hijo(raiz);
		}
		*predecesor = raiz->elemento;
		return abb_quitar_1_hijo(raiz);
	}

	raiz->der = abb_quitar_predecesor(raiz->der, predecesor);
	return raiz;
}

nodo_t *abb_quitar_funcion_aux(abb_t *abb, nodo_t *raiz, void *buscado,
			       void **encontrado)
{
	void *predecesor = NULL;
	int comparador = abb->comparador(buscado, raiz->elemento);
	if (comparador == 0) {
		abb->nodos -= 1;
		if (raiz->elemento && encontrado)
			*encontrado = raiz->elemento;
		if (raiz->izq && raiz->der) {
			raiz->izq =
				abb_quitar_predecesor(raiz->izq, &predecesor);
			raiz->elemento = predecesor;
			return raiz;

		} else
			return abb_quitar_1_hijo(raiz);
	}
	if (comparador < 0) {
		if (!raiz->izq)
			return raiz;
		raiz->izq = abb_quitar_funcion_aux(abb, raiz->izq, buscado,
						   encontrado);
	} else {
		if (!raiz->der)
			return raiz;
		raiz->der = abb_quitar_funcion_aux(abb, raiz->der, buscado,
						   encontrado);
	}
	return raiz;
}

bool abb_quitar(abb_t *abb, void *buscado, void **encontrado)
{
	if (!abb || !abb->raiz)
		return false;
	size_t cantidad_anterior = abb->nodos;
	abb->raiz = abb_quitar_funcion_aux(abb, abb->raiz, buscado, encontrado);
	if (cantidad_anterior == abb->nodos)
		return false;

	return true;
}

size_t abb_iterar_preorden_aux(abb_t *abb, bool (*f)(void *, void *), void *ctx,
			       bool *seguir_iterando)
{
	if (!abb || !f || !abb->raiz || *seguir_iterando == false)
		return 0;
	abb_t abb_temp = *abb;
	size_t contador = 0;
	*seguir_iterando = true;
	if (abb->raiz) {
		if (*seguir_iterando == false)
			return contador;
		*seguir_iterando = f(abb->raiz->elemento, ctx);
		++contador;
	}
	if (abb->raiz->izq) {
		abb_temp.raiz = abb->raiz->izq;
		contador += abb_iterar_preorden_aux(&abb_temp, f, ctx,
						    seguir_iterando);
	}
	if (abb->raiz->der) {
		abb_temp.raiz = abb->raiz->der;
		contador += abb_iterar_preorden_aux(&abb_temp, f, ctx,
						    seguir_iterando);
	}
	return contador;
}

size_t abb_iterar_preorden(abb_t *abb, bool (*f)(void *, void *), void *ctx)
{
	if (!abb || !f || !abb->raiz)
		return 0;
	bool iterar = true;
	return abb_iterar_preorden_aux(abb, f, ctx, &iterar);
}

size_t abb_iterar_inorden_aux(abb_t *abb, bool (*f)(void *, void *), void *ctx,
			      bool *seguir_iterando)
{
	if (!abb || !f || !abb->raiz || *seguir_iterando == false)
		return 0;
	abb_t abb_temp = *abb;
	size_t contador = 0;
	*seguir_iterando = true;
	if (abb->raiz->izq) {
		abb_temp.raiz = abb->raiz->izq;
		contador += abb_iterar_inorden_aux(&abb_temp, f, ctx,
						   seguir_iterando);
	}
	if (abb->raiz) {
		if (*seguir_iterando == false)
			return contador;
		*seguir_iterando = f(abb->raiz->elemento, ctx);
		++contador;
	}
	if (abb->raiz->der) {
		abb_temp.raiz = abb->raiz->der;
		contador += abb_iterar_inorden_aux(&abb_temp, f, ctx,
						   seguir_iterando);
	}
	return contador;
}

size_t abb_iterar_inorden(abb_t *abb, bool (*f)(void *, void *), void *ctx)
{
	if (!abb || !f || !abb->raiz)
		return 0;
	bool iterar = true;
	return abb_iterar_inorden_aux(abb, f, ctx, &iterar);
}

size_t abb_iterar_postorden_aux(abb_t *abb, bool (*f)(void *, void *),
				void *ctx, bool *seguir_iterando)
{
	if (!abb || !f || !abb->raiz || *seguir_iterando == false)
		return 0;
	abb_t abb_temp = *abb;
	size_t contador = 0;
	*seguir_iterando = true;
	if (abb->raiz->izq) {
		abb_temp.raiz = abb->raiz->izq;
		contador += abb_iterar_postorden_aux(&abb_temp, f, ctx,
						     seguir_iterando);
	}
	if (abb->raiz->der) {
		abb_temp.raiz = abb->raiz->der;
		contador += abb_iterar_postorden_aux(&abb_temp, f, ctx,
						     seguir_iterando);
	}
	if (abb->raiz) {
		if (*seguir_iterando == false)
			return contador;
		*seguir_iterando = f(abb->raiz->elemento, ctx);
		++contador;
	}
	return contador;
}

size_t abb_iterar_postorden(abb_t *abb, bool (*f)(void *, void *), void *ctx)
{
	if (!abb || !f || !abb->raiz)
		return 0;
	bool iterar = true;
	return abb_iterar_postorden_aux(abb, f, ctx, &iterar);
}

void *abb_obtener(abb_t *abb, void *elemento)
{
	if (!abb || !abb->raiz || !elemento)
		return NULL;
	abb_t abb_temp = *abb;
	int comparador = abb->comparador(elemento, abb->raiz->elemento);
	if (comparador == 0)
		return abb->raiz->elemento;

	if (abb->raiz->izq && comparador < 0) {
		abb_temp.raiz = abb->raiz->izq;
		return abb_obtener(&abb_temp, elemento);
	}
	if (abb->raiz->der && comparador > 0) {
		abb_temp.raiz = abb->raiz->der;
		return abb_obtener(&abb_temp, elemento);
	}
	return NULL;
}

/**
 * Recorre todo los nodos y los libera, si se le pasa una función destructora
 * libera tambíen los elementos de los nodos
 */
void liberar_nodos(abb_t *abb, void (*destructor)(void *))
{
	if (!abb || !abb->raiz)
		return;
	abb_t abb_temp;
	abb_temp.raiz = abb->raiz->izq;
	liberar_nodos(&abb_temp, destructor);
	abb_temp.raiz = abb->raiz->der;
	liberar_nodos(&abb_temp, destructor);
	if (destructor)
		destructor(abb->raiz->elemento);

	free(abb->raiz);
}

void abb_destruir(abb_t *abb)
{
	if (!abb)
		return;

	liberar_nodos(abb, NULL);
	free(abb);
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (!abb)
		return;
	liberar_nodos(abb, destructor);
	free(abb);
}

bool asignar_elemento_vector(void *elemento, void *vector)
{
	void ***vector_ = vector;
	**vector_ = elemento;
	(*vector_)++;
	return true;
}

size_t abb_vectorizar_inorden(abb_t *abb, void **vector, size_t tamaño)
{
	if (!abb || !vector || tamaño == 0)
		return 0;
	if (tamaño >= abb->nodos)
		return abb_iterar_inorden(abb, asignar_elemento_vector,
					  &vector);
	void **vector_aux = calloc(abb->nodos, sizeof(abb->raiz->elemento));
	void **vector_aux2 = vector_aux;
	abb_iterar_inorden(abb, asignar_elemento_vector, &vector_aux);
	for (size_t i = 0; i < tamaño; i++)
		vector[i] = vector_aux2[i];
	free(vector_aux2);
	return tamaño;
}

size_t abb_vectorizar_preorden(abb_t *abb, void **vector, size_t tamaño)
{
	if (!abb || !vector || tamaño == 0)
		return 0;
	if (tamaño >= abb->nodos)
		return abb_iterar_preorden(abb, asignar_elemento_vector,
					   &vector);
	void **vector_aux = calloc(abb->nodos, sizeof(abb->raiz->elemento));
	void **vector_aux2 = vector_aux;
	abb_iterar_preorden(abb, asignar_elemento_vector, &vector_aux);
	for (size_t i = 0; i < tamaño; i++)
		vector[i] = vector_aux2[i];
	free(vector_aux2);
	return tamaño;
}

size_t abb_vectorizar_postorden(abb_t *abb, void **vector, size_t tamaño)
{
	if (!abb || !vector || tamaño == 0)
		return 0;
	if (tamaño >= abb->nodos)
		return abb_iterar_postorden(abb, asignar_elemento_vector,
					    &vector);
	void **vector_aux = calloc(abb->nodos, sizeof(abb->raiz->elemento));
	void **vector_aux2 = vector_aux;

	abb_iterar_postorden(abb, asignar_elemento_vector, &vector_aux);
	for (size_t i = 0; i < tamaño; i++)
		vector[i] = vector_aux2[i];
	free(vector_aux2);
	return tamaño;
}