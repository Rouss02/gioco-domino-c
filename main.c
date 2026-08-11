#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 21    // Numero totale di tessere
#define N 10    // Numero di tessere date all'utente
#define A 6     // Limite del valore delle tessere

typedef struct {
    int sx;
    int dx;
} tessera;

void f_carica(tessera* domino);
void f_generazione_tessere_utente(int* tessere_uscite, tessera* domino);
int f_controllo_tessere(int i, char pos, int* tessere_utente, int* tessere_tavolino, int* dim_utente, int* dim_tavolino, tessera* domino);
void f_richiesta_utente(int* tess, char* pos, int dim_tavolino);
int f_gioco(tessera* domino, int* tessere_utente, int* tessere_tavolino, int* dim_utente, int* dim_tavolino, int tess, char pos);
int f_calcolo_punti(tessera* domino, int* tessere_tavolino, int dim_tavolino);
void swap(tessera* domino, int u);
int f_endgame(tessera* domino, int* tessere_utente, int* tessere_tavolino, int dim_utente, int dim_tavolino);
int f_controllo_endgame(tessera* domino, int* tessere_utente, int dim_utente, int valore);
void stampa(tessera* domino, int* tessere_utente, int* tessere_tavolino, int dim_utente, int dim_tavolino, int punti, int flag);
void pulisci_schermo(void);

// AI
tessera f_conta(tessera* domino, int* tessere_utente, int dim_utente, int numero);
int f_inizio_gioco(int* tessere_utente, int dim_utente, tessera massimo, tessera* domino);
int f_avanzamento_gioco(int* tessere_utente, int* tessere_tavolino, int dim_utente, int dim_tavolino, tessera* domino, char* testa_coda);

int main() {
    tessera domino[M];
    tessera ripetizioni;
    int tessere_utente[N];
    int tessere_tavolino[M];
    int dim_utente = N, dim_tavolino = 0, flag = 1, punti = 0, tessera_scelta = 0;
    char scelta = 'i', testa_coda = 't';

    srand((unsigned int)time(NULL));
    f_carica(domino);
    f_generazione_tessere_utente(tessere_utente, domino);

    printf("Seleziona modalita' (i = interattiva, A = AI): ");
    if (scanf(" %c", &scelta) != 1) scelta = 'i';
    while (getchar() != '\n');

    if (scelta == 'i') {
        while (flag) {
            pulisci_schermo();
            stampa(domino, tessere_utente, tessere_tavolino, dim_utente, dim_tavolino, punti, flag);
            f_richiesta_utente(&tessera_scelta, &testa_coda, dim_tavolino);
            punti = f_gioco(domino, tessere_utente, tessere_tavolino, &dim_utente, &dim_tavolino, tessera_scelta, testa_coda);
            flag = f_endgame(domino, tessere_utente, tessere_tavolino, dim_utente, dim_tavolino);
        }
        pulisci_schermo();
        stampa(domino, tessere_utente, tessere_tavolino, dim_utente, dim_tavolino, punti, flag);
    } else { // Modalità AI
        tessera massimo = {0, 0};
        for (int i = 0; i < 6; i++) {
            ripetizioni = f_conta(domino, tessere_utente, dim_utente, i + 1);
            if (massimo.dx < ripetizioni.dx) {
                massimo = ripetizioni;
            }
        }
        tessera_scelta = f_inizio_gioco(tessere_utente, dim_utente, massimo, domino);
        printf("%d - %d tessera: [%d|%d]\n\n", massimo.sx, massimo.dx, domino[tessera_scelta].sx, domino[tessera_scelta].dx);
        punti = f_gioco(domino, tessere_utente, tessere_tavolino, &dim_utente, &dim_tavolino, tessera_scelta, testa_coda);
        testa_coda = 'c';
        
        while (flag) {
            tessera_scelta = f_avanzamento_gioco(tessere_utente, tessere_tavolino, dim_utente, dim_tavolino, domino, &testa_coda);
            punti = f_gioco(domino, tessere_utente, tessere_tavolino, &dim_utente, &dim_tavolino, tessera_scelta, testa_coda);
            flag = f_endgame(domino, tessere_utente, tessere_tavolino, dim_utente, dim_tavolino);
            stampa(domino, tessere_utente, tessere_tavolino, dim_utente, dim_tavolino, punti, flag);
        }
    }

    return 0;
}

