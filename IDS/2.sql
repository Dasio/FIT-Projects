DROP TABLE Produkt CASCADE CONSTRAINTS;
DROP TABLE Pastelky CASCADE CONSTRAINTS;
DROP TABLE Skicaky CASCADE CONSTRAINTS;
DROP TABLE Dodavatel CASCADE CONSTRAINTS;
DROP TABLE Kosik CASCADE CONSTRAINTS;
DROP TABLE Zakaznik CASCADE CONSTRAINTS;
DROP TABLE Recenzia CASCADE CONSTRAINTS;
DROP TABLE Objednavka CASCADE CONSTRAINTS;
DROP TABLE Obsah_objednavky CASCADE CONSTRAINTS;
DROP TABLE Dodavatel_produktu CASCADE CONSTRAINTS;

CREATE TABLE Produkt (
  id NUMBER NOT NULL PRIMARY KEY,
  cena NUMBER DEFAULT 0 NOT NULL,
  dostupnost NUMBER DEFAULT 0 NOT NULL,
  predajnost NUMBER DEFAULT 0 NOT NULL,
  oblubenost NUMBER DEFAULT 0 NOT NULL
  );

  CREATE TABLE Pastelky (
  id NUMBER NOT NULL PRIMARY KEY REFERENCES Produkt(id),
  nazov VARCHAR(20) NOT NULL,
  dlzka NUMBER DEFAULT 0 NOT NULL,
  pocet NUMBER DEFAULT 0 NOT NULL,
  popis VARCHAR(100) DEFAULT '' NOT NULL,
  typ NUMBER DEFAULT 0 NOT NULL
  );

 CREATE TABLE Skicaky (
  id NUMBER NOT NULL PRIMARY KEY REFERENCES Produkt(id),
  nazov VARCHAR(20) NOT NULL,
  gramaz NUMBER DEFAULT 0 NOT NULL,
  pocet NUMBER DEFAULT 0 NOT NULL,
  velkost NUMBER DEFAULT 0 NOT NULL,
  typ NUMBER DEFAULT 0 NOT NULL
  );

  CREATE TABLE Dodavatel (
  id NUMBER NOT NULL PRIMARY KEY,
  nazov VARCHAR(20) NOT NULL,
  www VARCHAR(20) NOT NULL,
  pocet NUMBER NOT NULL,
  email VARCHAR(50) NOT NULL,
  telefon VARCHAR(20) NOT NULL,
  mesto VARCHAR(20) NOT NULL,
  ulica VARCHAR(30) NOT NULL,
  psc VARCHAR(10) NOT NULL
  );

  CREATE TABLE Zakaznik(
  id NUMBER NOT NULL PRIMARY KEY,
  login VARCHAR(20) NOT NULL,
  heslo VARCHAR(20) NOT NULL,
  meno VARCHAR(20) NOT NULL,
  priezvisko VARCHAR(20) NOT NULL,
  email VARCHAR(50) NOT NULL,
  telefon VARCHAR(20) NOT NULL,
  mesto VARCHAR(20) NOT NULL,
  ulica VARCHAR(30) NOT NULL,
  psc VARCHAR(30) NOT NULL
  );

  CREATE TABLE Kosik (
  id_zakaznik NUMBER NOT NULL REFERENCES Zakaznik(id),
  id_produkt NUMBER NOT NULL REFERENCES Produkt(id),
  mnozstvo NUMBER DEFAULT 1 NOT NULL,
  PRIMARY KEY(id_zakaznik,id_produkt)
  );

  CREATE TABLE Recenzia (
  id NUMBER NOT NULL PRIMARY KEY,
  id_produkt NUMBER NOT NULL REFERENCES Produkt(id),
  id_zakaznik NUMBER NOT NULL REFERENCES Zakaznik(id),
  hodnotenie NUMBER NOT NULL,
  komentar VARCHAR (255) NOT NULL,
  CHECK (hodnotenie BETWEEN 1 and 5)
  );

  CREATE TABLE Objednavka (
  id NUMBER NOT NULL PRIMARY KEY,
  datum_prijatia DATE DEFAULT SYSDATE NOT NULL,
  datum_zmeny_stavu DATE DEFAULT SYSDATE NOT NULL,
  datum_platby DATE,
  cena NUMBER NOT NULL,
  poznamka VARCHAR(30) DEFAULT '' NOT NULL,
  sposob_dopravy NUMBER DEFAULT 0 NOT NULL,
  stav_objednavky NUMBER DEFAULT 0 NOT NULL,
  sposob_platby NUMBER DEFAULT 0 NOT NULL
  );

  CREATE TABLE Obsah_objednavky (
  id_objednavka NUMBER NOT NULL REFERENCES Objednavka(id),
  id_produkt NUMBER NOT NULL REFERENCES Produkt(id),
  mnozstvo NUMBER DEFAULT 1 NOT NULL,
  cena NUMBER NOT NULL,
  PRIMARY KEY(id_objednavka,id_produkt)
  );

  CREATE TABLE Dodavatel_produktu (
  id_dodavatel NUMBER NOT NULL REFERENCES Dodavatel(id),
  id_produkt  NUMBER  NOT NULL REFERENCES Produkt(id),
  PRIMARY KEY(id_dodavatel, id_produkt)
  );

DROP SEQUENCE objednavka_seq;
CREATE SEQUENCE objednavka_seq START WITH 1;
CREATE OR REPLACE TRIGGER objednavka_auto_increment
  BEFORE INSERT ON Objednavka
  FOR EACH ROW
  WHEN (new.id is null)
BEGIN
  :new.id := objednavka_seq.nextval;
