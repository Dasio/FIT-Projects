Číselné hodnocení zde uvedené je v minibodech, jejichž převod na finální body se řídí několika pravidly:
  1) minibody za dokumentaci přepočteme kurzem 100 minibodů = 1 bod
  2) minibody za automatické hodnocení skriptu se vyhodnotí dle kurzu specifickém pro Vaši úlohu:
        CST	128 mb = 1 bod
Nakonec následuje korelace a zaokrouhlení dle informací o hodnocení v obecném zadání.

Hodnocené úlohy zatím nebyly testovány na plagiátorství (termín je do posledního termínu zkoušky).

Úloha byla nejdříve automaticky rozbalena z odevzdaného archivu a pak byly spuštěny testy na serveru Merlin. Hodnotící testy nebudou zveřejněny. Hodnocení v rámci dokumentace dále zahrnuje také hodnocení kvality zdrojových kódů, komentářů atd.

V případě nesouhlasu s hodnocením je možné využít osobní reklamace ve vypsaných termínech (viz IS FIT, Termíny IPP, Reklamace hodnocení 1. úlohy). Projděte si i komentář termínu s pokyny. Na individuální elektronické dotazy bude reagováno s nižší prioritou.


Hodnocení dokumentace:
----------------------
  Získané minibody: 200
  Komentář k hodnocení: OK;;

Vysvětlivky zkratek v dokumentaci:
  CH = pravopisné chyby, překlepy
  FORMAT = špatný formát vzhledu dokumentu (nedodrženy požadavky)
  SHORT = nesplňuje minimální požadavky na délku či obsah
  STRUCT = nevhodně strukturováno (např. bez nadpisů)
  MISSING = dokumentace nebyla odevzdána
  COPY = text obsahuje úryvky ze zadání nebo cizí necitované materiály
  STYLE = stylizace vět, nečitelnost, nesrozumitelnost
  COMMENT = chybějící nebo nedostatečné komentáře ve zdrojovém textu
  FILO = chybí či nedostatečná filosofie návrhu (abstraktní popis struktury programu, co následuje za čím)
  SRCFORMAT = opravdu velmi špatná štábní kultura zdrojového kódu
  LANG (jen pro informaci) = míchání jazyků (většinou anglické termíny v českém textu)
  HOV (jen pro informaci) = hovorové nebo nevhodné slangové výrazy
  PRED (jen pro informaci) = pozor na neslabičné předložky na konci řádků
  FORM (jen pro informaci) = nepěkná úprava, nekonzistentní velikost a typ písma apod.
  BLOK (jen pro informaci) = chybí zarovnaní do bloku místo méně pěkného zarovnání na prapor (doleva)
  KAPTXT (jen pro informaci) = mezi nadpisem a jeho podnadpisem by měl být vždy nějaký text
  MEZ (jen pro informaci) = za otevírající nebo před zavírající závorku mezera nepatří
  ICH (jen pro informaci) = ich-forma (psaní v první osobě jednotného čísla) není většinou vhodná pro programovou dokumentaci
  TERM = problematická terminologie (neobvyklá, nepřesná či přímo špatná)
  SAZBA (jen pro informaci) = alespoň identifikátory proměnných a funkcí se patří sázet písmem s jednotnou šířkou písmen (např. font Courier)
  OK = k dokumentaci byly maximálně minoritní připomínky


Informace o rozbalení odevzdaného archívu:
------------------------------------------
Extracting xmikus15/xmikus15-CST.zip archive..
Archive:  xmikus15-CST.zip
  inflating: ArgParser.php           
  inflating: CAnalyzer.php           
  inflating: CST-doc.pdf             
  inflating: cst.php                 
 extracting: rozsireni               
Setting read-right to *.php, *.class.php *.lib.php *.inc, and rozsireni...
Checking file ArgParser.php as script filename...
Checking file CAnalyzer.php as script filename...
Checking file cst.php as script filename...
Script filename (cst.php) is OK.


