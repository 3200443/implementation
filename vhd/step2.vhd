Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity step2 is
port(
  mt,it: in std_logic_vector(7 downto 0);
  CLK			: in std_logic;
  vtmoinsun 		: in std_logic_vector(7 downto 0);
  vtmoinsunout 		: out std_logic_vector(7 downto 0);
  ot,mtout		: out std_logic_vector(7 downto 0)
);
end entity ;

Architecture behav of step2 is

	signal res: std_logic_vector(7 downto 0);
	signal temp1, temp2 : std_logic_vector(7 downto 0);

	begin
	process (CLK)
		  variable iti:unsigned(7 downto 0)	:= to_unsigned(0, 8);
		  variable mti:unsigned(7 downto 0)	:= to_unsigned(0, 8);
		  variable oti:unsigned(7 downto 0)	:= to_unsigned(0, 8);
	begin
	if(rising_edge(CLK)) then
		iti 	:= (unsigned(it));
		mti    	:= (unsigned(mt));
		res <= std_logic_vector(to_unsigned(abs(to_integer(mti)-to_integer(iti)),8));
		temp1 <= vtmoinsun;
		temp2 <= mt;
	end if;
	end process;
	ot <= res;
	vtmoinsunout <= temp1;
	mtout <= temp2;
end behav;