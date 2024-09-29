library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

library core;
use core.CommonPkg.all;

entity ControlUnit is
  port (
    instr: in Word;
    addr_in: in Word;
    data_in: in Word;
    memory_in: in Word;
    is_neg: in std_logic;
    is_zero: in std_logic;
    X: in Word;
    Y: in Word;
    addr_out: Word;
    set_pc: in std_logic;
    destination_data: out std_logic;
    destination_addr: out std_logic;
    destination_ram: out std_logic;
    opcode: out std_logic_vector(1 downto 0)
  );
end entity ControlUnit;
