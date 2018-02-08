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
use IEEE.math_real.all;

entity tb_DataMem is
end tb_DataMem;

architecture archi_tbDataMem of tb_DataMem is
	constant Ntb: integer := 32;
	constant Ptb: integer := 4;
	constant Coltb: integer:=60;
	constant Ligtb: integer:=80;
	signal DataIn_tb : bus_array(Ptb - 1 downto 0)((Ntb*8-1) downto 0);
	signal WrEn_tb, CLK_tb, rst : std_logic := '0';
	signal Addr_tb : std_logic_vector(integer(ceil(LOG2(real(COLtb*LIGtb/Ntb)))) downto 0);
	signal DataOut_tb: bus_array(Ptb - 1 downto 0)((Ntb*8-1) downto 0);
	
	
	component VT is
	generic(P:integer := 4;
			N: integer := 32;
			Col:integer := 60;
			Lig: integer := 80
	);
	port(
	  reset, clk, We	: in std_logic;
	  addr				: in std_logic_vector(integer(ceil(LOG2(real(COL*LIG/N)))) downto 0);
	  inpout			: in bus_array(P - 1 downto 0)((N*8-1) downto 0);
	  outpout			: out bus_array(P - 1 downto 0)((N*8-1) downto 0)
	);

	end component ;

	
	begin
	
	tb: Vt
	generic map(P=>Ptb, N=> Ntb, Col => Coltb, Lig => Ligtb)
	port map(reset => rst, CLK => CLK_tb, We => WrEn_tb, Addr => Addr_tb, inpout => DataIn_tb, outpout => DataOut_tb);
	
	CLK_tb <= not CLK_tb after 5 ns;
	STIMULUS: process
	begin

		
		
			DataIn_tb(0) <= (Ntb*8-1 downto 7 => '0') & "1011100";
			DataIn_tb(1) <= (Ntb*8-1 downto 7 => '0') & "1101101";
			DataIn_tb(2) <= (Ntb*8-1 downto 7 => '0') & "1001111";
			DataIn_tb(3) <= (Ntb*8-1 downto 7 => '0') & "1111111";
			Addr_tb <= (integer(ceil(LOG2(real(COLtb*LIGtb/Ntb)))) downto 4 => '0') & "1111";
			wait for 100 ns;
			
			Addr_tb <= (integer(ceil(LOG2(real(COLtb*LIGtb/Ntb)))) downto 4 => '0') & "0101";
			WrEn_tb <= '1';
			
			wait for 100 ns;
			WrEn_tb <= '0';
			DataIn_tb(3) <= (Ntb*8-1 downto 7 => '0') & "0000000";

			wait for 100 ns;
			Addr_tb <= (integer(ceil(LOG2(real(COLtb*LIGtb/Ntb)))) downto 4 => '0') & "0000";

			wait for 100 ns;
		
		wait;
	end process STIMULUS;
	
	
	
end architecture;
	
	
	
	
	
	
	
	

