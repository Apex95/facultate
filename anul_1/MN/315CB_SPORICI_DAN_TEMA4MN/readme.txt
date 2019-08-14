// 80p

== readme ==



1) Implementare Trapeze

    Formula folosita este asemanatoare cu cea originala doar ca h (inalimtea
trapezului) nu mai este constanta ci este calculata pt oricare din cele 2 
puncte.



2) Implementare Simpson

    Se iau punctele din 3 in 3 si se determina o parabola care sa uneasca
cele 3 puncte (a*x^2 + b*x + c = y); inlocuind X si Y cu valorile celor 3
puncte rezulta un sistem cu 3 necunoscute si 3 ecuatii din care se determina
coeficientii (a, b, c). Stiind ecuatia parabolei, aceasta se poate integra,
iar in ecuatia (primitiva) obtinuta se inlocuiesc valorile X, obtinand astfel
aria de sub graficul parabolei, deci integrala.



3) Centroidul conturului
    
    Folosind formulele (adaptate de la bratul/momentul fortei):
    A * Cx = A1*Cx1 + A2*Cx2 + ... + An*Cxn
    A * Cy = A1*Cy1 + A2*Cy2 + ... + An*Cyn

    unde A = aria totala (determinata cu Simpson); 
         A1, A2...An = ariile triunghiurilor ce intra in componenta conturului
         Cxn, Cyn = centrele de greutate ale triunghiurilor componente

    se pot determina coordonatele centroidului conturului (Cx si Cy).

    Considerand conturul avionului un poligon, acesta se poate imparti in
triunghiuri unind fiecare 2 puncte consecutive cu originea (0,0). Pentru
fiecare triunghi obtinut se poate calcula aria folosind metoda determinantului
=> Ai = 1/2 * (x(i) * y(i+1) - x(i+1) * y(i)).
    Tot pentru aceste triunghiuri se afla centrul de greutate: 
folosind relatia Cx = 1/3*(x(i) + x(i+1) + 0) (similar si pt Cy).

    Avand datele se pot determina Cx si Cy.



4) Ariile sectiunilor transversale 
    Se pot calcula prin integrarea directa a ecuatiei f(x) = y,  
x si y fiind date, se aplica direct metoda trapezelor sau Simpson 
(ariile de sub grafic nu se vor suprapune datorita diferentelor de sens 
- ramanand astfel doar aria sectiunii transversale).


5) Volumul avionului
    Se determina prin integrarea fiecarei suprafete a sectiunii transversale
in functie de Z.

6) Masa avionului
    Se integreaza fiecare zona dintre 2 sectiuni transversale si se inmulteste
cu densitatea medie corespunzatoare. La final se aduna rezultatele si se obtine
masa avionului.

7) Coordonatele centrului de masa
    n-am mai avut timp :(
