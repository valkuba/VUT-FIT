----------- Projekt do IDS: Systém pro rezervaci letenek ---------------------
-- Autoři: Milan Takáč (xtakac09), Jakub Valeš (xvales04)
-- Datum: 1.5.2023

-------- 1. Část -----------------------------------------------------------

-- Vytvoření ERD, UCD

-------- 2. Část - vytvoření objektů schématu databáze ---------------------

-- Abychom to mohli spouštět vícekrát
DROP TABLE letiste CASCADE CONSTRAINTS;
DROP TABLE letadlo CASCADE CONSTRAINTS;
DROP TABLE zavazadlo CASCADE CONSTRAINTS;
DROP TABLE uzivatel CASCADE CONSTRAINTS;
DROP TABLE spravce CASCADE CONSTRAINTS;
DROP TABLE cestujici CASCADE CONSTRAINTS;
DROP TABLE let CASCADE CONSTRAINTS;
DROP TABLE rezervace CASCADE CONSTRAINTS;
DROP TABLE sedadlo CASCADE CONSTRAINTS;
DROP SEQUENCE letadlo_seq;
DROP SEQUENCE uzivatel_seq;
DROP SEQUENCE rezervace_seq;
DROP SEQUENCE let_seq;
-- DROP TRIGGER trigger_obsad_sedadlo;
-- DROP TRIGGER trigger_uvolni_sedadlo;
DROP MATERIALIZED VIEW api;

-- Tabulka LETISTE
CREATE TABLE letiste(
	kod_letiste char(3) NOT NULL PRIMARY KEY,							-- PK
	nazev_letiste varchar(80) NOT NULL,									-- Přidáno
	zeme varchar(60) NOT NULL,									
	mesto varchar(100) NOT NULL											
);

-- Sekvence pro PK v letadle
CREATE SEQUENCE letadlo_seq 
START WITH 0 
INCREMENT BY 1
MINVALUE 0;

-- Tabulka LETADLO
CREATE TABLE letadlo(
	id_letadla int DEFAULT ON NULL letadlo_seq.NEXTVAL PRIMARY KEY, 	-- PK
	spolecnost varchar(50) NOT NULL,									
	model varchar(50) NOT NULL,											
	pocet_sedadel numeric(3,0) NOT NULL									
);		

-- Tabulka ZAVAZADLO
CREATE TABLE zavazadlo(
	typ_zavazadla varchar(45) NOT NULL PRIMARY KEY,						-- PK (nejdelsi bude třeba 10 Kg + 23Kg), bude se zadávat rucne
	poplatek int NOT NULL	
);	

-- Sekvence pro PK v uživateli 
CREATE SEQUENCE uzivatel_seq 
START WITH 0 
INCREMENT BY 1
MINVALUE 0;

-- Tabulka UZIVATEL
CREATE TABLE uzivatel(
	id_uzivatele int DEFAULT ON NULL uzivatel_seq.NEXTVAL PRIMARY KEY,	-- PK
	jmeno varchar(30) NOT NULL,													
	prijmeni varchar(30) NOT NULL,												
	datum_narozeni DATE NOT NULL,										-- YYYY-MM-DD  -- oracle ma datettime !!!!!!!!!!
	email varchar(70) NOT NULL,													
	mesto varchar(100) NOT NULL,																	
	zeme varchar(56) NOT NULL,											-- Přidáno
	ulice_cp varchar(100) NOT NULL,							
	CHECK(email LIKE '%_@__%.__%')										-- Zkontroluje jestli ma email požadovaný formát -> minimálně: x@xx.xx
);

-------------------------------------- GENERALIZACE/SPECIALIZACE
-- Pro obě tabulky je vytvořen primární klíč, který odkazuje na jejich index v tabulce Uživatel, a atribut specifický pro jejich specializaci

-- Tabulka SPRAVCE
CREATE TABLE spravce(
	id_uzivatele int DEFAULT ON NULL uzivatel_seq.NEXTVAL PRIMARY KEY, 	-- PK 
	sluzebni_telefon varchar(15) NOT NULL,										
	FOREIGN KEY (id_uzivatele) REFERENCES uzivatel(id_uzivatele)		-- FK		
);
	
