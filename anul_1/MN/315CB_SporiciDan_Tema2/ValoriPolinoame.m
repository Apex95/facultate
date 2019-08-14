function P = ValoriPolinoame(d, s, lambda)
    n = size(d);
    
    P(1) = 1;
    P(2) = d(1) - lambda;

    % calculeaza valoarea fiecarui polinom intr-un punct lambda
    % folosind formula de recurenta    
    for i=2:n
        P(i+1) = (d(i) - lambda)*P(i) - s(i-1)^2*P(i-1);
    endfor

endfunction
