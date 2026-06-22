#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ncurses.h>

#define H 30
#define L 40
#define MT 20
#define MAPL 11
#define NB 4
#define NL 5

#define joueur '^'
#define tir '|'
#define al 'A'
#define TAL '*'
#define BI '='
#define BE '_'
#define BPD '.'



typedef struct 
{
    int x, y;
    int vivant;
}PNJ;


void ecran(WINDOW *win , char aff[H][L], int posJ , int tirH[MT] , int tirL[MT], int tirAH[MT], int tirAL[MT] , PNJ alien[NL][MAPL] , PNJ bouclier[NB]);
void mouv_j(char *mouv, int *posJ , int tirH[MT] , int tirL[MT] , int *entr);
void maj_tir_j ( int tirH[MT] , int tirL[MT] , PNJ alien[NL][MAPL] , int *score , int *ma, int *victoire , PNJ bouclier[NB] );
void maj_tir_alien(int tirAH[MT], int tirAL[MT] , int posJ , int *perdu , int *vieJ , PNJ bouclier[NB]);
void mvmt_al(PNJ alien[NL][MAPL], int *perdu , int *directionA , int tirAH[MT], int tirAL[MT] , int *ma , int *entr);


int main() //fonction principale
{
	char mouv = '\0', aff[H][L] ;
	int posJ = L/2 , vieJ = 3 , tirH[MT], tirL[MT] , score = 0 , tirAH[MT], tirAL[MT] , perdu = 0 , victoire = 0 , directionA = 1 , ma = MAPL * NL , entr = 0 ;

	PNJ alien[NL][MAPL];
	PNJ bouclier[NB]; 

	srand(time(NULL));

	initscr();//démarre ncurses
	cbreak();//permet d'entrer un caractère sans utiliser entrée
	noecho();//désactive l'affichage des touches saisies
	curs_set(0);//curseur invisible

	WINDOW *win = newwin(H *2 , L * 2, 0, 0);//création du tableau de jeu
	keypad(win, TRUE);//traduit les touches et tris les bonnes des mauvaises 

	//initialisation des aliens
	for (int i = 0; i < NL; i++) //lignes
	{
		for (int j = 0; j < MAPL; j++) //colonnes
		{
			alien[i][j].x = 3 * (j + 1) + 1;
			alien[i][j].y = 2 + i ;
			alien[i][j].vivant = 1 ;
		}
	}
	

	//initialisation des tirs
	for (int i = 0; i < MT; i++) 
	{
		tirH[i] = -1; // Pas de tirs actifs
		tirL[i] = -1;
		tirAH[i] = -1;
		tirAL[i] = -1;
	}

	//initialisation des boucliers
	for (int i = 0; i < NB; i++) 
	{
		bouclier[i].x = (i + 1) * ( L/NB) - 6;
		bouclier[i].y = H - 4;
		bouclier[i].vivant = 3;
	}

	while (1)
	{
		ecran(win , aff, posJ, tirH , tirL , tirAH , tirAL , alien , bouclier);
		mvwprintw(win , H - 1, 1,"\nVies restantes: %d\nMouvements: Q = Gauche, D = Droite, Z = Tirer, A = Quitter \nScore: %d\n" , vieJ , score);//"printf" de ncurses, mv pour le déplacement des caractères et w pour la spécification à la fenêtre
		wrefresh(win);//rafraichit ce qu'on voit sans "clignotements"

		int ch = wgetch(win);//ch vaut à ce qu'on va taper
		mouv = (char)ch;//on viet attribuer ce qu'on a attribuer à mouv pour faire bouger le joueur

		mouv_j (&mouv, &posJ, tirH, tirL , &entr);
		maj_tir_j (tirH, tirL, alien, &score, &ma , &victoire , bouclier);
		maj_tir_alien (tirAH, tirAL, posJ, &perdu, &vieJ, bouclier);
		mvmt_al (alien, &perdu, &directionA , tirAH, tirAL , &ma , &entr);
		if (mouv == 'a' || mouv == 'A' || perdu == 1 || victoire == 1 || vieJ == 0) 
		{
			wrefresh(win);
			break;
		}
	}	
	delwin(win);//libère la fenêtre
	endwin();//fermeture de la fenêtre
	if (mouv == 'a' || mouv == 'A') 
	{
		printf("Vous avez abandonnez la Terre !!! \nscore: %d\n" , score);
	}

	if (perdu == 1) 
	{
		printf("Les aliens ont atteint la Terre !!! \nScore: %d\n", score);
	}
		
	if (victoire == 1) 
	{
		printf( "Vous avez vaincu l'ennemi!!! \nScore: %d\n", score);
	}

	if (vieJ == 0) 
	{
		printf( "L'ennemi vous a eu !!! \nScore: %d\n", score);
	}
	return EXIT_SUCCESS;
}

	
void ecran(WINDOW *win , char aff[H][L], int posJ , int tirH[MT] ,int tirL[MT] , int tirAH[MT], int tirAL[MT] , PNJ alien[NL][MAPL] , PNJ bouclier[NB])//dessiner tt l'écran avec tt ce qui le compose 
{
	werase(win);//"system("clear");" de ncurses

	//initialisation de l'écran
	for (int i = 0; i < H; i++) 
	{
		for (int j = 0; j < L; j++) 
		{
			aff[i][j] = ' '; //remplir avec des espaces
		}
	}

	//dessiner les limites horizontales
	for (int n = 0; n < L; n++) 
	{
		aff[0][n] = '-';//haut
		aff[H-1][n] = '-';//bas
	}

	//dessiner les limites verticales
	for (int i = 0; i < H; i++) 
	{
       		aff[i][0] = '|';//gauche
        	aff[i][L - 1] = '|';//droite
	}
	
	//afficher le joueur
	aff[H-2][posJ] = joueur;
		
	//afficher le(s) tir(s) du joueur
	for (int i = 0; i < MT; i++)
	{
		if (tirH[i] >= 0) 
		{
			aff[tirH[i]][tirL[i]] = tir;
		}
	}

	// Afficher les aliens
	for(int i = 0 ; i < NL ; i++)
	{
		for (int j = 0; j < MAPL; j++) 
		{
			if (alien[i][j].vivant > 0)
			{
				aff[alien[i][j].y][alien[i][j].x] = al;
			}
		}
	}

	//afficher le(s) tir(s) de l'alien
	for (int i = 0; i < MT; i++)
	{
		if (tirAH[i] >= 0 && tirAH[i] < H && tirAL[i] >= 0 && tirAL[i] < L)
		{
			aff[tirAH[i]][tirAL[i]] = TAL;
		}
	}

	//affiche les boucliers
	for (int i = 0; i < NB; i++)
	{
		if (bouclier[i].vivant > 0)
		{
			if (bouclier[i].vivant == 3) 
			{
				aff[bouclier[i].y][bouclier[i].x] = BI; //bouclier intact
			} 
			else if (bouclier[i].vivant == 2) 
			{
				aff[bouclier[i].y][bouclier[i].x] = BE; //bouclier endommagé
			} 
			else if (bouclier[i].vivant == 1) 
			{
				aff[bouclier[i].y][bouclier[i].x] = BPD; //bouclier presque détruit
			}
		}

	}
			
	//afficher l'écran
	for (int i = 0; i < H; i++) 
	{
		for (int j = 0; j < L; j++) 
		{
			mvwaddch(win, i, j, aff[i][j]);//"putchar" de ncurses, pour afficher les caractères un par un 
		}
	}
}


