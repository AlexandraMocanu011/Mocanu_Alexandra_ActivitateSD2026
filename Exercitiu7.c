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

Factura initFactura(const char* client, int id, float suma) {
	Factura f;
	f.id = id;
	f.suma = suma;
	f.client = (char*)malloc(sizeof(char) * (strlen(client) + 1));
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
	Factura f = (*varf)->info;
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
	else {
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

void afisareFacturiPesteSuma(Nod* coada, float prag) {
	while (coada != NULL) {
		if (coada->info.suma > prag) {
			afisareFactura(coada->info);
		}
		coada = coada->next;
	}
}

Factura cautaFacturaDupaId(Nod* coada, int id) {
	while (coada != NULL) {
		if (coada->info.id == id) {
			return coada->info;
		}
		coada = coada->next;
	}
	return initFactura("", 0, 0);
}

void stergeFacturaDupaId(Nod** coada, int id) {
	Nod* curent = *coada;
	Nod* anterior = NULL;

	while (curent != NULL) {
		if (curent->info.id == id) {
			if (anterior == NULL) {
				*coada = curent->next;
			}
			else {
				anterior->next = curent->next;
			}
			free(curent->info.client);
			free(curent);
			return;
		}
		anterior = curent;
		curent = curent->next;
	}
}

void sortareDupaSuma(Nod* coada) {
	for (Nod* i = coada; i != NULL; i = i->next) {
		for (Nod* j = i->next; j != NULL; j = j->next) {
			if (i->info.suma > j->info.suma) {
				Factura temp = i->info;
				i->info = j->info;
				j->info = temp;
			}
		}
	}
}

void salvareInFisier(Nod* coada, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "w");
	if (f == NULL) return;

	while (coada != NULL) {
		fprintf(f, "%s %d %.2f\n",
			coada->info.client,
			coada->info.id,
			coada->info.suma);
		coada = coada->next;
	}
	fclose(f);
}

void actualizeazaSumaDupaId(Nod* coada, int id, float procent) {
	while (coada != NULL) {
		if (coada->info.id == id) {
			coada->info.suma = coada->info.suma * (1 + procent / 100);
			return;
		}
		coada = coada->next;
	}
}

int main() {
	Nod* coada = NULL;

	put(&coada, initFactura("Popescu Ana", 1, 120.5));
	put(&coada, initFactura("Ionescu Mihai", 2, 89.99));
	put(&coada, initFactura("Georgescu Vlad", 3, 250.75));

	printf("\nSuma totala: %.2f", calculeazaSumaTotala(coada));

	printf("\nFacturi peste 100:");
	afisareFacturiPesteSuma(coada, 100);

	// cautare
	Factura gasita = cautaFacturaDupaId(coada, 2);
	printf("\nCautare ID 2:");
	if (gasita.id != 0) afisareFactura(gasita);

	// stergere
	stergeFacturaDupaId(&coada, 2);
	printf("\nDupa stergere ID 2:");
	afisareFacturiPesteSuma(coada, 0);

	// sortare
	sortareDupaSuma(coada);
	printf("\nDupa sortare:");
	afisareFacturiPesteSuma(coada, 0);

	// actualizare suma
	actualizeazaSumaDupaId(coada, 1, 10);
	printf("\nDupa majorare cu 10%% pentru ID 1:");
	afisareFacturiPesteSuma(coada, 0);

	// salvare
	salvareInFisier(coada, "facturi.txt");

	printf("\nTraversare finala:");
	Factura f;
	while (coada != NULL) {
		f = pop(&coada);
		afisareFactura(f);
		free(f.client);
	}

	return 0;
}