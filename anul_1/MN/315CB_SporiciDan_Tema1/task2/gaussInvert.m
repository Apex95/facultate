function [invertedA] = gaussInvert(A)
    [n n] = size(A);
    invertedA = eye(n);

    % calculeaza inversa unei matrice folosind Gauss-Jordan    
    for i = 1:n
        invertedA(i, :) = invertedA(i, :) / A(i, i);
        A(i, :) = A(i, :)/A(i,i); 

        for j = 1:n
            if (i != j)
                invertedA(j, :) = invertedA(j, :) - invertedA(i, :) * A(j, i); 
                A(j, :) = A(j, :) - A(i, :) * A(j, i);
            endif
        endfor
    endfor
    
endfunction
