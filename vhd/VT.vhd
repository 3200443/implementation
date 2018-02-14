
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.bus_multiplexer_pkg.all;
use IEEE.math_real.all;


entity VT is
generic(P:integer := 4;
		N: integer := 32;
		Col:integer := 60;
		Lig: integer := 80
);
port(
  reset, clk, We	: in std_logic;
  addrIn			: in std_logic_vector(integer(ceil(LOG2(real(COL*LIG*8/N)))) downto 0);
  addrOut			: in std_logic_vector(integer(ceil(LOG2(real(COL*LIG*8/N)))) downto 0);
  inpout			: in bus_array(P - 1 downto 0)((N*8-1) downto 0);
  outpout			: out bus_array(P - 1 downto 0)((N*8-1) downto 0)

);

end entity ;

Architecture behav of VT is

	-- Declaration Type Tableau Memoire
	type table is array((Col*Lig*8)/N downto 0) of std_logic_vector((N*8-1) downto 0);
	-- Fonction d'Initialisation du bancVt de Registres
	function init_bancVt return table is
	variable result : table;
	begin
	for i in (Col*Lig*8)/N downto 0 loop
	result(i) :=(others=>'0'); --A l'initialisation on met la valeur a 2
	end loop;
	return result;
	end init_bancVt;
	-- Declaaddrtion et Initialisation du bancVt de Registres 16x32 bits
	signal bancVt: table:=init_bancVt;


 begin
  	WriteVtOut:for I in 0 to P-1 generate
    	outpout(I) <= bancVt(to_integer(unsigned(addrOut))+I);
	end generate WriteVtOut;


    process(reset, clk)
	begin
		if reset = '1' then
			bancVt <= init_bancVt;
		else
			if(We = '1' and rising_edge(clk)) then
				WriteVt:for I in 0 to P-1 loop
					bancVt(to_integer(unsigned(addrIn))+I) <= inpout(I);
				end loop WriteVt;
			end if;
		end if ;
		

	end process;

end behav;
