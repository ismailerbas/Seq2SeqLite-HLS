-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_z_0_1_RrcU is 
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


architecture rtl of student_infer_frame_streaming8_gru_cell_step_Pipeline_VITIS_LOOP_69_1_p_ZL26sencgru_recurrent_kernel_z_0_1_RrcU is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "000000", 1 => "111111", 2 => "000001", 3 => "100100", 
    4 => "001001", 5 => "001011", 6 => "101000", 7 => "100111", 
    8 => "000110", 9 => "111100", 10 => "110010", 11 => "110010", 
    12 => "100001", 13 => "101001", 14 => "001111", 15 => "101110", 
    16 => "100101", 17 => "101110", 18 => "010010", 19 => "111100", 
    20 => "110101", 21 => "110100", 22 => "101010", 23 => "000011", 
    24 => "101100", 25 => "110101", 26 => "111011", 27 => "111001", 
    28 => "110001", 29 => "111101", 30 => "010000", 31 => "100000");



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

