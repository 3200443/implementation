Library ieee;

-- Commonly imported packages:

	-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
use ieee.numeric_std.all; 
use ieee.std_logic_1164.all;

	
entity step3bis is port(
	Vtin,mt: in std_logic_vector(7 downto 0);
	Vtout,mtout: out std_logic_vector(7 downto 0);
	clk: in std_logic
	);
end entity step3bis;

architecture archi_step3bis of step3bis is
	signal temp1 : std_logic_vector(7 downto 0);
	signal res : unsigned(7 downto 0) := to_unsigned(0,8);


	begin
	

	Vtout <= std_logic_vector(res);
	mtout <= temp1;
  
	calcul: process(clk)
		variable intVtout : unsigned(7 downto 0) := to_unsigned(0,8);
		variable intVtin : unsigned(7 downto 0) := to_unsigned(0,8);
		variable Vmax : unsigned(7 downto 0) := to_unsigned(255,8);
		variable Vmin : unsigned(7 downto 0) := to_unsigned(2,8);
		variable mini : unsigned(7 downto 0) := to_unsigned(0,8);
		--variable thomas : unsigned(7 downto 0) := to_unsigned(0,8);
		--variable thomas2 : unsigned(7 downto 0) := to_unsigned(0,8);
	begin
		if(rising_edge(clk)) then
			temp1 <= mt;
			
			--intVt <= max(min(intVt,Vmax),Vmin);
			if(intVtin<Vmax) then --min(intVt,Vmax)
				mini := intVtin;
			else
				mini := Vmax;
			end if;

			if(mini>Vmin) then --max(mini, Vmin)
				intVtout := mini;
			else
				intVtout :=  Vmin;
			end if;

			
			res <= intVtout;
		end if;
	end process calcul;


end architecture archi_step3bis;
