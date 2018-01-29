Library ieee;

-- Commonly imported packages:

	-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
use ieee.numeric_std.all; 
use ieee.std_logic_1164.all;

	
entity step4 is port(
	Ot, Vt,mt: in std_logic_vector(7 downto 0);
	Et: out std_logic :='0';
	mtout,vtout : out std_logic_vector(7 downto 0);
	clk: in std_logic
	);
end entity step4;

architecture archi_step4 of step4 is
	signal intVt : unsigned(7 downto 0) := to_unsigned(0,8);
	signal intOt : unsigned(7 downto 0) := to_unsigned(0,8);
	signal procEt: std_logic;
	signal temp1, temp2 : std_logic_vector(7 downto 0);
	begin
	intVt <= (unsigned(Vt));
	intOt <= (unsigned(Ot));
	Et <= ProcEt;
	mtout <= temp1;
	vtout <= temp2;
  
	calcul: process(clk)
	begin
	if(rising_edge(clk)) then
		temp1 <= mt;
		temp2 <= Vt;
		if(Ot<Vt) then -- Si Ot < Vt => Et = 0
			ProcEt <= '0';
		else --Sinon Et = 1
			ProcEt <= '1';
		end if;
	end if;
	end process calcul;


end architecture archi_step4;
