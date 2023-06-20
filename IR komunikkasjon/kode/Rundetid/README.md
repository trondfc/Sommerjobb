# Rundetid

- [Rundetid](#rundetid)
  - [Arduino](#arduino)
    - [Kode eksempel](#kode-eksempel)
  - [Zumo](#zumo)
    - [Kode eksempel](#kode-eksempel-1)

Eksempel på tidtaking av rundetid for zumo bilene. Koden en en modifisert versjon av "CallAndAnswer" koden, hvor tidtakings portalen ber bilen om og identifisere segselv, for og så bruke den raporterte IDen til og holde styr på bilens induviduelle rundetid.

Eksempelkoden har deffinert noen ID-er og kommandoer for NEC komunikkasjonen.
|Enhet| ID|
|-----|---|
|Lyskryss| 0x01|
|Runde tidtaker | 0xC8  


|Kommando | Kode |
|----|---|
|Rødt lys | 0x34 |
|Gult lys | 0x35 |
|Grønnt lys | 0x36 |
|Identifiser bil | 0xF0|
|Svar på identifikasjon| 0x02 |
|Dette er en bomstasjon| 0xF1 |
|Lokk opp bom | 0x01 |

## Arduino
### Kode eksempel
## Zumo
### Kode eksempel