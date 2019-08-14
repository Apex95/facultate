function numvp = NrValProprii(d, s, lambda)

    % obtine polinoamele P1(lambda), P2(lambda)...
    P = ValoriPolinoame(d, s, lambda);
    n = length(P);
    numvp = 0;

    % cauta schimbarile de semn in sirul Sturm  
    zeroSign = 0; % retine ultimul 'semn' atribuit lui 0 (opusul valorii anterioare)
    for i=1:n-1
        if sign(P(i)) != sign(P(i+1)) % daca semnele difera => exista o valoare proprie
            if zeroSign == 0 || zeroSign != sign(P(i+1))
                numvp++;
                zeroSign = 0;
            endif;
            if sign(P(i+1)) == 0
                zeroSign = sign(P(i)) * (-1); %considera ca semnul lui 0 este sign(-P(i))
            endif;  
        endif;
                 
    endfor
endfunction
