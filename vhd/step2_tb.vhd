Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity step2_tb is
end entity ;

Architecture test of step2_tb is

	signal mt,it 		:  std_logic_vector(7 downto 0);
  	signal CLK			:  std_logic;
  	signal ot			:  std_logic_vector(7 downto 0);

begin
	I0: entity work.step2(behav) port map(it => it, mt => mt, CLK => CLK, ot => ot);
	process
	begin
		clk <= '0';
		wait for 10 ns;
		clk <= '1';
		wait for 10 ns;
	end process;

	process
	begin
		--------
      	it 	<=	X"FF";
		mt	<= 	X"FF";
		--------
		wait for 20 ns;
		it 	<=	X"FF";
		mt	<= 	X"00";
		--------
		wait for 20 ns;
		it 	<=	X"00";
		mt	<= 	X"00";
		--------
		wait for 20 ns;
		it 	<=	X"00";
		mt	<= 	X"FF";
		--------
		wait for 20 ns;
		it 	<=	X"00";
		mt	<= 	X"FF";
		--------
		wait for 20 ns;
	end process;
end test;