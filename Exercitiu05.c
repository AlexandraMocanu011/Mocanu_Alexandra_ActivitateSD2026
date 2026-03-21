#define _CRT_SECURE_NO_WARNINGS	
#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Angajat Angajat;

struct Angajat {
	int id;
	float salariu;
	int varsta;
	char* nume;
	char departament;
};

struct Angajat initializare(int id, float salariu, int varsta, const char* nume, char departament) {
	struct Angajat s;
	s.id = id;
	s.salariu = salariu;
	s.varsta = varsta;
	s.nume = (char*)malloc(sizeof(char) * (strlen(nume) + 1));
	strcpy(s.nume, nume);
	s.departament = departament;
	return s;
}

Angajat copiaza(Angajat a) {
	return initializare(a.id, a.salariu, a.varsta, a.nume, a.departament);
}

void afisare(struct Angajat s) {
	printf("%d. Angajatul %s din departamentul %c are salariul de %5.2f si varsta de %d.\n",
		s.id, s.nume, s.departament, s.salariu, s.varsta);
}

void afisareVector(struct Angajat* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(*(vector + i));
	}
}

struct Angajat* copiazaPrimeleNElemente(Angajat* vector, int nrElemente, int nrElementeCopiate) {
	if (nrElementeCopiate <= nrElemente) {
		struct Angajat* vectorNou = (struct Angajat*)malloc(sizeof(Angajat) * nrElementeCopiate);
		for (int i = 0; i < nrElementeCopiate; i++) {
			vectorNou[i] = copiaza(vector[i]);
		}
		return vectorNou;
	}
	else {
		return NULL;
	}
}

void dezalocare(struct Angajat** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++) {
		free((*vector)[i].nume);
	}
	free(*vector);
	*nrElemente = 0;
	*vector = NULL;
}

void copiazaAnumiteElemente(struct Angajat* vector, int nrElemente, float prag, struct Angajat** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].salariu < prag) {
			(*dimensiune)++;
		}
	}
	*vectorNou = (struct Angajat*)malloc(sizeof(Angajat) * (*dimensiune));
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].salariu < prag) {
			(*vectorNou)[*dimensiune] = copiaza(vector[i]);
			(*dimensiune)++;
		}
	}
}

struct Angajat getPrimulElementConditionat(struct Angajat* vector, int nrElemente, const char* numeCautat) {
	Angajat s;
	s.id = -1;
	s.salariu = 0;
	s.varsta = 0;
	s.nume = NULL;
	s.departament = '-';
	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(numeCautat, vector[i].nume) == 0) {
			s = copiaza(vector[i]);
			break;
		}
	}
	return s;
}

int main() {
	struct Angajat angajat = initializare(1, 4500.5, 30, "Andrei", 'A');
	afisare(angajat);

	int nrAngajati = 4;
	Angajat* vector = (Angajat*)malloc(sizeof(Angajat) * nrAngajati);
	vector[0] = initializare(2, 3200, 25, "Maria", 'B');
	vector[1] = initializare(3, 5600, 41, "Amazon", 'A');
	vector[2] = initializare(4, 4700, 36, "George", 'C');
	vector[3] = initializare(5, 3400, 29, "Emag", 'A');

	afisareVector(vector, nrAngajati);
	printf("\n\n");

	int nrCopiate = 2;
	Angajat* scurt = copiazaPrimeleNElemente(vector, nrAngajati, nrCopiate);

	afisareVector(scurt, nrCopiate);
	dezalocare(&scurt, &nrCopiate);

	Angajat* filtrat = NULL;
	int dimensiune = 0;
	copiazaAnumiteElemente(vector, nrAngajati, 4000, &filtrat, &dimensiune);
	printf("\nVector filtrat:\n");
	afisareVector(filtrat, dimensiune);
	dezalocare(&filtrat, &dimensiune);

	Angajat angajatCautat = getPrimulElementConditionat(vector, nrAngajati, "Amazon");
	printf("\nAngajat cautat:\n");
	if (angajatCautat.id != -1) {
		afisare(angajatCautat);
		free(angajatCautat.nume);
	}
	else {
		printf("Nu a fost gasit angajatul cautat.\n");
	}

	free(angajat.nume);
	dezalocare(&vector, &nrAngajati);
	return 0;
}