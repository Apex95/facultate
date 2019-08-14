function I = Trapez(x, y)
    n = length(x);
    I = 0;

    % metoda trapezelor, doar ca H nu mai este constant ci este calculat
    % pt cele 2 puncte    
    for i=1:n-1
        I += (x(i+1)-x(i))/2 * (y(i) + y(i+1)); 
    endfor

    
    if (I < 0)
        I *= (-1);
    endif


endfunction
