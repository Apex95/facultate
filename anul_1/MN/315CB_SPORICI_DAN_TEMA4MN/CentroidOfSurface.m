function [x0, y0] = CentroidOfSurface(x, y)
    
    n = length(x);
    Cx = 0;
    Cy = 0;
    A = 0;

    % ariile portiunilor se calculeaza separat pt fiecare 2 puncte
    % prin formarea unui triunghi cu originea si calcularea ariei acestuia
    % acele arii obtinute se inmultesc cu centrele de greutate ale 
    % triunghiurilor, suma care pe urma se imparte la aria totala si rezulta
    % centroidul avionului
    for i=1:n-1
        Cx += (x(i) + x(i+1))*(x(i)*y(i+1) - x(i+1)*y(i));
        Cy += (y(i) + y(i+1))*(x(i)*y(i+1) - x(i+1)*y(i));
    endfor

    % aria totala calculata cu Simpson
    A = Simpson(x, y);

    Cx ./= (6*A);
    Cy ./= (6*A);

    x0 = Cx;
    y0 = Cy;
endfunction
