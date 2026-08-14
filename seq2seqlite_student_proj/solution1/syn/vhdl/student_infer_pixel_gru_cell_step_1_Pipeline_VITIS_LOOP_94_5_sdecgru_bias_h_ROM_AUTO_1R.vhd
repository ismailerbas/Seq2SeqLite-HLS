-- ==============================================================
-- Vitis HLS - High-Level Synthesis from C, C++ and OpenCL v2022.2 (64-bit)
-- Version: 2022.2
-- Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
-- ==============================================================
library ieee; 
use ieee.std_logic_1164.all; 
use ieee.std_logic_unsigned.all;

entity student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_sdecgru_bias_h_ROM_AUTO_1R is 
    generic(
             DataWidth     : integer := 4; 
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


architecture rtl of student_infer_pixel_gru_cell_step_1_Pipeline_VITIS_LOOP_94_5_sdecgru_bias_h_ROM_AUTO_1R is 
 
signal address0_tmp : std_logic_vector(AddressWidth-1 downto 0); 

type mem_array is array (0 to AddressRange-1) of std_logic_vector (DataWidth-1 downto 0); 

signal mem0 : mem_array := (
    0 => "1110", 1 => "1111", 2 => "0011", 3 => "0001", 
    4 => "0011", 5 => "1101", 6 => "1110", 7 => "1111", 
    8 => "0001", 9 => "0011", 10 => "0001", 11 => "1111", 
    12 => "1111", 13 => "1111", 14 => "0001", 15 => "1011", 
    16 => "0001", 17 => "0001", 18 => "0011", 19 => "0001", 
    20 => "0010", 21 => "0000", 22 => "0001", 23 => "0001", 
    24 => "0010", 25 => "1110", 26 => "0000", 27 => "0000", 
    28 => "0000", 29 => "0001", 30 => "1111", 31 => "0000");



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

