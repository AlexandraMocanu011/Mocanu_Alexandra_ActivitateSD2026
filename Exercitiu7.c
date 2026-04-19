#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Factura Factura;
typedef struct Nod Nod;

struct Factura
{
	char* client;
	int id;
	float suma;
};

struct Nod {
	Factura info;
	Nod* next;
};

Factura initFactura(const char* client, int id,
	float suma) {
	Factura f;
	f.id = id;
	f.suma = suma;
	f.client = (char*)malloc(sizeof(char)
		* (strlen(client) + 1));
	strcpy(f.client, client);

	return f;
}

void afisareFactura(Factura f) {
	printf("\nFactura clientului %s are id-ul %d, iar suma este %.2f!",
		f.client, f.id, f.suma);
}

void push(Nod** varf, Factura f) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = *varf;
	nou->info = f;
	*varf = nou;
}

Factura pop(Nod** varf) {
	if ((*varf) == NULL) {
		return initFactura("", 0, 0);
	}
	Factura f;
	f = (*varf)->info;
	Nod* aux = *varf;
	*varf = (*varf)->next;
	free(aux);

	return f;
}

void put(Nod** coada, Factura f) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->next = NULL;
	nou->info = f;

	if (*coada == NULL) {
		*coada = nou;
	}
	else
	{
		Nod* aux = *coada;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = nou;
	}
}

float calculeazaSumaTotala(Nod* coada) {
	float sumaTotala = 0;
	while (coada != NULL) {
		sumaTotala += coada->info.suma;
		coada = coada->next;
	}
	return sumaTotala;
}

int main() {
	Nod* stiva = NULL;
	push(&stiva, initFactura("Popescu Ana", 1, 120.5));
	push(&stiva, initFactura("Ionescu Mihai", 2, 89.99));
	push(&stiva, initFactura("Georgescu Vlad", 3, 250.75));

	Factura f;
	printf("\nTraversare stiva:");
	while (stiva != NULL) {
		f = pop(&stiva);
		afisareFactura(f);
		free(f.client);
	}

	Nod* coada = NULL;
	put(&coada, initFactura("Popescu Ana", 1, 120.5));
	put(&coada, initFactura("Ionescu Mihai", 2, 89.99));
	put(&coada, initFactura("Georgescu Vlad", 3, 250.75));

	printf("\nSuma totala a facturilor este %.2f", calculeazaSumaTotala(coada));

	printf("\nTraversare coada:");
	while (coada != NULL) {
		f = pop(&coada);
		afisareFactura(f);
		free(f.client);
	}

	return 0;
}