I problemi concreti da correggere sono tre:

1. Un client lento blocca mappe periodiche e timeout per gli altri.
   Contrasta con la richiesta di gestire contemporaneamente i client e inviare periodicamente le mappe senza bloccarli. Bisogna evitare che un invio blocchi il timer, gestire gli errori e impedire invii sovrapposti sulla stessa connessione.
2. Il riutilizzo degli slot cancella i risultati dei giocatori disconnessi.
   Può violare le regole del vincitore: abbiamo provato che Alice, unica uscita, perde il proprio risultato quando Carlo occupa il suo slot. Bisogna conservare i risultati dei partecipanti fino alla fine della partita, distinguendoli dalle connessioni attive.
3. La fine anticipata non viene rivalutata quando qualcuno si disconnette.
   Se Alice è uscita e Bob, ultimo ancora dentro, si scollega, la partita aspetta il timeout. Qui la traccia non chiarisce come considerare gli abbandoni: se si considerano i soli giocatori ancora connessi, come fa attualmente il codice, bisogna ricontrollare la condizione anche alla disconnessione.