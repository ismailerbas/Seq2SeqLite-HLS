-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_z_4_2cnw is 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_z_4_2cnw is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "00001110", 1 => "00000001", 2 => "00101000", 3 => "00010101", 
    4 => "00010111", 5 => "11110010", 6 => "11111001", 7 => "11101001", 
    8 => "00000010", 9 => "10111010", 10 => "11110111", 11 => "00000111", 
    12 => "00001100", 13 => "11101110", 14 => "00000111", 15 => "11101101", 
    16 => "00000011", 17 => "11111111", 18 => "00101011", 19 => "00000101", 
    20 => "00001110", 21 => "00010001", 22 => "11110111", 23 => "00000010", 
    24 => "00010100", 25 => "11111101", 26 => "11111100", 27 => "00010001", 
    28 => "00000110", 29 => "00100011", 30 => "00010010", 31 => "00001101");



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

