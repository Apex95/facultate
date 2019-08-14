function vp = AproxValProprie(i, d, s, a, b, eps)

    mij = (a + b) / 2;
    prevMij = mij + 100 * eps;

    nrvp = NrValProprii(d, s, a);
    %disp(nrvp);
    % pt intervalul [a,b] aproximeaza valoarea proprie folosind metoda bisectiei
    % pana cand eroarea e mai mica decat eps
    while (abs(mij - prevMij) > eps)
         
        Pa = ValoriPolinoame(d, s, a)(end);
        Pb = ValoriPolinoame(d, s, b)(end);
        Pmij = ValoriPolinoame(d, s, mij)(end);
     
        % se alege jumatatea de interval in care exista schimbarea de semn
        if sign(Pa) != sign(Pmij)
            b = mij + eps;
        else
            if sign(Pmij) != sign(Pb)
                a = mij;
            endif
        endif
        
        prevMij = mij;

        mij = (a + b) / 2;

    endwhile
    
    % rezultatul obtinut
    vp = mij;
endfunction