-- Tabulka CESTUJICI
CREATE TABLE cestujici(
	id_uzivatele int DEFAULT ON NULL uzivatel_seq.NEXTVAL PRIMARY KEY,	-- PK 
	rodne_cislo int NOT NULL,														
	FOREIGN KEY (id_uzivatele) REFERENCES uzivatel(id_uzivatele)
		-- Tady pro jednoduchost při plnění tabulek nebudeme kontrolovat korektnost rodného čísla
);

----------------------------------------------------------------

-- Sekvence pro PK v letu
CREATE SEQUENCE let_seq 
START WITH 0 
INCREMENT BY 1
MINVALUE 0;

-- Tabulka LET
CREATE TABLE let(
	id_letu int DEFAULT ON NULL let_seq.NEXTVAL PRIMARY KEY,			-- PK
	vychozi_letiste char(3) NOT NULL REFERENCES letiste(kod_letiste),	-- FK
	cilove_letiste char(3) NOT NULL REFERENCES letiste(kod_letiste),	-- FK
	datum_cas_odletu DATE NOT NULL,										-- V Oracle je DATE = YYYY-MM-DD HH-MI-SS
	datum_cas_pristani DATE NOT NULL,											
	delka_letu numeric(4,0) NOT NULL,											
	vzdalenost numeric(5,0) NOT NULL,											
	id_letadla int REFERENCES letadlo(id_letadla) NOT NULL,				-- FK
	vypsal int REFERENCES spravce(id_uzivatele) NOT NULL,				-- FK 
	stav varchar(10) NOT NULL,											-- Zrušen, přesunut, ...
	CHECK(vychozi_letiste <> cilove_letiste)
);

-- Sekvence pro PK v rezervaci
CREATE SEQUENCE rezervace_seq 
START WITH 0 
INCREMENT BY 1
MINVALUE 0;

-- Tabulka REZERVACE
CREATE TABLE rezervace(	
	id_rezervace int DEFAULT ON NULL rezervace_seq.NEXTVAL PRIMARY KEY,	-- PK
	id_letu int NOT NULL REFERENCES let(id_letu),						-- FK
	id_uzivatele int NOT NULL REFERENCES uzivatel(id_uzivatele),		-- FK
	sedadlo int NOT NULL,														
	stav varchar(10) NOT NULL,											-- Přijato, potvrzeno, zrušeno, proběhlo.
	typ_zavazadla varchar(45) DEFAULT 'Pouze Příruční zavazadlo' REFERENCES zavazadlo(typ_zavazadla)		-- FK
);

-- Tabulka SEDADLO	
CREATE TABLE sedadlo(
	cislo_sedadla varchar(4) NOT NULL,									
	id_letu int NOT NULL REFERENCES let(id_letu), 						-- FK
	trida varchar(8) NOT NULL,											-- First, business, economy
	cena int NOT NULL,
	obsazenost numeric(1,0) DEFAULT 0,									-- BOOLEAN nefunguje pro ORACLE - 0 = volné, 1 = obsazené
	PRIMARY KEY(cislo_sedadla, id_letu)									-- PK složený
);


---- Naplnění tabulek daty ----
INSERT INTO ZAVAZADLO VALUES ('Pouze příruční zavazadlo', 0);
INSERT INTO ZAVAZADLO VALUES ('Příruční a malé zavazadlo (do 10 kg)', 500);
INSERT INTO ZAVAZADLO VALUES ('Příruční a velké zavazadlo (do 23 kg)', 1000);

INSERT INTO LETADLO VALUES ('', 'Smart Wings','Boeing 737','123');
INSERT INTO LETADLO VALUES ('', 'Smart Wings','Airbus A320','150');
INSERT INTO LETADLO VALUES ('', 'Smart Wings','Airbus A320','150');
INSERT INTO LETADLO VALUES ('', 'Emirates','Boeing 777','364');
INSERT INTO LETADLO VALUES ('', 'Emirates','Boeing 777','364');
INSERT INTO LETADLO VALUES ('', 'British Airlines','Airbus A380','591');
INSERT INTO LETADLO VALUES ('', 'Ryanair','Boeing 737','123');
INSERT INTO LETADLO VALUES ('', 'Pepikova Aerolinka','Boeing 767','400');

