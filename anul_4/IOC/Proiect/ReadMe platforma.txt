------ Intro ------

Intreaga platforma este construita in PHP, avand in spate o baza de data mysql,
in care se retin informatiile utilizatorilor.

Scopul acesteia este de a realiza conexiunea intre mai multi clienti si de a
colecta informatii (primite prin GET) sau de a raspunde clientilor prin mesaje
in format JSON.


------ Structura ------

Baza de date consta in 5 tabele:

 - users - include strict datele de autentificare ale fiecarui utilizator + email si data ultimei autentificari
 - profiles - contine informatii aditionale despre utilizatori (nume complet, zi de nastere, biografie, etc.)
 - activities - cuprinde toate activitatile create de clienti si informatii despre acestea (locatie, timp, nr de membri, raza, etc.) 
 - tags - cuprinde tagurile pt activitatile mentionate anterior (astfel incat o activitate sa poata avea oricate taguri custom)
 - parties - realizeaza asocierea intre utilizatori si activitati, in scopul participarii (grupeaza utilizatorii pt o activitate)
 
 
-- auth.php

Folosit pt autentificare si creare de conturi.

Functionalitati:
- login: /auth.php?action=login&username=NumeUtilizator&password=ParolaUtilizator
- inregistrare: /auth.php?action=register&username=NumeUtilizator2&password=ParolaUtilizator&email=EmailUtilizator2@ceva.tld
- logout: /auth.php?action=logout

exemplu de JSON returnat:
{"type":1,"text":"Login successful"} 


unde type = id al mesajului pt identificare in cadrul aplicatiei
     text = scurta descriere


-- profile.php

Pt preluarea si actualizarea informatiilor ce tin de profilul utilizatorilor.

Functionalitati:
- update al diverselor informatii
    /profile.php?action=update&description=that guy
    /profile.php?action=update&birthday=12-12-2000
    /profile.php?action=update&first_name=Ion
    /profile.php?action=update&last_name=Popescu
    
    - /profile.php?action=update cu o imagine trimita in POST realizeaza upload-ul noului avatar si actualizarea in BD.
    
- vizualizare profil
    /profile.php?action=view - ofera informatii despre profilul utilizatorului curent
    /profile.php?action=view&username=ionPopescu - ofera informatii despre profilul lui 'ionPopescu'
    
exemplu de JSON returnat:
{"type":13,"text":"Profile information","first_name":"Ion","last_name":"Popescu","img_url":"\/viewer.php?img=ionPopescu-19825125.png","birthday":29-09-1999,"description":"that guy"}



-- activities.php

Permite preluarea si adaugarea de activitati +participarea la acestea (sau anularea participarii).

Functionalitati:

- adaugarea unei noi activitati cu un request de forma:
    /activity.php?action=add&time=13000&lat=44.445719&long=26.052611&max_members=4&tags=bere,ciocolata_calda,ioc&max_range=2

    unde tags = lista de taguri ce permit cautarea personalizata a activitatii
         time = unix timestamp pt cand e planificata activitatea
         lat/long = coordonatele (latitudine si longitudine) folosite pt a determina distanta (cu haversine)
         max_members = nr maxim de participanti
         max_range = distanta maxima (in km)
         
- preluarea listei de activitati:
    /activity.php?action=list&tags=ciocolata_calda,ioc&lat=44.445719&long=26.052611&max_range=2

Exemplu de JSON intors, pt o cerere cu tagul "ioc":

{"type":15,"text":"List of activities, given the tags","activity":{"1584427984948200":{"author":"dan","time":"1970-01-01 04:53:20","lat":"44.445719","long":"26.052611","max_members":"4","tags":"ioc,"},"1584425390559500":{"author":"dan","time":"1970-01-01 04:36:40","lat":"44.445719","long":"26.052611","max_members":"4","tags":"ciocolata_calda,bere,ioc,"}}}


- participarea la o activitate:
    /activity.php?action=attend&lat=44.445719&long=26.052611&activity_id=1584436384829200
    
    * se dau: locatia curenta si id-ul activitatii - astfel incat sa se poata verifica distanta fata de eveniment.
    
- anularea participarii:
    /activity.php?action=quit&activity_id=1584436384829200
    
    
-- viewer.php

    Simplu utilitar ce permite afisarea unor poze fara a face hotlinking la acestea.
    Un link catre acesta este intors in atributul `img_url` din raspunsul JSON primit la vizionarea unui profil.
    
-- db.php

    Clasa ce realizeaza conexiunea la baza de date si exporta cateva functii ajutatoare (pt a mai reduce din cod).
    
-- messages.php
    
    Contine toate mesajele de raspuns in format JSON:
        JSON_CONSTS - numele etichetelor/tagurilor
        MESSAGE_TYPE - id-uri de mesaje (valori)
        MESSAGE_DESC - descrieri ale mesajelor (valori)
        
-- miscs.php

    Functii generice / ajutatoare printre care si `constructResponse()`, ce genereaza un mesaj JSON folosind 2 array-uri
    (unul cu nume de etichete si altul cu valorile corespunzatoare). Functioneaza si pe array-uri nested.
    
-- user.php

    Functii ce tin de starea utilizatorului (creeaza o sesiune de autentificare + stergerea sesiunii + preluarea username-ului, etc.)
 
------ Note aditionale ------

Mesajele (si datele de autentificare) sunt trimise in plain text si prin GET doar pt a face mai usoara partea de debugging.