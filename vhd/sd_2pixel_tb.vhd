Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sd_2pixel_tb is
end entity ;

Architecture test of sd_2pixel_tb is

	signal itinT,mtmoinsunT	: std_logic_vector(15 downto 0);
  	signal CLKT				: std_logic;
  	signal vtmoinsunT		: std_logic_vector(15 downto 0);
  	signal mtoutT, vtoutT		: std_logic_vector(15 downto 0);
  	signal etT				: std_logic_vector(1 downto 0);

begin
	I0: entity work.sd_2pixel(behav) 
		generic map ( N => 2)
		port map(itinN => itinT, mtmoinsunN => mtmoinsunT, CLKN => CLKT, vtmoinsunN => vtmoinsunT, etN => etT, mtoutN => mtoutT, vtN => vtoutT);
	process
	begin
		CLKT <= '1';
		wait for 10 ns;
		CLKT <= '0';
		wait for 10 ns;
	end process;

	process
	begin
		--------
		itinT 		<=	X"7C7C"; --124
		mtmoinsunT	<= 	X"9495"; --148
		vtmoinsunT	<= 	X"1714"; --23
		wait for 20 ns;
		--------
		--itin 		<=	X"7C"; --124
		--mtmoinsun	<= 	X"94"; --148
		--vtmoinsun	<= 	X"17"; --23
		--wait for 20 ns;
		--------
  --    	itinT 		<=	X"FF";
		--mtmoinsunT	<= 	X"FF";
		--vtmoinsunT	<= 	X"FF";
		--wait for 20 ns;
		----------
		--itinT 		<=	X"7C"; --124
		--mtmoinsunT	<= 	X"94"; --148
		--vtmoinsunT	<= 	X"18"; --24	
		--wait for 20 ns;
		----------
		--itinT 		<=	X"7C";
		--mtmoinsunT	<= 	X"95";
		--vtmoinsunT	<= 	X"14"; --MARCHE
		--wait for 20 ns;
		----------
		--itinT 		<=	X"7C";
		--mtmoinsunT	<= 	X"95";
		--vtmoinsunT	<= 	X"12";
		--wait for 20 ns;
		----------
		--itinT 		<=	X"00";
		--mtmoinsunT	<= 	X"00";
		--vtmoinsunT	<= 	X"00";
		--wait for 20 ns;
		----------
		--itinT 		<=	X"80";
		--mtmoinsunT	<= 	X"7F";
		--vtmoinsunT	<= 	X"80";
		--wait for 20 ns;
		----------
	end process;
end test;