-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_4_cXB is 
    generic(
             DataWidth     : integer := 8; 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_120_5_p_ZL26sdecgru_recurrent_kernel_h_4_cXB is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "00011011", 1 => "11110011", 2 => "00000011", 3 => "00011000", 
    4 => "11111110", 5 => "00001010", 6 => "00001001", 7 => "00000001", 
    8 => "00100111", 9 => "11110010", 10 => "00000000", 11 => "00011000", 
    12 => "11011011", 13 => "00010010", 14 => "11101011", 15 => "00000001", 
    16 => "11010101", 17 => "11101010", 18 => "11011001", 19 => "10110111", 
    20 => "11100111", 21 => "00010100", 22 => "11101100", 23 => "11011111", 
    24 => "00010110", 25 => "00001001", 26 => "00010110", 27 => "00100011", 
    28 => "00001111", 29 => "00001101", 30 => "11110111", 31 => "00010000");



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

