// 84p

-- Readme--

Implementarea consta doar in rezolvarea problemelor 
de sincronizare intre device-uri (cazul cu 1 thread per device).

Rezolvari:

- adaugarea unei bariere reentrante (creata de un device lider (rank 0))
si transmisa catre celelalte; dupa o iteratie a DeviceThread se va astepta
ca toate threadurile sa ajunga la bariera cu scopul de a mentine
toate deviceurile in acelasi timepoint

- adaugarea unui dictionar de lockuri pentru un set de locatii, astfel
incat intr-o zona, toate scripturile sa se execute "pe rand" si sa nu
apara comporament nedefinit

Sporici Dan / 335CB
