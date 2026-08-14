-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_sencgru_bias_h_ROM_AUTO_1R is 
    generic(
             DataWidth     : integer := 6; 
             AddressWidth     : integer := 5; 
             AddressRange    : integer := 32
    ); 
    port (
 
          address0        : in std_logic_vector(AddressWidth-1 downto 0); 
          ce0             : in std_logic; 
          q0              : out std_logic_vector(DataWidth-1 downto 0);

          reset               : in std_logic;
          clk                 : in std_logic
    ); 
end entity; 


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_sencgru_bias_h_ROM_AUTO_1R is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "000101", 1 => "111001", 2 => "111111", 3 => "111100", 
    4 => "111110", 5 => "111000", 6 => "000110", 7 => "000110", 
    8 => "000001", 9 => "111001", 10 => "001100", 11 => "111010", 
    12 => "110101", 13 => "000010", 14 => "101100", 15 => "000011", 
    16 => "111100", 17 => "110010", 18 => "111110", 19 => "111101", 
    20 => "111100", 21 => "001001", 22 => "000110", 23 => "101101", 
    24 => "111100", 25 => "110010", 26 => "111101", 27 => "000111", 
    28 => "000101", 29 => "010001", 30 => "000111", 31 => "111110");



begin 

 
memory_access_guard_0: process (address0) 
begin
      address0_tmp <= address0;
--synthesis translate_off
      if (CONV_INTEGER(address0) > AddressRange-1) then
           address0_tmp <= (others => '0');
      else 
           address0_tmp <= address0;
      end if;
--synthesis translate_on
end process;

p_rom_access: process (clk)  
begin 
    if (clk'event and clk = '1') then
 
        if (ce0 = '1') then  
            q0 <= mem0(CONV_INTEGER(address0_tmp)); 
        end if;

end if;
end process;

end rtl;

