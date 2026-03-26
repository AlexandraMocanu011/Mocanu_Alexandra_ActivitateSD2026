#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraClient {
	int id;
	float sold;
	int varsta;
	char* numeClient;
	char tipCont;
};
typedef struct StructuraClient Client;


struct Nod {
	Client info;
	struct Nod* next;
};
typedef struct Nod Nod;


struct HashTable {
	int dim;
	Nod** vector;
};
typedef struct HashTable HashTable;

Client citireClientDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;

	Client c1;
	c1.id = -1;
	c1.sold = 0;
	c1.varsta = 0;
	c1.numeClient = NULL;
	c1.tipCont = '-';

	if (fgets(buffer, 100, file) != NULL) {
		aux = strtok(buffer, sep);
		c1.id = atoi(aux);

		c1.sold = atof(strtok(NULL, sep));
		c1.varsta = atoi(strtok(NULL, sep));

		aux = strtok(NULL, sep);
		c1.numeClient = (char*)malloc(strlen(aux) + 1);
		strcpy_s(c1.numeClient, strlen(aux) + 1, aux);

		c1.tipCont = *strtok(NULL, sep);
	}

	return c1;
}

void afisareClient(Client client) {
	printf("Id: %d\n", client.id);
	printf("Sold: %.2f\n", client.sold);
	printf("Varsta: %d\n", client.varsta);
	printf("Nume client: %s\n", client.numeClient);
	printf("Tip cont: %c\n\n", client.tipCont);
}

void afisareListaClienti(Nod* cap) {
	while (cap) {
		afisareClient(cap->info);
		cap = cap->next;
	}
}

void adaugaClientInLista(Nod** cap, Client clientNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = clientNou;
	nou->next = NULL;

	if (*cap == NULL) {
		*cap = nou;
	}
	else {
		Nod* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);

	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int id, int dimensiune) {
	return id % dimensiune;
}

void inserareClientInTabela(HashTable ht, Client client) {
	int pozitie = calculeazaHash(client.id, ht.dim);
	adaugaClientInLista(&(ht.vector[pozitie]), client);
}

HashTable citireClientiDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(5);

	if (file) {
		while (!feof(file)) {
			Client c = citireClientDinFisier(file);
			if (c.id != -1) {
				inserareClientInTabela(ht, c);
			}
		}
		fclose(file);
	}
	return ht;
}

void afisareTabelaDeClienti(HashTable ht) {
	for (int i = 0; i < ht.dim; i++) {
		printf("\nCluster %d:\n", i);
		afisareListaClienti(ht.vector[i]);
	}
}

void dezalocareTabelaDeClienti(HashTable* ht) {
	for (int i = 0; i < ht->dim; i++) {
		Nod* p = ht->vector[i];
		while (p) {
			free(p->info.numeClient);
			Nod* aux = p;
			p = p->next;
			free(aux);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
}

float* calculeazaSolduriMediiPerClustere(HashTable ht, int* nrClustere) {
	float* vector = (float*)malloc(sizeof(float) * ht.dim);
	*nrClustere = 0;

	for (int i = 0; i < ht.dim; i++) {
		float suma = 0;
		int count = 0;

		Nod* p = ht.vector[i];
		while (p) {
			suma += p->info.sold;
			count++;
			p = p->next;
		}

		if (count > 0) {
			vector[*nrClustere] = suma / count;
			(*nrClustere)++;
		}
	}

	return vector;
}

Client getClientDupaId(HashTable ht, int idCautat) {
	Client c;
	c.id = -1;
	c.sold = 0;
	c.varsta = 0;
	c.numeClient = NULL;
	c.tipCont = '-';

	int pozitie = calculeazaHash(idCautat, ht.dim);
	Nod* p = ht.vector[pozitie];

	while (p) {
		if (p->info.id == idCautat) {
			return p->info;
		}
		p = p->next;
	}
	return c;
}

int main() {

	HashTable ht = citireClientiDinFisier("clienti.txt");

	afisareTabelaDeClienti(ht);

	int nr;
	float* medii = calculeazaSolduriMediiPerClustere(ht, &nr);

	printf("\nSolduri medii:\n");
	for (int i = 0; i < nr; i++) {
		printf("%.2f\n", medii[i]);
	}

	Client c = getClientDupaId(ht, 3);
	if (c.id != -1) {
		printf("\nClient gasit:\n");
		afisareClient(c);
	}

	free(medii);
	dezalocareTabelaDeClienti(&ht);

	return 0;
}