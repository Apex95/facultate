
Pentru executie:

make build
make run fisier


-- Arborele de parsare --


	Este construit de clasa TextParser; initial expresia (linia) citita o retin intr-un nod Expresie, ce va contine legatura catre tree-ul ce urmeaza sa fie generat folosind acel TextParser.
	Inainte de parsarea propriu zisa se sterg parantezele (si spatiile...) redundante (ce incadreaza intreaga expresie si nu-si au rostul) - insa pentru a reface expresia la afisare folosesc o variabila boolean in interiorul clasei Operator (areParanteze) ce specifica daca expresia era incadrata sau nu.
	TextParser ia continutul expresiei si il imparte in functie de precedenta fiecarui operator, facand mai intai o scurta verificare cu regex pentru a determina daca expresia este o adunare, inmultire, impartire, etc. Daca respecta formatul cunoscut aceasta e impartita in 3 noduri: 2 operanzi si un operator; acei 2 operanzi pot fi la randul lor expresii prin urmare se determina recursiv arborele lor; in momentul apelului se transmite tipul de date (dorit) al operanzilor (Termen / Factor / Ternar), determinat in functie de operator.
	Afisarea se face determinand numarul de nivele...pe loc; functia de afisare intoarce true cat timp, pentru nivelul curent, nu toate nodurile sunt frunze.
	

-- Analiza semantica --
	

	
	Pentru erorile cerute cred ca iesea mai usor cu un regex, dar...am incercat sa-l fac "ca la carte" folosind arborele de parsare.
	Verific intai daca radacina arborelui este operatorul de
atribuire ('=') altfel..."nu exista erori". Pe urma verific daca toate nodurile din stanga sunt variabile (respecta conditia de nume). Daca o respecta, mai poatea aparea cealalta eroare si anume ramura din dreapta sa contina variabile neatribuite: pentru a rezolva asta efectuez o parcurgere recursiva a ramurii din dreapta si creez 2 liste - una cu variabile initializate si una cu variabile neinitializate. Prima lista o pastrez (fiindca o variabila se poate initializa intr-o expresie anterioara) iar cealalta trebuie stearsa pentru fiecare expresie. Din cea de-a 2-a lista preiau primul element (prima variabila neinitializata) si caut pozitia ei in expresia originala (retinuta in nodul Expresie) => coloana la care se afla eroarea. Linia este chiar numarul de ordine al expresiei. Rezultatul intors de functia de verificare e chiar mesajul cerut.


-- Evaluarea expresiei --

	Se face tot folosind arborele de la primul exercitiu si un HashMap<string, string> ce contine valorile variabilelor. La inceput parcurg intreg arborele iar la intoarcerea din recursivitate evaluez fiecare expresie (de la frunze spre radacina), actualizand informatia din fiecare nod nod Operator cu rezultatul operatiei.
	Inainte de executia propriu-zisa trebuiesc tratate erorile de la exercitiul 2 si inlocuite variabilele ce au fost declarate anterior (inlocuirea o fac in timpul evaluarii expresiei).
	Operatorul ternar este tratat astfel: evaluez intai expresia si retin ce rezultat s-ar alege (1 = primul / 2 = al 2-lea) - acesta e retinut in variabila _indexRezultatTernar. Pe urma, incerc sa aduc rezultatul din dreapta operatorului '?' in locul fiulul operatorului ':'  - astfel voi avea ambele rezultate ca fiind 2 noduri cu acelasi parinte si pot alege raspunsul ecuatei bazandu-ma pe valoarea din _indexRezultatTernar.

// nu este adaptat sa parseze mai multe expresii ternare (una in alta) - am considerat doar cazurile din exemplu (in care expresia ternara e incadrata in paranteze).

Restul operatiilor se fac prin convertirea la intreg a stringurilor si pe urma trecerea rezultatului obtinut in noduri.



Obs / decizii de implementare:

- am nevoie ca nodul Expresie ca contina expresia originala deoarece nu mai pot deduce corect coloana la care se afla eroarea...si nici linia.

- recunoasterea expresiei cu regex (inainte de parsare) e facuta doar pentru a determina (si valida cat de cat) structura, astfel incat sa nu am expresii de forma '+3' '=5'.

- mai pot aparea functii mentionate de 2 ori (o data privat si o data public, cu diferenta probabil de un _) - deoarece una din functii e recursiva si are nevoie de mici modificari la apelare (la parametri).

- variabila 'areParanteze' este necesara altfel nu pot afla (prea usor) unde se aflau parantezele in expresia originala. 

- getter & setter pentru variabilele protected (chiar daca acestea sunt accesibile oricum) pentru un debugging mai eficient - sa urmaresc modificari ale variabilei.

	
