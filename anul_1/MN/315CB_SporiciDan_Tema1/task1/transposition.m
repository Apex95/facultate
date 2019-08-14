function [key] = transposition()
    fid = fopen("input1C", "r");
    kn = str2num(fgets(fid));
    k = kn(1);
    n = kn(2);

    str = fgets(fid);
        
    if str(length(str)) == '\n'
        str = str(1:length(str)-1);
    endif

    fclose(fid);

    strDim = length(str);

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
   
    % construieste o matrice de forma I(n+1), doar ca pe ultima coloana
    % se afla K (dat in problema) 
    key = eye(n+1);
    key(:,n+1) = k;

    % afiseaza matricea folosita pentru primele 1000 de codificari
    fid = fopen("key1C", 'w');
    for i=1:n+1
        fprintf(fid, '%0.6f ', key(i,:));
        fprintf(fid, '\n');
    endfor    
    fclose(fid);

    i = 1;
    while (i <= strDim)
        % actualizeaza matricea de transpozitie - in cazul in care
        % se trece la urmatoarea grupa de 1000 de caractere
        if (key(1, n) != fix((i-1) / 1000) + k)
            key(:,n+1) = k + fix((i-1) / 1000);
        endif

        % trebuie luati vectori de dimensiune n
        % pana in 1000 de caractere

        % se calculeaza lungimea vectorului ce trebuie inmultit
        % astfel incat sa nu se afle intre 2 grupe 
        % de ex intre 1-1000 si 1001-2000
        nOfChars = min(n, mod(fix((1000+i)/1000) * 1000 - i, 1000) + 1);
        
        % se verifica apoi ca indicele sa nu depaseasca nr de caratere
        % ramase pana la sfarsitul textului
        nOfChars = min(nOfChars, strDim - i + 1);
        
        % completez cu 'nOfChars' caractere din str
        v(1:nOfChars, 1) = str(i:i+nOfChars-1);
        
        % mai adaug cativa de 1 pana ce vectorul ajunge la dimensiunea
        % corecta
        v(nOfChars+1:n, 1) = 1;

        % verific sa existe un 1 pe ultima pozitie astfel incat
        % inmultirea cu matricea sa ofere rezultatul dorit
        v(n+1, 1) = 1;

        % inmultirea propriu-zisa
        v = key * v;

        % sterg tot ce am adaugat si las doar vectorul original
        v = v(1:nOfChars);

        % refac textul pt vectorul curent
        encryptedStr(1, i:i+nOfChars-1) = v(1:nOfChars, 1);

        % incrementez indicele de la care se va continua criptarea textului
        i += nOfChars;
        
    endwhile
    
    
    encryptedStr = mod(encryptedStr, 29);
        
    fid = fopen("output1C", "w");
    for (i=1:strDim-1)
        if (encryptedStr(1, i) == 0)
            fprintf(fid, " ");
        else if (encryptedStr(1, i) == 27)
                fprintf(fid, ".");
             else
                if (encryptedStr(1, i) == 28)
                    fprintf(fid, "'");
                else
                    fprintf(fid, '%c', char(encryptedStr(1, i) + 96));
                endif
             endif
        endif
    endfor
    fclose(fid);
endfunction

