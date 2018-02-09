Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;



package bus_multiplexer_pkg is
        type bus_array is array(natural range <>) of std_logic_vector;
end package;



library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.bus_multiplexer_pkg.all;

entity sd_3pixel is
generic(P:integer := 4;
		N:integer := 16
);
port(
  itin,mtmoinsun	: in bus_array(P - 1 downto 0)((N*8-1) downto 0);
  CLK				: in std_logic;
  vtmoinsun			: in bus_array(P - 1 downto 0)((N*8-1) downto 0);
  mtout,vt 			: out bus_array(P - 1 downto 0)((N*8-1) downto 0);
  et				: out bus_array(P - 1 downto 0)((N-1) downto 0)
);

end entity ;


Architecture behav of sd_3pixel is
component sd_2pixel is
generic(N:integer :=N);
port(
  itinN,mtmoinsunN	: in std_logic_vector (N*8-1 downto 0);
  CLKN				: in std_logic;
  vtmoinsunN		: in std_logic_vector (N*8-1 downto 0);
  mtoutN,vtN		: out std_logic_vector(N*8-1 downto 0);
  etN				: out std_logic_vector((N-1) downto 0)
);
end component;
begin
	GEN_REG:for I in 0 to P-1 generate
		pixel : sd_2pixel
		generic map (N=>N)
		port map
	    (
	    	itinN 		=> itin(I),
	    	mtmoinsunN 	=> mtmoinsun(I),
	    	CLKN		=> CLK,
	    	vtmoinsunN	=> vtmoinsun(I),
	    	mtoutN		=> mtout(I),
	    	vtN			=> vt(I),
	    	etN			=> et(I)
	    );
	end generate GEN_REG;
end behav;