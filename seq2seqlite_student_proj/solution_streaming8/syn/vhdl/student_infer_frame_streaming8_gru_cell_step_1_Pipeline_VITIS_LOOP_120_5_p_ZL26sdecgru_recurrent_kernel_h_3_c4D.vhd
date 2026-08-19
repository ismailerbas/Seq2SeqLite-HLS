-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_3_c4D is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_3_c4D is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0000010", 1 => "1111100", 2 => "1111011", 3 => "0000000", 
    4 => "1100111", 5 => "0000000", 6 => "1011101", 7 => "0000110", 
    8 => "0000101", 9 => "0000011", 10 => "1111101", 11 => "1101100", 
    12 => "0010000", 13 => "0001010", 14 => "1101110", 15 => "1111100", 
    16 => "0000001", 17 => "0001001", 18 => "1111111", 19 => "0011111", 
    20 => "0001011", 21 => "1110011", 22 => "0010000", 23 => "0001111", 
    24 => "1111000", 25 => "1111010", 26 => "1110011", 27 => "1110010", 
    28 => "0001010", 29 => "1110100", 30 => "0000000", 31 => "1101100");



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

