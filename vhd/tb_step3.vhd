Library ieee;

	use ieee.numeric_std.all; 
	use ieee.std_logic_1164.all;
	
entity tb_step3 is
end tb_step3;

architecture archi_tbStep3 of tb_step3 is
	
	signal clk_tb : std_logic:= '0';
	signal Vtm1_tb, Ot_tb : std_logic_vector(7 downto 0);
	signal Vt_tb,mtout,mt : std_logic_vector(7 downto 0);

	signal intVtm1 : integer := 0;
	signal intOt : integer := 0;

	

	component step3 is 
	port(
		Vtm1, Ot,mt: in std_logic_vector(7 downto 0);
		Vt,mtout: out std_logic_vector(7 downto 0);
		clk: in std_logic
	);
	end component step3;

	begin
	tb: step3
	port map(Vtm1 => Vtm1_tb, Ot => Ot_tb, Vt => Vt_tb, clk => clk_tb, mtout => mtout, mt => mt);
	clk_tb <= not clk_tb after 5 ns;
	Vtm1_tb <= std_logic_vector(to_unsigned(intVtm1,8));
	Ot_tb <= std_logic_vector(to_unsigned(intOt,8));

	STIMULUS: process	
		begin
		
			intVtm1 <= 50;
			intOt <= 25;
		
			wait for 45 ns;
			intVtm1 <= 50;
			intOt <= 10;

			wait for 45 ns;
			intVtm1 <= 254;
			intOt <= 200;

			wait for 45 ns;
			intVtm1 <= 255;
			intOt <= 200;

			wait for 45 ns;
			intVtm1 <= 127;
			intOt <= 64;

			wait for 45 ns;
			intVtm1 <= 1;
			intOt <= 50;

			
					



		wait;
	
		end process STIMULUS;


end archi_tbStep3;