Hodnocení skriptu (základní i rozšířené testy):
-----------------------------------------------
Následuje dvojtečkou oddělený seznam testů.
  Jméno testu:Vaše hodnocení [mb]:[rozšíření]:Metadata testu:Komentář do konce řádku.
Na konci naleznete celkový součet v nenormalizovaných minibodech.

tests/basic/test01:10:[]:tests/basic/test01.cmd:test01: zkouska parametru --help (nekontroluje vystup na stdout)
tests/basic/test02:20:[]:tests/basic/test02.cmd:test02: test parametru --help s jinym
tests/basic/test03:10:[]:tests/basic/test03.cmd:test03: Zkouska --help vypisovaneho na stdout
tests/cst/test04:5:[]:tests/cst/test04.cmd:test4: Zkouska nefunkcniho otevreni vstupniho souboru
tests/cst/test05:5:[]:tests/cst/test05.cmd:test5: Zkouska nejzakladnejsiho vstupu s minimem parametru + --input s abs_path
tests/cst/test06:5:[]:tests/cst/test06.cmd:test6: Zkouska nejzakladnejsiho vstupu s minimem parametru vcetne --input s abs_path a --output
tests/cst/test07:4:[]:tests/cst/test07.cmd:test7: Zkouska nejzakladnejsiho vstupu s minimem parametru (s relativni cestou)
tests/cst/test08:4:[]:tests/cst/test08.cmd:test8: Zkouska nejzakladnejsiho vstupu s minimem parametru (s relativni cestou 2)
tests/cst/test09:4:[]:tests/cst/test09.cmd:test9: Zkouska nejzakladnejsiho vstupu s minimem parametru (se slozitejsi relativni cestou)
tests/cst/test10:18:[]:tests/cst/test10.cmd:test10: basic: a file containing no data
tests/cst/test11:18:[]:tests/cst/test11.cmd:test11: basic: missing --output parameter (output goes to stdout)
tests/cst/test12:18:[]:tests/cst/test12.cmd:test12: basic: file with comments containing diacritic
tests/cst/test13:50:[]:tests/cst/test13.cmd:test13: basic: recursively traverse the given directory (-o)
tests/cst/test14:18:[]:tests/cst/test14.cmd:test14: basic: do not check the extension of a direct file path
tests/cst/test15:45:[]:tests/cst/test15.cmd:test15: basic: keywords -k (1, basic)
tests/cst/test16:27:[]:tests/cst/test16.cmd:test16: basic: keywords -k (2, advanced)
tests/cst/test17:18:[]:tests/cst/test17.cmd:test17: basic: keywords -k (3, not keywords)
tests/cst/test18:45:[]:tests/cst/test18.cmd:test18: basic: identifiers -i (1, basic)
tests/cst/test19:9:[]:tests/cst/test19.cmd:test19: basic: identifiers -i (2, advanced)
tests/cst/test20:50:[]:tests/cst/test20.cmd:test20: basic: empty directory
tests/cst/test21:27:[]:tests/cst/test21.cmd:test21: basic: operators -o (1, basic)
tests/cst/test22:27:[]:tests/cst/test22.cmd:test22: basic: operators -o (2, advanced)
tests/cst/test23:18:[]:tests/cst/test23.cmd:test23: basic: operators -o (3, not operators)
tests/cst/test24:36:[]:tests/cst/test24.cmd:test24: basic: patterns -w
tests/cst/test25:50:[]:tests/cst/test25.cmd:test25: basic: proper indention of numbers in the output
tests/cst/test26:22:[]:tests/cst/test26.cmd:test26: basic: comments in strings and string in comments
tests/cst/test27:15:[]:tests/cst/test27.cmd:test27: basic: no subdir
tests/cst/test28:18:[]:tests/cst/test28.cmd:test28: basic: macros and comments over more lines
tests/cst/test29:20:[]:tests/cst/test29.cmd:test29: basic: -p -k with a single file
tests/cst/test30:50:[]:tests/cst/test30.cmd:test30: basic: -p -w with more files
tests/cst/test31:27:[]:tests/cst/test31.cmd:test31: basic: -c single line comments
tests/cst/test32:18:[]:tests/cst/test32.cmd:test32: basic: -c multi line comments
tests/cst/test33:10:[]:tests/cst/test33.cmd:test33: error: invalid parameter
tests/cst/test34:10:[]:tests/cst/test34.cmd:test34: error: --help with another parameter
tests/cst/test35:5:[]:tests/cst/test35.cmd:test35: error: multiple occurrences of --input
tests/cst/test36:5:[]:tests/cst/test36.cmd:test36: error: multiple occurrences of --output
tests/cst/test37:5:[]:tests/cst/test37.cmd:test37: error: multiple occurrences of -k
tests/cst/test38:20:[]:tests/cst/test38.cmd:test38: basic: case sensitivity of -w (and -p)
tests/cst/test39:5:[]:tests/cst/test39.cmd:test39: error: multiple occurrences of -i
tests/cst/test40:5:[]:tests/cst/test40.cmd:test40: error: multiple occurrences of -w
tests/cst/test41:5:[]:tests/cst/test41.cmd:test41: error: multiple occurrences of -c
tests/cst/test42:10:[]:tests/cst/test42.cmd:test42: error: combined several parameters
tests/cst/test43:10:[]:tests/cst/test43.cmd:test43: error: non-existing input file
tests/cst/test44:10:[]:tests/cst/test44.cmd:test44: error: unopenable input file
tests/cst/test45:10:[]:tests/cst/test45.cmd:test45: error: unopenable output file
tests/cst/test46:18:[TER]:tests/cst/test46.cmd:test46: TER: ternary and comma operators (1, ternary operator)
tests/cst/test47:27:[TER]:tests/cst/test47.cmd:test47: TER: ternary and comma operators (2, basic comma operator)
tests/cst/test48:15:[TER]:tests/cst/test48.cmd:test48: TER: ternary and comma operators (3, advanced comma operator)
tests/cst/test49:45:[COM]:tests/cst/test49.cmd:test49: COM: comments in macros
tests/cst/test50:18:[IND]:tests/cst/test50.cmd:test50: IND: index operators (1, basic)
tests/cst/test51:27:[IND]:tests/cst/test51.cmd:test51: IND: index operators (2, advanced)
tests/cst/test52:4:[FNC]:tests/cst/test52.cmd:test52: FNC: function calls (1, basic)
tests/cst/test53:10:[FNC]:tests/cst/test53.cmd:test53: FNC: function calls (2, advanced)
tests/cst/test54:15:[]:tests/cst/test54.cmd:test54: basic: -o -p + advanced constants
tests/cst/test55:15:[]:tests/cst/test55.cmd:test55: basic: -o -p + advanced constants and operations
tests/cst/test56:15:[]:tests/cst/test56.cmd:test56: basic: -k + files with same name
tests/cst/test57:15:[]:tests/cst/test57.cmd:test57: basic: -w -p + overlaping text
tests/cst/test58:15:[]:tests/cst/test58.cmd:test58: basic: -c -p + complicated commnets
tests/cst/test59:15:[]:tests/cst/test59.cmd:test59: basic: -i -p + complicated commnets
tests/cst/test60:15:[]:tests/cst/test60.cmd:test60: basic: -k -p + complicated commnets
tests/cst/test61:15:[]:tests/cst/test61.cmd:test61: basic: -o -p + complicated syntax
tests/cst/test62:15:[]:tests/cst/test62.cmd:test62: basic: -k -p + complicated strings
ZAKLAD MINIBODU:956
BONUS  MINIBODU:164
CELKEM MINIBODU:1120
