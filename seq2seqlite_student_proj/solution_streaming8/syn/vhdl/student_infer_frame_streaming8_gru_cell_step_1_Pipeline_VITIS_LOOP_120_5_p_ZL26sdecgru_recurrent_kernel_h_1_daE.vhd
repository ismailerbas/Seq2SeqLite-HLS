-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_1_daE is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_1_daE is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1110110", 1 => "0010000", 2 => "0000011", 3 => "0000101", 
    4 => "0010011", 5 => "1110111", 6 => "0001110", 7 => "1110001", 
    8 => "1111111", 9 => "0011101", 10 => "0001101", 11 => "0011000", 
    12 => "0000110", 13 => "1100110", 14 => "1101100", 15 => "1111100", 
    16 => "0000010", 17 => "1110001", 18 => "0001000", 19 => "1110010", 
    20 => "1111000", 21 => "1111100", 22 => "1011011", 23 => "0000010", 
    24 => "1111100", 25 => "1111111", 26 => "0000110", 27 => "0001110", 
    28 => "1100011", 29 => "0011000", 30 => "0010001", 31 => "1110001");



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

