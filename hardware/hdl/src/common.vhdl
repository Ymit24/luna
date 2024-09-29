library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

package CommonPkg is
  type opcode_type is (ADD, SUB, AND_OP, OR_OP);
  type jump_cond_type is (JMP, JEQ, JNQ, JLT, JLQ, JGT, JGQ, JNO);

  subtype Word is std_logic_vector(15 downto 0);

  function to_opcode(op_vec: std_logic_vector(1 downto 0)) return opcode_type;
  function to_jump_cond(j_vec: std_logic_vector(2 downto 0)) return jump_cond_type;
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
  function to_jump_cond(j_vec: std_logic_vector(2 downto 0))
    return jump_cond_type is
      variable jumpcode : jump_cond_type;
  begin
    case j_vec is
      when "000" => jumpcode := JNO;
      when "001" => jumpcode := JGT;
      when "010" => jumpcode := JEQ;
      when "011" => jumpcode := JGQ;
      when "100" => jumpcode := JLT;
      when "101" => jumpcode := JNE;
      when "110" => jumpcode := JLQ;
      when "111" => jumpcode := JMP;
      when others =>
        jumpcode := JNO;
    end case;
    return jumpcode;
  end function to_jump_cond;
end package body CommonPkg;
