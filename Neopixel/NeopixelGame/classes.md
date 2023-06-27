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
- `void setupButton(int pin, int debounceTime);`
  
Setup kanppe GPIO og debounce tid.
- `void begin();`
  
Start knappen.
- `bool isDebounced();`
  
returnerer om tilstanden til knappen er debounced eller ikke.
- `bool hasBeenPressed();`
  
Returnerer om knappen har blitt trykket.
- `bool hasBeenReleased();`
  
Returnerer om knappen har blitt sluppet

- `bool state;`

Variable for knappens nåværende tilstand.

## enemy
- `void start(int pos);`
  
Start fiende objet på gitt posisjon.
- `void update();`
  
Oppdater fiende objekt.
- `bool isColliding(int pos);`
  
Returnerer om den gitte posisjonen koliderer med fiende objektet eller ikke.
- `bool isActive();`

Returnerer om fiende objetet er aktivt eller ikke.
- `bool isHiting(int pos);`

Returnerer om fiende objektet treffer den oppgitte posisjonen med et angrep.
- `bool isDefending();`

Returnerer om fiende objektet forsvarer seg eller ikke.
- `int getPos();`

Returnerer posisjonen til fiende objektet.
- `void setPos(int pos);`

Flytter fiende objektet.
- `int getSize();`

Returnerer størelsen på fiende objektet. brukes til angreps animasjon.
- `uint32_t getColor();`

Returnerer fargen til fiende objektet.
- `bool hit();`

Slå fiende objektet. Returnerer om slaget treffer.
## archer
- `void start(int pos);`

Start skytter på gitt posisjon.
- `void start(int pos, int minAttackTime, int maxAttackTime);`

Start skytter på gitt posisjon med gitte timing parametere.
- `void update();`

Oppdater skytter objekt.
- `bool isColliding(int pos);`

Returnerer om skyteren koliderer med gitt posisjon.
- `bool isActive();`
  
Returnerer om skytter objektet er aktivt.
- `bool isShooting();`

Returnerer om skytter objektet er klar til og skyte.
- `int getPos();`

Returnerer skytterens nåværende posisjon.
- `void setPos(int pos);`

Setter skytterens posisjon.
- `uint32_t getColor();`

Returnerer skytterens farge.
- `bool hit();`

Slå skytteren. Returnerer om slaget  treffer. 

## arrow
- `void start(int pos);`

Start pil objekt på gitt posisjon med standar hastighet.
- `void start(int pos, int speed);`
  
Start pil objekt på gitt posisjon med gitt hastighet.
- `void update();`

Oppdater pil objekt.
- `bool isColliding(int pos);`

Returnerer om git posisjon koliderer med pilen.
- `bool isActive();`
  
Returnerer om pil objektet er aktivt eller ikke.
- `int getPos();`

Returnerer pilens posisjon.
- `void setPos(int pos);`

Setter pilens posisjon.
- `uint32_t getColor();`

Returnerer pilens farge.

## lava
- `void start(int pos, int size);`

Startet lava objektet med gitt posisjon og størelse.
- `void start(int pos, int size, int minOnTime, int maxOnTime, int minOffTime,  int maxOffTime);`

Starter lava objektet med gitt posisjon, størelse og timing.
- `void update();`

Oppdaterer lava objektet.
- `void remove();`

Fjerner lava objektet fra spillet og setter det som deaktivert.
- `bool isColliding(int pos);`

Returnerer om gitt posisjon koliderer med lavaen eller ikke.
- `bool isActive();`

Returnerer om lavaen er aktiv eller ikke.
- `int getPos();`

returnerer posisjonen til lavaen.
- `int getSize();`

Returnerer størrelsen til lavaen.
- `uint32_t getColor();`

Returnerer fargen til lavaen.

## deathAnimation
- `void start(int pos, uint32_t color);`

Start døds animasjon på git posisjon med gitt farge.
- `void update();`

Oppdater døds animasjon.
- `bool isActive();`

Returnerer om døsdanimasjon objektet er aktivt eller ikke.
- `int getPos();`

Returnerer posisjonen til dødsanimasjon objektet.
- `uint32_t pixelColors[DEATH_ANIMATION_SIZE];`

Liste med fargene for alle pikselene i dødsanimasjonen. 
