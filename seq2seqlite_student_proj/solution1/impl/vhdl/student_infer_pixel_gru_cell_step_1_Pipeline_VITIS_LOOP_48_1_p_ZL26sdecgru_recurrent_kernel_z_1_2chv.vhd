-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_z_1_2chv is 
    generic(
             DataWidth     : integer := 8; 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_z_1_2chv is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "00011111", 1 => "00010111", 2 => "00001010", 3 => "00000000", 
    4 => "00001000", 5 => "00001101", 6 => "11110001", 7 => "00001110", 
    8 => "00010011", 9 => "10110001", 10 => "11110111", 11 => "00000111", 
    12 => "11011011", 13 => "00000110", 14 => "11111110", 15 => "00000101", 
    16 => "00001001", 17 => "00000000", 18 => "00011010", 19 => "00010100", 
    20 => "00011011", 21 => "00011010", 22 => "11110111", 23 => "00001111", 
    24 => "00000111", 25 => "11101101", 26 => "00000110", 27 => "00010000", 
    28 => "00001011", 29 => "00001001", 30 => "11110111", 31 => "00001011");



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

