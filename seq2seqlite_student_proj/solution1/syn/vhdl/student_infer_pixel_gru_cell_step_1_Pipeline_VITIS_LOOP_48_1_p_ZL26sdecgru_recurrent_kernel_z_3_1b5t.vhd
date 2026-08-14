-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_z_3_1b5t is 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_z_3_1b5t is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0001010", 1 => "0001011", 2 => "1101011", 3 => "0100110", 
    4 => "1101111", 5 => "1101011", 6 => "0010000", 7 => "1100111", 
    8 => "1110010", 9 => "1110100", 10 => "1111111", 11 => "1101001", 
    12 => "0010011", 13 => "0001011", 14 => "1101101", 15 => "1110101", 
    16 => "1111000", 17 => "1111100", 18 => "1101011", 19 => "1110010", 
    20 => "1111110", 21 => "1111101", 22 => "1101011", 23 => "0001000", 
    24 => "1101100", 25 => "1110110", 26 => "1110010", 27 => "1111111", 
    28 => "1110111", 29 => "1111110", 30 => "0010111", 31 => "0100001");



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

