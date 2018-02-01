Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sd_pixel_tb is
end entity ;

Architecture test of sd_pixel_tb is

	signal itin,mtmoinsun	: std_logic_vector(7 downto 0);
  	signal CLK				: std_logic;
  	signal vtmoinsun		: std_logic_vector(7 downto 0);
  	signal mtout, vtout		: std_logic_vector(7 downto 0);
  	signal et				: std_logic;

begin
	I0: entity work.sd_pixel(behav) port map(itin => itin, mtmoinsun => mtmoinsun, CLK => CLK, vtmoinsun => vtmoinsun, et => et, mtout => mtout, vt => vtout);
	process
	begin
		CLK <= '1';
		wait for 10 ns;
		CLK <= '0';
		wait for 10 ns;
	end process;

	process
	begin
		--------
		itin 		<=	X"7C"; --124
		mtmoinsun	<= 	X"94"; --148
		vtmoinsun	<= 	X"17"; --23
		wait for 20 ns;
		--------
		--itin 		<=	X"7C"; --124
		--mtmoinsun	<= 	X"94"; --148
		--vtmoinsun	<= 	X"17"; --23
		--wait for 20 ns;
		--------
      	itin 		<=	X"FF";
		mtmoinsun	<= 	X"FF";
		vtmoinsun	<= 	X"FF";
		wait for 20 ns;
		--------
		itin 		<=	X"7C"; --124
		mtmoinsun	<= 	X"94"; --148
		vtmoinsun	<= 	X"18"; --24	
		wait for 20 ns;
		--------
		itin 		<=	X"7C";
		mtmoinsun	<= 	X"95";
		vtmoinsun	<= 	X"14";
		wait for 20 ns;
		--------
		itin 		<=	X"7C";
		mtmoinsun	<= 	X"95";
		vtmoinsun	<= 	X"12";
		wait for 20 ns;
		--------
		itin 		<=	X"00";
		mtmoinsun	<= 	X"00";
		vtmoinsun	<= 	X"00";
		wait for 20 ns;
		--------
		itin 		<=	X"80";
		mtmoinsun	<= 	X"7F";
		vtmoinsun	<= 	X"80";
		wait for 20 ns;
		--------
	end process;
end test;