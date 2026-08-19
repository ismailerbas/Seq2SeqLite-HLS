-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_5_deE is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_5_deE is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "100111", 1 => "000011", 2 => "010111", 3 => "010011", 
    4 => "001010", 5 => "000110", 6 => "000000", 7 => "111010", 
    8 => "100101", 9 => "000000", 10 => "111011", 11 => "101011", 
    12 => "110100", 13 => "111101", 14 => "111001", 15 => "110110", 
    16 => "001101", 17 => "000101", 18 => "010110", 19 => "111000", 
    20 => "010111", 21 => "000011", 22 => "001110", 23 => "010000", 
    24 => "111010", 25 => "010010", 26 => "101101", 27 => "111001", 
    28 => "111000", 29 => "011100", 30 => "110100", 31 => "101111");



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

