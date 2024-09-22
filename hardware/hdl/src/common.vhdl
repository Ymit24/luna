library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

package CommonPkg is
  type opcode_type is (ADD, SUB, AND_OP, OR_OP);

  subtype Word is std_logic_vector(15 downto 0);

  function to_opcode(op_vec: std_logic_vector(1 downto 0)) return opcode_type;
end package CommonPkg;

package body CommonPkg is
  function to_opcode(op_vec: std_logic_vector(1 downto 0)) return opcode_type is
    variable opcode : opcode_type;
  begin
    case op_vec is
      when "00" => opcode := ADD;
      when "01" => opcode := SUB;
      when "10" => opcode := AND_OP;
      when "11" => opcode := OR_OP;
      when others => -- 'U', 'X', '-', etc.
        opcode := ADD;
    end case;
    return opcode;
  end function to_opcode;
end package body CommonPkg;
