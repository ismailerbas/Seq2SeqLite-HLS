-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_r_2_0_Rg8j is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_r_2_0_Rg8j is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1111100", 1 => "0000001", 2 => "1101101", 3 => "1111011", 
    4 => "1101100", 5 => "1111001", 6 => "1110100", 7 => "1111100", 
    8 => "1111100", 9 => "1110111", 10 => "0001100", 11 => "0001011", 
    12 => "1110110", 13 => "1111001", 14 => "1101110", 15 => "1011000", 
    16 => "1110110", 17 => "0011111", 18 => "1110000", 19 => "1101101", 
    20 => "1100010", 21 => "0000011", 22 => "0000010", 23 => "1111000", 
    24 => "0000011", 25 => "1101111", 26 => "1110100", 27 => "0001100", 
    28 => "1011101", 29 => "1110111", 30 => "0000100", 31 => "1111111");



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

