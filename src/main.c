#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/syslimits.h>
#include <unistd.h>

void readFile(const char* fileName, int** data, int* size) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
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

void initializeValues(int* data, int* days, int* tradingCards) {
    *days = data[0];
    *tradingCards = data[1];
}

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

int** returnCardData(int** matrix, int rows, int colOffset) {
    int** playerData = (int**)malloc(rows * sizeof(int*));
    if (playerData == NULL) {
        printf("Errore di allocazione della memoria per playerData.\n");
        exit(1);
    }

    for (int i = 0; i < rows; i++) {
        playerData[i] = (int*)malloc(2 * sizeof(int));
        if (playerData[i] == NULL) {
            printf("Errore di allocazione della memoria per la riga %d di playerData.\n", i);
            exit(1);
        }

        playerData[i][0] = matrix[i][colOffset+1];
        playerData[i][1] = matrix[i][colOffset];
    }

    return playerData;
}


double getMaxDeal(int* matrix, int buy, int sell){





}

int main() {
    const char* fileName = "../instances/instance_10_10.txt";
    int* data = NULL;
    int dataSize = 0;
    int days = 0, tradingCards = 0;

    readFile(fileName, &data, &dataSize);
    initializeValues(data, &days, &tradingCards);
    int cols = tradingCards * 2;

    int** matrix = createMatrix(data + 2, days, cols);

    printf("Days: %d\n", days);
    printf("TradingCards: %d\nData:\n", tradingCards);

    for (int i = 0; i < days; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    for (int x = 0; x < tradingCards; x++) {
        int colOffset = x * 2;
        int** cardData = returnCardData(matrix, days, colOffset);

        printf("CardData for columns %d and %d:\n", colOffset, colOffset + 1);
        for (int i = 0; i < days; i++) {
            for (int j = 0; j < 2; j++) {

                printf("%d ", cardData[i][j]);
            }
            printf("\n");
        }
        printf("\n");


        double deal=getMaxDeal(matrix,0,0);


        /*
         * NB Here we have the single matrix (Daysx2) with the data of a single player
         * the data HAS BEEN INVERTED BY US, SO IT'S [BuyPrice, SellPrice] where SellPrice≤BuyPrice
         **/
        //FIXME

        //pass to the recursive function the matrix of the current playerData.


    }

    for (int i = 0; i < days; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(data);

    return 0;
}
