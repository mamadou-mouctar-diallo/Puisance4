#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include "jeu.h"
#define LIGNE 7
#define COLONNE 7
#define TRUE 1
#define FALSE 0

void initialiserJeton( struct Jeton *jeton){ // Fonction qui initialse la structure du Jeton de jeu
    (*jeton).blanc = ' ';
    (*jeton).rouge = 'O';
    (*jeton).jaune = 'X';
}

int initialiserGrille(struct Grille *grille, struct Jeton jeton){ // Initialisation de la grille du jeu
    int estInitialise = FALSE;
    if(!estInitialise){
        for(int i = 0; i < LIGNE; i++){
            for(int j = 0; j < COLONNE; j++){
                (*grille).grille[i][j] = jeton.blanc;
            }
        }
        estInitialise = TRUE;
    }
    return estInitialise;
}

void afficheGrille(struct Grille grille){ // Affichage de la grille du jeu
    printf(" ===============================================================================================================\n");
    printf("| 8 = Inverser Grille | 9 = tourner Grille a Droit | 10 = tourner Grille a Gauche | 11 = Sauvegarder et Quitter |\n");
    printf(" ===============================================================================================================\n");
    for(int k = 0; k <= LIGNE; k++)printf("——");
    printf("\n");
    for(int i = 0; i < LIGNE; i++){
        printf("|");
        for(int j = 0; j < COLONNE; j++){
            printf("%c", grille.grille[i][j]);
            printf("|");
        }
        printf("\n");
    }
    for(int k = 0; k <= LIGNE; k++)printf("——");
    printf("\n ");
    for(int k = 0; k < LIGNE; k++)printf("%d ",k + 1);
    printf("\n");
}

char joueurEnCours(int joueurNext, struct Jeton jeton){ // Fonction qui recupère le joueur en cours de jeu
    printf("C'est le tour du joueur: ");
    if(joueurNext == 1){
        printf("%c\n",jeton.rouge);
        return jeton.rouge;
    }
    if(joueurNext == 2){
        printf("%c\n",jeton.jaune);
        return jeton.jaune;
    }
}
int estPleineColonne(struct Grille grille, struct Jeton jeton, int *Ligne, int colonne){
    int colonnePleine = FALSE, ligne = *Ligne;
    while(!colonnePleine && grille.grille[ligne][colonne] != jeton.blanc){
        if(ligne == 0){
            colonnePleine = TRUE;
        }
        ligne--;
    }
    *Ligne = ligne;
    if(colonnePleine)return TRUE;
    return FALSE;
}
int placerJeton(struct Grille *grille, int colonne, char joueur, struct Jeton jeton){ // Fonction qui place le Jeton du jeu dans la grille de jeu a chaque coup valide
    int colonnePleine = FALSE, ligne = LIGNE - 1;
    if(colonne >= 1 && colonne <= 7){
        colonne--;
        colonnePleine = estPleineColonne(*grille, jeton, &ligne, colonne);
        if(colonnePleine){
            printf("Impossible de jouer a la colonne: %d car elle est pleine\n", colonne + 1);
            return FALSE;
        }else{
            if(jeton.rouge == joueur)
            (*grille).grille[ligne][colonne] = jeton.rouge;
            else (*grille).grille[ligne][colonne] = jeton.jaune;
            return TRUE;
        }
    }else if(colonne == 8){
        return TRUE;
    }else if(colonne == 9){
        return TRUE;
    }else if(colonne == 10){
        return TRUE;
    }else if(colonne == 11){
        return TRUE;
    }
    printf("Impossible de jouer à la colonne: %d car elle n'existe pas\n", colonne);
    return FALSE;
}

void joueurSuivant(int *joueurNext){ // Fonction qui recupère le joueur suivant
    if(*joueurNext == 1)*joueurNext = 2;
    else if(*joueurNext == 2 )*joueurNext = 1;
}

int jetonsIdentiques(struct Grille grille, int ligneInitiale, int colonneInitiale, int ligneACompter, int colonneACompter, struct Jeton jeton){ // Fonction qui compte le nombre de jetons identiques 
    int jetonGagnant = 0, ligne = ligneInitiale, colonne = colonneInitiale;
    while(ligne >= 0 && colonne >= 0 && ligne < LIGNE && colonne < COLONNE && grille.grille[ligneInitiale][colonneInitiale] == grille.grille[ligne][colonne] && grille.grille[ligneInitiale][colonneInitiale] != jeton.blanc){
        ligne += ligneACompter;
        colonne += colonneACompter;
        jetonGagnant++;
    }
    return jetonGagnant;
}

