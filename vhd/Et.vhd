

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.bus_multiplexer_pkg.all;
use IEEE.math_real.all;


entity Et is
generic(P:integer := 4;
		N: integer := 32;
		Col:integer := 60;
		Lig: integer := 80
);
port(
  reset, clk, We	: in std_logic;
  addr				: in std_logic_vector(integer(ceil(LOG2(real(COL*LIG/N)))) downto 0);
  inpout			: in bus_array(P - 1 downto 0)((N-1) downto 0);
  outpout			: out std_logic_vector((N-1) downto 0)

);

end entity ;

Architecture behav of Et is

	-- Declaration Type Tableau Memoire
	type table is array((Col*Lig)/N downto 0) of std_logic_vector((N-1) downto 0);
	-- Fonction d'Initialisation du bancEt de Registres
	function init_bancEt return table is
	variable result : table;
	begin
	for i in (Col*Lig)/N downto 0 loop
	result(i) :=(others=>'0'); --A l'initialisation on met la valeur a 2
	end loop;
	return result;
	end init_bancEt;
	-- Declaaddrtion et Initialisation du bancEt de Registres 16x32 bits
	signal bancEt: table:=init_bancEt;


 begin
    outpout <= bancEt(to_integer(unsigned(addr)));
	


    process(reset, clk)
	begin
		if reset = '1' then
			bancEt <= init_bancEt;
		else
			if(We = '1' and rising_edge(clk)) then
				WriteEt:for I in 0 to P-1 loop
					bancEt(to_integer(unsigned(addr))+I) <= inpout(I);
				end loop WriteEt;
			end if;
		end if ;
		

	end process;

end behav;
