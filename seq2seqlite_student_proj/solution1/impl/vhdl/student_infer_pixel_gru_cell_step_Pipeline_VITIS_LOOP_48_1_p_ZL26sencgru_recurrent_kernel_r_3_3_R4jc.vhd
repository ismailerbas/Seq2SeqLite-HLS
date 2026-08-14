-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_r_3_3_R4jc is 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_r_3_3_R4jc is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1111110", 1 => "0001000", 2 => "1101110", 3 => "0000100", 
    4 => "0000010", 5 => "0000111", 6 => "1110010", 7 => "0000111", 
    8 => "0001101", 9 => "0000001", 10 => "0001000", 11 => "1111011", 
    12 => "1111010", 13 => "0000101", 14 => "0000001", 15 => "0000100", 
    16 => "0001101", 17 => "1010111", 18 => "1111001", 19 => "0011100", 
    20 => "0011101", 21 => "0001001", 22 => "0010001", 23 => "1111010", 
    24 => "1101101", 25 => "1110111", 26 => "1110011", 27 => "1111111", 
    28 => "0010011", 29 => "0001010", 30 => "0000000", 31 => "1110110");



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

