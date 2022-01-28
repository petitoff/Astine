# Astine

Jest to gra kółko i krzyżyk. Prosta w wyglądzie ale w pełni funkcjonalna. Z zamysłem nauki języka c++.

## Opis

### Specyfikacja

Gra napisana w języku:

- c++

Wykorzystująca bibliotekę:

- [SFML](https://www.sfml-dev.org/)

### Funkcje

Gra posiada trzy funkcje rozgrywki.
  
## Główne założenia
Głównymi założeniami przy budowie projekty były:
### Prosty interfejs
### Posiadanie trzech trybów gry
* Gra na jednym komputerze z drugą osobą (tzw. 1vs1).
* Gra przeciwko komputerowi.
* Gra przez Internet z drugą osobą.

## Aktualny stan gry
### Menu
Gra posiada w pełni sprawne menu dzięki któremu można wybrać tryb gry.
### Logika gry
Została opracowana oraz zaimplementowana logika gry dzięki której gra analizuje czy ktoś nie wygrał bądź nie doszło do remisu.
### 1 vs 1
Tryb został zaimplementowany w grze oraz jest w pełni funkcjonalny.

### 1 vs pc
Aktualnie gra posiada bardzo uproszczony tryb ponieważ w pierwotnej wersji gra korzystała ze starszej biblioteki jednak nie umożliwiała ona wielu funkcji. Musieliśmy więc zbudować grę od początku przez co musimy jeszcze popracować nad logiką komputera.

### Multiplayer
Gra sieciowa działa w stopniu dobrym. Posiada kilka problemów czy braków w optymalizacji ale samo założenie posiadania tryby multiplayer zostało spełnione.

# Jak grać

## Uruchamianie gry

### Sposób 1

- pobrać plik z GitHub: [Astine.7z](https://github.com/petitoff/Astine/tree/main/Astine)
- rozpakować programem 7zip
- uruchomić plik .exe

### Sposób 2

- pobrać plik z [linku](https://www.dropbox.com/sh/9j0u0eteo3r1bgo/AADAVxzSF2xkEaevCuDJC2oVa?dl=0)
- rozpakować programem 7zip
- uruchomić plik .exe
