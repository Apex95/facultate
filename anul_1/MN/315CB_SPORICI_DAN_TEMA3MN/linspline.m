function [y0] = linspline(x0, x, y)
    y0 = 0;
    n = length(x);

    % pt fiecare punct x0, determin un xMin si un xMax
    % iMin, iMax = indicii acelor puncte in vectorul X
    iMin = -1;
    iMax = -1;

    xMax = inf;
    xMin = -inf;

    for i = 1:n
        if (x(i) >= x0 && x(i) < xMax)
            xMax = x(i);
            iMax = i;
        endif

        if (x(i) <= x0 && x(i) > xMin)
            xMin = x(i);
            iMin = i;
        endif
    endfor
     

    % caz special in care xMin == x0 == xMax (deci x0 se afla in X)
    if (iMin == iMax)
        iMax++; % interp. folosind x0 si urmatorul punct din vectorul X
        
        % caz in care iMin = n, deci iMax ar iesi din vectorul X 
        if iMax > n
            iMax -=n % interpoleaza folosind prima si ultima valoare din X
        endif
    endif

    % interpolarea folosind splineuri liniare
    if (iMin != -1 && iMax != -1)
        y0 = y(iMin) + (y(iMax)-y(iMin))/(x(iMax) - x(iMin))*(x0-x(iMin));
    endif

endfunction
