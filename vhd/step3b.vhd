Library ieee;

-- Commonly imported packages:

	-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
use ieee.numeric_std.all; 
use ieee.std_logic_1164.all;

entity step3b is port(
	Vtemp,mt: in std_logic_vector(7 downto 0);
	Vt,mtout: out std_logic_vector(7 downto 0);
	clk: in std_logic
	);
end entity step3b;

architecture behav of step3b is
	signal res 	: std_logic_vector(7 downto 0);
	--signal intOt 	: unsigned(7 downto 0) := to_unsigned(0,8);
	--signal N 		: unsigned(7 downto 0) := to_unsigned(2,8);
	--signal thomas : unsigned(7 downto 0) := to_unsigned(0,8);
	--signal thomas2: unsigned(7 downto 0) := to_unsigned(0,8);
	signal temp1 	: std_logic_vector(7 downto 0);
	

	begin

	Vt <= res;
	mtout <= temp1;

	calcul: process(clk)
		--variable NfoisOt 	: unsigned(7 downto 0) := to_unsigned(0,8);
		--variable intVtm1 	: unsigned(7 downto 0) := to_unsigned(0,8);
		variable Vmax 	: unsigned(7 downto 0) := to_unsigned(255,8);
		variable Vmin 	: unsigned(7 downto 0) := to_unsigned(2,8);
		variable mini 	: unsigned(7 downto 0) := to_unsigned(0,8);
		variable intVt 	: unsigned(7 downto 0) := to_unsigned(0,8);
	begin
		if(rising_edge(clk)) then
			temp1 <= mt;
			intVt := unsigned(Vtemp);
			--intVt <= max(min(intVt,Vmax),Vmin);
			if(intVt<Vmax) then --min(intVt,Vmax)
				mini := intVt;
			else
				mini := Vmax;
			end if;

			if(mini>Vmin) then --max(mini, Vmin)
				intVt := mini;
			else
				intVt :=  Vmin;
			end if;
			res <= std_logic_vector(intVt);
		end if;
	end process calcul;
end architecture behav;
