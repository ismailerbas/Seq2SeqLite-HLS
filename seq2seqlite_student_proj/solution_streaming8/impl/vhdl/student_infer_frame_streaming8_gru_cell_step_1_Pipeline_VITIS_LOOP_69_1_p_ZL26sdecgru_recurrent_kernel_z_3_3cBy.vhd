-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_z_3_3cBy is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_z_3_3cBy is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1101110", 1 => "1110110", 2 => "1101100", 3 => "1111111", 
    4 => "1011010", 5 => "1111101", 6 => "0000110", 7 => "0000010", 
    8 => "0000100", 9 => "0100110", 10 => "1111111", 11 => "0011110", 
    12 => "0010000", 13 => "1111100", 14 => "1101110", 15 => "0000100", 
    16 => "1110111", 17 => "1100010", 18 => "1010100", 19 => "0010100", 
    20 => "0010101", 21 => "0001110", 22 => "1101110", 23 => "1111101", 
    24 => "0001101", 25 => "0000100", 26 => "1100100", 27 => "1110011", 
    28 => "0010001", 29 => "1111011", 30 => "1110001", 31 => "0100000");



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

