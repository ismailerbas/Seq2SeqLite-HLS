-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_7_2_bAo is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_7_2_bAo is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1110000", 1 => "0011011", 2 => "1110101", 3 => "0000000", 
    4 => "0001000", 5 => "1110100", 6 => "0001001", 7 => "1011101", 
    8 => "0010001", 9 => "1111010", 10 => "1110000", 11 => "1111010", 
    12 => "1101010", 13 => "1110010", 14 => "0001101", 15 => "0000100", 
    16 => "1110100", 17 => "0001000", 18 => "1110111", 19 => "0100010", 
    20 => "0001100", 21 => "0000101", 22 => "0000001", 23 => "1111001", 
    24 => "0010011", 25 => "0001101", 26 => "0100001", 27 => "1100101", 
    28 => "1111011", 29 => "1100101", 30 => "0000000", 31 => "1110000");



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

