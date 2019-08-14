function y = neville(x0, x, y)

    n = length(x);
    p = zeros(n);

    % matrice in care se retin valorile fiecarui polinom in x0
    % initial pe diagonala sunt chiar valorile Y date
    p = diag(y);

    for i=1:n
        for j=1:n-i
            % relatia de recurenta din algoritmul lui neville 
            p(j, i+j) = ((x0 - x(i+j)) * p(j, i+j-1) - (x0 - x(j)) * p(j+1, i+j)) / (x(j) - x(i+j)); 
        endfor
    endfor
    
    % polinomul final de interpolare este cel obtinut
    % pe prima linie, ultima coloana 
    y = p(1, n);
endfunction
