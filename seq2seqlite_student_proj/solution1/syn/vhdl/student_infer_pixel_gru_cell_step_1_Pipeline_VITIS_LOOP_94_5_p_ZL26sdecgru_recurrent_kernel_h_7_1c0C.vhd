-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_7_1c0C is 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_7_1c0C is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0010100", 1 => "0000011", 2 => "1100110", 3 => "1111011", 
    4 => "0010100", 5 => "0100101", 6 => "0001101", 7 => "0001001", 
    8 => "1111110", 9 => "1111000", 10 => "1110100", 11 => "1110110", 
    12 => "1111011", 13 => "1110000", 14 => "0000101", 15 => "0000000", 
    16 => "0010011", 17 => "0010000", 18 => "1100110", 19 => "1111100", 
    20 => "0001100", 21 => "1110011", 22 => "0001010", 23 => "1011001", 
    24 => "0000111", 25 => "1111011", 26 => "1110010", 27 => "0000010", 
    28 => "0000110", 29 => "1111010", 30 => "0000010", 31 => "0010100");



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

