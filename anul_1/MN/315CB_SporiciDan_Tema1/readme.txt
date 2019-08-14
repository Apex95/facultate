// 100p

Dan Sporici

Task1:

    A. Se citeste textul, si se adauga la final spatii pana cand lungimea sa
        devine multiplu de n. Fiecare caracter e transformat in numar 
        scazand 64 din pozitia sa in codul ASCII (exceptie facand " ",  "." 
        si "'"). Rezultatul se transpune intr-un vector coloana ce se imparte
        in vectori de dimensiune n - acesti vectori se inmultesc succesiv 
        cu matricea data iar la final se concateneaza. Ramane de facut conversia
        inapoi la text.

    B. Fiind data matricea, se calculeaza inversa modulo 29 cu metoda
        Gauss-Jordan doar ca aici se va inmulti fiecare linie
        cu inversul in Z29 al elementului de pe diagonala principala
        (pentru a obtine 1). Pentru a obtine 0-uri, linia curenta se 
        inmulteste cu un numar astfel incat atunci cand este adunata la
        o alta linie, suma a 2 elemente aflate pe coloana ce continea 1
        sa fie multiplu de 29, deci sa devina 0.

    C. Matricea gasita este A(n+1, n+1), avand elementele de pe diagonala
        principala egale cu 1 iar pe ultima coloana se afla k (dat in enunt).
        
        Un vector coloana de dimensiune n se prelungeste cu un 1, astfel incat
        sa se poata inmulti cu matricea A. 
        
        |1 0 k|   |a|   |a+k|
        |0 1 k| * |b| = |b+k|
        |0 0 k|   |1|   | k |

        In cazul in care vectorul trece dintr-o grupa de 1000 de caractere
        in alta (de ex: o parte din vectorul de n caractere se afla in
        grupa 1-1000 iar cealalta parte in 1001-2000) se decupeaza 
        doar partea ce se afla in prima grupa si se completeaza pana la n+1
        cu 1, apoi se inmulteste.

        Dupa inmultire se elimina din vector toate caracterele in plus.

Task2:
    
    Matricea data se inverseaza cu metoda partitionarii: initial se 
     alege un indice (nDiv2) care sa imparta matricea in 4 matrice egale.
     Daca acest lucru nu este posibil (matricea A1 (stanga-sus) sau matricea
     A4 (dreapta-jos sunt singulare) se incrementeaza / decrementeaza nDiv2
     si se verifica iar.

    Cand o partitionare favorabila este gasita, se incearca inversarea 
     pentru A1 si A4 - se repeta pana cand n <= 16, cand inversarea
     se face cu Gauss-Jordan.

    Pentru a determina A^(-n) stiind A^(-1):
     - se cauta o putere p (cat se poate de mare) astfel incat 2^p < n.
     - se calculeaza A^(-2) = A(-1) * A(-1), A(-4) = A^(-2) * A^(-2),
        A^(-8) = A^(-4) * A(-4), etc. - pana la A^(-p) si se salveaza
        rezultatele intr-o matrice 3D (asta pentru a nu efectua de mai
        multe ori aceeasi inmultire => timp de executie mai mic).
     - pornind de la A^(-p) se face inmultirea cu valorile
      calculate anterior pana cand se ajunge la A^(-n) (greedy).

    
