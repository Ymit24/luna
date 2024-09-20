library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

use work.CommonPkg.all;

entity ALU is
  port (
    X: in std_logic_vector(15 downto 0);
    Y: in std_logic_vector(15 downto 0);
    OP: in std_logic_vector(3 downto 0);
    ZX: in std_logic;
    ZY: in std_logic;
    NO: in std_logic;

    Result: out std_logic_vector(15 downto 0);
    IsNeg: out std_logic;
    IsZero: out std_logic;
  );
end entity ALU;

architecture rtl of ALU is
  signal opcode : opcode_type;
begin
  opcode <= to_opcode(OP);

  IsNeg <= Result(15);

  if Result = (others => '0') then
    IsZero <= '1';
  else
    IsZero <= '0';
  end if;


  opcodes: process(X, Y, OP, ZX, ZY, NO)
  begin
    case opcode is
      when ADD => Result <= X + Y
      when SUB => Result <= X - Y
      when AND_OP => Result <= X & Y
      when OR_OP => Result <= X | Y
    end case;
  end process proc_name;
end architecture rtl;
