struct Grille{
    char grille[7][7];
};
struct Jeton{
    char blanc;
    char rouge;
    char jaune;
};

typedef struct Grille Grille; 
typedef struct Jeton Jeton;
void jeuPrincipal();