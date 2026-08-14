-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_5_1_Rbqm is 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_5_1_Rbqm is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "000100", 1 => "000011", 2 => "000010", 3 => "111010", 
    4 => "001100", 5 => "101101", 6 => "011000", 7 => "011011", 
    8 => "000111", 9 => "001001", 10 => "000110", 11 => "110101", 
    12 => "000010", 13 => "000111", 14 => "000001", 15 => "001110", 
    16 => "110101", 17 => "110001", 18 => "100011", 19 => "011001", 
    20 => "010000", 21 => "111101", 22 => "011011", 23 => "101000", 
    24 => "000111", 25 => "111010", 26 => "000001", 27 => "011000", 
    28 => "011000", 29 => "011111", 30 => "001100", 31 => "000000");



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

