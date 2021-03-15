#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
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

void afficheGrille(struct Grille grille, int mode){ // Affichage de la grille du jeu
    if(mode !=  0){
        printf(" ======================================================================================================================\n");
        printf("| 8 = Inverser grille | 9 = tourner a Droit | 10 = tourner a Gauche | 11 = Sauvegarder la partie | 0 = Quitter le jeu |\n");
        printf(" ======================================================================================================================\n");
        printf(" Pour jouer tapez une valeur entre 1 et 7  et n'oubliez pas les instructions ci-dessus pour faire basculer le jeu |\n");
        printf(" =================================================================================================================\n");
    }
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
    }else if(colonne == 0){
        return TRUE;
    }
    printf("Impossible de jouer à la colonne: %d car elle n'existe pas\n", colonne);
    return FALSE;
}

void joueurSuivant(int *joueurNext){ // Fonction qui recupère le joueur suivant
    if(*joueurNext == 1)*joueurNext = 2;
    else if(*joueurNext == 2 )*joueurNext = 1;
}



int joueurAuto(int colonne){ // Fonction qui permet de jouer en mode autonome
    srand(time(NULL));
    return 1+rand()%colonne;
}

int intelligenceArtifficielle(struct Grille grille, struct Jeton jeton, char joueur){ // Fonction qui permet l'IA de jouer
    double fmax( double x, double y );
    int compteJeton(struct Grille grille, int ligneInitiale, int colonneInitiale, int ligneACompter, int colonneACompter, struct Jeton jeton, char joueur, int *Colonne, int *Ligne){ // Fonction qui compte le nombre de jetons identiques 
        int maxJeton = 0, ligne = ligneInitiale, colonne = colonneInitiale;
        while(ligne >= 0 && colonne >= 0 && ligne < LIGNE && colonne < COLONNE && joueur == grille.grille[ligne][colonne] && grille.grille[ligneInitiale][colonneInitiale] != jeton.blanc){
            ligne -= ligneACompter;
            colonne -= colonneACompter;
            maxJeton++;
        }
        *Colonne = colonne;
        *Ligne = ligne;
        return maxJeton;
    }
    int obtenirAlignementJeton(struct Grille grille, struct Jeton jeton, char joueur, char *alignement, int *posLigne, int *posColonne){
        int maxJetonIdentique, horizontale, verticale, diagonaleDroit, diagonaleGauche, posLigneJeton, posColonneJeton;
        for(int i = LIGNE - 1; i >= 0; i--){
            for(int j = COLONNE - 1; j >= 0; j--){
                horizontale = compteJeton(grille, i, j, 0, 1, jeton, joueur, &posColonneJeton, &posLigneJeton);
                verticale = compteJeton(grille, i, j, 1, 0, jeton, joueur, &posColonneJeton, &posLigneJeton);
                diagonaleDroit = compteJeton(grille, i, j, -1, 1, jeton, joueur, &posColonneJeton, &posLigneJeton);
                diagonaleGauche = compteJeton(grille, i, j, 1, 1, jeton, joueur, &posColonneJeton, &posLigneJeton);
                if(
                    verticale >= 2
                    ||
                    horizontale >= 2
                    ||
                    diagonaleDroit >= 2
                    ||
                    diagonaleGauche >= 2
                ){ 
                    maxJetonIdentique = (int)fmax(fmax(verticale, horizontale), fmax(diagonaleDroit, diagonaleGauche));
                    if(maxJetonIdentique == horizontale){
                        if(j < 6 ){
                            if(grille.grille[i][j + 1] == jeton.blanc){
                                *alignement = 'H';
                                *posColonne = j + 1;
                                return horizontale;
                            }else
                            if(j > 2 && grille.grille[i][j - horizontale] == jeton.blanc){
                                *alignement = 'H';
                                *posColonne = (j - horizontale);
                                return horizontale;
                            }
                        }
                    }
                    if(maxJetonIdentique == verticale){
                       if(i >= 1 && grille.grille[i - verticale][j] == jeton.blanc){
                            *alignement = 'V';
                            *posColonne = j;
                            return verticale;
                       }
                    }
                    if(maxJetonIdentique == diagonaleDroit){
                        if(i >= 1 && j < 6 && grille.grille[i - 1][j + 1] == jeton.blanc && grille.grille[i - 2][j + 1] != jeton.blanc){
                            *alignement = 'D';
                            *posColonne = j + 1;
                            return diagonaleDroit;
                        }
                    }
                    if(maxJetonIdentique == diagonaleGauche){
                         if(i >= 1 && j >= 1 && grille.grille[i - 1][j - 1] == jeton.blanc && grille.grille[i - 2][j - 1] != jeton.blanc){
                            *alignement = 'G';
                            *posColonne = j - 1;
                            return diagonaleGauche;
                        }
                    }
                }
            }
        }
        *alignement =  'I';
        return 0;
    }
    int caseAJouer(struct Grille grille, struct Jeton jeton, int ligne, int colonne, char alignement){
        int ligneTeste = LIGNE - 1;
        if(alignement == 'V'){
            if(estPleineColonne(grille, jeton, &ligneTeste, colonne)){ return joueurAuto(colonne);}
            else{
                return colonne + 1;
            }
        }
        if(alignement == 'H'){
            if(estPleineColonne(grille, jeton, &ligneTeste, colonne)){ return joueurAuto(colonne);}
            else{
                return colonne + 1;
            }
        }
        if(alignement == 'D'){
            if(estPleineColonne(grille, jeton, &ligneTeste, colonne)){ return joueurAuto(colonne - 1);}
            else{
                return colonne + 1;
            }
        }
        if(alignement == 'G'){
            if(estPleineColonne(grille, jeton, &ligneTeste, colonne)){ return joueurAuto(colonne);}
            else{
                return colonne + 1;
            }
        }
        if(alignement == 'I')return joueurAuto(7);
    }
    char alignementRouge, alignementJaune;
    int posLigneJetonRouge = 0, posColonneJetonRouge = 0, posLigneJetonJaune = 0, posColonneJetonJaune = 0;
    int maxJetonRouge = obtenirAlignementJeton(grille, jeton, jeton.rouge, &alignementRouge, &posLigneJetonRouge, &posColonneJetonRouge),
        maxJetonJaune = obtenirAlignementJeton(grille, jeton, jeton.jaune, &alignementJaune, &posLigneJetonJaune, &posColonneJetonJaune);
    if(maxJetonJaune >= 1 || maxJetonRouge >= 1){
        if(maxJetonRouge > maxJetonJaune){
            return caseAJouer(grille, jeton, posLigneJetonRouge, posColonneJetonRouge, alignementRouge);
        }else 
        if(maxJetonRouge < maxJetonJaune){
            return caseAJouer(grille, jeton, posLigneJetonJaune, posColonneJetonJaune, alignementJaune);
        }else{
            return caseAJouer(grille, jeton, posLigneJetonRouge, posColonneJetonRouge, alignementRouge);
        }
    }else{
        return joueurAuto(7);
    }
}

