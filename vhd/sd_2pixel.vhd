Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sd_2pixel is
generic(N:integer :=1);
port(
  itinN,mtmoinsunN	: in std_logic_vector((N*8-1) downto 0);
  CLKN				: in std_logic;
  vtmoinsunN			: in std_logic_vector((N*8-1) downto 0);
  mtoutN,vtN 			: out std_logic_vector((N*8-1) downto 0);
  etN				: out std_logic_vector((N-1) downto 0)
);

end entity ;


Architecture behav of sd_2pixel is
component sd_pixel is
port(
  itin,mtmoinsun	: in std_logic_vector(7 downto 0);
  CLK				: in std_logic;
  vtmoinsun			: in std_logic_vector(7 downto 0);
  mtout,vt			: out std_logic_vector(7 downto 0);
  et				: out std_logic
);
end component;
begin
	GEN_REG:for I in 1 to N generate
		pixel : sd_pixel port map
	    (
	    	itin 		=> itinN((I*8-1) downto (I-1)*8),
	    	mtmoinsun 	=> mtmoinsunN((I*8-1) downto (I-1)*8),
	    	CLK		=> CLKN,
	    	vtmoinsun	=> vtmoinsunN((I*8-1) downto (I-1)*8),
	    	mtout		=> mtoutN((I*8-1) downto (I-1)*8),
	    	vt			=> vtN((I*8-1) downto (I-1)*8),
	    	et			=> etN(I-1)
	    );
	end generate GEN_REG;
end behav;