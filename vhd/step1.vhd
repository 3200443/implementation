Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity step1 is
port(
  itin,mtmoinsun	: in std_logic_vector(7 downto 0);
  vtmoinsun 		: in std_logic_vector(7 downto 0);
  vtmoinsunout 		: out std_logic_vector(7 downto 0);
  CLK				: in std_logic;
  mt,itout			: out std_logic_vector(7 downto 0)
);

end entity ;

Architecture behav of step1 is

	signal res: std_logic_vector(7 downto 0);
	signal temp1, temp2 : std_logic_vector( 7 downto 0);
	
	begin
	process (CLK)
		variable iti:unsigned(7 downto 0)			:= to_unsigned(0, 8);
		variable mti:unsigned(7 downto 0)			:= to_unsigned(0, 8);
		variable mtmoinsuni:unsigned(7 downto 0)	:= to_unsigned(0, 8);
	begin
	if(rising_edge(CLK)) then
		iti 			:= (unsigned(itin));
		mtmoinsuni    	:= (unsigned(mtmoinsun));

		if mtmoinsuni < iti then mti:= mtmoinsuni+1;
		elsif mtmoinsuni > iti then mti:= mtmoinsuni-1;
		else mti := mtmoinsuni;
		End if;

		res <= (std_logic_vector(mti));
		temp1 <= itin;
		temp2 <= vtmoinsun;
	end if;
	end process;
	mt <= res;
	itout <= temp1;
	vtmoinsunout <= temp2;
end behav;
