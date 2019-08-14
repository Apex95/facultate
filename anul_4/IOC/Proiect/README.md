# Platforma

/* let there be documentatie */


## Informatii de baza

Toate mesajele cu care raspunde "API"-ul sunt in **messages.php** (un id de mesaj si un text care sa descrie mesajul). Probabil folosim doar id pt identificarea lor.
Also...functia de generare a raspunsului JSON nu merge decat cu diacritice encodate cu UTF-8...this means: raspunsul se da encodat...decodarea ar trebui facuta pe aplicatie.

Pt nesabuitii care vor sa contribuie la partea de PHP: functia aia bizara care generaza JSON (`constructResponse()`) ia 2 array-uri de dimensiune egala ca parametri;
primul contine numele etichetelor JSON iar al 2-lea valorile; functia din PHP interpreteaza orice array de forma ["nume_eticheta" => "valoare_eticheta"].


## Exemple de query-uri disponibile

// am ip dinamic, sanse mari sa nu mai mearga fix aceleasi adrese :/

### Autentificare

Login:

`86.127.43.7/platform/auth.php?action=login&username=dan&password=dan`


Exemplu de JSON:

`{"type":1,"text":"Login successful"}`

Register + login:

`http://86.127.43.7/platform/auth.php?action=register&username=dan2&password=dan&email=dan@dan.dan`


Logout:

`http://86.127.43.7/platform/auth.php?action=logout`


### Profil

Modifica datele de profil pt contul curent:

`http://127.0.0.1/platform/profile.php?action=update&description=that guy`

`http://127.0.0.1/platform/profile.php?action=update&birthday=12-12-2000`

`http://127.0.0.1/platform/profile.php?action=update&first_name=Dan`

`http://127.0.0.1/platform/profile.php?action=update&last_name=Sporici`


Pt poza de profil: trimiti poza prin POST in variabila **profile_image** tot catre `/profile.php?action=update`.

La afisare: un request catre `/viewer.php?img=dan-1511105501-me.jpg` si iti intoarce direct imaginea (linkul il gasesti in raspunsul JSON, **img_url**).


Preia datele de profil pt contul curent:

`http://86.127.43.7/platform/profile.php?action=view`

Preia datele de profil pt **dan** (=username):

`http://86.127.43.7/platform/profile.php?action=view&username=dan`

Exemplu de raspuns JSON:

`{"type":13,"text":"Profile information","first_name":"Dan","last_name":"Sporici","img_url":"\/viewer.php?img=dan-19825125.png","birthday":null,"description":"that guy"}`

// nota: se intoarce un URL catre poza de profil (ca sa n-o tin in BD si sa ne facem viata mai grea) -> `http://86.127.43.7/platform/viewer.php?img=dan-19825125.png`


### Activitati

Adauga o noua activitate:

`http://127.0.0.1/platform/activity.php?action=add&time=13000&lat=44.445719&long=26.052611&max_members=4&tags=bere,ciocolata_calda,ioc&max_range=2`

**time** = timestamp (convertesti un moment de timp si-l trimiti asa ca sa avem precizie pt ore si minute)

**max_range** e in **km** (deocamdata)

JSON intors:

`{"type":17,"text":"Successfully added a new activity"}`


Preia toate activitatile cu tag **ciocolata_calda** si **ioc**, cu range mai mic de **2 u.m.** fata de locatia curenta **(44.445719, 26.052611)**:

`http://127.0.0.1/platform/activity.php?action=list&tags=ciocolata_calda,ioc&lat=44.445719&long=26.052611&max_range=2`

obs: conditia de **range** trebuie sa fie respectata din ambele parti; adica...trebuie sa indeplineasca si conditia de max_range
data de utilizator (activitatea sa nu fie prea departe) si conditia setata la crearea activitatii respective (utilizatorul sa nu fie prea departe). 



Exemple de raspunsuri JSON:

`{"type":15,"text":"List of activities, given the tags","activity":{"1584425390559500":{"author":"dan","time":"1970-01-01 04:36:40","lat":"44.445719","long":"26.052611","max_members":"4","tags":"ciocolata_calda,bere,ioc,"}}}`

`{"type":15,"text":"List of activities, given the tags","activity":{"1584427984948200":{"author":"dan","time":"1970-01-01 04:53:20","lat":"44.445719","long":"26.052611","max_members":"4","tags":"ioc,"},"1584425390559500":{"author":"dan","time":"1970-01-01 04:36:40","lat":"44.445719","long":"26.052611","max_members":"4","tags":"ciocolata_calda,bere,ioc,"}}}`


Participa la o activitate:

N-am implementat partea cu approve / reject... **TODO** I guess.

`http://127.0.0.1/platform/activity.php?action=attend&lat=44.445719&long=26.052611&activity_id=1584436384829200`

Anuleaza participarea:

`http://127.0.0.1/platform/activity.php?action=quit&activity_id=1584436384829200`

**TODO:** la participare, o verificare sa nu se inscrie de mai multe ori; la anulare, o verificare & stergere a activitatii daca nu mai raman membri ?
eventual de inclus si autorul in tabelul "parties" cand creeaza activitatea.


## Detalii de implementare

Altadata ca-s mort de somn...


# Aplicatie

// TODO daca e nevoie