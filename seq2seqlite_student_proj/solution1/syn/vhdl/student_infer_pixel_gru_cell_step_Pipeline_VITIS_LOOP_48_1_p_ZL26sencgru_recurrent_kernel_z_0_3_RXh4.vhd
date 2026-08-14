-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_z_0_3_RXh4 is 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_z_0_3_RXh4 is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1111010", 1 => "1101000", 2 => "0010110", 3 => "0010101", 
    4 => "0001100", 5 => "0000101", 6 => "1110011", 7 => "1100000", 
    8 => "0011101", 9 => "0001001", 10 => "0000001", 11 => "0010111", 
    12 => "1111001", 13 => "0001110", 14 => "0000000", 15 => "1111111", 
    16 => "1101011", 17 => "0001101", 18 => "1110101", 19 => "0001010", 
    20 => "0001010", 21 => "0100000", 22 => "0010100", 23 => "1111100", 
    24 => "1110001", 25 => "1101110", 26 => "0000111", 27 => "1110110", 
    28 => "0000100", 29 => "0010000", 30 => "1111101", 31 => "1111100");



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