int joueurAuto(){ // Fonction qui permet de jouer en mode autonome
    srand(time(NULL));
    return 1+rand()%10;
}

int intelligenceArtifficielle(struct Grille grille, struct Jeton jeton, char joueur){
    double fmax( double x, double y );

    int compteJeton(struct Grille grille, int ligneInitiale, int colonneInitiale, int ligneACompter, int colonneACompter, struct Jeton jeton, char joueur){ // Fonction qui compte le nombre de jetons identiques 
        int maxJeton = 0, ligne = ligneInitiale, colonne = colonneInitiale;
        while(ligne >= 0 && colonne >= 0 && ligne < LIGNE && colonne < COLONNE && joueur == grille.grille[ligne][colonne] && grille.grille[ligneInitiale][colonneInitiale] != jeton.blanc){
            ligne += ligneACompter;
            colonne += colonneACompter;
            maxJeton++;
        }
        return maxJeton;
    }

    for(int i = 0; i < LIGNE; i++){
        for(int j = 0; j < COLONNE; j++){
            if(
                compteJeton(grille, i, j, 0, 1, jeton) >= 3
                ||
                compteJeton(grille, i, j, 1, 0, jeton) >= 3
                ||
                compteJeton(grille, i, j, 1, -1, jeton) >= 3
                ||
                compteJeton(grille, i, j, -1, 1, jeton) >= 3
            ){ 
                printf("Max: %d \n", (int)fmax(fmax(compteJeton(grille, i, j, 0, 1, jeton), compteJeton(grille, i, j, 1, 0, jeton)), fmax(compteJeton(grille, i, j, 1, -1, jeton), compteJeton(grille, i, j, -1, 1, jeton))));
                printf("Ligne: %d Colonne: %d\n", i, j);
                return TRUE;
            }
        }
    }
    return FALSE;
}

int estGagne(struct Grille grille, struct Jeton jeton, char *joueurGagnant){ // Fonction qui teste si un joueur a gagné le match
    for(int i = 0; i < LIGNE; i++){
        for(int j = 0; j < COLONNE; j++){
            if(
                jetonsIdentiques(grille, i, j, 0, 1, jeton) >= 4
                ||
                jetonsIdentiques(grille, i, j, 1, 0, jeton) >= 4
                ||
                jetonsIdentiques(grille, i, j, 1, -1, jeton) >= 4
                ||
                jetonsIdentiques(grille, i, j, -1, 1, jeton) >= 4
            ){ 
                *joueurGagnant = grille.grille[i][j]; 
                return TRUE;
            }
        }
    }
    return FALSE;
}

int estVideCase(char kase, struct Jeton jeton){ // Fonction qui teste si une case de la grille du jeu est vide ou non
    if(kase != jeton.blanc)return TRUE;
    else return FALSE;
}

void inverserGrille(struct Grille *grille , char joueur, struct Jeton jeton){ // Fonction qui inverse la grille du jeu
    int ligne = LIGNE - 1, ligneAux;
    int colonne;
    char tmp;
    while(ligne >= 0){
        if(ligne == 0)ligneAux = LIGNE - 1;
        else ligneAux = ligne - 1;
        colonne = COLONNE - 1;
        while(colonne >= 0){
            if(!estVideCase((*grille).grille[ligneAux][colonne], jeton) || !estVideCase((*grille).grille[ligne][colonne], jeton)){
                colonne--;
                continue;
            }else{
                tmp = (*grille).grille[ligneAux][colonne];
                (*grille).grille[ligneAux][colonne] = (*grille).grille[ligne][colonne];
                (*grille).grille[ligne][colonne] = tmp;
                colonne--;
            }
        }
        ligne--;
    }
    afficheGrille(*grille);
    printf("Le joueur: ");
    if(jeton.rouge == joueur)printf("%c ", jeton.rouge);
    if(jeton.jaune == joueur)printf("%c ", jeton.jaune);
    printf("vient d'inverser la Grille du jeu\n");
}

