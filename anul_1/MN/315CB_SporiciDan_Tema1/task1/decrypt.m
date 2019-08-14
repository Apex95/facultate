function decrypt()
    fid = fopen("input1B", "r");
    str = fgets(fid);
    fclose(fid);

    fid = fopen("key1B", "r");
    n = str2num(fgets(fid));
   
    keyMatrix = zeros(n,n);
     
    for i=1:n
        _s = fgets(fid);
        keyMatrix(i,:) = sscanf(_s, '%d');
    endfor

    fclose(fid);
   
    % functie ce realizeaza inversa modula 29 a matricei    
    keyMatrix = invert(keyMatrix);
    
    strDim = length(str);

    % transformare litere => numere
    for i=1:strDim
        if (str(i) == " ")
            str(i) = 0;
        else 
            if (str(i) == ".")
                str(i) = 27;
            else
                if (str(i) == "'")
                    str(i) = 28;
                else
                    str(i) = uint8(str(i)) - 64;
                    if (str(i) >= 33)
                        str(i) -= 32;
                    endif
                endif
            endif
        endif
    endfor

    % transpunere in vector colana si inmultire cu matricea mod 29
    str = str';
    for (i=0:strDim/n-1)
        str(i*n+1:(i+1)*n) = mod(keyMatrix * str(i*n+1:(i+1)*n), 29);
    endfor

    % sterge zero-urile adaugate initial la criptare
    str = str(1:find(str, 1, 'last'));
    strDim = length(str);

    fid = fopen("output1B", "w");

    % conversie inapoi la text & afisare
    for (i=1:strDim)
        if (str(i, 1) == 0)
            fprintf(fid, " ");
        else if (str(i, 1) == 27)
                fprintf(fid, ".");
             else
                if (str(i, 1) == 28)
                    fprintf(fid, "'");
                else
                    fprintf(fid, '%c', char(str(i, 1) + 96));
                endif
             endif
        endif
    endfor
    fclose(fid);    
    
endfunction