END;
/
INSERT INTO Produkt(id,cena,dostupnost,predajnost,oblubenost) VALUES(1,10,42,0,0);
INSERT INTO Pastelky(id,nazov,dlzka,pocet,popis,typ) VALUES(1,'PROGRESSO', 10, 12, 'Souprava 12 ks akvarelových pastelek bez dřeva v laku', 0);

INSERT INTO Produkt(id,cena,dostupnost,predajnost,oblubenost) VALUES(3,10,42,0,0);
INSERT INTO Pastelky(id,nazov,dlzka,pocet,popis,typ) VALUES(3,'SUBLIME', 10, 15, 'Souprava 15 ks akvarelových pastelek bez dřeva v laku', 0);

INSERT INTO Produkt(id,cena,dostupnost,predajnost,oblubenost) VALUES(4,42,80,50,80);
INSERT INTO Skicaky (id,nazov,gramaz,pocet,velkost,typ) VALUES(4, 'Fashion girl', 100, 10, 1, 0);

INSERT INTO Dodavatel (id,nazov,www,pocet,email,telefon,mesto,ulica,psc) VALUES(101, 'iNexia s.r.o.', 'http://www.inexia.cz', 50,'info@inexia.cz', '+420 777 512 301','Olomouc','Slavíčkova 1a','638 00');
INSERT INTO Dodavatel (id,nazov,www,pocet,email,telefon,mesto,ulica,psc) VALUES(4,'Multip s.r.o.', 'http://www.multip.cz', 30,'obchodni@multip.cz', '+420 556 77 00 41','Brno','Palackého 1135/27',' 741 01');
INSERT INTO Dodavatel (id,nazov,www,pocet,email,telefon,mesto,ulica,psc) VALUES(103, 'Kenast s.r.o.', 'http://www.kenast.cz', 50,'info@kenast.cz', '420 603 462 631','Praha','Dělnická 54','170 00');

INSERT INTO Dodavatel_produktu(id_dodavatel,id_produkt) VALUES(101,4);

INSERT INTO Zakaznik (id,login,heslo,meno,priezvisko,email,telefon,mesto,ulica,psc) VALUES (164, 'mintenker', 'mojeheslo', 'Dusan','Dusansky','mojeposta@seznam.cz', '+420 777 517 601','Ceske Budejovice','Nadrazni 2','668 02');
INSERT INTO Zakaznik (id,login,heslo,meno,priezvisko,email,telefon,mesto,ulica,psc) VALUES (165, 'yagoo', 'mojeheslo2', 'Karolina','Libusina','mojeposta2@gmail.com', '+420 775 547 678','Karlovy Vary','Marianskolazenska','360 01');
INSERT INTO Zakaznik (id,login,heslo,meno,priezvisko,email,telefon,mesto,ulica,psc) VALUES (166, 'apofix', 'mojeheslo3', 'Marketa','Koznarova','mojeposta3@gmail.com', '+420 775 465 351','Praha','Vinohradska 4','130 00');

INSERT INTO Kosik (id_zakaznik,id_produkt,mnozstvo) VALUES(164,1,3);
INSERT INTO Kosik (id_zakaznik,id_produkt,mnozstvo) VALUES(165,3,2);
INSERT INTO Kosik (id_zakaznik,id_produkt,mnozstvo) VALUES(166,4,5);

INSERT INTO Recenzia (id,id_produkt,id_zakaznik,hodnotenie,komentar) VALUES(53,1,164,5,'SUPER ZBOZI');
INSERT INTO Recenzia (id,id_produkt,id_zakaznik,hodnotenie,komentar) VALUES(54,3,165,4,'Vyhovujici cena');
INSERT INTO Recenzia (id,id_produkt,id_zakaznik,hodnotenie,komentar) VALUES(55,4,166,2,'Nizka kvalita');

INSERT INTO Objednavka (datum_prijatia,datum_zmeny_stavu,datum_platby,cena,poznamka,sposob_dopravy,stav_objednavky,sposob_platby) VALUES(TO_DATE('12.04.2014','DD.MM.YYYY'),TO_DATE('13.04.2014','DD.MM.YYYY'),TO_DATE('15.04.2014','DD.MM.YYYY'),160, 'nevolejte mi rano',97,98,99);
INSERT INTO Objednavka (datum_prijatia,datum_zmeny_stavu,datum_platby,cena,poznamka,sposob_dopravy,stav_objednavky,sposob_platby) VALUES(TO_DATE('17.05.2014','DD.MM.YYYY'),TO_DATE('17.05.2014','DD.MM.YYYY'),TO_DATE('19.05.2014','DD.MM.YYYY'),320, 'Blok A03',96,97,105);
INSERT INTO Objednavka (datum_prijatia,datum_zmeny_stavu,datum_platby,cena,poznamka,sposob_dopravy,stav_objednavky,sposob_platby) VALUES(TO_DATE('12.04.2014','DD.MM.YYYY'),TO_DATE('27.03.2015','DD.MM.YYYY'),TO_DATE('27.03.2014','DD.MM.YYYY'),250, 'Do 28.3 budu v zahranici',97,98,99);

INSERT INTO Obsah_objednavky (id_objednavka,id_produkt,mnozstvo,cena) VALUES(1,1,3,160);
INSERT INTO Obsah_objednavky (id_objednavka,id_produkt,mnozstvo,cena) VALUES(2,3,2,320);
INSERT INTO Obsah_objednavky (id_objednavka,id_produkt,mnozstvo,cena) VALUES(3,4,5,250);

ALTER session SET nls_date_format='DD.MM.YYYY';
