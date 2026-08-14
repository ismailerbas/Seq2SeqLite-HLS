-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_r_7_1ceu is 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_r_7_1ceu is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "110101", 1 => "001100", 2 => "001001", 3 => "000001", 
    4 => "001101", 5 => "001111", 6 => "010100", 7 => "100000", 
    8 => "011000", 9 => "111100", 10 => "001111", 11 => "110001", 
    12 => "111010", 13 => "101101", 14 => "111000", 15 => "111001", 
    16 => "111100", 17 => "110000", 18 => "110011", 19 => "111011", 
    20 => "001010", 21 => "111011", 22 => "001000", 23 => "110010", 
    24 => "101110", 25 => "000000", 26 => "001111", 27 => "011111", 
    28 => "011110", 29 => "001010", 30 => "110111", 31 => "100110");



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

