function [invertedA] = invert(A)
    [n n] = size(A);
    invertedA = eye(n);

    A = mod(A, 29);
 
    % metoda Gauss-Jordan doar ca aici se inmulteste
    % cu inversul numarului in Z29 in loc sa se imparta

    %inversul e dat de functia invmod29(nr)
       
    for i = 1:n
        invertedA(i, :) = mod(invertedA(i, :) * invmod29(A(i, i)), 29);
        A(i, :) = mod(A(i, :) * invmod29(A(i,i)), 29); 

        for j = 1:n
            if (i != j)
                invertedA(j, :) = mod(invertedA(j, :) + invertedA(i, :) * (29 - A(j, i)), 29); 
                A(j, :) = mod(A(j, :) + A(i, :) * (29 - A(j, i)), 29);
            endif
       endfor
    endfor
    
endfunction
