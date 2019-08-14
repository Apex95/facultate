function [x y z] = CitesteDate(fisier)
    fid = fopen(fisier, "r");

    % citirea datelor (contur, date0, date1, etc.)
    % densitatea e citita separat    
    i = 1;
    while (!feof(fid))
        date = str2num(fgetl(fid));
        x(i) = date(1);
        y(i) = date(2);
        
        if length(date) <3
            z(i) = 0;
        else
            z(i) = date(3);
        endif

        i++;
    endwhile
    fclose(fid);
endfunction
