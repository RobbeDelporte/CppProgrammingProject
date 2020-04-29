# TODO

### Bij impact meerdere blokken raken afhankelijk van impactsrichting en snelheid
  - Bij impact berekenen van welke richting deze gebeurde
  - bv. bij hoge verticale snelheid en zijdelings impact blokken in een verticale lijn achter het geraakte blok(ken) verwijderen
  
### EInde van level detecteren en victory message displayen
  - Mss een counter intialiseren bij het laden van het level met het aantal targets
  - dit bijhouden in context
  - bij het raken van target 1 afterken

### Een live timer toevoegen en op het scherm tonen
  - Bij begin van de game een counter in de context waarbij elke update upgeteld wordt
  - de counter op 60 is dan 1sec, afhenkelijk van FPS
  - Kan ook in de context bijgehouden worden
  
### REPLAY SYSTEM
  - inputs bijhouden in een file
  - File opslaan bij highscore
  
  replay.cpp heeft wrsch een game loop gelijkaardig aan game.cpp
  maar de inputs komen niet via playerinput maar deze highscorefile

  !!!Frequentie van de fileinputs moet waarshijnlijk bepaald worden
  als er 60 keer per sec een !input lezen uit de file moet ook enkel de input wegeschreven worden 60 keer per sec
  de input continu wegschrijven in de while en continu uitlezen zal wrsch voor problemen zorgen
  Een apparte timer is waarsch onontkoombaar in de replay.cpp main gameloop

### Commentaar toevoegen
  - Code leesbaardermaken
  
### Verslag schrijven
  - 

