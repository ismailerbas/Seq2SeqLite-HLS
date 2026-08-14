-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_r_0_1_Rsc4 is 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_48_1_p_ZL26sencgru_recurrent_kernel_r_0_1_Rsc4 is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1111100", 1 => "1011101", 2 => "1110100", 3 => "0001010", 
    4 => "0000010", 5 => "1111111", 6 => "1111010", 7 => "0010010", 
    8 => "0001001", 9 => "0000111", 10 => "0001100", 11 => "0001011", 
    12 => "0000000", 13 => "1011101", 14 => "1101101", 15 => "0000110", 
    16 => "1011011", 17 => "0010010", 18 => "0001100", 19 => "1100111", 
    20 => "0000000", 21 => "0001010", 22 => "1011100", 23 => "0000000", 
    24 => "1110110", 25 => "1111011", 26 => "1111101", 27 => "0001101", 
    28 => "1111110", 29 => "1111111", 30 => "1110101", 31 => "0000001");



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

