----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 11.12.2017 15:28:23
-- Design Name: 
-- Module Name: testImage - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity testImage is
    Port (
    
        DataOut : out std_logic;
        lig: in std_logic_vector(31 downto 0);
        col: in std_logic_vector(31 downto 0)
     );
end testImage;

architecture Behavioral of testImage is
-- Declaration Type Tableau Memoire
		type table is array(15 downto 0) of std_logic_vector(31 downto 0);
-- Fonction d'Initialisation du Banc de Registres
		function init_banc return table is
			variable result : table;
			begin
				for i in 11 downto 3 loop
					result(i) := (others=>'0');
				end loop;
				result(15):=X"00040030";
				result(14):=X"07080030";
				result(13):=X"0709A030";
				result(12):=X"07080030";
				result(2):=X"0708F030";
				result(1):=X"4060F001";
				result(0):=X"CF20D051";
			return result;
		end init_banc;
-- Déclaration et Initialisation du Banc de Registres 16x32 bits
		signal DataMemory: table:=init_banc;
		signal intLig : integer:=0;
		signal intCol : integer:=0;
		signal intLig2 : integer:=0;
        signal intCol2 : integer:=0;
		signal ligne: std_logic_vector(31 downto 0);

begin
intLig <= to_integer(signed(lig));
intCol <= to_integer(signed(col));

ligne <= DataMemory(intLig2);
DataOut <= ligne(intCol2);

count_pixel: process(intLig, intCol)
begin
    intLig2 <= intLig mod 15;
    intCol2 <= intCol mod 32;
end process;

end Behavioral;
