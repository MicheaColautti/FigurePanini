#include <stdio.h>
#include <stdlib.h>

#include <errno.h> // Per ottenere il codice di errore
#include <string.h> // Per stampare una descrizione dell'errore
#include <sys/syslimits.h>
#include <unistd.h>

void readFile(const char* fileName, int** data, int* size) {

    // Opening file
    FILE* file;

    file = fopen(fileName, "r");
    if (file == NULL) {
        // Stampa il codice di errore e una descrizione dell'errore
        printf("Errore nell'apertura del file: %s\n", strerror(errno));
        exit(1);
    }

    int temp, count = 0;
    while (fscanf(file, "%d", &temp) != EOF) {
        count++;
    }

    *data = (int*)malloc(count * sizeof(int));
    if (*data == NULL) {
        printf("Errore di allocazione della memoria.\n");
        exit(1);
    }

    rewind(file);
    for (int i = 0; i < count; i++) {
        fscanf(file, "%d", &((*data)[i]));
    }

    *size = count;
    fclose(file);
}

// Funzione per estrarre i primi due valori e assegnarli a days e tradingCards
void initializeValues(int* data, int* days, int* tradingCards) {
    *days = data[0];
    *tradingCards = data[1];
}

// Funzione per creare una matrice a partire dai dati
int** createMatrix(int* data, int days, int tradingCards) {
    int** matrix = (int**)malloc(days * sizeof(int*));
    if (matrix == NULL) {
        printf("Errore di allocazione della memoria per la matrice.\n");
        exit(1);
    }

    for (int i = 0; i < days; i++) {
        matrix[i] = (int*)malloc(tradingCards * sizeof(int));
        if (matrix[i] == NULL) {
            printf("Errore di allocazione della memoria per la riga %d della matrice.\n", i);
            exit(1);
        }

        for (int j = 0; j < tradingCards; j++) {
            matrix[i][j] = data[i * tradingCards + j];
        }
    }

    return matrix;
}


int main() {
    const char* fileName = "../data/input.txt";
    int* data = NULL;
    int dataSize = 0;
    int days = 0, tradingCards = 0;

    readFile(fileName, &data, &dataSize);
    initializeValues(data, &days, &tradingCards);
    int** matrix = createMatrix(data + 2, days, tradingCards);

    // Stampa la matrice
    printf("Days: %d\n",days);
    printf("TradingCards: %d\nData:\n",tradingCards);
    for (int i = 0; i < days; i++) {
        for (int j = 0; j < tradingCards; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }





    //fixme implement function to get deals



    // Libera la memoria
    for (int i = 0; i < days; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(data);

    return 0;
}

