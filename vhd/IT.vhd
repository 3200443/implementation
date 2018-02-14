
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.bus_multiplexer_pkg.all;
use IEEE.math_real.all;


entity It is
generic(P:integer := 4;
		N: integer := 32;
		Col:integer := 60;
		Lig: integer := 80
);
port(
  reset, clk, We	: in std_logic;
  addr				: in std_logic_vector(integer(ceil(LOG2(real(COL*LIG*8/N)))) downto 0);
  inpout			: in std_logic_vector((N*8-1) downto 0);
  outpout			: out bus_array(P - 1 downto 0)((N*8-1) downto 0)

);

end entity ;

Architecture behav of It is

	-- Declaration Type Tableau Memoire
	type table is array((Col*Lig*8)/N downto 0) of std_logic_vector((N*8-1) downto 0);
	-- Fonction d'Initialisation du bancIt de Registres
	function init_bancIt return table is
	variable result : table;
	begin
	for i in (Col*Lig*8)/N downto 0 loop
	result(i) :=(others=>'0'); --A l'initialisation on met la valeur a 2
	end loop;
	return result;
	end init_bancIt;
	-- Declaaddrtion et Initialisation du bancIt de Registres 16x32 bits
	signal bancIt: table:=init_bancIt;


 begin
  	WriteItOut:for I in 0 to P-1 generate
    	outpout(I) <= bancIt(to_integer(unsigned(addr))+I);
	end generate WriteItOut;


    process(reset, clk)
	begin
		if reset = '1' then
			bancIt <= init_bancIt;
		else
			if(We = '1' and rising_edge(clk)) then
				bancIt(to_integer(unsigned(addr))) <= inpout;
			end if;
		end if ;
		

	end process;

end behav;
