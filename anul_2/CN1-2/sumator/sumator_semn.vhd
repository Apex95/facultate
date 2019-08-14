library IEEE;
use ieee.numeric_std.ALL;
use IEEE.Std_logic_unsigned.all;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.std_logic_arith.all;

entity sumator_semn is
port(
		input1, input2: in std_logic_vector(3 downto 0);
		final_output: out std_logic_vector(3 downto 0);
		cin: in std_logic;
		op: in std_logic;
		semn: out std_logic;
		cout: out std_logic
);
end sumator_semn;

architecture Behavioral of sumator_semn is
	signal cond, mux_select: std_logic;
	signal aux_carry, aux_cout, aux_cout1, aux_cout2, aux_semn: std_logic;
	signal output1, output2, output3, output4, output5: std_logic_vector(3 downto 0);
	signal aux_input: std_logic_vector(3 downto 0);
	signal valoare_9: std_logic_vector(3 downto 0);
	signal not_input2, not_output3: std_logic_vector(3 downto 0);

begin

	cond <= '1' when (input1 < input2) else '0';
	not_input2 <= (not input2);
	valoare_9 <= "1001";
	sumator1: entity work.sumator4 port map (valoare_9, not_input2, '1',output2, aux_cout);
	aux_input <= input2 when (op = '0') else output2; 
	zcb: entity sumator_zcb port map(input1, aux_input, cin, output3, aux_carry);
	sumator2: entity sumator4 port map ("0000", output3, aux_carry, output4, aux_cout1);
	cout <= aux_carry and (not op);
	not_output3 <= (not output3);
	sumator3: entity sumator4 port map (valoare_9, not_output3, '1', output1, aux_cout2);
	mux_select <= not(op and (not cond));
	output5 <= output4 when (mux_select = '0') else output3;
	semn <= (op and cond);
	aux_semn <= (op and cond);
	final_output <= output5 when (aux_semn = '0') else output1;
	
end Behavioral;

