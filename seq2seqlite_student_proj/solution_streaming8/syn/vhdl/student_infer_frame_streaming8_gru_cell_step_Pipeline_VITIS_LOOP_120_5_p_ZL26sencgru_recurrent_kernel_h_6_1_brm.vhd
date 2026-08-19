-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_6_1_brm is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_6_1_brm is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "11101111", 1 => "11111100", 2 => "00000001", 3 => "11110100", 
    4 => "00000011", 5 => "11110000", 6 => "11100111", 7 => "00001100", 
    8 => "11111000", 9 => "01000100", 10 => "00010101", 11 => "00000100", 
    12 => "11111011", 13 => "00001111", 14 => "00010010", 15 => "00000100", 
    16 => "11111011", 17 => "00001101", 18 => "11110001", 19 => "00001111", 
    20 => "00001101", 21 => "11110100", 22 => "11101100", 23 => "00001101", 
    24 => "11110101", 25 => "00001111", 26 => "00000011", 27 => "11111110", 
    28 => "11110100", 29 => "11111011", 30 => "11111000", 31 => "11110000");



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

