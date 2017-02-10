-- Adminer 4.2.2fx MySQL dump

SET NAMES utf8;
SET time_zone = '+00:00';
SET foreign_key_checks = 0;
SET sql_mode = 'NO_AUTO_VALUE_ON_ZERO';

DROP TABLE IF EXISTS `cart`;
CREATE TABLE `cart` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_zakaznik` int(11) NOT NULL,
  `id_produkt` int(11) NOT NULL,
  `mnozstvo` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_bin;

INSERT INTO `cart` (`id`, `id_zakaznik`, `id_produkt`, `mnozstvo`) VALUES
(1, 0,  0,  1),
(2, 0,  0,  1),
(3, 0,  0,  1);

DROP TABLE IF EXISTS `comments`;
CREATE TABLE `comments` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `product_id` int(11) NOT NULL,
  `zakaznik_id` int(11) NOT NULL,
  `content` text COLLATE latin2_bin NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `body` double NOT NULL DEFAULT '1',
  PRIMARY KEY (`id`),
  KEY `product_id` (`product_id`),
  KEY `zakaznik_id` (`zakaznik_id`),
  CONSTRAINT `comments_ibfk_2` FOREIGN KEY (`zakaznik_id`) REFERENCES `users` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  CONSTRAINT `comments_ibfk_3` FOREIGN KEY (`product_id`) REFERENCES `product` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_bin;

INSERT INTO `comments` (`id`, `product_id`, `zakaznik_id`, `content`, `created_at`, `body`) VALUES
(28,  34, 1,  'super\n',  '2015-12-03 21:52:44',  5),
(29,  34, 1,  'sss',  '2015-12-03 21:54:53',  1),
(30,  34, 1,  'sssss',  '2015-12-03 22:00:46',  1);

DROP TABLE IF EXISTS `dodavatel`;
CREATE TABLE `dodavatel` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nazev` varchar(50) NOT NULL,
  `www` varchar(50) NOT NULL,
  `email` varchar(50) NOT NULL,
  `telefon` int(15) NOT NULL,
  `mesto` varchar(30) NOT NULL,
  `ulica` varchar(30) NOT NULL,
  `psc` varchar(30) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `dodavatel` (`id`, `nazev`, `www`, `email`, `telefon`, `mesto`, `ulica`, `psc`) VALUES
(1, 'Firma s.r.o',  'www.firma.com',  'firma@firma.com',  123456, 'Brno', 'Husitska', '12345'),
(2, 'Skicak s.r.o', 'www.skicakbrno.cz',  'skicakbrno@gmail.cz',  777842016,  'Brno', 'Hybesova', '61000'),
(3, 'Pastelky s.r.o', 'www.brnopastelky.cz',  'brnopastelky@seznam.cz', 774123456,  'Praha',  'Hlavni 26',  '123456');