void pulisci_schermo(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void f_carica(tessera* domino) {
    int k = 0;
    for (int i = 1; i <= A; i++) {
        for (int j = i; j <= A; j++) {
            domino[k].sx = i;
            domino[k].dx = j;
            k++;
        }
    }
}

void f_generazione_tessere_utente(int* tessere_uscite, tessera* domino) {
    int flag;
    int riempimento = 0;
    while (riempimento < N) {
        int x = rand() % M + 1;
        flag = 1;
        for (int i = 0; (i < riempimento) && flag; i++) {
            if (tessere_uscite[i] == x - 1) {
                flag = 0;
            }
        }
        if (flag) {
            tessere_uscite[riempimento] = x - 1;
            riempimento++;
        }
    }
}

void swap(tessera* domino, int u) {
    int app = domino[u].dx;
    domino[u].dx = domino[u].sx;
    domino[u].sx = app;
}

int f_controllo_tessere(int i, char pos, int* tessere_utente, int* tessere_tavolino, int* dim_utente, int* dim_tavolino, tessera* domino) {
    int flag = 0;

    if (*dim_tavolino == 0) { // Inserimento prima tessera
        tessere_tavolino[0] = tessere_utente[i];
        *dim_tavolino += 1;
        for (int z = i; z < *dim_utente - 1; z++) {
            tessere_utente[z] = tessere_utente[z + 1];
        }
        *dim_utente -= 1;
        flag = 1;
    } else { // Inserimento tessere successive
        int u = tessere_utente[i];
        if (pos == 't') { // Inserimento in testa
            int t = tessere_tavolino[0];
            if ((domino[t].sx == domino[u].dx) || (domino[t].sx == domino[u].sx)) {
                if (domino[t].sx == domino[u].sx) {
                    swap(domino, u);
                }
                for (int j = *dim_tavolino - 1; j >= 0; j--) {
                    tessere_tavolino[j + 1] = tessere_tavolino[j];
                }
                tessere_tavolino[0] = u;
                *dim_tavolino += 1;
                for (int z = i; z < *dim_utente - 1; z++) {
                    tessere_utente[z] = tessere_utente[z + 1];
                }
                *dim_utente -= 1;
                flag = 1;
            }
        } else { // Inserimento in coda
            int t = tessere_tavolino[*dim_tavolino - 1];
            if ((domino[t].dx == domino[u].dx) || (domino[t].dx == domino[u].sx)) {
                if (domino[t].dx == domino[u].dx) {
                    swap(domino, u);
                }
                tessere_tavolino[*dim_tavolino] = u;
                *dim_tavolino += 1;
                for (int z = i; z < *dim_utente - 1; z++) {
                    tessere_utente[z] = tessere_utente[z + 1];
                }
                *dim_utente -= 1;
                flag = 1;
            }
        }
    }

    return flag;
}

void f_richiesta_utente(int* tess, char* pos, int dim_tavolino) {
    printf("Che tessera vuoi giocare? ");
    if (scanf("%d", tess) == 1) {
        *tess -= 1;
    }
    while (getchar() != '\n');

    if (dim_tavolino > 0) {
        printf("Vuoi metterla in testa o in coda (t/c)? ");
        if (scanf(" %c", pos) != 1) *pos = 't';
        while (getchar() != '\n');
    } else {
        *pos = 't';
    }
}

int f_gioco(tessera* domino, int* tessere_utente, int* tessere_tavolino, int* dim_utente, int* dim_tavolino, int tess, char pos) {
    int punti;

    for (int i = 0, sw = 1; (i < *dim_utente) && sw; i++) {
        if (tess == tessere_utente[i]) {
            sw = 0;
            f_controllo_tessere(i, pos, tessere_utente, tessere_tavolino, dim_utente, dim_tavolino, domino);
        }
    }
    punti = f_calcolo_punti(domino, tessere_tavolino, *dim_tavolino);

    return punti;
}

int f_calcolo_punti(tessera* domino, int* tessere_tavolino, int dim_tavolino) {
    int punti = 0;

    for (int i = 0; i < dim_tavolino; i++) {
        int indice = tessere_tavolino[i];
        punti += domino[indice].dx;
        punti += domino[indice].sx;
    }

    return punti;
}

int f_endgame(tessera* domino, int* tessere_utente, int* tessere_tavolino, int dim_utente, int dim_tavolino) {
    if (dim_tavolino == 0 || dim_utente == 0) return 0;

    int indice_tessera_head = tessere_tavolino[0];
    int valore_testa = domino[indice_tessera_head].sx;

    int indice_tessera_tail = tessere_tavolino[dim_tavolino - 1];
    int valore_coda = domino[indice_tessera_tail].dx;

    int flag = f_controllo_endgame(domino, tessere_utente, dim_utente, valore_testa);
    if (!flag) {
        flag = f_controllo_endgame(domino, tessere_utente, dim_utente, valore_coda);
    }

    return flag;
}

int f_controllo_endgame(tessera* domino, int* tessere_utente, int dim_utente, int valore) {
    int flag = 0;

    for (int i = 0; (i < dim_utente) && !flag; i++) {
        int indice = tessere_utente[i];
        if ((valore == domino[indice].sx) || (valore == domino[indice].dx)) {
            flag = 1;
        }
    }

    return flag;
}

void stampa(tessera* domino, int* tessere_utente, int* tessere_tavolino, int dim_utente, int dim_tavolino, int punti, int flag) {
    int decremento = A;
    int k = A;

    for (int i = 0; i < M; i++) {
        if ((i % k == 0) && (i != 0)) {
            printf("\n");
            decremento--;
            k = k + decremento;
        }
        printf("%4d)[%d|%d]", i + 1, domino[i].sx, domino[i].dx);
    }

    printf("\n\n\nTESSERE SUL TAVOLINO:\n\n");
    for (int i = 0; i < dim_tavolino; i++) {
        int x = tessere_tavolino[i];
        printf(" [%d|%d]", domino[x].sx, domino[x].dx);
    }

    printf("\n\n\nEcco a te le seguenti tessere in mano:\n\n");
    for (int i = 0; i < dim_utente; i++) {
        int x = tessere_utente[i];
        printf("%4d)[%d|%d]", x + 1, domino[x].sx, domino[x].dx);
    }

    printf("\n\nTotale punti: %d\n\n", punti);

    if (!flag) {
        if (dim_utente == 0) {
            printf("Fine gioco - Hai messo tutte le tessere!\n\n");
        } else {
            printf("Fine gioco - Non ci sono piu' mosse valide disponibili.\n\n");
        }
    }
}

tessera f_conta(tessera* domino, int* tessere_utente, int dim_utente, int numero) {
    tessera ripetizioni;

    if (dim_utente == 0) {
        ripetizioni.sx = numero;
        ripetizioni.dx = 0;
    } else {
        int indice = tessere_utente[dim_utente - 1];
        ripetizioni = f_conta(domino, tessere_utente, dim_utente - 1, numero);
        if ((domino[indice].sx == numero) || (domino[indice].dx == numero)) {
            ripetizioni.dx += 1;
        }
    }

    return ripetizioni;
}

int f_inizio_gioco(int* tessere_utente, int dim_utente, tessera massimo, tessera* domino) {
    int tes = tessere_utente[0], flag = 1;

    for (int i = 0; (i < dim_utente) && flag; i++) {
        int indice = tessere_utente[i];
        if ((domino[indice].dx == domino[indice].sx) && (domino[indice].dx == massimo.sx)) {
            tes = indice;
            flag = 0;
        }
    }

    for (int i = 0; (i < dim_utente) && flag; i++) {
        int indice = tessere_utente[i];
        if ((domino[indice].dx == massimo.sx) || (domino[indice].sx == massimo.sx)) {
            tes = indice;
            flag = 0;
        }
    }

    return tes;
}

int f_avanzamento_gioco(int* tessere_utente, int* tessere_tavolino, int dim_utente, int dim_tavolino, tessera* domino, char* testa_coda) {
    int tessera_scelta = tessere_utente[0], indice_utente, indice_tavolino, flag;

    if (*testa_coda == 'c') {
        flag = 0;
        indice_tavolino = tessere_tavolino[dim_tavolino - 1];
        for (int i = 0; (i < dim_utente) && !flag; i++) {
            indice_utente = tessere_utente[i];
            if ((domino[indice_utente].sx == domino[indice_tavolino].dx) || (domino[indice_utente].dx == domino[indice_tavolino].dx)) {
                tessera_scelta = indice_utente;
                flag = 1;
            }
        }
        if (!flag) {
            *testa_coda = 't';
        }
    }

    if (*testa_coda == 't') {
        indice_tavolino = tessere_tavolino[0];
        for (int i = 0; i < dim_utente; i++) {
            indice_utente = tessere_utente[i];
            if ((domino[indice_utente].sx == domino[indice_tavolino].sx) || (domino[indice_utente].dx == domino[indice_tavolino].sx)) {
                tessera_scelta = indice_utente;
            }
        }
    }

    return tessera_scelta;
}
