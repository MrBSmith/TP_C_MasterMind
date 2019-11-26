#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define NB_CHIFFRE 4

#define FLASE 0
#define TRUE 1

#define VERT 2
#define JAUNE 14
#define ROUGE 12
#define NOIR 0
#define BLANC 15

typedef int placement;
enum{wrong, badlyPlaced, wellPlaced, Empty};

// Prototypes
void initialize_result(int n_Result[NB_CHIFFRE]);
void print_array(int *array);
void player_input(int *n_player_try, int nombre);
void get_results(int *n_Player_try, int *n_Result, placement *pla_Result);
int check_victory(placement *pla_Result);
void color(int t, int f);
void print_result(int *n_Player_try, placement *pla_Result);

int main(){

    srand(time(NULL));

    // Define arrays
    int n_Result[NB_CHIFFRE];
    int n_Player_try[NB_CHIFFRE];
    placement pla_Result[NB_CHIFFRE];

    // Define variables
    int n_max_tries = 12;
    int n_tries_left;

    // Initialize the expected result to discover randomly
    initialize_result(n_Result);

    /*
    // Print it (Debug purpose)
     print_array(n_Result);
    */

    // Game loop procedure
    for(n_tries_left = n_max_tries; n_tries_left > 0; n_tries_left--){

        if(n_tries_left != n_max_tries){
            printf("\nEncore %d essais\n", n_tries_left);
        } else {
            printf("J'ai cree une suite de %d chiffres, saurez vous la trouver?\n\n", NB_CHIFFRE);
        }

        // Ask for the player to enter NB_CHIFFRE numbers
        player_input(n_Player_try, NB_CHIFFRE);

        // Compare the player's input with the expected result
        get_results(n_Player_try, n_Result, pla_Result);

        // Print the results
        print_result(n_Player_try, pla_Result);

        // Check for victory
        if(check_victory(pla_Result) == TRUE){
            printf("Vous avez gagne!\n");
            return 0;
        } else {
            // Check for defeat
            if(n_tries_left <= 0){
                printf("Vous avez perdu!\n");
            }
        }

    }

    return 0;
}

// Print the given array (not currently used in the code, may be usefull for debugging)
void print_array(int *array){
    for(int i=0; i<NB_CHIFFRE; i++){
        printf("%d", array[i]);
    }
    printf("\n");
}

// Initialize the asked result with random integer between 0 and 9
void initialize_result(int *n_Result){
    for(int i=0; i<NB_CHIFFRE; i++){
        n_Result[i] = rand() % 10;
    }
}

// Ask the player to enter NB_CHIFFRE numbers, and stock it in the array n_player_tries
void player_input(int *n_Player_try, int nombre){

    printf("Entrez %d entiers en 0 et 9:\n", NB_CHIFFRE);

    for(int i=0; i<nombre; i++){
        do{
            char c = getch();

            if((int)c == 8){ // If the player hit backspace
                if(i > 0){
                    // set the previous value to null, and go back to the previous iteration, so the player can overwrite his previous input
                    i--;
                    n_Player_try[i] = (int)NULL;
                    i--;
                }
                printf("\b");
            } else{ // If the player has entered anything else
                 n_Player_try[i] = c - 48;
                if (n_Player_try[i] >= 0 || n_Player_try[i] <= 9){ // If the player hit a number between 0 and 9
                    printf("%d", n_Player_try[i]);
                } else {
                    i--;
                }
            }

        } while(n_Player_try[i] < 0 || n_Player_try[i] > 9);
    }

    printf("\n");
}

// Check the placement of each numbers in the player's try, and stock the results in the array pla_result
void get_results(int *n_Player_try, int *n_Result, placement *pla_Result){

    for(int i = 0; i < NB_CHIFFRE; i++){
        pla_Result[i] = Empty; // initialize the pla_result current cell at Empty

        // If the current cell of the player inputs array is equal to the result array, stock wellPlaced in pla_result
        if(n_Player_try[i] == n_Result[i]){
            pla_Result[i] = wellPlaced;
        } else {
            // In the other case, check if the number exist elsewhere in the array, and check for any double. Stock badly placed where appropriate.
            for(int j = 0; j < NB_CHIFFRE; j++){
                if(n_Player_try[i] == n_Result[j] && n_Player_try[j] != n_Result[j]){
                    pla_Result[i] = badlyPlaced;
                }
            }

            // If the current cell result is neither wellPlaced and badlyPlaced, set it to wrong
            if(pla_Result[i] == Empty){
                pla_Result[i] = wrong;
            }
        }
    }
}

// Check for the victory, return TRUE if the player wins, FALSE if not
int check_victory(placement *pla_Result){
    for(int i = 0; i < NB_CHIFFRE; i++){
        if(pla_Result[i] != wellPlaced){
            return FALSE;
        }
    }
    return TRUE;
}

// Define colors
void color(int t, int f){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole,f*16+t);
}

// Print the results in color, and a review of the results, to help the player understand it
void print_result(int *n_Player_try, placement *pla_Result){

    int n_wellPlaced_num = 0;
    int n_badlyPlaced_num = 0;
    int n_wrong_num = 0;

    // Print the results itself in the write color, depending on the nature of its result (wellPlced, badlyPlaced or wrong)
    for(int i = 0; i < NB_CHIFFRE; i++){

        switch(pla_Result[i]){
        case wellPlaced:
            color(BLANC, VERT);
            n_wellPlaced_num++; break;
        case badlyPlaced:
            color(NOIR, JAUNE);
            n_badlyPlaced_num++; break;
        case wrong:
            color(BLANC, ROUGE);
            n_wrong_num++; break;
        }

        printf("%d", n_Player_try[i]);
    }

    // Print the review of the results
    color(BLANC, NOIR);
    printf("\nVous avez ");
    color(BLANC, VERT);
    printf("%d nombre(s) bien place(s), ", n_wellPlaced_num);
    color(NOIR, JAUNE);
    printf("%d nombre(s) mal(s) place(s), ", n_badlyPlaced_num);
    color(BLANC, ROUGE);
    printf("%d nombre(s) faux\n", n_wrong_num);
    color(BLANC, NOIR);
}
