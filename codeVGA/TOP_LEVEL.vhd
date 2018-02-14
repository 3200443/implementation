----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 29.09.2016 10:06:20
-- Design Name: 
-- Module Name: TOP_LEVEL - Behavioral
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
use ieee.numeric_std.all; 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity TOP_LEVEL is
    Port ( clk_inp : in STD_LOGIC;
           reset_inp : in STD_LOGIC;
           VSYNC_outp : out STD_LOGIC;
           HSYNC_outp : out STD_LOGIC;
           data_R_outp : out STD_LOGIC_VECTOR (3 downto 0);
           data_G_outp : out STD_LOGIC_VECTOR (3 downto 0);
           data_B_outp : out STD_LOGIC_VECTOR (3 downto 0));
end TOP_LEVEL;

architecture Behavioral of TOP_LEVEL is

component VGA_timing_controller is
    Port ( 
        clk_100MHz_inp      : in STD_LOGIC;
        rst_inp             : in STD_LOGIC;
        clk_out             : out STD_LOGIC; 
        hsync_outp          : out STD_LOGIC;
        vsync_outp          : out STD_LOGIC;
        video_active_outp   : out STD_LOGIC;
        counter_pixel_sigp  : out std_logic_vector(31 downto 0);
        counter_line_sigp    : out std_logic_vector(31 downto 0));
end component VGA_timing_controller;

component testImage is
    Port (
    
        DataOut : out std_logic;
        lig: in std_logic_vector(31 downto 0);
        col: in std_logic_vector(31 downto 0)
     );
end component testImage;


signal clk_VGA      : std_logic;
signal video_active : std_logic;
signal B_value      : std_logic_vector(3 downto 0);
signal R_value      : std_logic_vector(3 downto 0);
signal G_value      : std_logic_vector(3 downto 0);
signal counter_pixel_INT    : INTEGER;
signal counter_line_INT     : INTEGER;
signal counter_pix_STD      : std_logic_vector(31 downto 0);
signal counter_line_STD      : std_logic_vector(31 downto 0);

signal value: std_logic;
constant limitColor  : INTEGER   := 500;  -- Nb clocks back proch

begin

counter_pixel_INT <= to_integer(signed(counter_pix_STD));
counter_line_INT <= to_integer(signed(counter_line_STD));

data_R_outp <= R_value when video_active = '1' else "0000";
data_G_outp <= G_value when video_active = '1' else "0000";
data_B_outp <= B_value when video_active = '1' else "0000";

--count_pixel: process(counter_pixel_INT, counter_line_INT)
--begin
--    if(counter_line_INT > limitColor ) then
 --       R_value <= "1111";
  --      G_value <= "0000";
    --    B_value <= "0000";
  --   else
   --     R_value <= "0000";
     --   G_value <= "1111";
    --    B_value <= "0000";
    -- end if;
--end process;

count_pixel: process(counter_pixel_INT, counter_line_INT)
begin
    if(value = '1') then
          R_value <= "1111";
          G_value <= "1111";
          B_value <= "1111";
    else
         R_value <= "0000";
         G_value <= "0000";
         B_value <= "0000";
    end if;
end process;
 

VGA_timing_controller_0 : VGA_timing_controller
port map (
    clk_100MHz_inp      => clk_inp,
    rst_inp             => reset_inp,
    clk_out             => clk_VGA, 
    hsync_outp          => HSYNC_outp,
    vsync_outp          => VSYNC_outp,
    video_active_outp   => video_active,
    counter_pixel_sigp  => counter_pix_STD,
    counter_line_sigp    => counter_line_STD
);

testImage0: testImage
port map(
    DataOut => value,
    lig => counter_line_STD,
    col => counter_pix_STD
);

end Behavioral;
