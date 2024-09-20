library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

package CommonPkg is
  type opcode_type is (ADD, SUB, AND_OP, OR_OP);

  function to_opcode(op_vec: std_logic_vector(3 downto 0)) return opcode_type;
end package Common;

package body CommonPkg is
  function to_opcode(op_vec: std_logic_vector(3 downto 0)) return opcode_type is
    variable opcode : opcode_type;
  begin
    case op_vec is
      when "00" => opcode := ADD;
      when "01" => opcode := SUB;
      when "10" => opcode := AND_OP;
      when "11" => opcode := OR_OP;
    end case;
  end function to_opcode;
end package body CommonPkg;
