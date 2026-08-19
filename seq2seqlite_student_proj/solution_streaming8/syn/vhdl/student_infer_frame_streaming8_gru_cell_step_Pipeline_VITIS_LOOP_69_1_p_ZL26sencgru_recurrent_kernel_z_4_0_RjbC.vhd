-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_z_4_0_RjbC is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_z_4_0_RjbC is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1101110", 1 => "0001101", 2 => "1101111", 3 => "1100010", 
    4 => "1111000", 5 => "0100000", 6 => "1110100", 7 => "1010111", 
    8 => "1110101", 9 => "1101111", 10 => "1111000", 11 => "1011010", 
    12 => "1111111", 13 => "1101101", 14 => "0010000", 15 => "1111110", 
    16 => "1110011", 17 => "1110111", 18 => "0000111", 19 => "1110000", 
    20 => "0001000", 21 => "0001000", 22 => "1101011", 23 => "1100011", 
    24 => "0000000", 25 => "0001100", 26 => "0000100", 27 => "1100111", 
    28 => "1101001", 29 => "1110101", 30 => "1100000", 31 => "0000001");



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

