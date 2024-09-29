library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

library core;
use core.CommonPkg.all;

entity ALU is
  port (
    X: in std_logic_vector(15 downto 0);
    Y: in std_logic_vector(15 downto 0);
    OP: in std_logic_vector(1 downto 0);
    ZX: in std_logic;
    ZY: in std_logic;
    NO: in std_logic;

    result: out std_logic_vector(15 downto 0);
    IsNeg: out std_logic;
    IsZero: out std_logic
  );
end entity ALU;

architecture rtl of ALU is
  signal opcode : opcode_type;
  signal intermediate_result: Word;
begin
  opcode <= to_opcode(OP);

  IsNeg <= intermediate_result(15);

  result <= intermediate_result;

  process(X, Y, OP, ZX, ZY, NO)
  begin
    case opcode is
      when ADD    => intermediate_result <= std_logic_vector(signed(X) + signed(Y));
      when SUB    => intermediate_result <= std_logic_vector(signed(X) - signed(Y));
      when AND_OP => intermediate_result <= X and Y;
      when OR_OP  => intermediate_result <= X or Y;
    end case;
    if intermediate_result = x"0000" then
      IsZero <= '1';
    else
      IsZero <= '0';
    end if;
  end process;
end architecture rtl;
