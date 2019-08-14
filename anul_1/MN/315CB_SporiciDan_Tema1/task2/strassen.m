function strassen(in, out)
    
    fid = fopen(in, 'r');
    n = str2num(fgets(fid));
    fclose(fid);
    
    % citieste matricea data in fisier
    A = dlmread(in, ' ', 1, 0);
    A= A(1:end, 1:end-1);
    
    % calculeaza inversa cu metoda partitionarii
    invA = pInvert(A);
    Q = invA;


    p = 1;

    % se determina o putere maxima astfel incat 2^maxPw < n
    maxPw = fix(log2(n-1));

    QPowers(:, :, 1) = Q;
 
    % se ridica matricea la acea putere
    while (p * 2 <= maxPw)
        p *= 2;
        Q = multiply(Q, Q);
	
        % putin memoization - ca sa nu fac aceleasi inmultiri de mai multe ori
        % salvez valorile anterioare (A^-1, A^-2, A^-4, A^-8, etc)
	    QPowers(:, :, p) = Q(:, :); 
    endwhile

    
    i = p;
    % plecand de la A^-maxPw (A la puterea maxima, cea mai apropiata de n)
    % mai fac cateva inmultiri, astfel incat sa ajung la A^-n
    while p < n-1  % (greedy)
        if i + p <= n-1
		    p += i;
		    Q = multiply(Q, QPowers(:, :, i)); %inmultesc cu valorile anterioare 
	    else
		    i = i / 2;
	    endif
    endwhile 

    % o ultima multiplicare
    Q = multiply(Q, invA);
    
	 
    [m n] = size(Q);

    % afisez matricea obtinuta
    fid = fopen(out, 'w');
    for i=1:m
        fprintf(fid, '%0.3f ', Q(i,:)); %3 zecimale
        fprintf(fid, '\n');
    endfor    
    fclose(fid);
    
endfunction
