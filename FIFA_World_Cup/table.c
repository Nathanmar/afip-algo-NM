#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_TEAMS 32
#define MAX_LINE_LENGTH 100

typedef struct {
    int position;
    char team[50];
    int gamesPlayed;
    int win;
    int draw;
    int loss;
    int goalsFor;
    int goalsAgainst;
    int goalDifference;
    int points;
} TeamData;

// Déclaration des fonctions
TeamData* charger_donnees(const char* fichier);
char** supprimer_entete(const char* fichier);
void afficher_nom_trois_lettres(const char* str);
void enregistrer_fichier(TeamData* teams, int numTeams, const char* nomFichier);
char** melanger_lignes(char** lines, int numLines);


int main() {
    // Utilisation des fonctions
    TeamData* teams = charger_donnees("FIFA-2022.txt");
    char** lines = supprimer_entete("FIFA-2022.txt");

    char** lignes_melangees = melanger_lignes(lines, MAX_TEAMS);

    // Affichage des données sous forme de tableau
    printf("| %-4s | %-4s | %-10s | %-3s | %-4s | %-4s | %-8s | %-8s | %-5s | %-6s |\n",
           "Pos", "Team", "Matchs Joués", "G", "N", "P", "Buts Pour", "Buts Contre", "Diff", "Points");

    for (int j = 0; j < MAX_TEAMS; j++) {
        printf("| %-4d | ", teams[j].position);
        afficher_nom_trois_lettres(teams[j].team);
        printf(" | %-13d | %-3d | %-4d | %-4d | %-9d | %-11d | %-5d | %-6d |\n",
               teams[j].gamesPlayed,
               teams[j].win,
               teams[j].draw,
               teams[j].loss,
               teams[j].goalsFor,
               teams[j].goalsAgainst,
               teams[j].goalDifference,
               teams[j].points);
    }

    // Enregistrement des données dans un nouveau fichier
    enregistrer_fichier(teams, MAX_TEAMS, "nouveau_fichier.txt");

    // Libération de la mémoire
    free(teams);
    for (int i = 0; i < MAX_TEAMS; i++) {
        free(lines[i]);
    }
    free(lines);

    return 0;
}



// Définition des fonctions

TeamData* charger_donnees(const char* fichier) {
    FILE *file = fopen(fichier, "r");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    TeamData* teams = malloc(MAX_TEAMS * sizeof(TeamData));
    if (teams == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Lecture des données du fichier
    int i = 0;
    char line[MAX_LINE_LENGTH];

    // Ignorer la première ligne (en-têtes)
    fgets(line, MAX_LINE_LENGTH, file);

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && i < MAX_TEAMS) {
        sscanf(line, "%d,%49[^,],%d,%d,%d,%d,%d,%d,%d,%d",
               &teams[i].position,
               teams[i].team,
               &teams[i].gamesPlayed,
               &teams[i].win,
               &teams[i].draw,
               &teams[i].loss,
               &teams[i].goalsFor,
               &teams[i].goalsAgainst,
               &teams[i].goalDifference,
               &teams[i].points);
        i++;
    }

    fclose(file);
    return teams;
}

char** supprimer_entete(const char* fichier) {
    FILE *file = fopen(fichier, "r");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    // Compter le nombre de lignes dans le fichier
    int linesCount = 0;
    char ch;
    while (!feof(file)) {
        ch = fgetc(file);
        if (ch == '\n') {
            linesCount++;
        }
    }

    // Aller au début du fichier
    rewind(file);

    // Allouer de la mémoire pour stocker chaque ligne
    char** lines = malloc(linesCount * sizeof(char*));
    if (lines == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Lire chaque ligne du fichier
    int i = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL && i < linesCount) {
        // Allouer de la mémoire pour stocker la ligne
        lines[i] = malloc(strlen(line) + 1);
        if (lines[i] == NULL) {
            perror("Erreur d'allocation de mémoire");
            exit(1);
        }

        // Copier la ligne dans le tableau
        strcpy(lines[i], line);
        i++;
    }

    fclose(file);
    return lines;
}

void afficher_nom_trois_lettres(const char* str) {
    // Affiche les trois premières lettres du nom en majuscules
    for (int i = 0; i < 3 && str[i] != '\0'; i++) {
        putchar(toupper(str[i]));
    }
}


void enregistrer_fichier(TeamData* teams, int numTeams, const char* nomFichier) {
    FILE *file = fopen(nomFichier, "w");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }

    // Écriture de l'en-tête dans le fichier
    fprintf(file, "Position,Team,Games Played,Win,Draw,Loss,Goals For,Goals Against,Goal Difference,Points\n");

    // Écriture des données dans le fichier
    for (int i = 0; i < numTeams; i++) {
        fprintf(file, "%d,%s,%d,%d,%d,%d,%d,%d,%d,%d\n",
                teams[i].position,
                teams[i].team,
                teams[i].gamesPlayed,
                teams[i].win,
                teams[i].draw,
                teams[i].loss,
                teams[i].goalsFor,
                teams[i].goalsAgainst,
                teams[i].goalDifference,
                teams[i].points);
    }

    fclose(file);
}

char** melanger_lignes(char** lines, int numLines) {
    // Allouer de la mémoire pour copier les lignes
    char** lignes_copie = malloc(numLines * sizeof(char*));
    if (lignes_copie == NULL) {
        perror("Erreur d'allocation de mémoire");
        exit(1);
    }

    // Copier les lignes dans le tableau de copie
    for (int i = 0; i < numLines; i++) {
        lignes_copie[i] = malloc(strlen(lines[i]) + 1);
        if (lignes_copie[i] == NULL) {
            perror("Erreur d'allocation de mémoire");
            exit(1);
        }
        strcpy(lignes_copie[i], lines[i]);
    }

    // Initialiser le générateur de nombres aléatoires avec une graine basée sur l'heure
    srand((unsigned int)time(NULL));

    // Appliquer l'algorithme de Fisher-Yates pour mélanger les lignes
    for (int i = numLines - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Échanger les lignes
        char* temp = lignes_copie[i];
        lignes_copie[i] = lignes_copie[j];
        lignes_copie[j] = temp;
    }

    return lignes_copie;
}
