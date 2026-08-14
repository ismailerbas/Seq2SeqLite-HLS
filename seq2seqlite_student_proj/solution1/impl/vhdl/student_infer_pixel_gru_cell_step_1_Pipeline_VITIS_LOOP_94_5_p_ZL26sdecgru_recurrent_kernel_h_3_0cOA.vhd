-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_3_0cOA is 
    generic(
             DataWidth     : integer := 7; 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_3_0cOA is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0001001", 1 => "1110001", 2 => "0100011", 3 => "1110001", 
    4 => "1111110", 5 => "1110101", 6 => "1111011", 7 => "1101011", 
    8 => "1101001", 9 => "0000110", 10 => "0000011", 11 => "0010110", 
    12 => "1111101", 13 => "1110011", 14 => "1110111", 15 => "1100101", 
    16 => "1100001", 17 => "0001000", 18 => "1111100", 19 => "1101000", 
    20 => "0000101", 21 => "0001101", 22 => "1111000", 23 => "1111010", 
    24 => "0001010", 25 => "0001000", 26 => "0011010", 27 => "0001101", 
    28 => "1110101", 29 => "0011000", 30 => "0000100", 31 => "0001010");



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