int estPleineGrille(struct Grille grille, struct Jeton jeton){ // Fonction qui teste si la grille de jeu de pleine
    for(int i = 0; i < LIGNE; i++){
            for(int j = 0; j < COLONNE; j++){
                if(grille.grille[i][j] == jeton.blanc)return FALSE;
            }
    }
    return TRUE;
}

void swapGrille(struct Grille grilleAux, struct Grille *grille){//Fonction qui copie les cases de la GrilleAux du jeu à la Grille
    int ligne, colonne = COLONNE -1;
    while(colonne >= 0){
        ligne = LIGNE - 1;
        while(ligne >= 0){
            (*grille).grille[ligne][colonne] = grilleAux.grille[ligne][colonne];
            ligne--;
        }
        colonne--;
    }
}

void tournerGrille(struct Grille *grilleAux, struct Grille *grille, struct Jeton jeton, char joueur, int direction){ //Fonction qui permet de tourner la grille a 90
    int ligne = 0, colonne, ligneAux, colonneAux = COLONNE - 1, testeDroit = FALSE, testeGauche = FALSE;
    if(direction){
        while(ligne < LIGNE){
            colonne = COLONNE - 1;
            ligneAux = LIGNE - 1;
            while(colonne >= 0){
                if(estVideCase((*grille).grille[ligne][colonne], jeton)){
                    (*grilleAux).grille[ligneAux][colonneAux] = (*grille).grille[ligne][colonne];
                    testeDroit = TRUE;
                    ligneAux--;
                }
                colonne--;
            }
            if(testeDroit){
                colonneAux--;
                testeDroit = FALSE;
            }
            ligne++;
        }
        printf("Le joueur: %c vient de tourner la grille à droit à 90°\n", joueur);
    }else{
        ligne = 0, colonne, ligneAux = LIGNE - 1, colonneAux = 0;
        while(ligne < LIGNE){
            colonne = 0;
            ligneAux = LIGNE - 1;
            while(colonne < COLONNE){
                if(estVideCase((*grille).grille[ligne][colonne], jeton)){
                (*grilleAux).grille[ligneAux][colonneAux] = (*grille).grille[ligne][colonne];
                testeGauche = TRUE;
                ligneAux--;
                }
                colonne++;
            }
            if(testeGauche){
                colonneAux++;
                testeGauche = FALSE;
            }
            ligne++;
        }
        printf("Le joueur: %c vient de tourner la grille à gauche à 90°\n", joueur);
    }
    swapGrille(*grilleAux, grille);
    initialiserGrille(grilleAux,jeton);
}

void afficheStats(char joueurGagnant, int estPleine, int gagne, struct Jeton jeton){ // Fonction qui affiche les stats du jeu une fois gagné ou match null
    printf("====================================================\n");
    if(gagne){
        printf("Le joueur: ");
        if(joueurGagnant == jeton.rouge)printf("%c ",joueurGagnant);
        if(joueurGagnant == jeton.jaune)printf("%c ",joueurGagnant);
        printf(" a remporté le Match\n");
    }
    if(estPleine){
        printf("Le Match %c vs %c est null\n", jeton.rouge, jeton.jaune);
    }
    printf("====================================================\n");
    printf("Merci! d'avoir participé au Jeu Puisance 4 au voir.\n");
    printf("====================================================\n");
}

void viderBuffer(FILE* file){ // Fonction qui permet de vider le "stdin" si jamais la valeur saisie était autre qu'un entier
    int c;
    while((c=fgetc(file)) != '\n' && c != EOF);
}
void sauvegardeJeu(struct Grille grille, int mode,  int *joueurNext){
    int ligne, colonne, c;
    FILE *fichierJeuWrite = fopen("jeu.txt", "w+");
    fprintf(fichierJeuWrite, "%d\n", mode);
    fprintf(fichierJeuWrite, "%d\n", *joueurNext);
    for(ligne = 0; ligne < LIGNE; ligne++){
        for(colonne = 0; colonne < COLONNE; colonne++){
            c = grille.grille[ligne][colonne];
            fputc(c, fichierJeuWrite);
        }
        fputc('\n', fichierJeuWrite);
    }
    fclose(fichierJeuWrite);
    *joueurNext = 0;
}