INSERT INTO UZIVATEL VALUES ('', 'Jan', 'Omáčka', TO_DATE('1975-05-14','YY-MM-DD'), 'jomacka@spravce.cz', 'Karviná', 'Česká republika', 'Trubačova 33');
INSERT INTO UZIVATEL VALUES ('', 'Josef', 'Volný', TO_DATE('1980-07-25','YY-MM-DD'), 'josvolny@volny.cz', 'Aš', 'Česká republika', 'Palackého 1');
INSERT INTO UZIVATEL VALUES ('', 'Petr', 'Klíč', TO_DATE('1999-12-12','YY-MM-DD'), 'klic99@gmail.com', 'Martin', 'Slovensko', 'Jánošíkova 13');
INSERT INTO UZIVATEL VALUES ('', 'Matěj', 'Prázdný', TO_DATE('2000-01-01','YY-MM-DD'), 'mathewprazdny@seznam.cz', 'Brno', 'Česká republika', 'Česká 1');
INSERT INTO UZIVATEL VALUES ('', 'Jožko', 'Černý', TO_DATE('1969-10-10','YY-MM-DD'), 'nemamemail@email.cz', 'Praha', 'Česká republika', 'Václavská 2');
INSERT INTO UZIVATEL VALUES ('', 'Adam', 'Nedám', TO_DATE('2001-07-12','YY-MM-DD'), 'anedam10@centrum.cz', 'Brno', 'Česká republika', 'Jánská 20');
INSERT INTO UZIVATEL VALUES ('', 'Josef', 'Námořník', TO_DATE('1993-04-02','YY-MM-DD'), 'pepeknamornik@spravce.cz', 'Humpolec', 'Česká republika', 'Hustá 3');

INSERT INTO SPRAVCE VALUES ((SELECT id_uzivatele from uzivatel WHERE email='jomacka@spravce.cz'), '+420123456789');
INSERT INTO SPRAVCE VALUES ((SELECT id_uzivatele from uzivatel WHERE email='pepeknamornik@spravce.cz'), '+420987654321');

INSERT INTO CESTUJICI VALUES ((SELECT id_uzivatele from uzivatel WHERE email='josvolny@volny.cz'), '8007251234');
INSERT INTO CESTUJICI VALUES ((SELECT id_uzivatele from uzivatel WHERE email='klic99@gmail.com'), '9912121212');
INSERT INTO CESTUJICI VALUES ((SELECT id_uzivatele from uzivatel WHERE email='mathewprazdny@seznam.cz'), '0001017845');
INSERT INTO CESTUJICI VALUES ((SELECT id_uzivatele from uzivatel WHERE email='nemamemail@email.cz'), '6910100001');
INSERT INTO CESTUJICI VALUES ((SELECT id_uzivatele from uzivatel WHERE email='anedam10@centrum.cz'), '0107125584');

INSERT INTO LETISTE VALUES ('BRQ', 'Mezinárodní letiště Brno-Tuřany', 'Česká republika', 'Brno');
INSERT INTO LETISTE VALUES ('VIE', 'Flughafen Wien', 'Rakousko', 'Víděň');
INSERT INTO LETISTE VALUES ('LHR', 'London Heathrow', 'Spojené království Velké Británie a Severního Irska', 'Londýn');
INSERT INTO LETISTE VALUES ('IAD', 'Washington Dulles International Airport', 'Spojené státy americké', 'Washington D.C.');
INSERT INTO LETISTE VALUES ('CPT', 'Cape Town International Airport', 'Jihoafrická republika', 'Kapské Město');

