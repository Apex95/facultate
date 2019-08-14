function vp = CalculezValProprii(d, s, m, eps)
    r = IntervaleValProprii(d, s, m);
    dimR = length(r);
    
    % se aproximeaza valoarea proprie pt fiecare interval din r
    for i=1:dimR-1
        vp(i) = AproxValProprie(i - 1, d, s, r(i), r(i+1), eps);
    endfor
endfunction
