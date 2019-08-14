// 100p

-- Implementare --

	Am incercat sa implementez acel Trie cat mai generic posibil, astfel incat sa trateze similar ambele tipuri de elemente (TrieWord1 pt case insensitive si TrieWord2 pt filtrarea caracterelor '-_()') - fara a modifica interfetele.

	Deoarece am avut la dispozitie doar un mijloc de a accesa TrieElement, functia toCharArray() va incerca initial sa intoarca cuvantul retinut in element; daca acesta este null, va intoarce caracterul (nodului). Pentru a face deosebirea dintre caracter si cuvant (ambele fiind returnate intr-un char[]), am adaugat un '0' (ElementInfo.HAS_CHAR) sau un '1' (ElementInfo.HAS_WORD) la finalul array-ului.


-- Add --

	Fiecare cuvant este adaugat caracter cu caracter (fiecarui caracter ii corespunde cate un nod si implicit cate un element). Elementul respectiv va contine caracterul curent (ce va fi preluat ulterior de nod) si cuvantul original. Acel cuvant este retinut doar de ultimul nod ce intra in compozitia cuvantului.
	La fiecare adaugare a unui caracter se verifica daca lastNode (care initial este chiar nodul radacina) are descendenti care sa contina deja acel caracter. Daca nu, resetez lastNode si caracterul se adauga pur si simplu - altfel pastrez valoarea lui lastNode pentru viitoare adaugari. La fel este si in cazul in care trebuie adaugat intreg cuvantul, doar ca atunci se insereaza un element nou in lista ultimului nod aflat in structura cuvantului.
	Tot la fiecare adaugare, nodul in care se retine elementul va prelua caracterul corespunzator - astfel atunci cand se sterg elemente, nodul se va putea utiliza in continuare si nu va "intrerupe" arborele.


-- Remove --

	Parcurg Trie-ul dupa fiecare caracter din cuvantul retinut in element si daca gasesc cuvantul respectiv, sterg un element din ultimul nod. 


-- Count --

	La fel, parcurg Trie-ul pt fiecare caracter si intorc dimensiunea listei de elemente a ultimului nod aflat in compozitia cuvantului cautat.
	Din acel numar mai este nevoie sa scad 1, in cazul in care in nod se afla si elementul original (cel ce contine doar un caracter si cuvantul null) - deoarece acesta nu e considerat cuvant.


-- List --

	Parcurg Trie-ul pentru prefixul dat si retin nodul in care se afla ultimul caracter al prefixului. Din acel nod, iau recursiv toate rutele spre extremitatile arborelui si cand gasesc un cuvant (HAS_WORD), adaug toate elementele din nodul la care am ajuns intr-o lista. Inevitabil vor exista duplicate, deoarece singurul mod de a compara 2 cuvinte este prin folosirea codificarii oferite de toCharArray(). Lista obtinuta se sorteaza folosind un comparator iar la afisare este necesar doar sa afisez cuvintele distincte si sa omit duplicatele.

--------------------------------

Mai multe detalii in comments.