void mouv_j(char *mouv, int *posJ , int tirH[MT] ,int tirL[MT] , int *entr)
{
	switch (*mouv)
	{
	case ('q'):
	case ('Q'): //gauche
		if (*posJ > 1) // empêcher de sortir de l'écran
		{
			(*posJ)--;
			*entr += 1 ;
		}
			break;

	case ('d'):
	case ('D'): //droite
		if (*posJ < L - 2) // empêcher de sortir de l'écran
		{
			(*posJ)++;
			*entr += 1 ;
		}
			break;

	case ('z'): 
	case ('Z'): 
		for (int i = 0; i < MT; i++) 
		{
			if (tirH[i] == -1) // si pas de tirs
			{
				tirH[i] = H - 3; //position à la verticale du tir
				tirL[i] = *posJ; //position à l'horizontale du tir
				*entr += 1 ;
				break;
			}
		}
		break;


	default:
		printf("Commande inconnue.\n");
		*entr += 1;
		break;
	}
}

void maj_tir_j ( int tirH[MT] , int tirL[MT] , PNJ alien[NL][MAPL] , int *score , int *ma , int *victoire , PNJ bouclier[NB] )
{
	for (int i = 0; i < MT; i++) //pour tous les tirs
	{
		if (tirH[i] >= 0) //si actif
		{
			tirH[i]--;//monter le tir d'une ligne
			if (tirH[i] < 1) // quand haut de l'écran atteint 
			{
				tirH[i] = -1;// réinitialiser le tir
				tirL[i] = -1;
			}
			else
			{
				for(int j= 0 ; j < NL ; j++)
				{
					for (int k = 0; k < MAPL; k++) //pour tous les aliens
					{
						if (alien[j][k].vivant > 0 && tirH[i] == alien[j][k].y && tirL[i] == alien[j][k].x) //quand tir touche un alien
						{
							alien[j][k].vivant -= 1;//on enlève une vie
							tirH[i] = -1;//le tir disparait
							tirL[i] = -1;
						
							if(alien[j][k].vivant == 0)
							{
								*ma -= 1;
								*score += 10;
		
								if(*ma == 0)
								{
									*victoire = 1;
									return;
								}
								break;
							}
						}
					}
				}
				for (int j = 0; j < NB; j++) //pour tous les boucliers
				{
					if (bouclier[j].vivant > 0 && tirH[i] == bouclier[j].y && tirL[i] == bouclier[j].x) 
					{
						bouclier[j].vivant--;//réduire la santé du bouclier
						tirH[i] = -1;//réinitialiser le tir
						tirL[i] = -1;
					}
				}
			}
		}
	}
}


