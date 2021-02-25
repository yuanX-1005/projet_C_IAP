/*
	sprint 5.c
	YE fangyuan Lisa 111
	BASHITI Mountaser Billah 111
	25/10/19
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#pragma warning (disable:4996)
#pragma warning (disable:6031)

#define NbPatineur 3 // definie le nombre de patineur par équipe
#define NbEquipe 2 // definie le nombre d'équipe pqr épreuves
#define MaxTours 10 //definie le nombre maximum de tours
#define MaxEpreuves 16 // definie le nombre maximum d'épreuves
#define lgMot 30 // definie la taille maximale des chaînes de caractères

typedef struct {
	char nom[lgMot + 1];
	unsigned int dossard;
} Patineur;

typedef struct {
	Patineur data[NbPatineur];
	char pays[lgMot + 1];
} Equipe;

typedef struct {
	Equipe equipe[MaxEpreuves * NbEquipe];
	unsigned int ins;
} Equipes;

typedef struct {
	double temps;
	unsigned char nbtour;
	unsigned char dossard;
} Mesure;

typedef struct {
	Mesure data[MaxEpreuves * NbEquipe * NbPatineur * MaxTours];
	int eng;
} Course;

typedef struct {
	int tours;
} Tours;

typedef struct {
	int epreuves;
} Epreuves;

typedef struct {
	double temps[MaxEpreuves * NbEquipe];
	int tempseq;
} Temps;

//Pour résumer toutes mes fonctions
void inscrire_equipe(Equipes* e);
void afficher_equipes(const Equipes* e, int* dernierd);
void enregistrer_temps(Course* crs, int* dernierd);
void afficher_temps(const Course* crs, const Equipes* e);
void afficher_temps_equipes(const Course* crs, const Equipes* e);
void definition_parcours(Tours* t);
void detection_fin_poursuite(Course* crs, Equipes* e, Tours* t, Temps* tp, int* nb);
void definir_nombre_epreuves(Epreuves* ep);
void detection_fin_competition(Equipes* e, Course* crs, Tours* t, Temps* tp);

/*inscrire les 3 patineurs de chaque equipe et les numéros de dossard
commencent à 101 seront automatiquement attribués par programme dans
l’ordre séquentiel d’inscription.*/
void inscrire_equipe(Equipes* e) {
	static unsigned int compt1 = 101;
	scanf("%s", e->equipe[e->ins].pays);
	for (int j = 0; j < NbPatineur; ++j) {
		scanf("%s", e->equipe[e->ins].data[j].nom);
		e->equipe[e->ins].data[j].dossard = compt1;
		printf("inscription dossard %d\n", compt1);
		++compt1;
	}
	e->ins = e->ins + 1;
}

/* afficher les pays,les noms de membres de l’équipe qui en train de jouer
et de leurs numéros respectifs de dossard.*/
void afficher_equipes(const Equipes* e, int* dernierd) {
	int eqencours;
	if (((*dernierd - 101) / 3) % 2 == 0) {
		eqencours = (*dernierd - 101) / 3;
		for (int i = 0; i < 2; ++i) {
			printf("%s ", e->equipe[i + eqencours].pays);
			for (int j = 0; j < NbPatineur; ++j) {
				if (j != 0)
					printf(" ");
				printf("%s %d", e->equipe[eqencours + i].data[j].nom,
					e->equipe[eqencours + i].data[j].dossard);
			}
			printf("\n");
		}
	}
	else {
		eqencours = ((*dernierd - 101) / 3) - 1;
		for (int i = 0; i < 2; ++i) {
			printf("%s ", e->equipe[i + eqencours].pays);
			for (int j = 0; j < NbPatineur; ++j) {
				if (j != 0)
					printf(" ");
				printf("%s %d", e->equipe[eqencours + i].data[j].nom,
					e->equipe[eqencours + i].data[j].dossard);
			}
			printf("\n");
		}

	}
}

