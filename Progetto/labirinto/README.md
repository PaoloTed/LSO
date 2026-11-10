# Progetto LSO — Traccia A: Labirinto con oggetti e uscita

Sistema client-server in C (UNIX/Linux) per un gioco di esplorazione di un labirinto.
Il server genera il labirinto, gestisce più giocatori contemporaneamente in thread dedicati, invia a ciascuno una vista locale e, periodicamente, una mappa globale mascherata.
La comunicazione avviene tramite socket TCP.


## Compilazione

Con `gcc` e `make` installati, compila dalla cartella che contiene il `Makefile`.

```bash
make
```


Il comando produce i due eseguibili `bin/server` e `bin/client`.
`make clean` elimina la cartella `bin/` e il file `server.log`.

## Avvio rapido

1. Avvia il **server** in un terminale. Resta in primo piano e scrive le attività di gioco in `server.log`. Gli errori di avvio vengono stampati su `stderr`.

```bash
./bin/server 5200 600 15
```

- `5200` è la porta TCP di ascolto.
- `600` è il timeout globale della partita in secondi, contato dall'avvio del server.
- `15` è l'intervallo di invio periodico della mappa globale in secondi.

2. Avvia uno o più **client**, ciascuno in un terminale separato:

```bash
./bin/client localhost 5200
```

Nel menu iniziale scegli **Accedi** oppure **Registrati**, inserisci nickname e password e poi seleziona **Gioca**.

## Avvio con Docker Compose

### Avvio con lo script

Per avviare il server e aprire più client in finestre separate, usa [launch_clients.sh](docker/launch_clients.sh).

```bash
cd docker
bash launch_clients.sh 0 3
```

Il primo argomento è `0` per compilare le immagini o `1` per riutilizzare quelle già disponibili. Il secondo è il numero di client. Lo script arresta i container del progetto, avvia il server e apre i client con `x-terminal-emulator`, `alacritty`, `konsole` o `xterm`.

### Funzionamento

Dopo la connessione TCP, il menu iniziale offre **Accedi**, **Registrati** ed **Esci**. Per accedere o registrarsi, inserisci nickname e password. Dopo la conferma, il menu successivo offre **Gioca** ed **Esci**.

Durante la partita, conferma ogni comando digitato con Invio.

| Comando | Azione |
|:-------:|--------|
| `w` `a` `s` `d` | Muovi su / sinistra / giù / destra |
| `l` | Visualizza i giocatori connessi, con punteggio e stato (in gioco o usciti dal labirinto) |
| `h` | Mostra il menu di aiuto con i comandi disponibili |
| `q` | Disconnessione e uscita ordinata (disponibile anche tramite `Ctrl+C` o `Ctrl+D`) |

## Regole del gioco

1. **Accesso e registrazione**: la registrazione richiede un nickname assente da `users.txt`. Per accedere serve un account registrato che non sia già connesso.
2. **Spawn**: il server posiziona ciascun giocatore in una cella interna percorribile, scelta a caso tra quelle senza oggetti. All'inizio il giocatore vede soltanto la propria cella.
3. **Movimento**: digitando `w`/`a`/`s`/`d` e premendo Invio, il giocatore si sposta. Ad ogni passo valido il server "rivela" una zona attorno a lui (raggio di 2 celle, vista 5×5) che resta permanentemente scoperta.
4. **Oggetti (`O`)**: entrando in una cella contenente un oggetto, il giocatore lo raccoglie: l'oggetto viene rimosso dalla mappa e il punteggio del giocatore aumenta di 1.
5. **Uscite (`E`)**: sono presenti **4 uscite**, una per lato, ciascuna in una posizione casuale adiacente a un corridoio interno.
6. **Mappa Globale**: periodicamente (ogni T secondi), il server invia a ogni giocatore l'intera mappa mascherata, in cui le celle non ancora esplorate da quel giocatore sono contrassegnate dal simbolo `?`.
7. **Fine partita e vincitore**: chi raggiunge un'uscita può restare connesso in attesa del risultato oppure disconnettersi con `q`. La partita termina allo scadere del timeout oppure quando, dopo un'uscita dal labirinto o una disconnessione, nessun giocatore connesso è ancora nel labirinto.
   - Se un solo giocatore ha raggiunto un'uscita, vince lui.
   - Se più giocatori hanno raggiunto un'uscita, vince chi ha raccolto più oggetti **tra quelli usciti**.
   - Se nessuno ha raggiunto un'uscita, vince chi ha raccolto più oggetti **tra tutti i partecipanti**, inclusi quelli disconnessi.
   - A parità di punteggio vince chi è entrato prima. Se non ci sono stati partecipanti, non c'è un vincitore.
   - Il client mostra l'esito e attende Invio prima di chiudere la connessione.

Simboli della mappa:
- `#` muro, `.` cella libera, `E` uscita, `O` oggetto, `P` posizione del giocatore, `?` cella nascosta.
