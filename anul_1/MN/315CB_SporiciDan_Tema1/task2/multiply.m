function C = multiply(A, B)
    
    [nA mA] = size(A);
    [nB mB] = size(B);

    % determin cea mai mare linie sau coloana 
    n = max(max(nA, nB), max(mA, mB));
    
    % daca e mai mica decat 32, fac inmultirea direct
    if n <= 32
        C = A * B;
	return;
    else
        % aduc cele 2 matrice la aceeasi dimensiune
		if nA + mA != nB + mB || nA - mA != nB - mB 
                if nA < n || mA < n
                    A(n, n) = 0;
                endif

                if nB < n || mB < n
                    B(n, n) = 0;
                endif
        endif
        
        % iar mai apoi le extind la 2^n * 2^n
        if bitand(n, n-1) != 0   % verif. daca n poate fi scris ca 2^p
        	n = 2^(fix(log2(n))+1);
        	A(n, n) = 0;
        	B(n, n) = 0;
	    endif

        % regulile de inmultire cu strassen
        M1 = multiply(A(1:n/2, 1:n/2) + A(n/2+1:n, n/2+1:n), 
                      B(1:n/2, 1:n/2) + B(n/2+1:n, n/2+1:n));
        
        M2 = multiply(A(n/2+1:n, 1:n/2) + A(n/2+1:n, n/2+1:n),
                      B(1:n/2, 1:n/2));

        M3 = multiply(A(1:n/2, 1:n/2),
                      B(1:n/2, n/2+1:n) - B(n/2+1:n, n/2+1:n));

        M4 = multiply(A(n/2+1:n, n/2+1:n),
                      B(n/2+1:n, 1:n/2) - B(1:n/2, 1:n/2));

        M5 = multiply(A(1:n/2, 1:n/2) + A(1:n/2, n/2+1:n),
                      B(n/2+1:n, n/2+1:n));

        M6 = multiply(A(n/2+1:n, 1:n/2) - A(1:n/2, 1:n/2),
                      B(1:n/2, 1:n/2) + B(1:n/2, n/2+1:n));

        M7 = multiply(A(1:n/2, n/2+1:n) - A(n/2+1:n, n/2+1:n),
                      B(n/2+1:n, 1:n/2) + B(n/2+1:n, n/2+1:n));

        % matricea rezultata
        C = [ M1 + M4 - M5 + M7, M3 + M5; M2 + M4, M1 - M2 + M3 + M6 ];
    endif
   
    % se elimina tot ce s-a adaugat (in plus) pentru inmultire
    C = C(1:nA, 1:mB);
     

endfunction
