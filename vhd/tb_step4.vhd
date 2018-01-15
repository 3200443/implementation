Library ieee;

	use ieee.numeric_std.all; 
	use ieee.std_logic_1164.all;
	
entity tb_Step4 is
end tb_Step4;

architecture archi_tbStep4 of tb_Step4 is
	
	signal clk_tb : std_logic:= '0';
	signal Vt_tb, Ot_tb : std_logic_vector(7 downto 0);
	signal Et_tb : std_logic := '0';

	signal intVt : integer := 0;
	signal intOt : integer := 0;

	

	component Step4 is 
	port(
		Ot, Vt: in std_logic_vector(7 downto 0);
		Et: out std_logic;
		clk: in std_logic
	);
	end component Step4;

	begin
	tb: Step4
	port map(Ot => Ot_tb, Vt => Vt_tb, Et => Et_tb, clk => clk_tb);
	clk_tb <= not clk_tb after 10 ns;
	Vt_tb <= std_logic_vector(to_unsigned(intVt,8));
	Ot_tb <= std_logic_vector(to_unsigned(intOt,8));

	STIMULUS: process	
		begin
		
			
			intOt <= 127;
			intVt <= 128;

			wait for 40 ns;
			intOt <= 128;
			intVt <= 127;

			wait for 40 ns;
			intOt <= 255;
			intVt <= 255;

			wait for 40 ns;
			intOt <= 0;
			intVt <= 0;


			wait for 40 ns;
			intOt <= 25;
			intVt <= 50;
			
				
		wait;
	
		end process STIMULUS;


end archi_tbStep4;