void lectureSauvegardeJeu(FILE *file, struct Grille *grille, int *mode, int *joueurNext, int *tourJeu){
    char c;
    int i = 0, j = 0;
    *tourJeu = 3;
    fscanf(file,"%d", mode);
    fscanf(file,"%d", joueurNext);
    fgetc(file);
    while((c = fgetc(file)) != EOF){
        if(c != '\n'){
            (*grille).grille[i][j] = c;
            j++;
        }else{
            i++;
            j = 0;
        }
        
    }
}

void testMode(int mode, int joueurNext, int *colonne, int *tourJeu){ // Fonction qui teste le mode de jeu choisi
    int colonneTemp, tour = *tourJeu;
    if(tour < 2){
        colonneTemp = joueurAuto();
        tour++;
        *tourJeu = tour;
    }else{
        if(mode == 1 ){
            scanf("%d",&colonneTemp);
            viderBuffer(stdin);
        }
        if(mode == 2){
            if(joueurNext == 1){
                colonneTemp = joueurAuto();
                sleep(2);
                printf("L'ordinateur vient de jouer\n");
            }
            if(joueurNext == 2){
                scanf("%d",&colonneTemp);
                viderBuffer(stdin);
            }
        }
    }
    *colonne = colonneTemp;
}
void corpsJeu(struct Grille *GrilleAux, struct Grille *Grille, struct Jeton Jeton, int *joueurNext, int mode, int *estPleine, int *gagne, char *joueurGagnant, int *tourJeu){ // Fonction qui permet de tourner tant que les conditions du jeu ne sont pas remplies
    int colonne, estValideCoup = FALSE;
    char joueur;
    do{
        joueur = joueurEnCours(*joueurNext, Jeton);
        testMode(mode, *joueurNext, &colonne, tourJeu);
        if(colonne == 8)inverserGrille(Grille, joueur, Jeton);
        if(colonne == 9)tournerGrille(GrilleAux, Grille, Jeton, joueur, TRUE);
        if(colonne == 10)tournerGrille(GrilleAux, Grille, Jeton, joueur, FALSE);
        estValideCoup = placerJeton(Grille, colonne, joueur, Jeton);
        *gagne = estGagne(*Grille, Jeton, joueurGagnant);
        *estPleine = estPleineGrille(*Grille, Jeton);
        // intelligenceArtifficielle(*Grille, Jeton);
        if(colonne == 11){sauvegardeJeu(*Grille, mode, joueurNext);
            printf(" Le joueur: %c vient de sauvegarder et quitter le jeu \n", joueur);
            printf(" ======================================================\n");
            printf("| Merci! d'avoir participé au Jeu Puisance 4 au voir.  |\n");
            printf(" ======================================================\n");
            joueurNext = 0;
        }
        else{
            if(estValideCoup)joueurSuivant(joueurNext);
        }
    }while(!estValideCoup && !*estPleine && !*gagne);
}
void testFinal(struct Grille grille, struct Jeton jeton, int *joueurNext, int estPleine, int gagne, char joueurGagnant, int *tourJeu, int mode){ // Fonction qui permet de verifier que toute les conditions du jeu sont remplies et met fin à la partie du jeu
    if(estPleine){
            afficheGrille(grille);
            afficheStats(joueurGagnant, estPleine, gagne, jeton);
            sauvegardeJeu(grille, mode, joueurNext);
            *joueurNext = 0;
            *tourJeu = 0;
        }else if(gagne){
            afficheGrille(grille);
            afficheStats(joueurGagnant, estPleine, gagne, jeton);
            sauvegardeJeu(grille, mode, joueurNext);
            *joueurNext = 0;
            *tourJeu = 0;
        }
}

