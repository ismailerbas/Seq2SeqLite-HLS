-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_r_0_2_RIfE is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_r_0_2_RIfE is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1110111", 1 => "0000010", 2 => "0001100", 3 => "0000010", 
    4 => "0000111", 5 => "1110110", 6 => "0010010", 7 => "0000100", 
    8 => "0001011", 9 => "1101111", 10 => "1110101", 11 => "1111000", 
    12 => "0010011", 13 => "1101000", 14 => "0000100", 15 => "1110001", 
    16 => "0000111", 17 => "0011110", 18 => "0000001", 19 => "1111000", 
    20 => "1101101", 21 => "0100001", 22 => "0001000", 23 => "0001001", 
    24 => "1110100", 25 => "1101101", 26 => "1101011", 27 => "0000110", 
    28 => "1111110", 29 => "0000010", 30 => "1110001", 31 => "0000011");



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

