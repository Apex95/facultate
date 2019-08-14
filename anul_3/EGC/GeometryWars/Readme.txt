// 100p+

1. Sporici Dan / 335CB

2. W/A/S/D - miscarea navei
   Mouse1 - lanseaza un proiectil
   

3. O clasa principala numita GameWorld (mosteneste SimpleScene)
ce se ocupa de dinamica jocului in general (translatii, coliziuni, rendering, etc.) + o clasa GameObject ce contine functii / variabile comune pentru celelalte obiecte (vieti, coordonate, unghi de rotatie, factori de scalare, nume, culoare).

Din clasa GameObject deriv restul claselor (adica...un SimpleEnemy este un GameObject cu alta mesha si cu cateva functii in plus care sa-l indrepte spre jucator).

Fiecare GameObject are o functie Update(deltaTime) in care se realizeaza modificari de coordonate / unghiuri / culoare; aceasta functie este apelata pt fiecare obiect in parte inainte de desenare. Tot atunci se verifica si coliziuni de forma jucator-inamic, scut-inamic, proiectil-inamic - in cazul celei din urma aplicand regula ca in momentul impactului sa scad 1 viata atat proiectilului cat si inamicului si sterg elementul care a ajuns la 0 vieti (asta ca sa pot implementa si proiectile care doboare mai multi inamici - la partea de bonus).



4. Nu chiar...dar alinierea sistemelor de coordonate pt cursor si nava (data fiind transformarea fereastra-poarta uniforma) a luat ceva timp.

5. Framework-ul de la laborator + sursele pt joc in folderul GeometryWars.

6. Pt bonus am incercat sa implementez 3 tipuri de iteme ce ofera cate un power-up, spawnate la intervale de 4 secunde, la pozitii random si fiind reprezentate prin triunghiuri de diverse culori. Cel verde ofera jucatorului un scut ce rezista la o coliziune, cel alb permite lansarea a 4 proiectile "imbunatatite" care sa nu se opreasca dupa doar 1 inamic distrus (si care sa provoace mai mult damage) iar cel rosu aduce o viata in plus.

//-- alte modificari (nu neaparat bonusuri)

- sansa ca un inamic albastru (cu o singura viata) sa "evolueze" in unul galben (2 vieti) in timpul jocului (dupa spawn).

- limitarea spatiului de joc

- stergerea proiectilelor dupa 7 secunde (moment in care nu mai nimeresc inamici si nu se mai vad).
