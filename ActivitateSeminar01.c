#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraCarte {
	int id;
	int nrPagini;
	float pret;
	char* titlu;
	char* autor;
};
typedef struct StructuraCarte Carte;
typedef struct Nod Nod;
typedef struct ListaDubla ListaDubla;

struct Nod {
	Carte info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* prim;
	Nod* ultim;
};

Carte citireCarteDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Carte c1;

	aux = strtok(buffer, sep);
	c1.id = atoi(aux);
	c1.nrPagini = atoi(strtok(NULL, sep));
	c1.pret = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c1.titlu = malloc(strlen(aux) + 1);
	strcpy_s(c1.titlu, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c1.autor = malloc(strlen(aux) + 1);
	strcpy_s(c1.autor, strlen(aux) + 1, aux);

	return c1;
}

void afisareCarte(Carte carte) {
	printf("Id: %d\n", carte.id);
	printf("Nr. pagini: %d\n", carte.nrPagini);
	printf("Pret: %.2f\n", carte.pret);
	printf("Titlu: %s\n", carte.titlu);
	printf("Autor: %s\n\n", carte.autor);
}

void afisareListaCarti(ListaDubla lista) {
	Nod* p = lista.prim;
	while (p) {
		afisareCarte(p->info);
		p = p->next;
	}
}

void afisareInversaListaCarti(ListaDubla lista) {
	Nod* p = lista.ultim;
	while (p) {
		afisareCarte(p->info);
		p = p->prev;
	}
}

void adaugaCarteInLista(ListaDubla* lista, Carte carteNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = carteNoua;
	nou->next = nou->prev = NULL;
	if (lista->ultim) {
		nou->prev = lista->ultim;
		lista->ultim->next = nou;
		lista->ultim = nou;
	}
	else {
		lista->ultim = lista->prim = nou;
	}
}

ListaDubla* citireLDCartiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	if (file) {
		ListaDubla* lista = malloc(sizeof(ListaDubla));
		lista->prim = NULL;
		lista->ultim = NULL;
		while (!feof(file)) {
			adaugaCarteInLista(lista, citireCarteDinFisier(file));
		}
		fclose(file);
		return lista;
	}
	else {
		ListaDubla* lista = malloc(sizeof(ListaDubla));
		lista->prim = NULL;
		lista->ultim = NULL;
		return lista;
	}
}


void dezalocareLDCarti(ListaDubla** lista) {
	Nod* p = (*lista)->prim;
	while (p) {
		free(p->info.titlu);
		free(p->info.autor);
		Nod* aux = p;
		p = p->next;
		free(aux);
	}
	free(*lista);
	*lista = NULL;
}


float calculeazaPretMediu(ListaDubla lista) {
	float suma = 0;
	int count = 0;
	Nod* p = lista.prim;
	while (p) {
		suma += p->info.pret;
		count++;
		p = p->next;
	}
	if (count > 0) {
		return suma / count;
	}
	else {
		return 0;
	}
}






int main() {

	ListaDubla* lista = citireLDCartiDinFisier("carti.txt");

	afisareListaCarti(*lista);

	printf("Pret mediu: %.2f\n", calculeazaPretMediu(*lista));

	dezalocareLDCarti(&lista);

	return 0;
}