void maj_tir_alien(int tirAH[MT] , int tirAL[MT] , int posJ , int *perdu , int *vieJ , PNJ bouclier[NB] )
{
	for (int i = 0; i < MT; i++) //pour tous les tirs 
	{
		if (tirAH[i] >= 0) //si le tir est en cours
		{
			tirAH[i]++; //descendre le tir d'une ligne

			if (tirAH[i] >= H - 1) //si le tir est tt en bas
			{
				tirAH[i] = -1; //réinitialiser le tir
				tirAL[i] = -1;
			}

			if (tirAH[i] == H - 2 && tirAL[i] == posJ)// si ça atteint le joueur
			{
				*vieJ -= 1;
				tirAH[i] = -1; //réinitialiser le tir
				tirAL[i] = -1;

				if ( *vieJ == 0)// quand le joueur n'a plus de vie
				{
					*perdu = 1 ;
					return;
				}
			}

			for (int j = 0; j < NB; j++) //pour tous les boucliers 
			{
				if (bouclier[j].vivant > 0 && tirAH[i] == bouclier[j].y && tirAL[i] == bouclier[j].x) 
				{
					bouclier[j].vivant--;//réduire la santé du bouclier de 1
					tirAH[i] = -1;//réinitialiser le tir
					tirAL[i] = -1;
    				}
			}
		}
	}
}

void tiralea_al (PNJ alien[NL][MAPL], int tirAH[MT], int tirAL[MT]);//rappel de la fonction 
void mvmt_al(PNJ alien[NL][MAPL], int *perdu , int *directionA , int tirAH[MT], int tirAL[MT] , int *ma , int *entr) 
{
	int descendre = 0 ;

	if(*ma <= (MAPL*NL)-2 && *entr > 3)
	{
		for ( int i=0 ; i < NL ; i++)
		{
			for (int j = 0; j < MAPL; j++) //pour tous les aliens 
			{
				if (alien[i][j].vivant > 0 ) //si alien vivant
				{ 
					if(alien[i][j].x == 1 || alien[i][j].x == L - 2)// si l'alien atteint 1 bord
					{
						descendre = 2;//on active la descente de 2 lignes
						*directionA *= -1;//équivaut à faire: directionA X -1 : chgmt de direction
						break;
					}
					if (alien[i][j].y >= H - 2)
					{
						*perdu = 1;// si bas de l'écran atteint, c'est perdu
						return;
					}
				}
			}
		}
		for (int i = 0; i < NL; i++) 
		{
			for (int l = 0; l < MAPL; l++) //pour tous les aliens, on va appliquer les mouvements
			{
				if (alien[i][l].vivant > 0)
				{	
					if (descendre == 2 )
					{
						alien[i][l].y += 2;// descendre de 2 ligne
					}
					alien[i][l].x += *directionA;//aller dans le sens directionA (1 droite et -1 gauche)
				}
			}
		}
		*entr = 0;
	}
	tiralea_al(alien, tirAH, tirAL);
	
}

void tiralea_al(PNJ alien[NL][MAPL], int tirAH[MT], int tirAL[MT])
{
	int alea ; 
	for (int i = 0; i < NL; i++) 
	{
		for (int j = 0; j < MAPL; j++) //pour tous les aliens
		{
			if (alien[i][j].vivant > 0) //si alien vivant
			{
				alea = rand() % 100;//regénérer un alea à chaque alien diff
	
				if (alea < 5) // 10% de chances que l'alien tire
				{
					for (int k = 0; k < MT; k++) //chercher un tir disponible
					{
						if (tirAH[k] == -1 && tirAL[k] == -1) //vérifier si libre pour un tir
						{
							tirAH[k] = alien[i][j].y + 1; //le tir part en dessous de l'alien
							tirAL[k] = alien[i][j].x;
							break; //un seul tir par alien et par tour
						}
					}
				}
			}
		}
	}
}
