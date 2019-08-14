// 100p+

1. Sporici Dan / 335CB
2. WASD - deplasare
   Mouse1 - lanseaza un proiectil
   Mouse2 - rotatie camera / caracter

3. Implementarea porneste din clasa GameWorld (derivata din SimpleScene) unde adaug initial shaderele, creez camera, turnurile si iconitele pt minimap.

La fiecare Update() incerc sa creez un inamic (Enemy_morph) pe care-l adaug in 2 liste - una numita "enemies" si cealalta numita "entities".

In momentul update-ului, pt fiecare element din lista "entities" se apeleaza o functie de update() locala (specifica fiecarei clase) ce se ocupa, in functie de caz, de deplasarea inamicilor, coliziunea proiectilelor, rotatia vietilor in jurul playerului, death-animation, etc.

Dupa desenarea spatiului 3D si a vietilor jucatorului, mut camera deasupra hartii si desenez in spatiul alocat minimapului.

Fiecare entitate are la baza clasa Entity ce contine informatii generale (pozitie, unghiuri de rotatie, vector de orientare, tip de material, raza de coliziune etc.) si o functie numita CanDespawn(). Aceasta e apelata in cadrul functiei GameWorld::Update() si este folosita pt a sterge entitati ce si-au indeplinit scopul (inamici care au ajuns la final sau care au 0 HP, proiectile care au facut coliziuni sau ajuns prea departe).

Mai exista 2 dictionare in WeaponsDB.h si MaterialsDB.h unde am trecut proprietatile materialelor si ale armelor / proiectilelor.




Entitati/Clase:

	- Enemy_Morph - modelul de inamic, primeste o lista de waypointuri si o referinta catre player; la update fie executa o deplasare catre waypoint, fie animatia de final fie scade jucatorului o viata si dispare.

	- Life - pt cele 3 vieti ale jucatorului - rotatiile & translatiile se fac din GameWorld::Update() deoarece trebuie facute cateva operatii in plus pe modelMatrix

	- MapIcon - modelul de iconita din minimap, facuta cu un plan colorat diferit in functie de entitatea reprezentata.

	- Player - primeste o referinta catre lista de vieti si alta catre lista de entitati (ca sa pot adauga eventualele proiectile lansate); in update exista doar animatia de final.
	
	- Projectile - primeste o lista de inamici si tipul armei din care a fost tras; astfel se poate face aici verificarea coliziunilor si se poate ajusta traseul proiectilului in functie de arma.

	- Tower - primeste o lista cu inamicii din harta si cu entitatile spawnate; proiectilele lansate sunt similare celor din grenade launcher (bolta / range / damage).

4. Determinarea unghiului de rotatie a inamicului catre urmatorul waypoint, cunoscand vectorul de orientare (in cod, unghiul dintre 'orientation' si 'forward'). Problema aparea atunci cand unghiul nu era format in sens trigonometric si trebuia inmultit cu -1.

6. Bonus?: niste lumina & culori (iluminare facuta in fragment shader / modelul Phong).

   





 
