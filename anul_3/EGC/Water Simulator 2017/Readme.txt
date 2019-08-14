// 100p

1. Sporici, Dan, 335CB
2. WASD - deplasare sfera
   Mouse2 (right click) + WASD - deplasare camera
   Space - lanseaza o picatura de apa
   Left Arrow - slow down
   Right Arrow - speed up
   Down Arrow - normal speed

3. 
	GameWorld - clasa principala derivata din SimpleScene
   	
      Entity - clasa generica (orice obiect e considerat o entitate)
             - contine pozitia obiectului, mesha, etc.

      Water - apa propriu zisa cu setarile stabilite pt valuri si array-urile de varfuri & indici in functie de care se creeaza gridul.

      WaterDroplet - picatura de apa spawnata care formeaza valul
                   - contine functiile care sa aplice cele 2 forte (G si tensiunea suprafetei de apa) asupra picaturii + o lista de valuri
                     generate de picatura respectiva + functie de despawn cand nu mai exista valuri in lista.

      Wave - un val generat
           - contine centrul valului + functie de despawn (dupa un interval de timp).


Modul de lucru: in general se deseneaza suprafata lina a apei (fara valuri); apoi se itereaza prin lista de picaturi, iar pe urma pt 
fiecare picatura prin lista sa de valuri generate; 

Pt fiecare val rulez shaderele si afisez rezultatul peste suprafata apei (valul ce trebuie desenat se afla intre 2 cercuri a caror raza e 
direct proportionala cu timpul si viteza de faza a undei). Tot ce nu se afla intre cele 2 cercuri ia discard in fragment shader.

Pt slowmotion / fastforward: retin segmente de timp inmultite cu viteza cu care se cere afisarea, fac suma lor si obtin un offset;
astfel pot modifica variabila timp din ecuatie fara sa se deplaseze undele.

4. Lucruri care mi-au luat mai mult timp:
   Determinarea vectorului de directie a undei cand picatura nu loveste fix in centrul meshei (aka problema din imaginile date in cerinta).
   Debugging la partea de creare a gridului (InitFromBuffer retinea nr de indici intr-un ushort, facea overflow...); tot are putine puncte.
   Gasirea unor coeficienti ca sa arate mai ok.
   
6. Cred ca tot fara bonus.
   
   
   





     