INSERT INTO LET VALUES ('', 'BRQ', 'LHR', TO_DATE('2023-05-03 08:00:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-05-03 10:02:00', 'YYYY-MM-DD HH24:MI:SS'), 62, 1201, '0', '0', 'Potvrzeno');
INSERT INTO LET VALUES ('', 'VIE', 'CPT', TO_DATE('2023-07-03 08:15:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-08-03 2:02:00', 'YYYY-MM-DD HH24:MI:SS'), 1067, 9136, '1', '0', 'Proběhlo');
INSERT INTO LET VALUES ('', 'VIE', 'IAD', TO_DATE('2023-04-10 10:55:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-04-10 21:00:00', 'YYYY-MM-DD HH24:MI:SS'), 605, 7173, '2', '0', 'Zrušeno');
INSERT INTO LET VALUES ('', 'LHR', 'IAD', TO_DATE('2023-04-17 13:00:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-04-17 15:00:00', 'YYYY-MM-DD HH24:MI:SS'), 120, 2514, '6', '6', 'Potvrzeno');
INSERT INTO LET VALUES ('', 'LHR', 'BRQ', TO_DATE('2023-05-10 18:00:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-05-10 20:15:00', 'YYYY-MM-DD HH24:MI:SS'), 135, 1201, '3', '0', 'Potvrzeno');
INSERT INTO LET VALUES ('', 'BRQ', 'VIE', TO_DATE('2023-12-23 10:15:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-12-23 11:00:00', 'YYYY-MM-DD HH24:MI:SS'), 45, 116, '4', '0', 'Zpožděno');
INSERT INTO LET VALUES ('', 'VIE', 'CPT', TO_DATE('2023-10-01 05:00:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-10-01 23:50:00', 'YYYY-MM-DD HH24:MI:SS'), 1067, 9136, '5', '0', 'Proběhlo');

INSERT INTO LET VALUES ('', 'LHR', 'BRQ', TO_DATE('2023-06-11 16:00:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-06-11 18:15:00', 'YYYY-MM-DD HH24:MI:SS'), 135, 1201, '3', '0', 'Potvrzeno');
INSERT INTO LET VALUES ('', 'BRQ', 'VIE', TO_DATE('2023-12-28 10:15:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-12-28 11:00:00', 'YYYY-MM-DD HH24:MI:SS'), 45, 116, '4', '0', 'Potvrzeno');
INSERT INTO LET VALUES ('', 'LHR', 'BRQ', TO_DATE('2023-12-12 12:00:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-12-12 14:15:00', 'YYYY-MM-DD HH24:MI:SS'), 135, 1201, '3', '0', 'Potvrzeno');
INSERT INTO LET VALUES ('', 'VIE', 'BRQ', TO_DATE('2023-12-12 12:00:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-12-12 14:15:00', 'YYYY-MM-DD HH24:MI:SS'), 45, 116, '3', '0', 'Potvrzeno');

INSERT INTO REZERVACE VALUES ('', '0', '1', 12, 'Přijato', 'Pouze příruční zavazadlo');
INSERT INTO REZERVACE VALUES ('', '1', '2', 1, 'Proběhlo', 'Příruční a malé zavazadlo (do 10 kg)');
INSERT INTO REZERVACE VALUES ('', '2', '3', 20, 'Přijato', 'Příruční a malé zavazadlo (do 10 kg)');
INSERT INTO REZERVACE VALUES ('', '3', '3', 21, 'Zrušeno', 'Příruční a velké zavazadlo (do 23 kg)');
INSERT INTO REZERVACE VALUES ('', '3', '4', 33, 'Zrušeno', 'Příruční a velké zavazadlo (do 23 kg)');
INSERT INTO REZERVACE VALUES ('', '5', '1', 5, 'Potvrzeno', 'Příruční a malé zavazadlo (do 10 kg)');
INSERT INTO REZERVACE VALUES ('', '5', '5', 13, 'Potvrzeno', 'Pouze příruční zavazadlo');
INSERT INTO REZERVACE VALUES ('', '10', '2', 27, 'Potvrzeno', 'Pouze příruční zavazadlo');

