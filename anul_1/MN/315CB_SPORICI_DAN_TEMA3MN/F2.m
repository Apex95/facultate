function F2
    [x y nrPct nrSet] = read("barcelona.txt");
    hold on;

    for i = 1:nrSet
        [x{i} y{i}] = sortpls(x{i}, y{i});        
        x2 = linspace(x{i}(1), x{i}(end), nrPct);

        % cam acelasi fisier ca la F1 (si F3 si FB...), 
        % doar ca se calculeaza Y folosind splineuri liniare
        for j = 1:nrPct
            y2(j) = linspline(x2(j), x{i}, y{i});

        endfor

        plot(x2, y2);
    endfor

endfunction
