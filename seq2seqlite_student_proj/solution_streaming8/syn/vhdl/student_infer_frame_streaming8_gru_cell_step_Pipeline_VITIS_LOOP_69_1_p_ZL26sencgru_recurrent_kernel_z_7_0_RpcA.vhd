-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_z_7_0_RpcA is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_z_7_0_RpcA is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0010001", 1 => "0010000", 2 => "0001001", 3 => "0010110", 
    4 => "1111110", 5 => "0001111", 6 => "0010100", 7 => "1101000", 
    8 => "1101111", 9 => "0010000", 10 => "1111100", 11 => "0100011", 
    12 => "0011111", 13 => "0010111", 14 => "0010100", 15 => "1111101", 
    16 => "0000101", 17 => "0000111", 18 => "0000001", 19 => "1111111", 
    20 => "0011100", 21 => "1101001", 22 => "0010001", 23 => "1110000", 
    24 => "0000101", 25 => "0000001", 26 => "0001111", 27 => "0000110", 
    28 => "1101110", 29 => "0000101", 30 => "0010001", 31 => "0010011");



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