INSERT INTO SEDADLO VALUES ('1', '0', 'economy', 7000, 0);
INSERT INTO SEDADLO VALUES ('12', '0', 'economy', 700, 1);
INSERT INTO SEDADLO VALUES ('31', '0', 'business', 13000, 0);
INSERT INTO SEDADLO VALUES ('1', '1', 'economy', 10000, 1);
INSERT INTO SEDADLO VALUES ('2', '1', 'economy', 10000, 0);
INSERT INTO SEDADLO VALUES ('20', '2', 'economy', 5000, 1);
INSERT INTO SEDADLO VALUES ('13', '3', 'business', 25000, 1);
INSERT INTO SEDADLO VALUES ('14', '3', 'first', 250000, 0);
INSERT INTO SEDADLO VALUES ('21', '3', 'first', 30000, 1);
INSERT INTO SEDADLO VALUES ('33', '3', 'first', 30000, 1);
INSERT INTO SEDADLO VALUES ('12', '4', 'economy', 7000, 0);
INSERT INTO SEDADLO VALUES ('5', '5', 'economy', 450, 0);
INSERT INTO SEDADLO VALUES ('30', '6', 'business', 13000, 0);
INSERT INTO SEDADLO VALUES ('28', '6', 'first', 30000, 0);
INSERT INTO SEDADLO VALUES ('11', '6', 'economy', 7000, 0);
INSERT INTO SEDADLO VALUES ('5', '7', 'economy', 10000, 0);
INSERT INTO SEDADLO VALUES ('7', '8', 'economy', 10000, 0);
INSERT INTO SEDADLO VALUES ('2', '9', 'economy', 5000, 0);
INSERT INTO SEDADLO VALUES ('50', '9', 'economy', 450, 0);
INSERT INTO SEDADLO VALUES ('89', '10', 'business', 25000, 0);
INSERT INTO SEDADLO VALUES ('17', '10', 'first', 250000, 0);
INSERT INTO SEDADLO VALUES ('27', '10', 'first', 30000, 0);


-- SELECT * FROM ZAVAZADLO;
-- SELECT * FROM LETADLO;
-- SELECT * FROM UZIVATEL;
-- SELECT * FROM LET;
-- SELECT * FROM REZERVACE;
-- SELECT * FROM SEDADLO;
-- SELECT * FROM SPRAVCE;
-- SELECT * FROM CESTUJICI;

-------- 3. Část - dotazy SELECT-----------------------------------------------

-- Zobrazí nejlevnější volne sedadlo letu, který letí z Vídně
-- Funkce: vyhledávání volných sedadel z daného letiště s nejnižší cenou
SELECT id_letu, TO_CHAR(datum_cas_odletu, 'YYYY-MM-DD HH24:MI:SS') datum_cas_odletu, cilove_letiste, cena, cislo_sedadla
FROM sedadlo NATURAL JOIN let 
WHERE obsazenost = 0 AND vychozi_letiste = 'VIE' AND cena = (SELECT MIN(cena) 
															 FROM sedadlo NATURAL JOIN let 
															 WHERE obsazenost = 0 AND vychozi_letiste = 'VIE');

-- Vypíše seznam letů, a jakými společnostmi jsou organizovány
SELECT l.id_letu, a.spolecnost
FROM let l JOIN letadlo a ON l.id_letadla = a.id_letadla;

-- Zobrazí čísla rezervací a emaily uživatelů se zpožděným letem
SELECT r.id_rezervace, u.email
FROM (rezervace r JOIN let l ON r.id_letu = l.id_letu) JOIN uzivatel u ON r.id_uzivatele = u.id_uzivatele
WHERE l.stav LIKE 'Zpožděno';

-- Zobrazení počtu rezervací pro jednotlivé lety
-- Funkce: Manažer letecké společnosti si může zobrazit, o které lety je největší zájem
SELECT F.id_letu, F.vychozi_letiste, F.cilove_letiste, COUNT(R.id_rezervace) pocet_rezervaci 
FROM let F LEFT JOIN rezervace R ON F.id_letu = R.id_letu 
GROUP BY F.id_letu, F.vychozi_letiste, F.cilove_letiste 
ORDER BY pocet_rezervaci DESC;

-- Zobrazení celkové ceny rezervací pro jednotlivé uživatele včetně poplatku za zavazadlo
-- Funkce: po přidání klauzule WHERE = 'id_uzivatele', se tento SELECT použije jako celková cena nakoupených letenek na profilu uživatele
SELECT jmeno, prijmeni, SUM(cena + poplatek) celkova_cena
FROM uzivatel NATURAL JOIN rezervace NATURAL JOIN sedadlo NATURAL JOIN zavazadlo
GROUP BY jmeno, prijmeni;

-- Zobrazení letadel, která létají pouze do Brna
SELECT DISTINCT L.id_letadla, L.spolecnost, L.model
FROM letadlo L, let F
WHERE L.id_letadla = F.id_letadla AND F.cilove_letiste = 'BRQ' AND NOT EXISTS (SELECT * 
																			   FROM let F 
																			   WHERE L.id_letadla = F.id_letadla AND F.cilove_letiste <> 'BRQ');

