Library ieee;

-- Commonly imported packages:

	-- STD_LOGIC and STD_LOGIC_VECTOR types, and relevant functions
use ieee.numeric_std.all; 
use ieee.std_logic_1164.all;

	
entity step3c is port(
	Vtm1, Ot,mt: in std_logic_vector(7 downto 0);
	Vt,mtout,otout: out std_logic_vector(7 downto 0);
	clk: in std_logic
	);
end entity step3c;

architecture behav of step3c is
	
	signal Vtemp,mtoutb	: std_logic_vector(7 downto 0);
	signal otemp : std_logic_vector(7 downto 0);

	begin
	I0: entity work.step3a(behav) port map(Vtm1 => Vtm1, Ot => Ot, CLK => CLK, mt => mt, Vtemp => Vtemp, mtout => mtoutb,otout => otemp);
	I1: entity work.step3b(behav) port map(Vtemp => Vtemp, CLK => CLK, mt => mtoutb,otin => otemp, Vt => Vt, mtout => mtout, otout => otout);

end architecture behav;