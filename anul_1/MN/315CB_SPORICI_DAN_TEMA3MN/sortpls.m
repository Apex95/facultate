function [xRez yRez] = sortpls(x, y)
    n = length(x);

    % functie clasica de sortare (crescatoare) pt vectorul X
    % nothing to see here, move along, move along

    for i = 1:n-1
        for j = i+1:n
            if x(i) > x(j)
                x(i) += x(j);
                y(i) += y(j);

                x(j) = x(i) - x(j);
                y(j) = y(i) - y(j);

                x(i) -= x(j);
                y(i) -= y(j);
            endif            
        endfor
    endfor

    xRez = x;
    yRez = y;
endfunction
