#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



struct StructuraCalculator {
	int id;
	int memorieRAM;

	float pret;

	char* model;
	char* producator;


	unsigned char generatie;

};
typedef struct StructuraCalculator Calculator;

void afisareCalculator(Calculator calculator) {
	printf("Id: %d\n", calculator.id);
	printf("Memorie RAM: %d\n", calculator.memorieRAM);
	printf("Pret: %.2f\n", calculator.pret);
	printf("Model: %s\n", calculator.model);
	printf("Producator: %s\n", calculator.producator);
	printf("Generatie: %c\n\n", calculator.generatie);
}



void afisareVectorCalculatoare(Calculator* calculatoare, int nrCalculatoare) {
	for (int i = 0; i < nrCalculatoare; i++) {
		afisareCalculator(calculatoare[i]);
	}
}

void adaugaCalculatorInVector(Calculator** calculatoare, int* nrCalculatoare, Calculator calculatorNou) {
	Calculator* aux = (Calculator*)malloc(sizeof(Calculator) * ((*nrCalculatoare) + 1));

	for (int i = 0; i < *nrCalculatoare; i++) {
		aux[i] = (*calculatoare)[i];
	}


	aux[*nrCalculatoare] = calculatorNou;

	free(*calculatoare);
	*calculatoare = aux;
	(*nrCalculatoare)++;
}



Calculator citireCalculatorFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Calculator c1;

	aux = strtok(buffer, sep);
	c1.id = atoi(aux);
	c1.memorieRAM = atoi(strtok(NULL, sep));

	c1.pret = (float)atof(strtok(NULL, sep));


	aux = strtok(NULL, sep);
	c1.model = (char*)malloc(strlen(aux) + 1);
	strcpy(c1.model, aux);

	aux = strtok(NULL, sep);
	c1.producator = (char*)malloc(strlen(aux) + 1);
	strcpy(c1.producator, aux);

	c1.generatie = *strtok(NULL, sep);

	return c1;

}

Calculator* citireVectorCalculatoareFisier(const char* numeFisier, int* nrCalculatoareCitite) {
	FILE* file = fopen(numeFisier, "r");
	Calculator* calculatoare = NULL;

	*nrCalculatoareCitite = 0;

	if (file) {
		while (!feof(file)) {

			adaugaCalculatorInVector(&calculatoare, nrCalculatoareCitite, citireCalculatorFisier(file));

		}
		fclose(file);
	}

	return calculatoare;
}

void dezalocareVectorCalculatoare(Calculator** vector, int* nrCalculatoare) {
	for (int i = 0; i < *nrCalculatoare; i++) {
		free((*vector)[i].model);
		free((*vector)[i].producator);
	}

	free(*vector);
	*vector = NULL;
	*nrCalculatoare = 0;

}

int main() {

	int nrCalculatoare = 0;

	Calculator* calculatoare = citireVectorCalculatoareFisier("calculatoare.txt", &nrCalculatoare);

	printf("Vectorul de calculatoare citit din fisier:\n\n");
	afisareVectorCalculatoare(calculatoare, nrCalculatoare);


	dezalocareVectorCalculatoare(&calculatoare, &nrCalculatoare);

	return 0;
}
// calc
