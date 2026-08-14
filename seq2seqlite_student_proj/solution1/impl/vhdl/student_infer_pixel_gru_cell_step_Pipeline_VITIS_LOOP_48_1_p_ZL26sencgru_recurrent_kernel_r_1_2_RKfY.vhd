-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_r_1_2_RKfY is 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_r_1_2_RKfY is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1100111", 1 => "1101010", 2 => "1101000", 3 => "0000001", 
    4 => "1110111", 5 => "1111010", 6 => "1110010", 7 => "1101111", 
    8 => "0000010", 9 => "1100001", 10 => "1110001", 11 => "0000000", 
    12 => "1111100", 13 => "1110111", 14 => "1010010", 15 => "1110011", 
    16 => "1101010", 17 => "0000111", 18 => "1110101", 19 => "1011001", 
    20 => "0000101", 21 => "0000101", 22 => "0000001", 23 => "1110110", 
    24 => "1110010", 25 => "1011101", 26 => "1111000", 27 => "1111111", 
    28 => "0011011", 29 => "1110100", 30 => "0001010", 31 => "1110100");



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

