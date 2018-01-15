Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sd_pixel is
port(
  itin,mtmoinsun	: in std_logic_vector(7 downto 0);
  CLK				: in std_logic;
  vtmoinsun			: in std_logic_vector(7 downto 0);
  et				: out std_logic
);

end entity ;


Architecture behav of sd_pixel is
	signal mt,itout		: std_logic_vector(7 downto 0);
	signal ot			: std_logic_vector(7 downto 0);
begin
	I0: entity work.step1(behav) port map(itin => itin, mtmoinsun => mtmoinsun, CLK => CLK, mt => mt, itout => itout);
	I1: entity work.step2(behav) port map(itout => it, mt => mt, CLK => CLK, ot => ot);

end behav;