-- Zobrazení letů vypsaných správcem Janem Omáčkou
SELECT id_letu, vychozi_letiste, cilove_letiste
FROM LET
WHERE vypsal IN (SELECT id_uzivatele 
				 FROM spravce NATURAL JOIN uzivatel
				 WHERE jmeno = 'Jan' AND prijmeni = 'Omáčka');


-------- 4. Část - vytvoření pokročilých objektů schématu databáze --------------------

-- Přidělení (definice) práv pro kolegu
-- https://docs.oracle.com/javadb/10.8.3.0/ref/rrefsqljgrant.html#rrefsqljgrant__grantprivtype
GRANT ALL PRIVILEGES on cestujici TO XVALES04;
GRANT ALL PRIVILEGES on let TO XVALES04;
GRANT ALL PRIVILEGES on uzivatel TO XVALES04;
GRANT ALL PRIVILEGES on sedadlo TO XVALES04;
GRANT ALL PRIVILEGES on rezervace TO XVALES04;

-- Triggery
-- V našem případě se nic extra složitého nedá vymyslet, to by se musely posílet emaily o potvrzení, apod.

	-- Po INSERT rezervace, nastaví sedadlo jako obsazené
CREATE OR REPLACE TRIGGER trigger_obsad_sedadlo
AFTER INSERT ON rezervace
FOR EACH ROW	-- Změní u každého řádku který byl ovlivněn
BEGIN
	UPDATE sedadlo
	SET obsazenost = 1
	WHERE cislo_sedadla = :NEW.sedadlo;		-- Na novém sedadle
END;
/
-- Ukázka funkčnosti
SELECT * FROM SEDADLO WHERE CISLO_SEDADLA = 14;
INSERT INTO REZERVACE VALUES ('', '5', '5', 14, 'Potvrzeno', 'Pouze příruční zavazadlo');
SELECT * FROM SEDADLO WHERE CISLO_SEDADLA = 14;


	-- Po DELETE rezervace, nastaví sedadlo jako volné
CREATE OR REPLACE TRIGGER trigger_uvolni_sedadlo
AFTER DELETE ON rezervace
FOR EACH ROW	-- Pro každý afektnutý řádek
BEGIN
	UPDATE sedadlo
	SET obsazenost = 0
	WHERE cislo_sedadla = :OLD.sedadlo;		-- Na smazaném sedadle
END;
/
-- Ukázka funkčnosti
SELECT * FROM SEDADLO WHERE CISLO_SEDADLA = 14;
DELETE FROM rezervace WHERE sedadlo = 14;
SELECT * FROM SEDADLO WHERE CISLO_SEDADLA = 14;


	-- Po UPDATE letu na zrušeno, zruší rezervace
CREATE OR REPLACE TRIGGER trigger_zrus_rezervace
AFTER UPDATE ON let
FOR EACH ROW
BEGIN 
	IF :NEW.stav = 'Zrušeno' THEN
		UPDATE rezervace
		SET stav = :NEW.stav
		WHERE stav = :OLD.stav;
	END IF;
END;
/

-- Ukázka funkčnosti
SELECT * FROM rezervace WHERE id_letu = 10;
UPDATE let SET stav = 'Zrušeno' WHERE id_letu = 10;
SELECT * FROM rezervace WHERE id_letu = 10;


-- Procedury
-- Procedura, která projde tabulku LET, a pokud je datum_cas_pristani před datum_cas_odletu aktualizuje datum_cas_pristani pomocí delka_letu
CREATE OR REPLACE PROCEDURE check_cas_letu AS
	v_cas_odletu  	let.datum_cas_odletu%TYPE;
	v_cas_pristani 	let.datum_cas_pristani%TYPE;
	v_delka_letu   	let.delka_letu%TYPE;
	v_id_letu 		let.id_letu%TYPE;
