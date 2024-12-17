#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

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

int** createMatrix(int* data, int days, int cols) {
    int** matrix = (int**)malloc(days * sizeof(int*));
    if (matrix == NULL) {
        printf("Errore di allocazione della memoria per la matrice.\n");
        exit(1);
    }

    for (int i = 0; i < days; i++) {
        matrix[i] = (int*)malloc(cols * sizeof(int));
        if (matrix[i] == NULL) {
            printf("Errore di allocazione della memoria per la riga %d della matrice.\n", i);
            exit(1);
        }

        for (int j = 0; j < cols; j++) {
            matrix[i][j] = data[i * cols + j];
        }
    }

    return matrix;
}

int main() {
    const char* fileName = "../instances/instance_100_100.txt";
    int* data = NULL;
    int dataSize = 0;
    int days = 0, tradingCards = 0;

    // Read data from the file
    readFile(fileName, &data, &dataSize);

    // Initialize the number of days and trading cards
    initializeValues(data, &days, &tradingCards);
    int cols = tradingCards * 2; // Each card has a buy and sell price

    // Create the matrix with all data starting from data[2]
    int** matrix = createMatrix(data + 2, days, cols);

    //tempo iniziale
    struct timespec start, end;
    // Ottieni il tempo di inizio
    clock_gettime(CLOCK_MONOTONIC, &start);

    int N = days;
    int F = tradingCards;

    // Dynamically allocate buy_price and sell_price arrays
    int** buy_price = (int**)malloc(N * sizeof(int*));
    int** sell_price = (int**)malloc(N * sizeof(int*));
    if (buy_price == NULL || sell_price == NULL) {
        printf("Errore di allocazione della memoria.\n");
        exit(1);
    }

    for (int i = 0; i < N; i++) {
        buy_price[i] = (int*)malloc(F * sizeof(int));
        sell_price[i] = (int*)malloc(F * sizeof(int));
        if (buy_price[i] == NULL || sell_price[i] == NULL) {
            printf("Errore di allocazione della memoria.\n");
            exit(1);
        }
    }

    // Assign the buy and sell prices correctly
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < F; j++) {
            int colOffset = j * 2;
            // Since your input is Vi Ci, we swap them to Ci Vi
            buy_price[i][j] = matrix[i][colOffset + 1]; // Ci,j (buy price)
            sell_price[i][j] = matrix[i][colOffset];    // Vi,j (sell price)
        }
    }

    // Dynamically allocate dp array
    double** dp = (double**)malloc((N + 1) * sizeof(double*));
    if (dp == NULL) {
        printf("Errore di allocazione della memoria.\n");
        exit(1);
    }
    for (int i = 0; i <= N; i++) {
        dp[i] = (double*)malloc((F + 1) * sizeof(double));
        if (dp[i] == NULL) {
            printf("Errore di allocazione della memoria.\n");
            exit(1);
        }
    }

    // Initialize DP table
    dp[0][0] = 1.0; // Start with 1 CHF
    for (int j = 1; j <= F; j++) {
        dp[0][j] = 0.0; // No cards held initially
    }

    // Compute DP table
    for (int i = 1; i <= N; i++) {
        // For cash on day i
        dp[i][0] = dp[i - 1][0]; // Start with previous day's cash
        for (int j = 1; j <= F; j++) {
            // Consider selling holdings from the previous day
            double cash_from_selling = dp[i - 1][j] * sell_price[i - 1][j - 1];
            if (cash_from_selling > dp[i][0]) {
                dp[i][0] = cash_from_selling;
            }
        }

        // For each card on day i
        for (int j = 1; j <= F; j++) {
            dp[i][j] = dp[i - 1][j]; // Start with previous day's holdings
            // Use dp[i][0], which includes cash from possible sales today
            double card_bought = dp[i][0] / buy_price[i - 1][j - 1];
            if (card_bought > dp[i][j]) {
                dp[i][j] = card_bought;
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    long seconds = end.tv_sec - start.tv_sec;
    long nanoseconds = end.tv_nsec - start.tv_nsec;
    double elapsed = seconds * 1000.0 + nanoseconds / 1.0e6;
    printf("Tempo trascorso: %.3f ms\n", elapsed);

    // The final capital is dp[N][0]
    printf("%.2lf\n", dp[N][0]);

    // Free allocated memory
    for (int i = 0; i < days; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(data);

    for (int i = 0; i < N; i++) {
        free(buy_price[i]);
        free(sell_price[i]);
    }
    free(buy_price);
    free(sell_price);

    for (int i = 0; i <= N; i++) {
        free(dp[i]);
    }
    free(dp);

    return 0;
}