int estGagne(struct Grille grille, struct Jeton jeton, char *joueurGagnant){ // Fonction qui teste si un joueur a gagné le match
    int jetonsIdentiques(struct Grille grille, int ligneInitiale, int colonneInitiale, int ligneACompter, int colonneACompter, struct Jeton jeton){ // Fonction qui compte le nombre de jetons identiques 
        int jetonGagnant = 0, ligne = ligneInitiale, colonne = colonneInitiale;
        while(ligne >= 0 && colonne >= 0 && ligne < LIGNE && colonne < COLONNE && grille.grille[ligneInitiale][colonneInitiale] == grille.grille[ligne][colonne] && grille.grille[ligneInitiale][colonneInitiale] != jeton.blanc){
            ligne -= ligneACompter;
            colonne -= colonneACompter;
            jetonGagnant++;
        }
        return jetonGagnant;
    }
    for(int i = LIGNE - 1; i >= 0; i--){
        for(int j = COLONNE - 1; j >= 0; j--){
            if(
                jetonsIdentiques(grille, i, j, 0, 1, jeton) >= 4
                ||
                jetonsIdentiques(grille, i, j, -1, 0, jeton) >= 4
                ||
                jetonsIdentiques(grille, i, j, -1, 1, jeton) >= 4
                ||
                jetonsIdentiques(grille, i, j, 1, 1, jeton) >= 4
            ){ 
                *joueurGagnant = grille.grille[i][j];
                return TRUE;
            }
        }
    }
    return FALSE;
}

