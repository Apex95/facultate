LIBRARY ieee;
USE ieee.numeric_std.ALL;
use IEEE.Std_logic_unsigned.all;
USE ieee.std_logic_1164.ALL;
use IEEE.std_logic_arith.all;
 
ENTITY test_sumator IS
END test_sumator;
 
ARCHITECTURE behavior OF test_sumator IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT sumator_semn
    PORT(
			cin : IN  std_logic;
			input1 : IN  std_logic_vector(3 downto 0);
			input2 : IN  std_logic_vector(3 downto 0);
			op : IN  std_logic;
         final_output : OUT  std_logic_vector(3 downto 0);
         semn : OUT  std_logic;
         cout : OUT  std_logic
       );
    END COMPONENT;
    

   --Inputs
   signal cin : std_logic := '0';
   signal input1 : std_logic_vector(3 downto 0) := (others => '0');
   signal input2 : std_logic_vector(3 downto 0) := (others => '0');
	signal op : std_logic := '0';
	
 	--Outputs
   signal final_output : std_logic_vector(3 downto 0);
   signal semn : std_logic;
   signal cout : std_logic;
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
 
 
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: sumator_semn PORT MAP (
          cin => cin,
          input1 => input1,
          input2 => input2,
			 op => op,
          final_output => final_output,
          semn => semn,
          cout => cout
        );


 

   -- Stimulus process
   stim_proc: process
   begin			
		-- Test suma
		op <= '0';
		input1 <= "0110";
		input2 <= "0111";
		
		wait for 500 ns;	
		-- Test diferenta
		
		op <= '1';
		input1 <= "0010";
		input2 <= "0101";
		
		
		wait;
   end process;

END;
