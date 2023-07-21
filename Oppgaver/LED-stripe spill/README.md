# LED-Stripe spill
- [LED-Stripe spill](#led-stripe-spill)
  - [Teori](#teori)
    - [Prell](#prell)
  - [Oppgaver](#oppgaver)
    - [ikke blokerende kode](#ikke-blokerende-kode)
    - [millis()](#millis)
    - [Prellfritt kanppetrykk](#prellfritt-kanppetrykk)
  - [Case](#case)

## Teori
  ### Prell
## Oppgaver
  ### ikke blokerende kode
  * eksempel på blokerende kode
    * blink 1Hz, lys i 3s ved knappetrykk
    * blink 500ms og 750ms
  ### millis()
  * lage ikke blokerende kode
  * time tid mellom trykk
  * time lengden på trykk
  ### Prellfritt kanppetrykk
  * eksempel på hvorfor
  * lage edge detector
  * lage debonced edge detector

## Case
Dere skal nå lage et spill på WS2812 LED-stripene.
Kravene til spillet er:
* Sillet skal bruke minst en LED-stripe.
* Spillet skal bruke minst to styre knapper.
  * Disse må være prellfrie *(Debounced)*
* Spillet skal ha minst to vansklighetsnivåer 
* Det skal spilles av en animasjon når man taper og vinner.
* Påengsummen til spilleren(e) skal vises frem mellom hver runde
* Koden skal være ikke blokenende 

Kreativitet er oppfordret