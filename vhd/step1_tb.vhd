Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity step1_tb is
end entity ;

Architecture test of step1_tb is

	signal itin,mtmoinsun,vtin	: std_logic_vector(7 downto 0);
  	signal CLK		: std_logic;
  	signal mt,itout,vtout		: std_logic_vector(7 downto 0);

begin
	I0: entity work.step1(behav) port map(itin => itin, mtmoinsun => mtmoinsun, CLK => CLK, mt => mt, itout => itout, vtmoinsunout => vtout, vtmoinsun =>vtin);
	process
	begin
		CLK <= '0';
		wait for 10 ns;
		CLK <= '1';
		wait for 10 ns;
	end process;

	process
	begin
		--------
      	itin 		<=	X"FF";
		mtmoinsun	<= 	X"00";
		vtin	<= 	X"FF";
		--------
		wait for 20 ns;
		itin 		<=	X"00";
		mtmoinsun	<= 	X"FF";
		vtin	<= 	X"FF";
		--------
		wait for 20 ns;
		itin 		<=	X"FF";
		mtmoinsun	<= 	X"FF";
		vtin	<= 	X"FF";
		--------
		wait for 20 ns;
		itin 		<=	X"7F";
		mtmoinsun	<= 	X"80";
		vtin	<= 	X"FF";
		--------
		wait for 20 ns;
		--------
		wait for 20 ns;
		itin 		<=	X"80";
		mtmoinsun	<= 	X"7F";
		vtin	<= 	X"FF";
		--------
		wait for 20 ns;
	end process;
end test;