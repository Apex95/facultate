library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity sumator is

port(	
	in1, in2, cin: in std_logic;
	out1, cout: out std_logic
);


end sumator;

architecture Behavioral of sumator is

begin

	out1 <= (in1 xor in2) xor cin;
	cout <= (in1 and in2) or (in1 and cin) or (in2 and cin);

end Behavioral;
