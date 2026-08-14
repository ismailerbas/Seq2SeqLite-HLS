-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_z_5_1_RBew is 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_z_5_1_RBew is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "001001", 1 => "111111", 2 => "111101", 3 => "000010", 
    4 => "000011", 5 => "110010", 6 => "111011", 7 => "011110", 
    8 => "110101", 9 => "010101", 10 => "111000", 11 => "111010", 
    12 => "001110", 13 => "101000", 14 => "111011", 15 => "010110", 
    16 => "110101", 17 => "100110", 18 => "111110", 19 => "111011", 
    20 => "010010", 21 => "010111", 22 => "110000", 23 => "110111", 
    24 => "001000", 25 => "000001", 26 => "101101", 27 => "101100", 
    28 => "010001", 29 => "011011", 30 => "110111", 31 => "110010");



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

