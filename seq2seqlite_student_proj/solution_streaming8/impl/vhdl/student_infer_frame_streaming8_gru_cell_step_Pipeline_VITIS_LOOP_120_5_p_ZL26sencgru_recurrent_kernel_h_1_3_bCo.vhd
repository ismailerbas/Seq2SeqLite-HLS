-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_1_3_bCo is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_120_5_p_ZL26sencgru_recurrent_kernel_h_1_3_bCo is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1101111", 1 => "1111010", 2 => "0000011", 3 => "1110100", 
    4 => "0000011", 5 => "0011010", 6 => "1100000", 7 => "0000100", 
    8 => "1111000", 9 => "0110110", 10 => "0000011", 11 => "0001000", 
    12 => "1110001", 13 => "1111101", 14 => "0010001", 15 => "0010001", 
    16 => "1110101", 17 => "1111110", 18 => "1110100", 19 => "0010000", 
    20 => "0001101", 21 => "1110011", 22 => "1110110", 23 => "1111000", 
    24 => "1110110", 25 => "0001110", 26 => "0000001", 27 => "1111000", 
    28 => "0001010", 29 => "0000100", 30 => "1101011", 31 => "0001010");



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

