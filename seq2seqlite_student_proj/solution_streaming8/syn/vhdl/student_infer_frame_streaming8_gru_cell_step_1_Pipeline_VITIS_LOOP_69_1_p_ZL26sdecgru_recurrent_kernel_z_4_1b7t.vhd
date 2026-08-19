-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_z_4_1b7t is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_z_4_1b7t is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "00000011", 1 => "01011010", 2 => "00000101", 3 => "00001011", 
    4 => "01001101", 5 => "00010100", 6 => "11100111", 7 => "00001000", 
    8 => "00001000", 9 => "00011111", 10 => "00001011", 11 => "00011100", 
    12 => "11110010", 13 => "11110110", 14 => "00001111", 15 => "11111101", 
    16 => "00111000", 17 => "11101000", 18 => "01101110", 19 => "11110011", 
    20 => "10111111", 21 => "01101110", 22 => "00110111", 23 => "00001111", 
    24 => "11011110", 25 => "00000101", 26 => "01010000", 27 => "11100110", 
    28 => "11111101", 29 => "00101001", 30 => "11111100", 31 => "00001101");



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

