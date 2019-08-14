function matrixCipher()
    fid = fopen("input1A", "r");
    str = fgets(fid);
    
    % citeste textul ce trebuie criptat si inlatura 
    % caracterul '\n' daca exista la capat de linie
    if str(length(str)) == '\n'
        str = str(1:length(str)-1);
    endif
    fclose(fid);

    % citeste n si-l transforma in integer
    fid = fopen("key1A", "r");
    n = str2num(fgets(fid));
   
    keyMatrix = zeros(n,n);

    % citeste matricea din key1A     
    for i=1:n
        _s = fgets(fid);
        keyMatrix(i,:) = sscanf(_s, '%d');
    endfor

    fclose(fid);

    % prelungeste textul cu spatii astfel incat sa se poata inmulti 
    % cu matricea data
    while (mod(length(str), n) != 0)
        str = [str " "];
    endwhile 

    strDim = length(str);

    % converteste literele in numere dupa regula data in enunt
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
    
    % transpune textul obtinut intr-un vector coloana (pt inmultire)
    str = str';
    
    % vectorul obtinut e imparit in bucati de dimensiune N
    % fiecare bucata e inmutita cu matricea data si se aplica mod 29
    for (i=0:strDim/n-1)
        str(i*n+1:(i+1)*n) = mod(keyMatrix * str(i*n+1:(i+1)*n), 29);
    endfor
     
    fid = fopen("output1A", "w");

    % conversie inapoi la litere
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