void jeuPrincipal(){ // Fonction qui est l'âme du jeu
    Grille Grille, GrilleAux;
    Jeton Jeton;
    initialiserJeton(&Jeton);
    initialiserGrille(&Grille, Jeton);
    initialiserGrille(&GrilleAux,Jeton);
    int colonne, estPleine, gagne, mode = 0, debut = 2, joueurNext = 3 , tourJeu = 0;
    char joueur, joueurGagnant;
    printf("==============================================\n");
    printf("====== Bienvenue dans le jeu Puisance 4 ======\n");
    printf("==============================================\n");
    printf("Veuillez svp suivre les instructions ci-dessous:\n");
    printf("==============================================\n");
    do{
        printf("Tapez 1 pour une nouvelle partie\n");
        printf("Tapez 0 pour continuer avec la partie en cours\n");
        scanf("%d",&debut);
        viderBuffer(stdin);
        if(debut != TRUE && debut != FALSE)printf("Choix erroné veuillez s'il vous plaît choisir une partie valide\n");
    }while(debut != TRUE && debut != FALSE);
    void demande(){
        do{
            printf("Tapez 1 pour le mode Confrontation entre Joueurs\n");
            printf("Tapez 2 pour le mode Autonome\n");
            scanf("%d",&mode);
            viderBuffer(stdin);
            if(mode != 1 && mode != 2)printf("Mode erroné veuillez s'il vous plaît faire un bon mode\n");
        }while(mode != 1 && mode != 2);
        do{
            printf("Tapez 1 pour le joueur: %c\n", Jeton.rouge);
            printf("Tapez 2 pour le joueur: %c\n", Jeton.jaune);
            scanf("%d",&joueurNext);
            viderBuffer(stdin);
            if(joueurNext != 1 && joueurNext != 2)printf("Choix erroné veuillez s'il vous plaît choisir un joueur valide\n");
        }while(joueurNext != 1 && joueurNext != 2);
    }
    void fonctionPrincipale(){
        if(mode == 1){
            printf("==========================================================\n");
            printf("===== Vous êtes en mode confrontation entre joueurs ======\n");
            printf("==========================================================\n");
        }
        if(mode == 2){
            printf("=======================================\n");
            printf("===== Vous êtes en mode autonome ======\n");
            printf("=======================================\n");
        }
        while(joueurNext){
            afficheGrille(Grille);
            corpsJeu(&GrilleAux, &Grille, Jeton, &joueurNext, mode, &estPleine, &gagne, &joueurGagnant, &tourJeu);
            testFinal(Grille, Jeton, &joueurNext, estPleine, gagne, joueurGagnant, &tourJeu, mode);
        }
    }

    if(debut){
        demande();
        fonctionPrincipale();
    }else{
        FILE  *fichierJeuRead = fopen("jeu.txt", "r+");
        lectureSauvegardeJeu(fichierJeuRead, &Grille, &mode, &joueurNext, &tourJeu);
        fclose(fichierJeuRead);
        gagne = estGagne(Grille, Jeton, &joueurGagnant);
        estPleine = estPleineGrille(Grille, Jeton);
        int choix = 0;
        if(gagne || estPleine){
            printf("=======================================\n");
            printf("Désolé cette partie est terminée.\n");
            printf("=======================================\n");
            printf("Voulez-vous continuer à une nouvelle partie ?\n");
            printf("1 = OUI\n");
            printf("0 = NON\n");
            scanf("%d", &choix);
            if(choix){
                initialiserGrille(&Grille, Jeton);
                tourJeu = 0;
                demande();
                fonctionPrincipale();
            }else{
                printf(" ======================================================\n");
                printf("| Merci! d'avoir participé au Jeu Puisance 4 au voir.  |\n");
                printf(" ======================================================\n");
            }

        }else{
            fonctionPrincipale();
        }
    }
}

/*
Quand vous parlez de decisions de l'IA vous faites allusion qu'il doit prendre des decisions comme tout joueur du jeu.....
Etant donnee une sauvegarde d'une partie du jeu par un joueur supposons que la prochaine fois qu'il revient continuer la partie qu'il a sauvegardee
pour afin terminer et apres que nous ayons sauvegarde la partie terminee, est-ce nous devons surpprimer le fichier d'une partie terminee ou toujours la 
garder dans le repertoire du jeu et tester si la partie chargee du fichier est finie (grille pleine ou jeu gagner) ?
S'il s'avere que chaque partie cree et sauvegardee pour etre terminer plus tard, comment peut-on faire reconnaitre au joueur sa partie sauvegardee (nom de son fichier)
sachant que s'il y a plusieurs sauvegardes avec des fichiers differents, il y aura plusieurs fichiers ?
S'il y a pluseurs sauvegardes, comment eviter de charger les fichiers contenant les parties finies ?

*/