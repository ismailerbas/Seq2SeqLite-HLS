-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_4_2_bxn is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_4_2_bxn is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1111011", 1 => "0011100", 2 => "0001010", 3 => "0000010", 
    4 => "0010000", 5 => "1100111", 6 => "1101100", 7 => "1110111", 
    8 => "0010011", 9 => "0111110", 10 => "0010001", 11 => "1111111", 
    12 => "1101110", 13 => "0000110", 14 => "0000001", 15 => "1110010", 
    16 => "1110111", 17 => "0000100", 18 => "0000010", 19 => "1110011", 
    20 => "0011000", 21 => "1111110", 22 => "0000000", 23 => "0000011", 
    24 => "1111101", 25 => "0011000", 26 => "0011010", 27 => "0011010", 
    28 => "1110011", 29 => "0000000", 30 => "1111111", 31 => "1100110");



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

