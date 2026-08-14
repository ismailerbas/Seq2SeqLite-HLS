-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_0_3_RbBo is 
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


architecture rtl of student_infer_pixel_gru_cell_step_Pipeline_VITIS_LOOP_94_5_p_ZL26sencgru_recurrent_kernel_h_0_3_RbBo is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "0001010", 1 => "1111100", 2 => "0001100", 3 => "1111011", 
    4 => "1111011", 5 => "0000010", 6 => "1110110", 7 => "1101010", 
    8 => "1110101", 9 => "0001100", 10 => "0001010", 11 => "0100000", 
    12 => "0010000", 13 => "1011110", 14 => "0010000", 15 => "0000001", 
    16 => "0001100", 17 => "1101110", 18 => "0000001", 19 => "1101110", 
    20 => "1111110", 21 => "0000000", 22 => "1111100", 23 => "0000110", 
    24 => "0010010", 25 => "1110101", 26 => "0001000", 27 => "1111110", 
    28 => "1111100", 29 => "1011011", 30 => "1111111", 31 => "0001100");



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

