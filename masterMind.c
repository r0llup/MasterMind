/************************************************************************/
/* Auteur : LECLER Quentin									    		*/
/* Groupe : 2102														*/
/* Application : masterMind.c											*/
/* Description : r�alise le jeu du masterMind							*/
/* Date de derni�re mise � jour : 06/02/10								*/
/************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ALEA_MIN	0
#define ALEA_MAX	5
#define COMB_LEN	4
#define VECT_LEN	6
#define COUP_MAX	10

/************************************************************************/

int genererNombre(int min, int max);
int verifierCouleur(char couleur);
int verifierOccurence(char *cu, int tailleComb, char couleur);

void lireCombinaison(char *cu, int tailleComb);
void genererCombinaison(char *vc, char *cc, int tailleComb, int min, int max);
void afficherCombinaison(char *cc, int tailleComb);
void verifierCombinaison(char *cc, char *cu, int tailleComb, int *bonneCouleur, \
	int *bonnePosition);
int rechercherCouleur(char *cc, int tailleComb, char couleur);
int verifierOccComb(char *cu, int tailleComb);
int verifierColComb(char *cu, int tailleComb);

/************************************************************************/

int main(int argc, char *argv[])
{
	char vecteurCouleur[VECT_LEN] = {'R', 'B', 'V', 'J', 'N', 'O'};
	char combinaisonCouleur[COMB_LEN]; /* combinaison g�n�r�e al�atoirement */
	char combinaisonUtilisateur[COMB_LEN]; /* combinaison saisie par l'utilisateur */
	int coupsTotal;
	int coupsRestants;
	int bonneCouleur; /* nombre de couleur(s) bien positionn�e */
	int bonnePosition; /* nombre de couleur(s) mal positionn�e */
	char recommencer; /* flag pour recommencer une nouvelle partie */
	int verifOcc; /* flag v�rifiant la redondance de la combinaison */
	int verifCol; /* flag v�rifiant si les couleurs de la combinaison sont valides */

	recommencer = 'o';

	srand((unsigned int) time(NULL));

	printf("Bienvenue dans le jeu du masterMind !\n");
	printf("_____________________________________\n\n");

	do
	{
		/*********************************/
		/* le joueur � droit � n essais */
		/*********************************/

		coupsTotal = COUP_MAX;
		coupsRestants = COUP_MAX + 1;

		/******************************************************/
		/* g�n�ration al�atoire de la combinaison de couleurs */
		/******************************************************/

		genererCombinaison(vecteurCouleur, combinaisonCouleur, COMB_LEN, ALEA_MIN, ALEA_MAX);

		/*********************/
		/* boucle principale */
		/*********************/

		do
		{
			/*******************************/
			/* remise � z�ro des compteurs */
			/*******************************/

			bonneCouleur = 0;
			bonnePosition = 0;

			/***************************************/
			/* on enl�ve un coup � chaque bouclage */
			/***************************************/

			coupsRestants--;

			/*********************************************/
			/* on demande � l'utilisateur sa combinaison */
			/*********************************************/

			do
			{
				printf("\nVeuillez saisir les quatres couleurs de votre combinaison : ");
				lireCombinaison(combinaisonUtilisateur, COMB_LEN);
				verifOcc = verifierOccComb(combinaisonUtilisateur, COMB_LEN);
				verifCol = verifierColComb(combinaisonUtilisateur, COMB_LEN);

				if(verifOcc == 1)
				{
					printf("\nChaque couleur ne peut etre utilisee qu\'une seule fois !\n");
				}
				if(verifCol == 1)
				{
					printf("\nLes couleurs valides sont {'R', 'B', 'V', 'J', 'N', 'O'} !\n");
				}
			} while((verifOcc == 1) || (verifCol == 1));

			/***********************************************************/
			/* v�rification de la combinaison donn�e par l'utilisateur */
			/***********************************************************/

			verifierCombinaison(combinaisonCouleur, combinaisonUtilisateur, COMB_LEN, \
				&bonneCouleur, &bonnePosition);

			/*******************************/
			/* on affiche les informations */
			/*******************************/

			printf("\nIl y a %d couleur(s) au bon endroit.\n", bonneCouleur);
			printf("Il y a %d couleur(s) mal positionnee.\n", bonnePosition);

		} while((coupsRestants > 0) && (bonneCouleur < 4));

		/*************************/
		/* l'utilisateur a perdu */
		/*************************/

		if(coupsRestants <= 0)
		{
			printf("\nVous avez perdu !\n");
			printf("La combinaison etait : ");
			afficherCombinaison(combinaisonCouleur, COMB_LEN);
			printf("\n\n");
		}

		/*************************/
		/* l'utilisateur a gagn� */
		/*************************/

		else
		{
			printf("\nVous avez gagne !\n");
			printf("Vous avez utilise %d/ %d coups !\n\n", \
				coupsTotal - coupsRestants + 1, coupsTotal);
		}

		/****************************************************/
		/* on demande � l'utilisateur s'il veut recommencer */
		/****************************************************/

		do
		{
			fflush(stdin); /* clear du buffer d'entr�e */
			printf("Voulez-vous recommencer (o/ n) ? ");
			recommencer = getchar();
		} while(recommencer != 'o' && recommencer != 'n');

	} while(recommencer == 'o');

	fflush(stdin); /* clear du buffer d'entr�e */

	printf("\nAppuyez sur une touche pour continuer... ");
	getchar();

	return 0;
}