DROP TABLE IF EXISTS `dodavatel_produkt`;
CREATE TABLE `dodavatel_produkt` (
  `id_dodavatel` int(11) NOT NULL,
  `id_produkt` int(11) NOT NULL,
  `doba` int(11) NOT NULL,
  KEY `id_dodavatel` (`id_dodavatel`),
  KEY `id_produkt` (`id_produkt`),
  CONSTRAINT `dodavatel_produkt_ibfk_1` FOREIGN KEY (`id_dodavatel`) REFERENCES `dodavatel` (`id`) ON DELETE CASCADE,
  CONSTRAINT `dodavatel_produkt_ibfk_2` FOREIGN KEY (`id_produkt`) REFERENCES `product` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `dodavatel_produkt` (`id_dodavatel`, `id_produkt`, `doba`) VALUES
(1, 27, 5),
(1, 36, 5),
(2, 27, 10),
(3, 40, 3),
(2, 34, 7),
(3, 35, 2),
(3, 37, 0),
(1, 38, 3),
(2, 39, 4);

DROP TABLE IF EXISTS `kosik`;
CREATE TABLE `kosik` (
  `id_zakaznik` int(11) NOT NULL,
  `id_produkt` int(11) NOT NULL,
  `mnozstvo` int(11) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_bin;

INSERT INTO `kosik` (`id_zakaznik`, `id_produkt`, `mnozstvo`) VALUES
(32,  27, 1);

DROP TABLE IF EXISTS `objednavka`;
CREATE TABLE `objednavka` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_zakaznik` int(11) NOT NULL,
  `cena` int(11) NOT NULL,
  `datum_prijatia` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `datum_zmeny_stavu` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `poznamka` varchar(30) COLLATE latin2_bin NOT NULL,
  `sposob_dopravy` varchar(30) COLLATE latin2_bin NOT NULL,
  `stav` varchar(30) COLLATE latin2_bin NOT NULL DEFAULT 'Prijata',
  `sposob_platby` varchar(30) COLLATE latin2_bin NOT NULL,
  `telefon` varchar(30) COLLATE latin2_bin NOT NULL,
  `mesto` varchar(30) COLLATE latin2_bin NOT NULL,
  `psc` varchar(30) COLLATE latin2_bin NOT NULL,
  `ulice` varchar(30) COLLATE latin2_bin NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_bin;

INSERT INTO `objednavka` (`id`, `id_zakaznik`, `cena`, `datum_prijatia`, `datum_zmeny_stavu`, `poznamka`, `sposob_dopravy`, `stav`, `sposob_platby`, `telefon`, `mesto`, `psc`, `ulice`) VALUES
(43,  1,  0,  '2015-11-30 22:09:04',  '2015-11-30 22:09:04',  '', 'osobni_odber', 'Prijata',  'hotove', 'rfrf', 'Brno', '6666', '11111'),
(44,  1,  0,  '2015-11-30 22:43:10',  '2015-11-30 22:43:10',  'ad', 'osobni_odber', 'Prijata',  'hotove', '+420123456', 'mesto',  '03401',  'ulica'),
(45,  1,  374,  '2015-11-30 23:48:22',  '2015-11-30 23:48:22',  'ad', 'osobni_odber', 'Prijata',  'hotove', '+420123456', 'mesto',  '03401',  'ulica'),
(46,  1,  0,  '2015-11-30 23:48:23',  '2015-11-30 23:48:23',  'ad', 'osobni_odber', 'Prijata',  'hotove', '+420123456', 'mesto',  '03401',  'ulica'),
(47,  1,  330,  '2015-11-30 23:49:14',  '2015-11-30 23:49:14',  '', 'doruceni', 'Prijata',  'hotove', '+420123456', 'mesto',  '03401',  'ulica'),
(48,  1,  506,  '2015-12-01 11:14:47',  '2015-12-01 23:31:12',  'adg',  'osobni_odber', 'Dorucene', 'hotove', '+420123456', 'mesto',  '03401',  'ulica'),
(49,  1,  660,  '2015-12-05 14:04:29',  '2015-12-05 14:04:29',  'asd',  'osobni_odber', 'Prijata',  'hotove', '0',  'Ružomberok', '03401',  'Bozetechova'),
(50,  1,  3300, '2015-12-05 14:08:26',  '2015-12-06 22:29:55',  '', 'posta',  'Expedicia',  'kartou', '1111', 'Brno', '11111',  'pidisficka');

DROP TABLE IF EXISTS `objednavka_produkt`;
CREATE TABLE `objednavka_produkt` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `id_dodavatel` int(11) NOT NULL,
  `id_predajca` int(11) NOT NULL,
  `id_produkt` int(11) NOT NULL,
  `mnozstvo` int(11) NOT NULL,
  `doba` int(11) NOT NULL,
  `stav` enum('Objednane','Dokoncene') NOT NULL DEFAULT 'Objednane',
  `datum` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `id_dodavatel` (`id_dodavatel`),
  KEY `id_produkt` (`id_produkt`),
  KEY `id_predajca` (`id_predajca`),
  CONSTRAINT `objednavka_produkt_ibfk_1` FOREIGN KEY (`id_dodavatel`) REFERENCES `dodavatel` (`id`),
  CONSTRAINT `objednavka_produkt_ibfk_2` FOREIGN KEY (`id_produkt`) REFERENCES `product` (`id`),
  CONSTRAINT `objednavka_produkt_ibfk_3` FOREIGN KEY (`id_predajca`) REFERENCES `users` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `objednavka_produkt` (`id`, `id_dodavatel`, `id_predajca`, `id_produkt`, `mnozstvo`, `doba`, `stav`, `datum`) VALUES
(2, 1,  1,  27, 10, 5,  'Dokoncene',  '2015-12-06 11:54:12'),
(3, 2,  1,  27, 200,  10, 'Dokoncene',  '2015-12-06 12:31:59'),
(4, 3,  34, 40, 2,  3,  'Objednane',  '2015-12-06 13:13:19');

DROP TABLE IF EXISTS `obsah_objednavky`;
CREATE TABLE `obsah_objednavky` (
  `id_objednavky` int(11) NOT NULL,
  `id_produkt` int(11) NOT NULL,
  `mnozstvo` int(11) NOT NULL,
  `cena` int(11) NOT NULL,
  KEY `id_produkt` (`id_produkt`),
  KEY `id_objednavky` (`id_objednavky`),
  CONSTRAINT `obsah_objednavky_ibfk_2` FOREIGN KEY (`id_produkt`) REFERENCES `product` (`id`),
  CONSTRAINT `obsah_objednavky_ibfk_4` FOREIGN KEY (`id_objednavky`) REFERENCES `objednavka` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

INSERT INTO `obsah_objednavky` (`id_objednavky`, `id_produkt`, `mnozstvo`, `cena`) VALUES
(44,  27, 9,  44),
(44,  35, 1,  330),
(45,  27, 1,  44),
(45,  34, 1,  330),
(47,  34, 1,  330),
(48,  27, 4,  44),
(48,  34, 1,  330),
(49,  34, 2,  330),
(50,  34, 10, 330);

DROP TABLE IF EXISTS `posts`;
CREATE TABLE `posts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(255) COLLATE latin2_bin NOT NULL,
  `content` varchar(255) COLLATE latin2_bin NOT NULL,
  `created_at` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_bin;

INSERT INTO `posts` (`id`, `title`, `content`, `created_at`) VALUES
(1, 'Skicak', 'Školní skicáky určené pro žáky, studenty, vysokoškoláky i amatérské výtvarníky. Nekyselý papír mírně strukturovaný nebo zrnitý, s povrchem dobře snášejícím mnohanásobné vymazávání a korekční opravy.', '2015-10-04 20:11:43');

DROP TABLE IF EXISTS `product`;
CREATE TABLE `product` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nazev` varchar(255) COLLATE latin2_bin NOT NULL,
  `popis` text COLLATE latin2_bin NOT NULL,
  `cena` int(11) NOT NULL,
  `sklad` int(11) NOT NULL,
  `pridani` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `hodnoceni` int(11) NOT NULL,
  `obrazek` text COLLATE latin2_bin NOT NULL,
  `kategorie` varchar(11) COLLATE latin2_bin NOT NULL,
  `body` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_bin;

INSERT INTO `product` (`id`, `nazev`, `popis`, `cena`, `sklad`, `pridani`, `hodnoceni`, `obrazek`, `kategorie`, `body`) VALUES
(27,  'Skicak', 'Nejaky', 42, 221,  '2015-10-06 12:00:05',  0,  'http://www.paragraph.cz/145-thickbox_default/skicak-b5-square-kraft.jpg',  'skicak', 0),
(34,  'Skicak', 'Pro skolaky',  330,  10, '2015-11-29 00:31:00',  0,  'http://www.paragraph.cz/136-thickbox_default/skicak-b5-square-kraft.jpg',  'skicak', 0),
(35,  'Skicak', 'Velikost 35x45', 330,  0,  '2015-11-29 00:32:47',  0,  'http://www.paragraph.cz/136-thickbox_default/skicak-b5-square-kraft.jpg',  'skicak', 0),
(36,  'Skicak 42x59,4 cm A2 (kroužková vazba na kratší straně)',  'XL skicák je lehký, 90 g/m2, slonovinový, bezkyselinný papír', 20, 0,  '2015-12-03 01:40:37',  0,  'http://www.side2.cz/sites/default/files/imagecache/obr2/h_skicak_01.jpg',  'skicak', 0),
(37,  'Stabilo Trio Scribbi', 'Špičkový fix nikdy nezklame: je téměř nerozbitný a vždy připravený tvořit, 8 ks i pro \"L\"',  326,  0,  '2015-12-03 17:44:21',  0,  'http://www.pastelka.eu/out/pictures/z1/img8848594e15939c9e9bd88f123d6ca_1__98921635fc44eb0e3acbdbe7eb1f008829afe75b_z1.jpg', 'pastelka', 0),
(38,  'Skicak', 'Bloky ze skicovacího světle šedého recyklovaného papíru o gramaži 110 g / m2. Bloky jsou vhodné pro kresbu tužkami, uhlem či křídami. Obsah 50 listů, blok je lepený po jedné kratší straně.', 106,  0,  '2015-12-03 17:47:23',  0,  'http://www.paragraph.cz/145-thickbox_default/skicak-b5-square-kraft.jpg',  'skicak', 0),
(39,  'Skicak', 'Nejlepsi na svete',  411,  0,  '2015-12-03 19:32:48',  0,  'http://www.paragraph.cz/136-thickbox_default/skicak-b5-square-kraft.jpg',  'skicak', 0),
(40,  'Pastelka', 'Super',  100,  0,  '2015-12-03 19:34:46',  0,  'http://www.optys.cz/data/foto/56/6060300056_l.jpg',  'pastelka', 0);

DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `jmeno` varchar(255) COLLATE latin2_bin NOT NULL,
  `prijmeni` varchar(255) COLLATE latin2_bin NOT NULL,
  `email` varchar(255) COLLATE latin2_bin NOT NULL,
  `telefon` int(11) NOT NULL,
  `mesto` varchar(255) COLLATE latin2_bin NOT NULL,
  `ulice` varchar(255) COLLATE latin2_bin NOT NULL,
  `psc` int(5) NOT NULL,
  `heslo` varchar(255) COLLATE latin2_bin NOT NULL,
  `role` varchar(255) COLLATE latin2_bin NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin2 COLLATE=latin2_bin;

INSERT INTO `users` (`id`, `jmeno`, `prijmeni`, `email`, `telefon`, `mesto`, `ulice`, `psc`, `heslo`, `role`) VALUES
(1, 'admin',  'adminP', 'admin',  0,  '', '', 0,  '7c4a8d09ca3762af61e59520943dc26494f8941b', 'staff'),
(25,  'Vlad', 'Nimik',  'vn22@gmail.com', 777842862,  'Brno', 'Poznanska 11', 11111,  '7c4a8d09ca3762af61e59520943dc26494f8941b', 'registered'),
(33,  'Albert', 'Kandrac',  'kandrac@gmail.com',  777823987,  'Olomouc',  'Nadrazni 42',  98765,  '66d30c93619758b4fd14eb88a7c7b9ff1d4c4b6e', 'registered'),
(34,  'Martin', 'Novak',  'firma@firma.cz', 774888223,  'Brno', 'Husitska', 12345,  '66d30c93619758b4fd14eb88a7c7b9ff1d4c4b6e', 'staff');

-- 2015-12-07 17:16:40