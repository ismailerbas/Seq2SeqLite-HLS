-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_4_c5D is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_4_c5D is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0010001", 1 => "1110111", 2 => "0001010", 3 => "0000001", 
    4 => "1111001", 5 => "0000111", 6 => "1111111", 7 => "1111101", 
    8 => "0010101", 9 => "0000110", 10 => "0000011", 11 => "1101110", 
    12 => "0101011", 13 => "0010011", 14 => "0010100", 15 => "1101110", 
    16 => "0001000", 17 => "0000011", 18 => "0001111", 19 => "1100001", 
    20 => "1011101", 21 => "0000101", 22 => "1011110", 23 => "1110000", 
    24 => "1101011", 25 => "0001101", 26 => "0000110", 27 => "1101011", 
    28 => "0011101", 29 => "1111011", 30 => "0000101", 31 => "1101000");



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

