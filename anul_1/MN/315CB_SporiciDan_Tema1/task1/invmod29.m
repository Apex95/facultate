function xinv = invmod29(x)
    [d a b] = gcd(x, 29);
    xinv = mod(a, 29);
endfunction
