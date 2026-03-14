#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_GOALS 4  // Les buts possibles vont de 0 à 3

// Structure pour représenter une équipe
typedef struct {
    char* name;
    int points;
    int goals_for;
    int goals_against;
} Team;

// Simulation d'un match de groupe (match régulier avec possibilité de match nul)
void simulerMatchGroupe(Team *team1, Team *team2) {
    int buts1 = rand() % MAX_GOALS;
    int buts2 = rand() % MAX_GOALS;
    
    // Mise à jour des statistiques
    team1->goals_for += buts1;
    team1->goals_against += buts2;
    team2->goals_for += buts2;
    team2->goals_against += buts1;
    
    // Attribution des points et affichage du résultat
    if(buts1 > buts2) {
        team1->points += 3;
        printf("Match : %s bat %s %d - %d\n", team1->name, team2->name, buts1, buts2);
    } else if(buts2 > buts1) {
        team2->points += 3;
        printf("Match : %s bat %s %d - %d\n", team2->name, team1->name, buts2, buts1);
    } else {
        team1->points += 1;
        team2->points += 1;
        printf("Match : %s et %s font match nul %d - %d\n", team1->name, team2->name, buts1, buts2);
    }
}

// Fonction de comparaison pour le classement dans le groupe
int comparerEquipe(const void *a, const void *b) {
    Team *t1 = (Team *)a;
    Team *t2 = (Team *)b;
    if(t2->points != t1->points)
        return t2->points - t1->points;
    int diff1 = t1->goals_for - t1->goals_against;
    int diff2 = t2->goals_for - t2->goals_against;
    if(diff2 != diff1)
        return diff2 - diff1;
    return t2->goals_for - t1->goals_for;
}

// Simulation de la phase de groupes pour un groupe donné
void simulerGroupe(char* groupes[8][4], int groupeIndex, Team equipeGroupe[4]) {
    // Initialisation des équipes du groupe
    for(int i = 0; i < 4; i++){
        equipeGroupe[i].name = groupes[groupeIndex][i];
        equipeGroupe[i].points = 0;
        equipeGroupe[i].goals_for = 0;
        equipeGroupe[i].goals_against = 0;
    }
    
    printf("\n=== Simulation du Groupe %d ===\n", groupeIndex + 1);
    // Les 6 rencontres du groupe : 0 vs 1, 0 vs 2, 0 vs 3, 1 vs 2, 1 vs 3, 2 vs 3
    simulerMatchGroupe(&equipeGroupe[0], &equipeGroupe[1]);
    simulerMatchGroupe(&equipeGroupe[0], &equipeGroupe[2]);
    simulerMatchGroupe(&equipeGroupe[0], &equipeGroupe[3]);
    simulerMatchGroupe(&equipeGroupe[1], &equipeGroupe[2]);
    simulerMatchGroupe(&equipeGroupe[1], &equipeGroupe[3]);
    simulerMatchGroupe(&equipeGroupe[2], &equipeGroupe[3]);
    
    // Tri du classement selon les points, la différence de buts et les buts marqués
    qsort(equipeGroupe, 4, sizeof(Team), comparerEquipe);
    
    printf("\nClassement du Groupe %d :\n", groupeIndex + 1);
    for(int i = 0; i < 4; i++){
        int diff = equipeGroupe[i].goals_for - equipeGroupe[i].goals_against;
        printf("%d. %s - Points: %d, GF: %d, GA: %d, Diff: %d\n", 
               i + 1, equipeGroupe[i].name, equipeGroupe[i].points, 
               equipeGroupe[i].goals_for, equipeGroupe[i].goals_against, diff);
    }
}

// Simulation d'un match à élimination directe (pas de match nul : en cas d'égalité, passage aux tirs au but)
Team simulerMatchKnockout(Team team1, Team team2) {
    int buts1 = rand() % MAX_GOALS;
    int buts2 = rand() % MAX_GOALS;
    
    printf("\nMatch : %s vs %s\n", team1.name, team2.name);
    printf("Score réglementaire : %d - %d\n", buts1, buts2);
    
    if(buts1 == buts2) {
        // En cas d'égalité, simuler un passage aux tirs au but
        printf("Match nul à la fin du temps réglementaire. Passage aux tirs au but...\n");
        int penaltyWinner = rand() % 2;
        if(penaltyWinner == 0) {
            printf("Après les tirs au but, %s l'emporte !\n", team1.name);
            return team1;
        } else {
            printf("Après les tirs au but, %s l'emporte !\n", team2.name);
            return team2;
        }
    } else if(buts1 > buts2) {
        printf("%s remporte le match !\n", team1.name);
        return team1;
    } else {
        printf("%s remporte le match !\n", team2.name);
        return team2;
    }
}

int main() {
    srand(time(NULL));
    
    // Définition des groupes avec des équipes réelles
    char* groupes[8][4] = {
        {"PSG", "Lyon", "Marseille", "Monaco"},
        {"Lille", "Rennes", "Saint-Etienne", "Nice"},
        {"Bordeaux", "Nantes", "Strasbourg", "Reims"},
        {"Lens", "Montpellier", "Angers", "Toulouse"},
        {"Guingamp", "Brest", "Metz", "Dijon"},
        {"Caen", "Le Havre", "Nîmes", "Auxerre"},
        {"Racing", "Troyes", "Grenoble", "Clermont"},
        {"Lorient", "Amiens", "Quevilly", "Chamois Niortais"}
    };
    
    // Phase de groupes : on retient les deux premières équipes de chaque groupe
    Team qualified[16];
    int qualIndex = 0;
    for(int g = 0; g < 8; g++){
        Team equipeGroupe[4];
        simulerGroupe(groupes, g, equipeGroupe);
        // Les deux premières équipes du groupe se qualifient
        qualified[qualIndex++] = equipeGroupe[0];
        qualified[qualIndex++] = equipeGroupe[1];
    }
    
    printf("\n=== Equipes qualifiées pour les phases éliminatoires ===\n");
    for (int i = 0; i < 16; i++) {
        printf("%d. %s\n", i + 1, qualified[i].name);
    }
    
    // Phase à élimination directe
    // 1. Round of 16
    Team round16[8];
    printf("\n=== Phase des 16èmes de finale ===\n");
    for(int i = 0; i < 16; i += 2) {
        round16[i / 2] = simulerMatchKnockout(qualified[i], qualified[i + 1]);
    }
    
    // 2. Quart de finale
    Team quarts[4];
    printf("\n=== Quart de finale ===\n");
    for(int i = 0; i < 8; i += 2) {
        quarts[i / 2] = simulerMatchKnockout(round16[i], round16[i + 1]);
    }
    
    // 3. Demi-finales
    Team semis[2];
    printf("\n=== Demi-finales ===\n");
    for(int i = 0; i < 4; i += 2) {
        semis[i / 2] = simulerMatchKnockout(quarts[i], quarts[i + 1]);
    }
    
    // 4. Finale
    printf("\n=== Finale ===\n");
    Team champion = simulerMatchKnockout(semis[0], semis[1]);
    printf("\n*** Champion du Tournoi : %s ***\n", champion.name);
    
    // Enregistrement des résultats finaux dans un fichier
    FILE *fichier = fopen("resultats_tournoi.txt", "w");
    if(fichier) {
        fprintf(fichier, "Champion du Tournoi : %s\n", champion.name);
        fclose(fichier);
        printf("\nLes résultats du tournoi ont été enregistrés dans 'resultats_tournoi.txt'\n");
    } else {
        printf("\nErreur lors de l'ouverture du fichier pour l'enregistrement.\n");
    }
    
    return 0;
}
