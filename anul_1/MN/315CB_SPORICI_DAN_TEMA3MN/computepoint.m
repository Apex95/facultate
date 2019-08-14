% determina y0 corespunzator unui x0, folosind coeficientii
% calculati si vectorul initial X (dat)
function y0=computepoint(x0, x, coef)
    i = 1;
    n = length(coef);
   
    % determina indicele (i) valorii maxime din X mai mica decat x0
    while (i < n/4) && (x0 > x(i+1))
        i++
    endwhile;

    % atunci i este indicele splineului folosit pt calcul
    x2(1:n) = 0;

    x2(4*i-3) = (x0-x(i))^3;
    x2(4*i-2) = (x0-x(i))^2;
    x2(4*i-1) = x0-x(i);
    x2(4*i) = 1;
    
    y0 = x2 * coef; % inmultirea cu vectorul de coeficienti
     
endfunction
