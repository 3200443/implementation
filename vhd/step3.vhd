Library ieee;

-- Commonly imported packages:

	-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
use ieee.numeric_std.all; 
use ieee.std_logic_1164.all;

	
entity step3 is port(
	Vtm1, Ot,mt: in std_logic_vector(7 downto 0);
	Vt,mtout: out std_logic_vector(7 downto 0);
	clk: in std_logic
	);
end entity step3;

architecture archi_step3 of step3 is
	signal temp1 : std_logic_vector(7 downto 0);
	signal res : unsigned(7 downto 0) := to_unsigned(0,8);
	signal link: std_logic_vector(7 downto 0);

	begin
	I0: entity work.step3bis(archi_step3bis)
	port map(Vtin => link,mt => mt,vtout => open, mtout => open, clk => clk); --open pour ne connecter les sorties a rien
	link <= std_logic_vector(res);
	mtout <= temp1;
  	Vt <= link;
	calcul: process(clk)
		variable intVt : unsigned(7 downto 0) := to_unsigned(0,8);
		variable intVtm1 : unsigned(7 downto 0) := to_unsigned(0,8);
		variable intOt : unsigned(7 downto 0) := to_unsigned(0,8);
		variable N : unsigned(7 downto 0) := to_unsigned(2,8);
		variable NfoisOt : unsigned(7 downto 0) := to_unsigned(0,8);
		--variable thomas : unsigned(7 downto 0) := to_unsigned(0,8);
		--variable thomas2 : unsigned(7 downto 0) := to_unsigned(0,8);
	begin
		if(rising_edge(clk)) then
			temp1 <= mt;
			intVtm1 := (unsigned(Vtm1));
			intOt := (unsigned(Ot));
			if(Ot>X"7F") then -- Si c'est superieur a 127, on met directement N*Ot au max
				NfoisOt := X"FF";
			else --Sinon on fait la multiplication
				NfoisOt := intOt + intOt;
			end if;
				

			if(intVtm1 < NfoisOt) then -- Si Vtm1 < N*Ot : Vt = Vtm1+1
				intVt := intVtm1 + 1;
			elsif(intVtm1 > NfoisOt) then -- Si Vtm1 > N*Ot : Vt = Vtm1+1
				intVt := intVtm1 - 1;
			else 						-- Sinon Vt=Vtm1 		
				intVt := intVtm1;
			end if;

			
			res <= intVt;
		end if;
	end process calcul;


end architecture archi_step3;
