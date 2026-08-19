-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_0_1_bll is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_0_1_bll is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "101000", 1 => "001001", 2 => "111100", 3 => "010001", 
    4 => "011100", 5 => "001011", 6 => "101000", 7 => "110001", 
    8 => "010010", 9 => "010110", 10 => "001000", 11 => "000101", 
    12 => "001001", 13 => "010101", 14 => "111000", 15 => "111101", 
    16 => "110111", 17 => "001101", 18 => "001100", 19 => "110001", 
    20 => "010010", 21 => "101101", 22 => "110110", 23 => "001001", 
    24 => "001010", 25 => "001010", 26 => "010010", 27 => "000011", 
    28 => "111000", 29 => "101011", 30 => "000110", 31 => "001010");



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

