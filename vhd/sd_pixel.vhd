Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sd_pixel is
port(
  itin,mtmoinsun	: in std_logic_vector(7 downto 0);
  CLK				: in std_logic;
  vtmoinsun			: in std_logic_vector(7 downto 0);
  mtout,vt :out std_logic_vector(7 downto 0);
  et			: out std_logic
);

end entity ;


Architecture behav of sd_pixel is
	signal mt,itout			: std_logic_vector(7 downto 0);
	signal ot				: std_logic_vector(7 downto 0);
	signal Vt_tb 			: std_logic_vector(7 downto 0);
	signal Vttempa,Vttempb 	: std_logic_vector(7 downto 0);
	signal mtout2,mtout3	: std_logic_vector(7 downto 0);

begin
	I0: entity work.step1(behav) port map(itin => itin, mtmoinsun => mtmoinsun, CLK => CLK, mt => mt, itout => itout, vtmoinsun => vtmoinsun, 
		vtmoinsunout => Vttempa);
	I1: entity work.step2(behav) port map(it => itout, mt => mt, CLK => CLK, ot => ot, vtmoinsun => Vttempa, vtmoinsunout => Vttempb, mtout => mtout2);
	I3: entity work.step3c(behav) port map(Vtm1 => Vttempb, Ot => ot, Vt => Vt_tb, clk => CLK, mt =>mtout2, mtout=>mtout3);
	I4: entity work.step4(archi_step4)port map(Ot => ot, Vt => Vt_tb, Et => et, clk => CLK, mt => mtout3,mtout => mtout, vtout =>vt);


end behav;