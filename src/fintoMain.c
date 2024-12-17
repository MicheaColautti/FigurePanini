#include <stdio.h>
#include <stdlib.h>

// Numero massimo di giorni e giocatori
#define MAX_DAYS 1000
#define MAX_PLAYERS 4
#define MAX_MONEY 10000  // Precisione: moltiplichiamo i valori in denaro per 100 (per usare interi)

// Funzione per trovare il massimo tra due valori
double max(double a, double b) {
    return (a > b) ? a : b;
}

// Funzione ricorsiva per calcolare il massimo profitto
double maxProfit(int prices[][MAX_PLAYERS * 2], int day, int days, double money) {
    if (day >= days) {
        return money;  // Se non ci sono più giorni, restituisci il denaro accumulato
    }

    double max_profit = maxProfit(prices, day + 1, days, money);  // Non fare nulla

    // Esplora tutte le possibilità di acquisto per ciascun giocatore
    for (int i = 0; i < MAX_PLAYERS; i++) {
        double buy_price = prices[day][i * 2];        // Prezzo di acquisto
        double sell_price = prices[day][i * 2 + 1];   // Prezzo di vendita

        // Se posso permettermi di acquistare almeno una frazione della carta
        if (buy_price > 0) {
            double fraction_owned = money / buy_price;         // Quante frazioni della carta posso comprare
            double new_money = fraction_owned * sell_price;    // Quanto denaro otterrò vendendo la frazione
            max_profit = max(max_profit, maxProfit(prices, day + 1, days, new_money));
        }
    }

    return max_profit;
}

int main() {
    // Matrice di prezzi: ogni riga è un giorno, ogni coppia di colonne è prezzo acquisto/vendita per un giocatore
    int prices[][MAX_PLAYERS * 2] = {
        {3, 2, 4, 3, 5, 4, 6, 5},  // Giorno 0: Giocatore 0 (3,2), Giocatore 1 (4,3), ecc.
        {5, 4, 6, 5, 7, 6, 8, 7},  // Giorno 1
        {6, 5, 7, 6, 8, 7, 9, 8},  // Giorno 2
        {2, 1, 3, 2, 4, 3, 5, 4}   // Giorno 3
    };

    int days = sizeof(prices) / sizeof(prices[0]);
    double initial_money = 1.0;  // Partiamo con 1 dollaro

    // Calcola il massimo profitto possibile partendo dal giorno 0 con 1 dollaro
    double result = maxProfit(prices, 0, days, initial_money);

    printf("Il massimo profitto ottenibile è: %.2f dollari\n", result);

    return 0;
}