int matchNull(struct Grille grille, struct Jeton jeton){
    int nombreMaxJeton(struct Grille grille, int ligneInitiale, int colonneInitiale, int ligneACompter, int colonneACompter, struct Jeton jeton, char joueur){ // Fonction qui compte le nombre de jetons identiques 
        int maxJeton = 0, ligne = ligneInitiale, colonne = colonneInitiale;
        while(ligne >= 0 && colonne >= 0 && ligne < LIGNE && colonne < COLONNE && joueur == grille.grille[ligne][colonne] && grille.grille[ligneInitiale][colonneInitiale] != jeton.blanc){
            ligne -= ligneACompter;
            colonne -= colonneACompter;
            maxJeton++;
        }
        return maxJeton;
    }
    int maxJetonSurAlignement(struct Grille grille, struct Jeton jeton, char joueur){
        for(int i = LIGNE - 1; i >= 0; i--){
            for(int j = COLONNE - 1; j >= 0; j--){
                if(
                    nombreMaxJeton(grille, i, j, 0, 1, jeton, joueur) >= 4
                    ||
                    nombreMaxJeton(grille, i, j, -1, 0, jeton, joueur) >= 4
                    ||
                    nombreMaxJeton(grille, i, j, -1, 1, jeton, joueur) >= 4
                    ||
                    nombreMaxJeton(grille, i, j, 1, 1, jeton, joueur) >= 4
                ){
                    return TRUE;
                }
            }
        }
        return FALSE;
    }
    if(maxJetonSurAlignement(grille, jeton, jeton.rouge) && maxJetonSurAlignement(grille, jeton, jeton.jaune))return TRUE;
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

void afficheStats(char joueurGagnant, int estPleine, int estNull, int gagne, struct Jeton jeton){ // Fonction qui affiche les stats du jeu une fois gagné ou match null
    printf("====================================================\n");
    if(estPleine || estNull){
        printf("Le Match entre %c vs %c est null\n", jeton.rouge, jeton.jaune);
    }else 
    if(gagne){
        printf("Le joueur: ");
        if(joueurGagnant == jeton.rouge)printf("%c ",joueurGagnant);
        if(joueurGagnant == jeton.jaune)printf("%c ",joueurGagnant);
        printf(" a remporté le Match\n");
    }
    printf("====================================================\n");
    printf("Merci! d'avoir participé au Jeu Puisance 4 au voir.\n");
    printf("====================================================\n");
}

void viderBuffer(FILE* file){ // Fonction qui permet de vider le "stdin" si jamais la valeur saisie était autre qu'un entier
    int c;
    while((c=fgetc(file)) != '\n' && c != EOF);
}
int checkFichier(char fichier1[], char fichier2[]){ // Fonction qui permet de tester si un fichier est valide
    int i = 0;
    // printf("%ld - %s : %c\n", strlen(fichierDir), fichierDir, fichierDir[strlen(fichierDir) - 4]);
    while(fichier1[i] != '\0' && fichier2[i] != '\0'){
        if(fichier1[i] != fichier2[i]) return FALSE;
        i++;
    }
    return TRUE;
}

int checkFichierDir(char fichierDir[], char fichier[]){ // Fonction qui permet de tester le fichier saisie est fichier de sauvegarde
    int i = strlen(fichierDir);
    // printf("%ld - %s : %c\n", strlen(fichierDir), fichierDir, fichierDir[strlen(fichierDir) - 4]);
    if(checkFichier(fichierDir, fichier)){
        if(i > 4 && fichierDir[i - 4] == '.' && fichierDir[i - 3] == 't')return TRUE;
        return FALSE;
    }
    return FALSE;
}
void sauvegardeJeu(char fichier[], struct Grille grille, int mode,  int *joueurNext){ // Fonction qui permet de sauvegarde une partie en cours de jeu
    int ligne, colonne;
    char c;
    if(!checkFichier(fichier, " ")){
        FILE *fichierJeuWrite = fopen(fichier, "w+");
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
    }
}

void lectureSauvegardeJeu(FILE *file, struct Grille *grille, int *mode, int *joueurNext){ // Fonction qui lit la sauvegarde et la charge dans la grille du jeu
    char c;
    int i = 0, j = 0;
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

void checkModeJeu(struct Grille grille, struct Jeton jeton, int mode, int joueurNext, int *colonne, int *tour, int *IA, int *tmp, char *joueurIA){ // Fonction qui teste le mode de jeu choisi
    int colonneTemp;
    if(mode == 1 ){
        printf(">>");
        scanf("%d",&colonneTemp);
        viderBuffer(stdin);
    }
    if(mode == 0){
         if(joueurNext == 1){
            colonneTemp = intelligenceArtifficielle(grille, jeton, jeton.rouge);
            sleep(2);
        }
        if(joueurNext == 2){
            colonneTemp = intelligenceArtifficielle(grille, jeton, jeton.jaune);
            sleep(2);
        }
    }
    if(mode == 2){
        if(*tour < 1){
            if(joueurNext == 1){
                *joueurIA == jeton.jaune;
                *IA = 2;
            }
            if(joueurNext == 2){
                *joueurIA == jeton.rouge;
                *IA = 1;
            }
            *tour = 2;
        }
        if(*IA){
            colonneTemp = intelligenceArtifficielle(grille, jeton, *joueurIA);
            sleep(2);
            printf("L'ordinateur vient de jouer\n");
            *tmp = *IA;
            *IA = 0;
        }else{
            printf(">>");
            scanf("%d",&colonneTemp);
            viderBuffer(stdin);
            *IA = *tmp;
        }
    }
    *colonne = colonneTemp;
}
void corpsJeu(char *nomFichier, struct Grille *GrilleAux, struct Grille *Grille, struct Jeton Jeton, int *joueurNext, int *estFini, int mode, int *estPleine, int *gagne, int *estNull, char *joueurGagnant, int *tour, int *IA, int *tmp, char *joueurIA){ // Fonction qui permet de tourner tant que les conditions du jeu ne sont pas remplies
    int colonne, estValideCoup = FALSE;
    char joueur;
    do{
        joueur = joueurEnCours(*joueurNext, Jeton);
        checkModeJeu(*Grille, Jeton, mode, *joueurNext, &colonne, tour, IA, tmp, joueurIA);
        if(colonne == 8)inverserGrille(Grille, joueur, Jeton);
        if(colonne == 9)tournerGrille(GrilleAux, Grille, Jeton, joueur, TRUE);
        if(colonne == 10)tournerGrille(GrilleAux, Grille, Jeton, joueur, FALSE);
        estValideCoup = placerJeton(Grille, colonne, joueur, Jeton);
        *gagne = estGagne(*Grille, Jeton, joueurGagnant);
        *estPleine = estPleineGrille(*Grille, Jeton);
        *estNull = matchNull(*Grille, Jeton);
        if(colonne == 11 || colonne == 0){
            if(colonne == 11){
                char chaine1[100];
                char chaine2[] = ".txt";
                printf("==============================================================\n");
                printf("| ==Veuillez s'il vous plaît retenir le nom de votre fichier== |\n");
                printf("==============================================================\n");
                printf("Entrez le nom du fichier de sauvegarde svp!\n");
                printf(">>");
                scanf("%s", chaine1);
                strcpy(nomFichier,strcat(chaine1, chaine2));
                sauvegardeJeu(nomFichier, *Grille, mode, joueurNext);
                printf(" Le joueur: %c vient de sauvegarder et quitter le jeu \n", joueur);
                *estFini = TRUE;
            }
            if(colonne == 0){
                printf(" Le joueur: %c a mis fin a la partie du jeu \n", joueur);
                *estFini = TRUE;

            }
            printf(" ======================================================\n");
            printf("| Merci! d'avoir participé au Jeu Puisance 4 au voir.  |\n");
            printf(" ======================================================\n");
        }
        else{
            if(estValideCoup)joueurSuivant(joueurNext);
        }
    }while(!estValideCoup && !*estPleine && !*gagne);
}
void checkFinalJeu(struct Grille grille, struct Jeton jeton, int *estFini, int *joueurNext, int estPleine, int gagne, int estNull, char joueurGagnant, int mode, char nomFichier[]){ // Fonction qui permet de verifier que toute les conditions du jeu sont remplies et met fin à la partie du jeu
    if(estPleine || estNull){
            afficheGrille(grille, mode);
            afficheStats(joueurGagnant, estPleine, estNull, gagne, jeton);
            sauvegardeJeu(nomFichier, grille, mode, joueurNext);
            *estFini = TRUE;
        }else if(gagne){
            afficheGrille(grille, mode);
            afficheStats(joueurGagnant, estPleine, estNull, gagne, jeton);
            sauvegardeJeu(nomFichier, grille, mode, joueurNext);
            *estFini = TRUE;
        }
}

void jeuPrincipal(){ // Fonction qui est l'âme du jeuestFini
    Grille Grille, GrilleAux;
    Jeton Jeton;
    initialiserJeton(&Jeton);
    initialiserGrille(&Grille, Jeton);
    initialiserGrille(&GrilleAux,Jeton);
    int colonne, estPleine, gagne, estNull, mode = 3, debut = 3, estFini = FALSE, joueurNext = 3, tour, IA, tmp;
    char joueur, joueurGagnant, nomFichier[1000] = " ", joueurIA;
    printf("\n\n");
    printf("==============================================\n");
    printf("====== Bienvenue dans le jeu Puisance 4 ======\n");
    printf("==============================================\n");
    printf("Veuillez svp suivre les instructions ci-dessous:\n");
    printf("==============================================\n");
    do{
        printf("Entrez 1 pour une nouvelle partie\n");
        printf("Entrez 2 pour continuer avec la partie en cours\n");
        printf("Entrez 0 pour quitter le jeu\n");
        printf(">>");
        scanf("%d",&debut);
        viderBuffer(stdin);
        if(debut == 0){
            printf(" ======================================================\n");
            printf("| Merci! d'avoir participé au Jeu Puisance 4 au voir.  |\n");
            printf(" ======================================================\n");
            return;
        }
        if(debut != 1 && debut != 2)printf("Choix erroné veuillez s'il vous plaît choisir une partie valide\n");
    }while(debut != 1 && debut != 2);
    void modeJeu(int *mode){ // Fonction qui affiche les messages du mode jeu a choisir
        do{
            printf("Entrez 1 pour le mode Confrontation entre humains\n");
            printf("Entrez 2 pour le mode Autonome\n");
            printf("Entrez 0 pour le mode Sans Humain\n");
            printf(">>");
            scanf("%d",mode);
            viderBuffer(stdin);
            if(*mode != 1 && *mode != 2 && *mode != 0)printf("Mode erroné veuillez s'il vous plaît faire un bon mode\n");
        }while(*mode != 1 && *mode != 2 && *mode != 0);
    }
    void confrontationJoueur(){ // Fonction qui affiche les messages la partie confrontation du jeu
        if(mode == 1){  
            printf("============================================================\n");
            printf("| ===== Vous êtes en mode confrontation entre humains ====== |\n");
            printf("=============================================================\n"); 
            printf("Faites le choix de joueurs\n"); 
            do{
                printf("Tapez 1 pour le joueur: %c\n", Jeton.rouge);
                printf("Tapez 2 pour le joueur: %c\n", Jeton.jaune);
                printf(">>");
                scanf("%d",&joueurNext);
                viderBuffer(stdin);
                if(joueurNext != 1 && joueurNext != 2)printf("Choix erroné veuillez s'il vous plaît choisir un joueur valide\n");
            }while(joueurNext != 1 && joueurNext != 2);
        }
    }
    void jeuSansHumain(){
        if(mode == 0){

            printf("==============================================\n");
            printf("| ===== Le jeu est en mode sans humain ====== |\n");
            printf("==============================================\n");
            joueurNext = 1;
        }
    }
    void jeuAutonome(){ // Fonction qui affiche les messages la partie autonome du jeu
        if(mode == 2){
            printf("==========================================\n");
            printf("| ===== Vous êtes en mode autonome ====== |\n");
            printf("==========================================\n");
            printf("Faites le choix de joueurs\n");
            do{
                printf("Tapez 1 pour le joueur: %c\n", Jeton.jaune);
                printf("Tapez 2 pour le joueur: %c\n", Jeton.rouge);
                printf(">>");
                scanf("%d",&joueurNext);
                viderBuffer(stdin);
                if(joueurNext != 1 && joueurNext != 2)printf("Choix erroné veuillez s'il vous plaît choisir un joueur valide\n");
            }while(joueurNext != 1 && joueurNext != 2);
        }
    }
    void fonctionPrincipale(){ // Fonction qui fait tourner le jeu tant qu'on a joueur qui joue
        while(!estFini){
            afficheGrille(Grille, mode);
            corpsJeu(nomFichier, &GrilleAux, &Grille, Jeton, &joueurNext, &estFini,  mode, &estPleine, &gagne, &estNull, &joueurGagnant, &tour, &IA, &tmp, &joueurIA);
            checkFinalJeu(Grille, Jeton, &estFini, &joueurNext, estPleine, gagne, estNull, joueurGagnant, mode, nomFichier);
        }
    }

    int lectureFichier(const char *nom, int indent, char fichier[]){ // Fonction qui permet de lire tous les fichiers dans mon repertoire de jeu
        DIR *dir;
        struct dirent *entree;
        int check = FALSE;
        if (!(dir = opendir(nom)))
            return FALSE;
        while ((entree = readdir(dir)) != NULL) {
            if (entree->d_type == DT_DIR) {
                char path[1024];
                if (strcmp(entree->d_name, ".") == 0 || strcmp(entree->d_name, "..") == 0)
                    continue;
                snprintf(path, sizeof(path), "%s/%s", nom, entree->d_name);
                lectureFichier(path, indent + 2, fichier);
            } else {
                if(checkFichierDir(entree->d_name, fichier)){
                    check = TRUE;
                }
            }
        }
        closedir(dir);
        if(check)return check;
        else return check;
    }
    void secondeChance(struct Grille *grille, struct Jeton jeton){ //Fonction qui permet de donner une seconde chance au joueur une fois qu'il tente de charger un fichier inexistant
        int choix;
        printf("Voulez-vous continuer à une nouvelle partie ?\n");
        printf("1 = OUI\n");
        printf("0 = NON\n");
        printf(">>");
        scanf("%d", &choix);
        if(choix){
            initialiserGrille(grille, jeton);
            modeJeu(&mode);
            confrontationJoueur();
            jeuAutonome();
            jeuSansHumain();
            fonctionPrincipale();
        }else{
            printf(" ======================================================\n");
            printf("| Merci! d'avoir participé au Jeu Puisance 4 au voir.  |\n");
            printf(" ======================================================\n");
        }
    }
    if(debut == 1){
        modeJeu(&mode);
        if(mode == 2){
            jeuAutonome();
            fonctionPrincipale();
        }
        if(mode == 1){
            confrontationJoueur();
            fonctionPrincipale();
        }
        if(mode == 0){
            jeuSansHumain();
            fonctionPrincipale();
        }
    }
    if(debut == 2){
        char chaine1[100];
        char chaine2[] = ".txt";
        printf("Entrez le  nom de votre fichier de sauvegarde\n");
        printf(">>");
        scanf("%s", chaine1);
        strcpy(nomFichier,strcat(chaine1, chaine2));
        if(!lectureFichier("./", 0, nomFichier)){
            printf("Desole ce fichier n'existe pas\n");
            strcpy(nomFichier, " ");
            secondeChance(&Grille, Jeton);

        }else{
            FILE  *fichierJeuRead = fopen(nomFichier, "r+");
            lectureSauvegardeJeu(fichierJeuRead, &Grille, &mode, &joueurNext);
            fclose(fichierJeuRead);
            gagne = estGagne(Grille, Jeton, &joueurGagnant);
            estPleine = estPleineGrille(Grille, Jeton);
            estNull = matchNull(Grille, Jeton);
            if(gagne || estPleine || estNull){
                printf("=======================================\n");
                printf("Désolé cette partie est terminée.\n");
                printf("=======================================\n");
                secondeChance(&Grille, Jeton);
            }else{
                fonctionPrincipale();
            }
        }
    }
}