/*cette fonction permet de memoriser les temps chronomètre de patineur
lors on entre un nombre de tour et le numero de dossard */
void enregistrer_temps(Course* crs, int* Dernierd) {
	char mot[lgMot + 1];
	scanf("%s", mot);
	crs->data[crs->eng].dossard = atof(mot);
	//pour retourner réel à la correspondant à la chaine de mot
	*Dernierd = atof(mot);
	// Il permet de distinguer les equipes qui en train de jouer
	scanf("%s", mot);
	crs->data[crs->eng].nbtour = atof(mot);
	scanf("%s", mot);
	crs->data[crs->eng].temps = atof(mot);
	crs->eng = crs->eng + 1;
}

// afficher tous les temps enregistrés d'un patineur repéré par son dossard 
void afficher_temps(const Course* crs, const Equipes* e) {
	unsigned int dossard;
	scanf("%d", &dossard);
	//les dossarts sont commencé à 101
	unsigned int debutant = 101;
	unsigned int n = dossard - debutant;
	unsigned int x = n / NbPatineur;
	/*les patineurs d'un même equipe vont avoir un même numero (x)*/
	unsigned int y = n % NbPatineur;
	//mais ils n'ont pas le même (y)
	for (int i = 0; i < crs->eng; ++i) {
		if (crs->data[i].dossard == dossard) {
			printf("%s %d %s %.1f\n", e->equipe[x].pays, crs->data[i].nbtour,
				e->equipe[x].data[y].nom, crs->data[i].temps);
		}
	}
}

/*cette fonction permet d'afficher les temps de tous les équipes
dans une tours spécifique*/
void afficher_temps_equipes(const Course* crs, const Equipes* e) {
	int tour;
	unsigned int debutant = 101;
	double temps;

	scanf("%d", &tour);
	for (int eq = 0; eq < e->ins; ++eq) {
		int k = 0;
		for (int j = 0; j < crs->eng; ++j) {
			if (crs->data[j].nbtour == tour &&
				(crs->data[j].dossard - debutant) / NbPatineur == eq) {
				/* enregistre le temps de la denier patineur
				de cette equipe comme le temps d'equipe.*/
				temps = crs->data[j].temps;
				k++;
			}
		}
		if (k == NbPatineur) {
			/*si les 3 membres d'une équipe ont tous joué,
			les temps d'equipes vont afficher */
			printf("%s ", e->equipe[eq].pays);
			printf("%.1f\n", temps);
		}
		else
			// sinon "indisponsable" va afficher
			printf("indisponsable");
	}
}

//définir le nombre de tours d’un parcours
void definition_parcours(Tours* t) {
	char mot[lgMot + 1];
	scanf("%s", mot);
	t->tours = atof(mot);
	/*verifier le nombre de tours est entre 2 et 10*/
	assert(t->tours >= 2 && t->tours <= 10);
}

/*détecter la fin d’une poursuite et afficher le classement
des deux derniers équipes.*/
void detection_fin_poursuite(Course* crs, Equipes* e, Tours* t,
	Temps* tp, int* nb) {
	int debutant = 101;
	/* Les programmes intervient si tous les patineurs ont effectué
	le nombre de tours qu'on a définit*/
	if (crs->eng % (NbEquipe * NbPatineur * t->tours * *nb) == 0) {
		printf("detection_fin_poursuite\n");
			for (int eq = tp->tempseq + 1; eq < NbEquipe * *nb; ++eq) {
				tp->tempseq++;
				for (int j = (NbEquipe * NbPatineur * t->tours * *nb) -
					(NbEquipe * NbPatineur * t->tours); j < crs->eng; ++j) {
					if ((crs->data[j].dossard - debutant) / NbPatineur == eq) {
						tp->temps[tp->tempseq] = crs->data[j].temps;
					}
				}
			}
		//afficher les equipes dans l'ordre croissant des temps finaux
		if ((tp->tempseq - 1) % 2 == 0) {
			if (tp->temps[tp->tempseq - 1] <= tp->temps[tp->tempseq]) {
				printf("%s ", e->equipe[tp->tempseq - 1].pays);
				printf("%.1f\n", tp->temps[tp->tempseq - 1]);
				printf("%s ", e->equipe[tp->tempseq].pays);
				printf("%.1f\n", tp->temps[tp->tempseq]);
			}
			else {
				printf("%s ", e->equipe[tp->tempseq].pays);
				printf("%.1f\n", tp->temps[tp->tempseq]);
				printf("%s ", e->equipe[tp->tempseq - 1].pays);
				printf("%.1f\n", tp->temps[tp->tempseq - 1]);
			}
		}
		*nb = *nb + 1;
	}
}

