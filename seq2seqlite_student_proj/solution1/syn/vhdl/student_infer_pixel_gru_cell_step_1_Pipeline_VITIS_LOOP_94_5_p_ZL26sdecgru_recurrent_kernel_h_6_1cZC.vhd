-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_6_1cZC is 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_6_1cZC is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0000110", 1 => "0000011", 2 => "1101000", 3 => "1011001", 
    4 => "1100001", 5 => "0010111", 6 => "1110110", 7 => "1110101", 
    8 => "1110010", 9 => "1110001", 10 => "0000100", 11 => "1111000", 
    12 => "1111110", 13 => "1111010", 14 => "0011010", 15 => "0100010", 
    16 => "1110110", 17 => "0001011", 18 => "1110100", 19 => "0010110", 
    20 => "0000011", 21 => "1111010", 22 => "1110011", 23 => "0010010", 
    24 => "1110110", 25 => "1101101", 26 => "1100010", 27 => "1101100", 
    28 => "0001000", 29 => "1010010", 30 => "0000100", 31 => "0001010");



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

