// 100p

-- Implementare (partiala) --

Conexiunea de la clienti la server e una simpla, prin TCP - comunicatia fiind facuta cu ajutorul structurilor "Command" si
"DataMsg"; prima folosita pt trimiterea de comenzi si linkuri iar a 2-a pt trimiterea de date & informatii despre fisiere.

Pentru fiecare client, memorez descriptorul, 2 campuri (isConnected - daca e in continuare conectat si isBusy - daca
e ocupat sau nu cu downloadul unei pagini) si structura sockaddr_in cu datele despre IP si port.

Atat clientul cat si serverul sunt implementati folosind select-uri cu un timeout de 1 secunda.

-- Comanda status --

Iterez prin array-ul de clienti (luand fiecare descriptor in parte) si verific starea clientului aflat
la indicele dat de descriptor (verific isConnected). Daca e conectat, preiau datele din structura sockaddr_in si afisez.

-- Comanda download --

Am incercat sa implementez direct versiunea "download everything" astfel: pastrez la server un hashtable cu pagini deja downloadate
si o stiva cu pagini ce urmeaza sa fie descarcate.

La fiecare iteratie prin array-ul de clienti (cand verific descriptorii) scot cate un url din stiva (daca sunt mai mult de 5 clienti)
si il trimit clientului curent, daca acesta nu e ocupat (isBusy). Clientul il consider ocupat din momentul in care i-am trimis o pagina pt download si pana cand el trimite ultimul mesaj din seria celor ce contin adrese parsate (cele extrase din <a href="...">). Fiecare adresa e adaugata si in hashtable astfel incat pagina sa nu fie downloadata de 2 ori.

Parsarea url-ului (pt identificarea hostului si a path-ului) + extragerea linkurilor din tag-urile <a> se fac cu 2 expresii RegEx, in
aplicatia Client.

Clientul primeste adresa, extrage hostname-ul, preia adresa IP (gethostbyname) si incearca o conexiune TCP la adresa respectiva, pe portul
80, unde trimite cererea pt path-ul primit (HTTP/1.0). La final, intoarce continutul ei (+headerul) la server. Daca ultimul element din url nu e fisier (nu contine extensie) se considera a fi folder si se concateneaza la final "/index.html".

Pe urma incepe sa trimita linkurile gasite. Serverul verifica daca un fisier deja exista cu acel nume - daca nu, inseamna ca se primeste 
primul mesaj, care va contine si headerul - care aparent, nu trebuie memorat. La final clientul trimite un mesaj cu comanda CMD_AVAILABLE
prin care informeaza ca nu mai este ocupat si poate accepta o noua comanda de download.



Known bugs:
- nu iau in calcul adrese ce contin ".." deoarece se pot obtine astfel f. multe linkuri care duc spre aceeasi pagina si nu le pot
deosebi doar cu un hashtable, fara parsare
- posibile probleme la ierarhia de fisiere, deoarece in site-ul de test sunt incluse si linkuri relative precedate de backslash