//Définir le nombre d’épreuves
void definir_nombre_epreuves(Epreuves* ep) {
	char mot[lgMot + 1];
	scanf("%s", mot);
	ep->epreuves = atof(mot);
	/*verifier le nombre d'epreuve est entre 1 et 16*/
	assert(ep->epreuves >= 1 && ep->epreuves <= 16);
}

/*détecter la fin de la compétition toutes les épreuves de poursuite
sont terminées. Si oui, le classement final sera affiché*/
void detection_fin_competition(Equipes* e, Course* crs, Tours* t, Temps* tp) {
	if (crs->eng == e->ins * NbPatineur * t->tours) {
		double v;
		int j;
		char p[lgMot + 1];
		printf("detection_fin_competition\n");
		//tri par sélection
		for (int i = 1; i < e->ins; ++i) {
			v = tp->temps[i];
			strcpy(p, e->equipe[i].pays);
			j = i;
			while (j > 0 && tp->temps[j - 1] > v) {
				tp->temps[j] = tp->temps[j - 1];
				strcpy(e->equipe[j].pays, e->equipe[j - 1].pays);
				j = j - 1;
			}
			tp->temps[j] = v;
			strcpy(e->equipe[j].pays, p);
		}
		//afficher tous les equipes dans l'ordre croissant des temps finaux
		for (int y = 0; y < e->ins; ++y) {
			printf("%s ", e->equipe[y].pays);
			printf("%.1f\n", tp->temps[y]);
		}
		exit(0); //sortie du programme
	}
}

//la fonction principale sert a faire appel aux commandes entrées
int main() {
	Equipes e;
	Course crs;
	Tours t;
	Epreuves ep;
	Temps tp;
	char mot[lgMot + 1];
	mot[0] = '\0';
	e.ins = 0;
	crs.eng = 0;
	tp.tempseq = 0 - 1;
	int n = 1;
	int dernierd = 101;

	while (1) {
		scanf("%s", mot); //scan la commande entrée
		if (strcmp(mot, "inscrire_equipe") == 0) {
			inscrire_equipe(&e);
		}
		else if (strcmp(mot, "afficher_equipes") == 0) {
			afficher_equipes(&e, &dernierd);
		}
		else if (strcmp(mot, "enregistrer_temps") == 0) {
			enregistrer_temps(&crs, &dernierd);
			detection_fin_poursuite(&crs, &e, &t, &tp, &n);
			detection_fin_competition(&e, &crs, &t, &tp);
		}
		else if (strcmp(mot, "afficher_temps") == 0) {
			afficher_temps(&crs, &e);
		}
		else if (strcmp(mot, "afficher_temps_equipes") == 0) {
			afficher_temps_equipes(&crs, &e);
		}
		else if (strcmp(mot, "definir_parcours") == 0) {
			definition_parcours(&t);
		}
		else if (strcmp(mot, "definir_nombre_epreuves") == 0) {
			definir_nombre_epreuves(&ep);
		}
		else if (strcmp(mot, "exit") == 0) {
			exit(0); //commande de sortie du programme
		}
	}
	system("pause");
	return 0;
}