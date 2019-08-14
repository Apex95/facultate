// 100p+


Am folosit laboratorul 8 pe post de schelet.


-- Descriere generala pt varianta cu 1 singur fisier --

La rulare, dupa deschiderea si citirea fisierului toate datele ce vor fi necesare
pt SPU (id, pointeri catre cele 2 structuri in care se retin imaginile, nr de blockuri,
mod de rulare, etc.) sunt incluse intr-un array de structuri io_addr_t.

Pt N threaduri SPU, vor exista N elemente in array iar la inceput fiecare primeste
adresa elementului corespunzator.

Fiecare SPU urmeaza sa citeasca de la adresa primita in "argp" structura io_addr_t si
folosindu-se de aceasta sa obtina adresele la care se afla cele 2 structuri de interes:
pnm_img_t si ccc_img_t.

De aici, fiecare SPU, cunoscandu-si id-ul unuic atribuit de PPU (tranmis tot prin strctura
io_addr_t) si numarul de SPU-uri folosite, va sti ce blockuri sa ia cu get(), sa prelucreze
si sa trimita inapoi spre PPU folosind put().

Preluarea unui bloc se face determinand coordonatele acestuia in functie de indice si
cerand cele BLOCK_SIZE linii cu get() din fiecare vector (red, green si blue).

La final blockul prelucrat este scris la adresa out_ccc.blocks + indicele blockului.

-- Analiza performantei --

Din graficele obtinute se observa performante bune chiar si cu un singur thread SPU
(versiunea seriala) pentru imaginile de dimensiuni mici - caz in care cresterea
numarului de threaduri adauga doar overhead (pentru crearea fiecarui thread).

Pentru imaginile de dimensiuni mari, se observa insa eficienta prelucrarii in paralel
a unor portiuni obtinandu-se timpi minimi daca se utilizeaza 4 threaduri SPU, acestia
crescand cu cat se reduce numarul de threaduri. Totodata se observa ca rularea cu 8 threaduri
implica o crestere a timpului de executie.





-- Vectorizare --

Implementata doar pt calculul luminantelor si al mediei (fara bitmap).
Rezolvarea consta in aducerea datelor din block din "unsigned char" in
"unsigned int" (cast si le trec intr-un array de unsigned int).

Pe urma trec array-urile obtinute la array-uri de "vector unsigned int"
pe care-i convertesc in cele din urma la "vector float" folosind spu_convtf().

Rezultatele le inmultesc cu vectori obtinuti din array-uri de 4 floaturi
identice, hardcodate, ce reprezinta coeficientii pt red, green si blue,
astfel incat sa pot prelucra 4 float-uri simultan.

Calculul luminantei medii o fac adunand toti vectorii rezultati la pasul
anterior si retinand suma intr-un alt vector. Fiecare din cele 4 valori
din vectorul suma se imparte la nr. de elemente din vector si obtin 4 medii
aritmetice. Din acest vector determin luminanta medie efectuand inca 
o medie aritmetica.

Nu am implementat si calcularea bitmap-ului folosind vectori.


-- Analiza performantei --

Se pot observa in unele cazuri timpi putin mai mici, comparativ cu varianta
originala, cel mai probabil datorati executarii operatiilor aritmetice simultan
pe cate 4 float-uri, in loc de 1 singur.



-- Modul de procesare a mai multor fisiere --

Problema intampinata aici a fost sa stiu exact cand terminau toate SPU-urile
de procesat blockurile din fisier astfel incat sa pot face scrierea si sa
trec la urmatoarea imagine.

In rezolvare am mai creat un set de threaduri care sa asigure comunicarea
dintre PPU si fiecare SPU in parte.

Inainte de a trimite o imagine spre procesare, PPU va face lock pe
"work_in_progress_lock". Dupa va incerca la final sa faca iar un lock
& unlock si va intra in asteptare.
 
Cand un SPU ramane fara blockuri de procesat trimite catre PPU prin mailbox
un mesaj cu status == 1. In PPU threadul pt comunicare primeste mesajul, 
face un lock si incrementeaza valoarea din variabila "nr_spus_finished", 
pe urma unlock. Tot atunci se verifica daca "nr_spus_finished" (var globala)
este egala cu nr de SPU-uri folosite. Daca da, se face unlock pe "work_in_progress_lock"
deblocand astfel threadul principal al PPU-ului care va putea scrie fisierul pozei.

De mentionat: SPU va astepta prin mailbox un mesaj de la PPU cand o noua poza
este disponibila pentru comprimare - pe urma va incepe preluarea datelor.

Cand nu mai exista imagini, PPU va trimite un status == 0 catre fiecare SPU,
oprind astfel buclele de procesare; SPU va trimite status == 2, anuntand ca se opreste
si va astepta un ultim mesaj de la PPU (altfel exista riscul ca PPU sa nu apuce sa citeasca
mesajele trimise de SPU); pe urma se inchide.



-- Double buffering --

Mecanismul de double buffering a fost folosit cu scopul de a efectua citiri
de pe magistrala si prelucrari in acelasi timp.

Fiecare SPU citeste initial un block pe urma intra intr-o bucla while
in care citeste un al 2-lea, asteapta ca primul sa fie primit (wait folosind
tag-ul asociat) si incepe prelucrarea acestuia. La final se trimite inapoi
blockul prelucrat, se modifica indicele de buffer (buf & nxt_buf) astfel 
incat sa se prelucreze blockul 2 si sa se citeasca al 3-lea.

Pt acest caz, while-ul face o nr_blocks+1 iteratii astfel incat la ultima
sa faca doar comprimare si put() (fara citirea unui alt bloc).


-- Analiza performantei --

Se observa mici imbunatatiri ale timpului de executie la aproape toate imaginile
(relevant fiind la cele de dimensiuni mari unde se transfera mai multe block-uri
pe magistrala) in cazul in care acestea folosesc 4 threaduri SPU sau mai putine.



Sporici Dan / 335CB

