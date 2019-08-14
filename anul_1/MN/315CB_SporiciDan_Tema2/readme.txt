// 82p

Dan Sporici

Cerinta 1:

Pornind de la primele 2 valori (1 si d(1) - lambda) se calculeaza iterativ 
valoarea fiecarui polinom in punctul dat (lambda), folosind relatia de recurenta.

Rezultatul intors este vectorul P1(lambda), P2(lambda)...

Cerinta 2:

Folosind functia de la cerinta 1, se determina valorile polinoamelor in lambda.
Deoarece polinoamele formeaza un sir Sturm, nr. de valori proprii se determina
comparand semnul valorilor polinoamelor (doua cate doua).

Cerinta 3:

Se calculeaza vectorul 't', unde t(i) este suma elementelor de pe linia i,
fara a le include pe cele de pe diagonala principala; pt matricea tridiagonala
simetrica, se aduna doar valorile de pe subdiagonala / supradiagonala.

Cu teorema lui Gershgorin, se determina limita inferioara si cea superioara, 
calculand minimul (maximul) din d(i)-t(i) (d(i)+t(i)) - d fiind vectorul 
ce contine diagonala.

Cerinta 4:

Daca, r este vectorul in care se retin intervalele, acesta va fi marginit
de limita inferioara si cea superioara (de la cerinta 3), iar mai apoi
se foloseste metoda din indicatie. Fiecare interval cerut este obtinut
prin impartirea intervalului original in jumatati pana cand numarul
de valori proprii mai mici decat mijlocul este egal cu indicele intervalului
curent. Procedeul se repeta pentru a genera toate intervalele.

Cerinta 5:

Se grupeaza doua cate doua valorile vectorului r obtinut la cerinta 4 si se 
aproximeaza cu metoda bisectiei (se injumatateste intervalul pana cand
mijlocul este destul de aproape de rezultatul cautat - adica diferenta dintre
ultimele 2 valori determinate este mai mica decat eroarea eps).


Cerinta 6:

n-am mai avut timp...
