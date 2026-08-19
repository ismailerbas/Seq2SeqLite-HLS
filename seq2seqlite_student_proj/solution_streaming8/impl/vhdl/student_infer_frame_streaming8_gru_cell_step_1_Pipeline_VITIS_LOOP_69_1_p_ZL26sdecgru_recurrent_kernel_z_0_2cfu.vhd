-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_z_0_2cfu is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_1_Pipeline_VITIS_LOOP_69_1_p_ZL26sdecgru_recurrent_kernel_z_0_2cfu is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0100101", 1 => "1111001", 2 => "1111101", 3 => "0001000", 
    4 => "0000010", 5 => "0001011", 6 => "1111101", 7 => "1110010", 
    8 => "0011100", 9 => "1011100", 10 => "1100101", 11 => "1101100", 
    12 => "0000101", 13 => "1111010", 14 => "1111110", 15 => "0000111", 
    16 => "1110010", 17 => "0010100", 18 => "0100010", 19 => "1110101", 
    20 => "0101110", 21 => "0010100", 22 => "1110100", 23 => "0011100", 
    24 => "0010001", 25 => "0001100", 26 => "1111010", 27 => "0011111", 
    28 => "1111110", 29 => "0010111", 30 => "0000110", 31 => "0001011");



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