BEGIN
	-- procházení všech záznamů v tabulce LET a načítání do proměnných
	FOR r IN (SELECT id_letu, datum_cas_odletu, datum_cas_pristani, delka_letu FROM let) LOOP
		v_cas_odletu := r.datum_cas_odletu;
		v_cas_pristani := r.datum_cas_pristani;
		v_delka_letu := r.delka_letu;
		v_id_letu := r.id_letu;

		-- kontrola jestli je doba letu nezáporné číslo
		IF v_delka_letu <= 0 THEN
			RAISE_APPLICATION_ERROR(-20001, 'Neplatna doba letu pro let ID: ' || v_id_letu);
    	END IF;
    	-- kontrola, zda je čas odletu dříve než čas příletu
		IF v_cas_odletu >= v_cas_pristani THEN
			-- aktualizace času příletu
			UPDATE let
			SET datum_cas_pristani = v_cas_odletu + (v_delka_letu/1440) -- délku letu vydělím celkovým počtem minut ve dnu, aby bylo možné aktualizovat datum_cas_pristani, které je typu DATE
			WHERE id_letu = r.id_letu;
		END IF;
	END LOOP;

	COMMIT; -- potvrzení změn
-- Vyjímka bude vyvolána například když jeden z parametrů, které procedura vyhledává v tabulce let nebude existrovat (NULL)
EXCEPTION
  WHEN OTHERS THEN
    RAISE_APPLICATION_ERROR(-20001, 'Chyba při zpracování letů: ' || SQLERRM);
END;
/

-- Procedura, která pro konkrétní rezervaci vypočítá její celkovou cenu
-- Vstupní parametr procedury je ID rezervace a na výstup vypíše její celkovou cenu
CREATE OR REPLACE PROCEDURE vypocet_ceny(v_id_rez IN NUMBER) AS
    v_typ_zavazadla 	rezervace.typ_zavazadla%TYPE;
    v_id_letu 			rezervace.id_letu%TYPE;
    v_cislo_sed 		rezervace.sedadlo%TYPE;
	v_cena_sed 			sedadlo.cena%TYPE;
	v_celk_cena 		NUMBER;

	-- Deklarace kurzorů pro tabulku s rezervacemi a sedadlem
	-- kurzor pro rezervace vybere typ zavazadla, ID letu a sedadlo z rezervace jejíž ID bylo zadáno
    CURSOR c_rezervace IS 
    SELECT r.typ_zavazadla, r.id_letu, r.sedadlo
    FROM rezervace r
    WHERE r.id_rezervace = v_id_rez;

	-- kurzor pro sedadlo nalezne, podle cisla sedadla a ID letu, cenu daného sedadla
	CURSOR c_sedadlo IS 
    SELECT s.cena
    FROM sedadlo s
    WHERE s.cislo_sedadla = v_cislo_sed AND s.id_letu = v_id_letu;

BEGIN
	-- Vytvoření kurzoru a definice do jakých proměnných má data zapsat
    OPEN c_rezervace;
    FETCH c_rezervace INTO v_typ_zavazadla, v_id_letu, v_cislo_sed;

	-- Pokud kurzor nalezne rezervaci s daným ID, vyhledá v tabulce se zavazadli poplatek pro dané zavazadlo
	-- pro zavazadla není vytvořen kurzor, protože tato tabulka má málo dat a neplánujeme ji rozšiřovat
    IF c_rezervace%FOUND THEN
        SELECT poplatek INTO v_celk_cena
		FROM zavazadlo
		WHERE typ_zavazadla = v_typ_zavazadla;

		-- Vytvoření kurzoru pro tabulku se sedadly
		OPEN c_sedadlo;
    	FETCH c_sedadlo INTO v_cena_sed;

		-- Pokud nalezne hledané sedadlo vypíše celkovou cenu
		IF c_sedadlo%FOUND THEN
			v_celk_cena := v_celk_cena + v_cena_sed;
			DBMS_OUTPUT.PUT_LINE('Celkova cena: ' || v_celk_cena);
		ELSE
			RAISE_APPLICATION_ERROR(-20001, 'Chyba: sedadlo s cislem : ' || v_cislo_sed || ' nebylo nalezeno');
		END IF;
    ELSE
        RAISE_APPLICATION_ERROR(-20001, 'Chyba: rezervace s daným ID: ' || v_id_rez || ' neexistuje');
    END IF;

	-- Zavření kurzorů
	CLOSE c_sedadlo;
    CLOSE c_rezervace;
END;
/

