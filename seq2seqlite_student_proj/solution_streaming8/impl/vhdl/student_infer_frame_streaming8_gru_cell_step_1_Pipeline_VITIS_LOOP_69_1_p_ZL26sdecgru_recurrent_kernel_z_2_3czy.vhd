-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_z_2_3czy is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_z_2_3czy is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "001101", 1 => "000110", 2 => "000110", 3 => "001000", 
    4 => "111111", 5 => "111011", 6 => "101101", 7 => "000110", 
    8 => "100011", 9 => "000000", 10 => "111011", 11 => "111100", 
    12 => "111011", 13 => "110110", 14 => "110000", 15 => "110010", 
    16 => "011010", 17 => "100000", 18 => "001100", 19 => "011100", 
    20 => "010100", 21 => "010111", 22 => "100101", 23 => "001101", 
    24 => "110111", 25 => "101100", 26 => "100111", 27 => "110110", 
    28 => "101011", 29 => "000000", 30 => "110111", 31 => "001111");



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

