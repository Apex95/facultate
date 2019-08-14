function FB
    [x y nrPct nrSet] = read("totti.txt");
    hold on; 
    
    for i = 1:nrSet
        [x{i} y{i}] = sortpls(x{i}, y{i});
        x2 = linspace(x{i}(1), x{i}(end), nrPct);     
        
        % la fel ca la F3, doar ca se folosesc splineurile tensionate
        coef = cubespline(x{i}, y{i}, 't');

        for j = 1:nrPct
            y2(j) = computepoint(x2(j), x{i}, coef);

        endfor
        plot(x2, y2);
    endfor

endfunction