-- Ukázka spuštění procedur
-- pro kontrolu casu odletů si do tabulky přidám hodnotu s nevalidním časem příletu/odletu
INSERT INTO LET VALUES ('', 'VIE', 'BRQ', TO_DATE('2023-12-17 08:15:00', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-12-11 7:00:00', 'YYYY-MM-DD HH24:MI:SS'), 45, 116, '7', '0', 'Potvrzeno');
BEGIN check_cas_letu(); END;
/
-- Po provedení si můžu vypsat tabulku pro přesvědčení, že se čas skutečně aktualizoval
SELECT ID_LETU, VYCHOZI_LETISTE, CILOVE_LETISTE, TO_CHAR(DATUM_CAS_ODLETU, 'YYYY-MM-DD HH24:MI:SS'), TO_CHAR(DATUM_CAS_PRISTANI, 'YYYY-MM-DD HH24:MI:SS'), DELKA_LETU from LET;

-- Výpočet celkové ceny rezervace s ID_rezervace = 2
BEGIN vypocet_ceny(2); END;
/

-- EXPLAIN PLAN + index
EXPLAIN PLAN FOR 
SELECT spolecnost, COUNT(*) as pocet_letadel
FROM let NATURAL JOIN letadlo
WHERE let.cilove_letiste = 'BRQ' AND let.stav <> 'Zrušeno'
GROUP BY spolecnost;

SELECT * FROM let;

SELECT plan_table_output FROM TABLE(DBMS_XPLAN.display());

CREATE INDEX let_index ON let (CILOVE_LETISTE, STAV);

EXPLAIN PLAN FOR 
SELECT spolecnost, COUNT(*) as pocet_letadel
FROM let NATURAL JOIN letadlo
WHERE let.cilove_letiste = 'BRQ' AND let.stav <> 'Zrušeno'
GROUP BY spolecnost;

SELECT plan_table_output FROM TABLE(DBMS_XPLAN.display());

DROP INDEX let_index;

-- pro procedury a mozna trigerry
-- GRANT EXECUTE ON PROCEDURE p TO george 

-- Materilaizovaný view
	-- API pro prohlížení letů - obsahuje zakladni informace o letu + cislo volneho sedadla
CREATE MATERIALIZED VIEW api
REFRESH COMPLETE ON COMMIT AS			-- refreshne se po commitu do zavisle tabulky
SELECT l.vychozi_letiste, l.cilove_letiste, l.datum_cas_odletu, l.datum_cas_pristani, s.cislo_sedadla
FROM let l, sedadlo s
WHERE l.id_letu = s.id_letu AND s.obsazenost = 0;

GRANT SELECT on api TO XVALES04;
-- Ukázka fuknčnosti 
	-- Po smazání sedadla které se v něm nachází se api refreshne a sedadlo se odstraní
SELECT * FROM api;
DELETE FROM sedadlo WHERE cislo_sedadla = 1 AND id_letu = 0;
SELECT * FROM api;
	-- Výpis letů s volnými sedadly a jejich počtem
SELECT vychozi_letiste, cilove_letiste, datum_cas_odletu, datum_cas_pristani, COUNT(*) pocet_volnych_sedadel 
FROM api 
GROUP BY vychozi_letiste, cilove_letiste, datum_cas_odletu, datum_cas_pristani;

-- SELECT + with + case na obsazenost letadla
WITH pocet_obsazenych AS
    (SELECT l.id_letu, SUM(s.obsazenost) pocet
    FROM let l, sedadlo s
    WHERE l.id_letu = s.id_letu
    GROUP BY l.id_letu)

    SELECT p.id_letu, l.vychozi_letiste, l.cilove_letiste, TO_CHAR(l.datum_cas_odletu, 'YYYY-MM-DD HH24:MI:SS') AS datum_cas_odletu, p.pocet,
    CASE 
        WHEN p.pocet = 0 THEN 'Prázdný let'
        WHEN p.pocet = a.pocet_sedadel THEN 'Plný let' 
        ELSE 'Poloprázdný let'
    END AS zaplnenost
FROM pocet_obsazenych p, let l, letadlo a 
WHERE p.id_letu = l.id_letu AND a.id_letadla = l.id_letadla
ORDER BY id_letu;

-- Odebrani prav
-- https://docs.oracle.com/javadb/10.8.3.0/ref/rrefsqljrevoke.html
-- REVOKE privilege-type ON [ TABLE ] { table-Name | view-Name } FROM grantees