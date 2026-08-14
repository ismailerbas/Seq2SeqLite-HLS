-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_3_1_Rbom is 
    generic(
             DataWidth     : integer := 6; 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_3_1_Rbom is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "111000", 1 => "010000", 2 => "111111", 3 => "011001", 
    4 => "000011", 5 => "001010", 6 => "111000", 7 => "101100", 
    8 => "101110", 9 => "010110", 10 => "101001", 11 => "000001", 
    12 => "001011", 13 => "000110", 14 => "000011", 15 => "101111", 
    16 => "110100", 17 => "010001", 18 => "111100", 19 => "000100", 
    20 => "101100", 21 => "000011", 22 => "110110", 23 => "001110", 
    24 => "010011", 25 => "001100", 26 => "000010", 27 => "111000", 
    28 => "110001", 29 => "100101", 30 => "111000", 31 => "110100");



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

