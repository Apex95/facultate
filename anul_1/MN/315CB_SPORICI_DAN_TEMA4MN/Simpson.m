function I = Simpson(x, y)
    warning('off', 'all');
    n = length(x);
    
    I = 0;

    % integrala pe un punct
    if (n == 1)
        return;
    endif

    % integrala pe 2 puncte
    if (n == 2)
        m = (y(2)-y(1))/(x(2)-x(1));
        I = m/2 * (x(2)^2 - x(1)^2) - y(1)*(x(2)-x(1)) - m*x(1)*(x(2)-x(1));
        if I < 0
            I *= -1;
        endif;
        return; 
    endif
    
    % integrala pe 3 puncte (determinarea unei curbe ce trece prin cele 3 pct)
    i = 1;
    while i <= n-2
        % A- matricea sistemului pt ecuatia parabolei
        A(1, 1) = x(i)^2;
        A(1, 2) = x(i);
        A(1, 3) = 1;

        A(2, 1) = x(i+1)^2;
        A(2, 2) = x(i+1);
        A(2, 3) = 1;

        A(3, 1) = x(i+2)^2;
        A(3, 2) = x(i+2);
        A(3, 3) = 1;

        % in rez sunt tinuti coeficientii 
        rez = [y(i) y(i+1) y(i+2)]';

        coef = A\rez;

        % se integreaza parabola respectiva, pt x(i) si x(i+1)
        I += coef(1)/3 * (x(i+1)^3 - x(i)^3) + coef(2)/2 * (x(i+1)^2 - x(i)^2) + coef(3) * (x(i+1)-x(i));

    i += 1;
    endwhile;
        
    I += coef(1)/3 * (x(i+1)^3 - x(i)^3) + coef(2)/2 * (x(i+1)^2 - x(i)^2) + coef(3) * (x(i+1)-x(i));

    if I < 0
        I *= (-1);
    endif 

endfunction
