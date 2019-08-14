function r = IntervaleValProprii(d, s, m)
    [limita_inf limita_sup] = LimiteValProprii(d, s);
  
    % in cazul in care cer mai multe intervale decat valori proprii 
    m = min(m, NrValProprii(d, s, limita_sup)); 

    r(1) = limita_inf;
    r(m+2) = limita_sup; %adaugat pentru calcule, eliminat la final
    prevMij = -1;
    
    % metoda descrisa in indicatie
    for k=m:-1:1
        mij = (r(k+2) +r(1))/2; 
        h = r(k+2)-mij;  
        
        % nr de valori proprii mai mici decat 'mij'
        numvp = NrValProprii(d, s, mij);
   
        % se repeta calculul pana cand se gasesc toate valorile proprii 
        while k != numvp 
            h /= 2;
            if numvp < k
                mij += h;
            endif

            if numvp > k
                mij -= h;
            endif
            
            % actualizeaza nr de valori proprii pentru noul interval
            numvp = NrValProprii(d, s, mij);
            
            prevMij = mij;
        endwhile
        
        r(k+1) = mij;
    endfor;
    r = r(1:m+1); % elimina ultimul element 
endfunction
