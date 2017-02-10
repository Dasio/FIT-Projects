Overeni cinnosti kodu CPU:
      testovany program (BF)        vysledek
  1.  ++++++++++                    ok
  2.  ----------                    ok
  3.  +>++>+++                      ok
  4.  <+<++<+++                     chyba
  5.  .+.+.+.                       ok
  6.  ,+,+,+,                       ok
  7.  [........]test[.........]     chyba
  8.  +++[.-]                       ok
  9.  +++++[>++[>+.<-]<-]           chyba

  Podpora jednoduchych cyklu: ano
  Podpora vnorenych cyklu: ne

Poznamky k implementaci:
  Mozne problematicke rizeni nasledujicich signalu: DATA_RDWR, OUT_DATA
  Je zapisovano do pameti vyhrazene pouze pro program (adresa 0 az 0xFFF)
Celkem bodu za CPU implementaci: 7 (z 14)
