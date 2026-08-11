# Relazione di Progetto: Gioco del Domino in C

## Introduzione
Questo progetto consiste nello sviluppo di un gioco del Domino implementato nel linguaggio di programmazione C (standard C99). Il gioco offre due modalità operative:
* **Modalità Interattiva:** l'utente può giocare manualmente scegliendo le proprie mosse.
* **Modalità AI (Intelligenza Artificiale):** il computer prende decisioni automatizzate basate su semplici strategie predefinite.

L'obiettivo principale del progetto è stato quello di creare un'esperienza di gioco fluida e intuitiva, gestendo efficacemente le strutture dati e implementando logiche di gioco coerenti sia per l'interazione umana che per l'intelligenza artificiale.

---

## Struttura del Progetto
Il progetto è strutturato in modo modulare, suddividendo le diverse funzionalità in specifiche funzioni per garantire una migliore leggibilità e manutenzione del codice.

### STRUTTURE DATI

* **`tessera`:** La struttura fondamentale del gioco è definita dal tipo `tessera`, contenente due interi (`sx` e `dx`) che rappresentano rispettivamente il valore sinistro e destro di una tessera del domino.
* **Array di Tessere:**
  * `domino[M]`: contiene tutte le possibili combinazioni di tessere del domino.
  * `tessere_utente[N]`: contiene le tessere assegnate all'utente o all'AI.
  * `tessere_tavolino[N]`: contiene le tessere attualmente sul tavolo di gioco.

### FUNZIONI PRINCIPALI

#### 1. Inizializzazione del Gioco
* `f_carica(tessera* domino)`: Popola l'array `domino` con tutte le combinazioni possibili di tessere, evitando duplicati e assicurando una corretta rappresentazione del set completo di tessere del domino standard.
* `f_generazione_tessere_utente(int* tessere_uscite, tessera* domino)`: Seleziona casualmente un insieme di tessere dal set completo e le assegna all'utente o all'AI. Viene garantita l'unicità delle tessere assegnate attraverso un controllo che evita duplicati all'interno della mano del giocatore.

#### 2. Gestione del Gameplay Interattivo
* `f_richiesta_utente(int* tess, char* pos, int dim_tavolino)`: Gestisce l'input dell'utente, chiedendo quale tessera giocare e dove posizionarla (in testa o in coda al tavolo). La funzione valida l'input e prepara i dati per le successive operazioni di gioco.
* `f_controllo_tessere(...)`: Verifica la validità della mossa scelta dall'utente, controllando se la tessera selezionata può essere posizionata nella posizione desiderata in base alle regole del domino. Se necessario, la funzione effettua lo *swap* dei valori della tessera per assicurare la corretta corrispondenza con le tessere già sul tavolo.
* `f_gioco(...)`: Coordina l'intero processo di una mossa di gioco, chiamando le funzioni di controllo e aggiornando lo stato del gioco (tessere dell'utente, tessere sul tavolo e punteggio).

#### 3. Gestione del Gameplay AI
* `f_conta(tessera* domino, int* tessere_utente, int dim_utente, int numero)`: Conta quante volte un determinato numero appare nelle tessere dell'AI, aiutando a determinare quale numero ha la maggior probabilità di essere giocato con successo.
* `f_inizio_gioco(...)`: Determina la migliore tessera iniziale da giocare per l'AI, basandosi sul numero con maggior frequenza nella mano e sulle possibili combinazioni favorevoli.
* `f_avanzamento_gioco(...)`: Gestisce le mosse successive dell'AI, scegliendo la tessera più appropriata da giocare in base alle tessere presenti sul tavolo e nella propria mano, ottimizzando la strategia per massimizzare le possibilità di vittoria.

#### 4. Funzioni di Supporto
* `swap(tessera* domino, int u)`: Inverte i valori `sx` e `dx` di una tessera specifica, facilitando l'adattamento delle tessere alle posizioni richieste sul tavolo.
* `f_calcolo_punti(...)`: Calcola il punteggio totale corrente sommando i valori di tutte le tessere presenti sul tavolo.
* `f_endgame(...)` e `f_controllo_endgame(...)`: Verificano se il gioco è terminato, controllando se il giocatore (utente o AI) ha esaurito le proprie tessere o se non ci sono più mosse valide disponibili.
* `stampa(...)`: Gestisce la visualizzazione dello stato corrente del gioco, stampando sul terminale le tessere disponibili, quelle sul tavolo e il punteggio attuale.
* `clear_window()`: Funzione di utilità che pulisce la finestra del terminale, garantendo una visualizzazione ordinata e aggiornata del gioco (compatibile sia con sistemi Windows che Unix-like grazie a direttive di precompilazione).

---

## FLUSSO DI GIOCO
Il gioco inizia con l'inizializzazione delle tessere e l'assegnazione delle stesse all'utente o all'AI. 
* In **modalità interattiva**, l'utente è guidato attraverso una serie di prompt che richiedono l'input per le mosse successive. 
* In **modalità AI**, il computer decide autonomamente le mosse basandosi sulle funzioni di conteggio e selezione implementate. 

Il gioco procede iterativamente fino a quando non si verificano le condizioni di fine partita, momento in cui viene dichiarato il risultato finale.

---

## ORGANIZZAZIONE DEL LAVORO
Il progetto è stato svolto **individualmente**, permettendo una gestione flessibile e personalizzata del processo di sviluppo. 
Questo ha consentito una profonda comprensione di ogni aspetto del codice e la possibilità di apportare modifiche e ottimizzazioni in maniera efficiente durante le diverse fasi di implementazione e testing.

---

## PRINCIPALI DIFFICOLTA INCONTRATE
* **Gestione delle Strutture Dati:** Organizzare efficacemente le tessere e le loro interazioni ha richiesto una pianificazione attenta delle strutture dati e degli algoritmi di manipolazione, assicurando che ogni operazione mantenesse la coerenza dello stato del gioco.
* **Implementazione dell'AI:** Sviluppare una strategia di gioco automatizzata ha comportato la creazione di funzioni in grado di analizzare la situazione corrente e prendere decisioni ottimali. È stato necessario bilanciare la complessità degli algoritmi con l'efficienza e la semplicità richieste dal progetto.
* **Gestione dell'Input Utente:** Garantire che il programma gestisse correttamente tutti i possibili input dell'utente, inclusi casi di input errato o non valido, ha richiesto l'implementazione di controlli e validazioni robuste per prevenire comportamenti indesiderati o crash dell'applicazione.
* **Visualizzazione e Interfaccia Utente:** Creare un'interfaccia testuale chiara e intuitiva è stato fondamentale per l'usabilità del gioco. Sono state effettuate diverse iterazioni per ottimizzare la presentazione delle informazioni e facilitare l'interazione dell'utente con il gioco.
* **Gestione della Casualità:** L'assegnazione casuale delle tessere e la riproducibilità dei risultati hanno richiesto una corretta inizializzazione e utilizzo delle funzioni di generazione di numeri casuali, assicurando al contempo una distribuzione equa e imprevedibile delle tessere.

---

## CONCLUSIONE
Il progetto è stato completato con successo, implementando un gioco del domino in C99 con un'AI che simula una partita automatica. 
