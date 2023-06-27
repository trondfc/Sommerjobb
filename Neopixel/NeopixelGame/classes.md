# Classes
- [Classes](#classes)
  - [debug](#debug)
  - [player](#player)
  - [button](#button)
  - [enemy](#enemy)
  - [archer](#archer)
  - [arrow](#arrow)
  - [lava](#lava)
  - [deathAnimation](#deathanimation)


## debug
## player
- `player(int forwardPin, int backwardPin,int attackPin, int defendPin, int lives);`
    
Initialiserer spiller objektet. bør ligge i `setup`.
- `void begin();`
  
Starter spiller objektet og setter opp knappene. bør ligge i `setup`.
- `void update();`  

Oppdaterer spiller objektet, leser av konapper og styrer angrep- og forsvar-status. Bør kjøres hver runde av `loop`.
- `int getPos();`
  
Returnerer spillerens nåværende posisjon.
- `void setPos(int pos);`

Flytter spilleren til gitt posisjon.
- `int getSize();`

Returnerer spillerens nåværende størelse (brukes for angreps animasjon)
- `uint32_t getColor();`

Returnerer spillerens nåværende farge.
- `int getLives();`

Retunrnerer spillerens gjenværende liv.
- `bool hit();`

Slå spilleren, returnerer om slaget traff.
- `bool isHiting(int pos);`
  
Returnerer om spilleren angriper gitt posisjon.
- `bool isDefending();`

Returnerer om spilleren er i forsvars modus eller ikke.
## button
## enemy
## archer
## arrow
## lava
## deathAnimation