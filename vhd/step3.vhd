Library ieee;

-- Commonly imported packages:

	-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
use ieee.numeric_std.all; 
use ieee.std_logic_1164.all;

	
entity step3 is port(
	Vtm1, Ot: in std_logic_vector(7 downto 0);
	Vt: out std_logic_vector(7 downto 0);
	clk: in std_logic
	);
end entity step3;

architecture archi_step3 of step3 is
	signal intVtm1 : unsigned(7 downto 0) := to_unsigned(0,8);
	signal res : unsigned(7 downto 0) := to_unsigned(0,8);
	signal intOt : unsigned(7 downto 0) := to_unsigned(0,8);
	signal N : unsigned(7 downto 0) := to_unsigned(2,8);
	signal NfoisOt : unsigned(7 downto 0) := to_unsigned(0,8);
	signal Vmax : unsigned(7 downto 0) := to_unsigned(255,8);
	signal Vmin : unsigned(7 downto 0) := to_unsigned(2,8);
	signal mini : unsigned(7 downto 0) := to_unsigned(0,8);
	signal thomas : unsigned(7 downto 0) := to_unsigned(0,8);
	signal thomas2 : unsigned(7 downto 0) := to_unsigned(0,8);


	begin
	intVtm1 <= (unsigned(Vtm1));
	intOt <= (unsigned(Ot));
	Vt <= std_logic_vector(res);

  
	calcul: process(clk)
		variable intVt : unsigned(7 downto 0) := to_unsigned(0,8);

	begin
		--if(rising_edge(clk)) then
			if(Ot>X"7F") then -- Si c'est superieur a 127, on met directement N*Ot au max
				NfoisOt <= X"FF";
			else --Sinon on fait la multiplication
				NfoisOt <= intOt + intOt;
			end if;
				

			if(intVtm1 < NfoisOt) then -- Si Vtm1 < N*Ot : Vt = Vtm1+1
				intVt := intVtm1 + 1;
			elsif(intVtm1 > NfoisOt) then -- Si Vtm1 > N*Ot : Vt = Vtm1+1
				intVt := intVtm1 - 1;
			else 						-- Sinon Vt=Vtm1 		
				intVt := intVtm1;
			end if;

			
			--intVt <= max(min(intVt,Vmax),Vmin);
			if(intVt<Vmax) then --min(intVt,Vmax)
				mini <= intVt;
			else
				mini <= Vmax;
			end if;

			if(mini>Vmin) then --max(mini, Vmin)
				intVt := mini;
			else
				intVt :=  Vmin;
			end if;

			
		--end if;
		res <= intVt;
	end process calcul;


end architecture archi_step3;
