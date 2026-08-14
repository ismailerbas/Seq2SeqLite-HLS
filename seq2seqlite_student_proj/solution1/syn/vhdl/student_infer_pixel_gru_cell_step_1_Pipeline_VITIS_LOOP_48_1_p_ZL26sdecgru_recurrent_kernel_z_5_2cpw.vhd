-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_z_5_2cpw is 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_48_1_p_ZL26sdecgru_recurrent_kernel_z_5_2cpw is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "111000", 1 => "101000", 2 => "011100", 3 => "011100", 
    4 => "000000", 5 => "110111", 6 => "001000", 7 => "011010", 
    8 => "111111", 9 => "110001", 10 => "111000", 11 => "101101", 
    12 => "110101", 13 => "111100", 14 => "011000", 15 => "010001", 
    16 => "111011", 17 => "000000", 18 => "110100", 19 => "111010", 
    20 => "101011", 21 => "000100", 22 => "000001", 23 => "101111", 
    24 => "011100", 25 => "010000", 26 => "000100", 27 => "111100", 
    28 => "110010", 29 => "000010", 30 => "000011", 31 => "000101");



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

