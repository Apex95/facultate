library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity sumator4 is
	port(	
		input1: in std_logic_vector(3 DOWNTO 0);
		input2: in std_logic_vector(3 DOWNTO 0);
		cin: in std_logic;
		output: out std_logic_vector(3 DOWNTO 0);
		cout:	out std_logic
	);
end sumator4;

architecture Behavioral of sumator4 is
  SIGNAL caux : std_logic_vector(2 DOWNTO 0);  
begin

	sumator1: entity work.sumator port map (input1(0), input2(0), cin, output(0), caux(0));
	sumator2: entity work.sumator port map (input1(1), input2(1), caux(0), output(1), caux(1));
	sumator3: entity work.sumator port map (input1(2), input2(2), caux(1), output(2), caux(2));
	sumator4: entity work.sumator port map (input1(3), input2(3), caux(2), output(3), cout);

end Behavioral;