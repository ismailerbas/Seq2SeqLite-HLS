-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_7_1_Rbsm is 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_7_1_Rbsm is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "00001011", 1 => "00010001", 2 => "11101101", 3 => "00000011", 
    4 => "11110110", 5 => "00010001", 6 => "00010111", 7 => "11111110", 
    8 => "11111011", 9 => "10101000", 10 => "11111000", 11 => "00000000", 
    12 => "00001000", 13 => "11110110", 14 => "11101011", 15 => "00001110", 
    16 => "11111000", 17 => "11111100", 18 => "11101111", 19 => "11110111", 
    20 => "11101100", 21 => "11111010", 22 => "00010110", 23 => "11110111", 
    24 => "11100101", 25 => "11110011", 26 => "11111111", 27 => "00000100", 
    28 => "00001111", 29 => "00011111", 30 => "00000111", 31 => "00001010");



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

