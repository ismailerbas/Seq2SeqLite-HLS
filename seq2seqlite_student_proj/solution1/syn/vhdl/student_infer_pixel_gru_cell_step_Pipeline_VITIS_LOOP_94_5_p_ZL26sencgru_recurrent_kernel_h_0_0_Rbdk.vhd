-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_0_0_Rbdk is 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_0_0_Rbdk is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0000010", 1 => "0101000", 2 => "1110111", 3 => "1111001", 
    4 => "1111101", 5 => "1100000", 6 => "1111010", 7 => "1110101", 
    8 => "1110101", 9 => "1011101", 10 => "1101011", 11 => "1101101", 
    12 => "0001011", 13 => "0000010", 14 => "1101101", 15 => "0000110", 
    16 => "0001111", 17 => "0001001", 18 => "1110110", 19 => "1101110", 
    20 => "1111001", 21 => "0000010", 22 => "0000111", 23 => "1110000", 
    24 => "1111000", 25 => "1011110", 26 => "1110100", 27 => "1111001", 
    28 => "0000000", 29 => "0001111", 30 => "1110111", 31 => "1111001");



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

