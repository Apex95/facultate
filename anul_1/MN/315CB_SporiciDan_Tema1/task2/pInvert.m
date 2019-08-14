function [invA] = pInvert(A)
    n = length(A);

    % pentru un n mai mic decat 16, inversez direct cu Gauss-Jordan
    if n <= 16
	    invA = gaussInvert(A);
	return;
    else
        nDiv2 = fix(n/2); % incerc sa partitionez matricea in 4 matrice egale

        A1 = A(1:nDiv2, 1:nDiv2);
        A4 = A(nDiv2+1:n, nDiv2+1:n);

        k = 1;
     
        % verific daca au fost impartite corect matricele (A1 & A4 nesingulare)
        while det(A1) == 0 || det(A4) == 0
            nDiv2 += k; % daca una din ele are det 0 => caut alta partitionare

            A1 = A(1:nDiv2, 1:nDiv2); 
            A4 = A(nDiv2+1:n, nDiv2+1:n);
        
            if nDiv2 > fix(n/2)
                k = -1;
            endif

            % daca nDiv2 ajunge sa fie negativ => nu se poate partitiona
            if nDiv2 <= 0
                return; % 
            endif
        endwhile
    
        % calculez inversele tot prin partitionare pt A1 & A4
        invA1 = pInvert(A1);
        invA4 = pInvert(A4);

        A2 = A(nDiv2+1:n, 1: nDiv2);
        A3 = A(1:nDiv2, nDiv2+1:n);
       
        % calculul X1, X2, X3, X4  (multiply face inmultirea cu strassen)
        X1 = pInvert(A1 - multiply(multiply(A3, invA4), A2));
        X2 = multiply(multiply(-invA4, A2), X1);
        X4 = pInvert(A4 - multiply(multiply(A2, invA1), A3));        
        X3 = multiply(multiply(-invA1, A3), X4);
    
        invA = [ X1, X3; X2, X4 ]; 
     

    endif

    
endfunction
