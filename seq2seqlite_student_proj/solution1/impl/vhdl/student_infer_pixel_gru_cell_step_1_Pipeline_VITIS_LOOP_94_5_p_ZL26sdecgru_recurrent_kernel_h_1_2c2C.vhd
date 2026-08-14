-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_1_2c2C is 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_p_ZL26sdecgru_recurrent_kernel_h_1_2c2C is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "000010", 1 => "001011", 2 => "001110", 3 => "111001", 
    4 => "001010", 5 => "000110", 6 => "111001", 7 => "000001", 
    8 => "110010", 9 => "010010", 10 => "011010", 11 => "111010", 
    12 => "001110", 13 => "010101", 14 => "111110", 15 => "111111", 
    16 => "000101", 17 => "101101", 18 => "010101", 19 => "101100", 
    20 => "001010", 21 => "100010", 22 => "110101", 23 => "111010", 
    24 => "101010", 25 => "111000", 26 => "001110", 27 => "100001", 
    28 => "111101", 29 => "110100", 30 => "111001", 31 => "000011");



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

