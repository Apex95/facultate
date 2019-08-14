function CenterOfFighterJet()
    
    [x y z] = CitesteDate("contur.in");
    [Cx Cy] = CentroidOfSurface(x, y);
   
    printf('Centroid of surface: %.3f %.3f', round(Cx*1000)/1000, round(Cy*1000)/1000); 
    printf('\n');

    [x y z] = CitesteDate(strcat("date0.in"));
    Tr(1) = Trapez(x, y); % rezultate cu metoda trapezelor
    Sim(1) = Simpson(x, y); % rezultate cu formula Simpson
    z1(1) = z(1);

    [x y z] = CitesteDate(strcat("date1.in"));
    Tr(2) = Trapez(x, y);
    Sim(2) = Simpson(x, y);
    z1(2) = z(2);

    [x y z] = CitesteDate(strcat("date2.in"));
    Tr(3) = Trapez(x, y);
    Sim(3) = Simpson(x, y);
    z1(3) = z(3);

    [x y z] = CitesteDate(strcat("date3.in"));
    Tr(4) = Trapez(x, y);
    Sim(4) = Simpson(x, y);
    z1(4) = z(4);

    [x y z] = CitesteDate(strcat("date4.in"));
    Tr(5) = Trapez(x, y);
    Sim(5) = Simpson(x, y);
    z1(5) = z(5);

    [x y z] = CitesteDate(strcat("date5.in"));
    Tr(6) = Trapez(x, y);
    Sim(6) = Simpson(x, y);
    z1(6) = z(6);

    [x y z] = CitesteDate(strcat("date6.in"));
    Tr(7) = Trapez(x, y);
    Sim(7) = Simpson(x, y); 
    z1(7) = z(7);

    for i=1:7
        printf('%.3f ', round(Tr(i)*1000)/1000);
    endfor
    
    printf('\n');

    for i=1:7
        printf('%.3f ', round(Sim(i)*1000)/1000);
    endfor
    
    printf('\n');

    % volumul avionului este calculat integrand suprafetele in functie de Z
    I = Trapez(z1, Tr);
    printf('Volume of plane: %.3f ', round(I*1000)/1000);    

    printf('\n');

    fid = fopen('densitati.in');
    i = 1;
    while (!feof(fid))
        dens(i) = str2num(fgetl(fid));
        i++;
    endwhile;
    fclose(fid);

    I = 0;

    % pt calculul masei: se integreaza suprafetele sectiunilor transversale
    % in functie de z (rezulta un volum ce se inmulteste cu densitatea coresp.)
    % la final se face suma tuturor maselor 
    for i = 1:6
        I += dens(i) * Trapez(z1(i:i+1), Tr(i:i+1)); 
    endfor
    printf('Mass of plane: %.3f', round(I*1000)/1000);
    printf('\n');
endfunction
