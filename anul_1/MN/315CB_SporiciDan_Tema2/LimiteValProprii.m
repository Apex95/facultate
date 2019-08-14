function [limita_inf, limita_sup] = LimiteValProprii(d, s)
    n = length(d);

    % suma dupa care se calculeaza valorile vectorului 't'
    % in cazul in care matricea e tridiagonala simetrica
    t(1) = abs(s(1));
    t(2:n-1) = abs(s(1:n-2)) + abs(s(2:n-1));
    t(n) = abs(s(n-1));

    % teorema lui Gershgorin
    limita_inf = min(d(:) - t(:));
    limita_sup = max(d(:) + t(:));
endfunction
