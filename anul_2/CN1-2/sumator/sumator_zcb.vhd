library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all; 

entity sumator_zcb is
	port(	
		input1: in std_logic_vector(3 DOWNTO 0);
		input2: in std_logic_vector(3 DOWNTO 0);
		cin: in std_logic;
		output: out std_logic_vector(3 DOWNTO 0);
		cout: out std_logic
	);

end sumator_zcb;

architecture Behavioral of sumator_zcb is
	signal aux_carry, aux_carry2: std_logic;
	signal aux_output: std_logic_vector(3 DOWNTO 0);
	signal bitcat : std_logic_vector(3 downto 0);
	signal aux: std_logic;
begin

	sumator1: entity work.sumator4 port map (input1, input2, cin, aux_output, aux_carry); 
	cout <= aux_carry or (aux_output(1) and aux_output(3)) or (aux_output(2) and aux_output(3));
	aux <= aux_carry or (aux_output(1) and aux_output(3)) or (aux_output(2) and aux_output(3));
	bitcat <= '0' & aux & aux & '0';
	sumator2: entity work.sumator4 port map (bitcat,aux_output,'0',output, aux_carry2); 

end Behavioral;

