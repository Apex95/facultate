function [x y nrPct nrSet] = read(fisier)
    fid = fopen(fisier, "r");
    date = str2num(fgetl(fid));

    for i=1:date(2)
        sirNr = str2num(fgetl(fid));
        
        x{i} = [sirNr(1:length(sirNr) / 2)];
        y{i} = [sirNr(1+length(sirNr) / 2:end)];
    endfor
    
    nrPct = date(1);
    nrSet = date(2);
endfunction
