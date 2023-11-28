#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int main() {
    // Utilisation des fonctions
    TeamData* teams = charger_donnees("FIFA-2022.txt");
    char** lines = supprimer_entete("FIFA-2022.txt");

    // Affichage des données sous forme de tableau
    printf("| %-4s | %-15s | %-13s | %-3s | %-4s | %-4s | %-8s | %-8s | %-15s | %-6s |\n",
           "Pos", "Equipe", "Matchs Joués", "G", "N", "P", "Buts Pour", "Buts Contre", "Différence", "Points");

    for (int j = 0; j < MAX_TEAMS; j++) {
        printf("| %-4d | ", teams[j].position);
        afficher_nom_trois_lettres(teams[j].team);
        printf(" | %-13d | %-3d | %-4d | %-4d | %-8d | %-8d | %-15d | %-6d |\n",
               teams[j].gamesPlayed,
               teams[j].win,
               teams[j].draw,
               teams[j].loss,
               teams[j].goalsFor,
               teams[j].goalsAgainst,
               teams[j].goalDifference,
               teams[j].points);
    }

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
