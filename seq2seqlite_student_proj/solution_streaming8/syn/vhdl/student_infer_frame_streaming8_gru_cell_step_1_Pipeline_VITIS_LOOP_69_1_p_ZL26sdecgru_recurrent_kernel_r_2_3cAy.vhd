-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_r_2_3cAy is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_r_2_3cAy is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1111110", 1 => "1101010", 2 => "0010011", 3 => "1111011", 
    4 => "1110011", 5 => "1110011", 6 => "1101110", 7 => "0001001", 
    8 => "0000000", 9 => "1111010", 10 => "0000100", 11 => "1111011", 
    12 => "1101000", 13 => "0010011", 14 => "0001111", 15 => "1111100", 
    16 => "1111111", 17 => "0010100", 18 => "0010101", 19 => "1111110", 
    20 => "0001001", 21 => "0001100", 22 => "1111010", 23 => "0100110", 
    24 => "1110100", 25 => "1100111", 26 => "0000011", 27 => "1110000", 
    28 => "1110010", 29 => "0001100", 30 => "0001011", 31 => "1110100");



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

