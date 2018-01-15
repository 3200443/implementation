Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity step2 is
port(
  mt,it: in std_logic_vector(7 downto 0);
  CLK			: in std_logic;
  ot		: out std_logic_vector(7 downto 0)
);
end entity ;

Architecture behav of step2 is

	signal res: std_logic_vector(7 downto 0);
	begin
	process (CLK)
		  variable iti:unsigned(7 downto 0)	:= to_unsigned(0, 8);
		  variable mti:unsigned(7 downto 0)	:= to_unsigned(0, 8);
		  variable oti:unsigned(7 downto 0)	:= to_unsigned(0, 8);
	begin

		iti 	:= (unsigned(it));
		mti    	:= (unsigned(mt));
		res <= std_logic_vector(to_unsigned(abs(to_integer(mti)-to_integer(iti)),8));
	end process;
	ot <= res;
end behav;