% aceeasi functie si pt F3 si pt FB
% diferenta e data de 'type' ('n' => naturale | 't' => tensionate)

% returneaza un vector cu coeficientii 
% sistemului (a0, b0, c0, d0, a1, b1...)'

function coef = cubespline(x, y, type)
    n = length(x);
    
    % behold...

    % conditii de interpolare (aceleasi si pt naturale si pt tensionate)
    for k = 1:n-1
            
        % splineul k pt valoarea x(k); dar x(k) - x(k) = 0
        A(2*k-1, k*4-3) = 0;
        A(2*k-1, k*4-2) = 0;
        A(2*k-1, k*4-1) = 0;
        A(2*k-1, k*4) = 1;

        rez(2*k-1) = y(k); % rezultatul dorit pt spline este y(k)


        % splineul de mai sus pt valoarea din x(k+1)
        A(2*k, k*4-3) = (x(k+1) - x(k))^3;
        A(2*k, k*4-2) = (x(k+1) - x(k))^2;
        A(2*k, k*4-1) = x(k+1) - x(k);
        A(2*k, k*4) = 1;
        
        rez(2*k) = y(k+1); % rezultatul: y(k+1)
        
        % matricea A se inmulteste cu un vector de forma
        % (a0, b0, c0, d0, a1, b1, c1, d1 ...)' 
        % deci va fi necesar un 'padding' pe coloane

    endfor


    % conditii de racord
    for k = 1:n-2
        % sare primele 2*(n-1) linii care contin cond de interpolare
    
        % conditiile ce tin de derivate sunt trecute 
        % sub forma unei ecuatii egale cu 0
        % ex: pt s'[0](1) = s'[1](1) => s'[0](1) - s'[1](1) = 0

        % derivata de ordin 1
        A(2*(n-1+k)-1, 4*(k+1)-7) = 3*(x(k+1)-x(k))^2;
        A(2*(n-1+k)-1, 4*(k+1)-6) = 2*(x(k+1)-x(k));
        A(2*(n-1+k)-1, 4*(k+1)-5) = 1;
        A(2*(n-1+k)-1, 4*(k+1)-4) = 0; 

        A(2*(n-1+k)-1, 4*(k+1)-3) = 0;
        A(2*(n-1+k)-1, 4*(k+1)-2) = 0;
        A(2*(n-1+k)-1, 4*(k+1)-1) = -1;
        A(2*(n-1+k)-1, 4*(k+1)) = 0;
        
        rez(2*(n-1+k)-1) = 0;


        % la fel si la derivata de ordin 2
        A(2*(n-1+k), 4*(k+1)-7) = 6*(x(k+1)-x(k));
        A(2*(n-1+k), 4*(k+1)-6) = 2;
        A(2*(n-1+k), 4*(k+1)-5) = 0;
        A(2*(n-1+k), 4*(k+1)-4) = 0;

        A(2*(n-1+k), 4*(k+1)-3) = 0;
        A(2*(n-1+k), 4*(k+1)-2) = -2;
        A(2*(n-1+k), 4*(k+1)-1) = 0;
        A(2*(n-1+k), 4*(k+1)) = 0;

        rez(2*(n-1+k)) = 0;
    endfor

    % diferenta dintre problema 3 si bonus
    % (se schimba ultimele 2 ecuatii)

    if (type == 'n')

        % conditii pt spline-uri naturale
        A(4*n-5, 1) = 0;
        A(4*n-5, 2) = 2;
        A(4*n-5, 3) = 0;
        A(4*n-5, 4) = 0;
     
        rez(4*n-5) = 0;


        A(4*n-4, 4*n-7) = 6 * (x(n) - x(n-1));
        A(4*n-4, 4*n-6) = 2;
        A(4*n-4, 4*n-5) = 0;
        A(4*n-4, 4*n-4) = 0;

        rez(4*n-4) = 0;
    endif

    if (type == 't')
        
        % conditii pt spline-uri tensionate
        A(4*n-5, 1) = 0;
        A(4*n-5, 2) = 0;
        A(4*n-5, 3) = 1;
        A(4*n-5, 4) = 0;

        % rezultatul ecuatiei este derivata din capatul stang
        rez(4*n-5) = (y(2)-y(1))/(x(2)-x(1));


        A(4*n-4, 4*n-7) = 3 * (x(n) - x(n-1))^2;
        A(4*n-4, 4*n-6) = 2 * (x(n) - x(n-1));
        A(4*n-4, 4*n-5) = 1;
        A(4*n-4, 4*n-4) = 0;

        % derivata in capatul drept
        rez(4*n-4) = (y(n)-y(n-1))/(x(n)-x(n-1));
    endif


    % 'rezolva' sistemul si determina coeficientii
    coef = A\rez'
endfunction
