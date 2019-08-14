// 100p

1. Sporici Dan / 335CB
2. Pt caracterul din stanga:
	A/D - deplasare (accelerata) la stanga/dreapta
	Space - lovitura
	B - blocare lovituri

   Pt caracterul din dreapta:
	Left/Right - deplasare (accelerata) la stanga/dreapta
	P - lovitura
      O - blocare lovituri

   Z - porneste sau opreste auto-orientarea caracterelor
       (on = cele 2 caractere se deplaseaza astfel incat sa fie mereu fata catre oponent // off = deplasare obisnuita + orientare in sensul directiei)

3. GameWorld - folosita pt rendering si interactiunea cu jocul
   
   Character - clasa specifica caracterului (contine structura meshei (quad-ului), animatia curenta (+cadru) pt fiecare caracter, animatiile posibile, implementarea loviturilor si orientarea).
   
   GameObject - clasa mai generica, retine proprietatile generale (pozitie, factor de scalare) + o functie de update apelata pt fiecare obiect in parte (in care se fac animatiile "automate" - schimbarea cadrelor nu depinde de jucator).

Implementarea porneste de la clasa GameWorld, ce se ocupa in general de desfasurarea jocului. Sunt create 2 caractere (character1 si character2) pe care sunt mapate cadrele specifice unei animatii. Coordonatele de textura sunt extrase din fisierul XML alaturi de factorul de scalare pe axa OX (asta deoarece unele cadre sunt mai late decat altele) si cu ajutorul acestora se creeaza array-ul texCoords (in functie de sens); pt caracterul al 2-lea, sensul este invers astfel incat textura sa para oglindita. Toata partea asta se face in functia UpdateTexCoords().

4. Nu prea...cautarea coordonatelor a luat ceva mai mult timp.

5. Structura originala a framework-ului + cele 2 fisiere si imaginea in folderul Textures/

6. No bonus.
