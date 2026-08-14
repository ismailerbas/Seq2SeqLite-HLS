-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_1_1cUB is 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_1_1cUB is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0000111", 1 => "1101110", 2 => "0000011", 3 => "0100000", 
    4 => "1101011", 5 => "1111011", 6 => "1111101", 7 => "1110100", 
    8 => "1110101", 9 => "1111010", 10 => "0000010", 11 => "0010010", 
    12 => "0000000", 13 => "1111111", 14 => "1101101", 15 => "1110101", 
    16 => "1011000", 17 => "1110110", 18 => "1111011", 19 => "1111101", 
    20 => "0000111", 21 => "1101100", 22 => "1100011", 23 => "1101100", 
    24 => "0000110", 25 => "0000100", 26 => "0000110", 27 => "0001011", 
    28 => "1101100", 29 => "0001011", 30 => "0000010", 31 => "1111110");



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

