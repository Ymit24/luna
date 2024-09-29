library IEEE;
use IEEE.std_logic_1164.all;

library core;
use core.CommonPkg.all;

entity ShouldJumpUnit is
  port (
    isNeg: in std_logic;
    isZero: in std_logic;
    jumpCnd: in std_logic_vector(2 downto 0)
  );
end entity ShouldJumpUnit;

architecture rtl of ShouldJumpUnit is
begin
  process(isNeg, isZero, jumpCnd)
  begin
    
  end process;
end architecture rtl;
