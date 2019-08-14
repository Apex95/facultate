function F1

    [x y nrPct nrSet] = read("dodel.txt"); 
    hold on;

    for i = 1:nrSet
        % sorteaza datele de intrare
        [x{i} y{i}] = sortpls(x{i}, y{i});

        % genereaza un set propriu de X-uri 
        x2 = linspace(x{i}(1), x{i}(end), nrPct);

        % calculeaza valorile Y coresp, folosind Neville
        for j = 1:nrPct
            y2(j) = neville(x2(j), x{i}, y{i});
        endfor

        plot(x2, y2);
    endfor


endfunction