/************************************************************************/
/* INPUT : nombre minimum � g�n�rer (min)								*/
/*		   nombre maximum � g�n�rer (max)								*/
/* PROCESS : g�n�ration al�atoire d'un nombre compris entre	min et max 	*/
/* OUTPUT : nombre al�atoire compris entre min et max					*/
/************************************************************************/

int genererNombre(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

/************************************************************************/
/* INPUT : couleur � v�rifier (col)										*/
/* PROCESS : v�rifie si la couleur est valide							*/
/* OUTPUT : 1/ 0														*/
/************************************************************************/

int verifierCouleur(char col)
{
	int verif;

	verif = 1;

	switch(col)
	{
		case 'R':
		case 'B':
		case 'V':
		case 'J':
		case 'N':
		case 'O':
			verif = 0;
	}

	return verif;
}

/************************************************************************/
/* INPUT : combinaison de l'utilisateur (*cu)							*/
/*		   taille de la combinaison (tailleComb)						*/
/*		   couleur � v�rifier (couleur)									*/
/* PROCESS : v�rifie les occurences d'une m�me couleur					*/
/* OUTPUT : nombre d'occurence(s) de la couleur							*/
/************************************************************************/

int verifierOccurence(char *cu, int tailleComb, char couleur)
{
	int compteurComb;
	int verif;

	for(compteurComb = 0, verif = 0; compteurComb < tailleComb; compteurComb++, cu++)
	{
		/*****************************************/
		/* si la couleur est dans la combinaison */
		/*****************************************/

		if(*cu == couleur)
		{
			verif++;
		}
	}

	return verif;
}

/************************************************************************/
/* INPUT : combinaison de l'utilisateur (*cu)							*/
/*		   taille de  la combinaison (tailleComb)						*/
/* PROCESS : encodage d'une combinaison de couleurs						*/
/* OUTPUT : /															*/
/************************************************************************/

void lireCombinaison(char *cu, int tailleComb)
{
	int compteurCol;

	compteurCol = 0;

	fflush(stdin); /* clear du buffer d'entr�e */

	do
	{
		*cu = getchar();
		cu++;
		compteurCol++;

	} while(compteurCol < tailleComb);
}

/************************************************************************/
/* INPUT : vecteur de couleurs (*vc)									*/
/*		   combinaison de couleurs (*cc)								*/
/*		   taille de la combinaison � g�n�rer (tailleComb)				*/
/*	       index minimum (min)											*/
/*		   index maximum (max)											*/
/* PROCESS : g�n�ration al�atoire d'une combinaison de couleurs			*/
/* OUTPUT : /															*/
/************************************************************************/

void genererCombinaison(char *vc, char *cc, int tailleComb, int min, int max)
{
	int compteurComb1;
	int compteurComb2;
	int index;
	int trouve; /* flag v�rifiant la r�p�tition de la couleur */
	char *pDebut; /* pointeur de t�te de la combinaison de couleurs (*cc) */
	char *pCourant;

	pDebut = cc;

	for(compteurComb1 = 0; compteurComb1 < tailleComb; compteurComb1++, cc++)
	{
		trouve = 1;

		do
		{
			trouve = 0;
			index = genererNombre(min, max);
			for(compteurComb2 = 0, pCourant = pDebut; compteurComb2 < compteurComb1; \
				compteurComb2++, pCourant++)
			{
				if(*(vc + index) == *pCourant)
				{
					trouve = 1;
				}
			}
		} while(trouve == 1);
		*cc = *(vc + index);
	}
}

/************************************************************************/
/* INPUT : combinaison de couleurs (*cc)								*/
/*		   taille de la combinaison � afficher (tailleComb)				*/
/* PROCESS : affichage de la combinaison de couleurs					*/
/* OUTPUT : /															*/
/************************************************************************/

void afficherCombinaison(char *cc, int tailleComb)
{
	int compteurComb;

	for(compteurComb = 0; compteurComb < tailleComb; compteurComb++, cc++)
	{
		printf("%c ", *cc);
	}
}

/************************************************************************/
/* INPUT : combinaison de couleurs (*cc)								*/
/*		   combinaison de l'utilisateur (*cu)							*/
/*		   taille de la combinaison de couleurs (tailleComb)			*/
/*		   compteur de bonne(s) couleurs (*bonneCouleur)				*/
/*		   compteur de bonne(s) positions (*bonnePosition)				*/
/* PROCESS : v�rifie la combinaison donn�e par l'utilisateur			*/
/* OUTPUT : /															*/
/************************************************************************/

void verifierCombinaison(char *cc, char *cu, int tailleComb, int *bonneCouleur, int *bonnePosition)
{
	int compteurComb;
	char *pDebut; /* pointeur de t�te de la combinaison de couleur (*cc) */

	pDebut = cc;

	for(compteurComb = 0; compteurComb < tailleComb; compteurComb++, cc++, cu++)
	{
		/**************************************************/
		/* la couleur est bien pla��e dans la combinaison */
		/**************************************************/

		if(*cu == *cc)
		{
			*bonneCouleur = *bonneCouleur + 1;
		}

		/********************************************************/
		/* la couleur est mal pla��e ou pas dans la combinaison */
		/********************************************************/

		else
		{
			/**************************************/
			/* la couleur est dans la combinaison */
			/**************************************/

			if(rechercherCouleur(pDebut, tailleComb, *cu) == 1)
			{
				*bonnePosition = *bonnePosition + 1;
			}
		}
	}
}

/************************************************************************/
/* INPUT : combinaison de couleurs (*cc)								*/
/*		   taille de la combinaison de couleurs (tailleComb)			*/
/*		   couleur � rechercher dans le vecteur (couleur)				*/
/* PROCESS : v�rifie si la couleur est pr�sente dans le combinaison		*/
/* OUTPUT : 1/ 0														*/
/************************************************************************/

int rechercherCouleur(char *cc, int tailleComb, char couleur)
{
	int compteurComb;
	int trouve;

	for(compteurComb = 0, trouve = 0; (compteurComb < tailleComb) && (trouve == 0); \
		compteurComb++, cc++)
	{
		/***********************************************/
		/* la couleur est pr�sente dans la combinaison */
		/***********************************************/

		if(*(cc) == couleur)
		{
			trouve = 1;
		}
	}

	return trouve;
}

/************************************************************************/
/* INPUT : combinaison de l'utilisateur (*cu)							*/
/*		   taille de la combinaison de l'utilisateur (tailleComb)		*/
/* PROCESS : v�rifie le nombre d'occurence(s) d'une certaine couleur	*/
/* OUTPUT : 1/ 0														*/
/************************************************************************/

int verifierOccComb(char *cu, int tailleComb)
{
	int compteurComb;
	int verif;
	char *pDebut; /* pointeur de t�te de la combinaison */

	pDebut = cu;

	for(compteurComb = 0, verif = 0; (compteurComb < tailleComb) && (verif == 0); \
		compteurComb++, cu++)
	{	
		/********************************************************************/
		/* la combinaison n'est pas valide s'il y a plus d'une m�me couleur */
		/********************************************************************/

		if(verifierOccurence(pDebut, tailleComb, *cu) > 1)
		{
			verif = 1;
		}
	}

	return verif;
}

/************************************************************************/
/* INPUT : combinaison de l'utilisateur (*cu)							*/
/*		   taille de la combinaison de l'utilisateur (tailleComb)		*/
/* PROCESS : v�rifie la validit� des couleurs de la combinaison			*/
/* OUTPUT : 1/ 0														*/
/************************************************************************/

int verifierColComb(char *cu, int tailleComb)
{
	int compteurComb;
	int verif;

	for(compteurComb = 0, verif = 0; (compteurComb < tailleComb) && (verif == 0); \
		compteurComb++, cu++)
	{
		/*************************************/
		/* v�rifie la validit� de la couleur */
		/*************************************/

		if(verifierCouleur(*cu) == 1)
		{
			verif = 1;
		}
	}

	return verif;
}