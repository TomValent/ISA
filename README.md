# Síťové aplikace a správa sítí - Projekt

## Čtečka novinek ve formátu Atom a RSS s podporou TLS

### Autor: Tomáš Valent <xvalen27@stud.fit.vutbr.cz>

### Popis programu
Program vypisuje vybrané informácie uvedené v stiahnutých feedoch, ktoré 
sú vo formáte Atom a RSS. Program po spustení stiahne zadané zdroje a na štandardný
výstup vypíše informace požadované uživatelem.

### Vytvoření spustitelného programu ze zdrojových souborů
Extrahovanie archívu:
```bash
tar -xf xvalen27.tar
```
Preklad a vytvorenie spustiteľného súboru prebieha pomocou `Makefile` príkazom `make` 
alebo `make feedreader`. Obidva tieto príkazy vytvoria spustiteľný súbor `feedreader`.
Ďalšie možnosti Makefile sú: `make clean`, ktoré zmaže všetky objektové súbory 
(súbory s príponou .o), spustiteľný súbor feedreader a xvalen27.tar, `make tar`, 
ktoré uloží do archívu všetky potrebné zdrojové súbory (súbory s príponami .cpp .h), 
Makefile, README.md a dokumentáciu.

### Použití programu
Po preložení je možné spúšťať vytvorený spustiteľný súbor `feedreader`.  
**Použitie:**   

```bash
./feedreader <URL | -f <feedfile>> [-c <certfile>] [-C <certaddr>] [-T] [-a] [-u]
```

Poradie parametrov je ľubovolné.  
**Popis parametrov:**

* Povinne je uvedená buď URL požadovaného zdroja (pričom podporované schémy sú http 
a https) alebo parameter -f s dodatočným parametrom určujúcim umiestnenie súboru feedfile. 
Súbor feedfile je textový súbor, kde je na každom riadku uvedená jedna adresa zdroja 
vo formátu Atom, či RSS. Prázdne riadky alebo riadky začínajúce znakom '# v súboru 
feedfile sa ignorujú.
    
* Voliteľný parameter -c definuje súbor <certfile> s certifikátmi, ktorý sa použije 
pre overenie platnosti certifikátu SSL/TLS predloženého serverom.
    
* Voliteľný parameter -C určuje adresár <certaddr>, v kterom sa majú vyhľadávať 
certifikáty, ktoré sa použijú pre overenie platnosti certifikátu SSL/TLS predloženého serverom.
    
* Ak nie je uvedený parameter -c ani -C, potom sa používa úložiško certifikátov 
získané funkciou SSL_CTX_set_default_verify_paths().
    
* Pri spustení s parametrom -T sa pre každý záznam zobrazí informácia o čase zmeny 
záznamu, či vytvorenia záznamu (ak je daná informácia v stiahnutom feede).
    
* Pri spustení s parametrom -T sa pre každý záznam zobrazí meno autora, či jeho e-mailová adresa (ak je daná informácia v stiahnutom feede).
    
* Pri spustení s parametrom -T sa pre každý záznam zobrazí asociované URL (ak je daná informácia v stiahnutom feede).
