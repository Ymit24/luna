library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

-- Testbench entity: No ports are needed for a testbench
entity ALU_tb is
end entity;

architecture Behavioral of ALU_tb is
  -- Component declaration for ALU
  component ALU
    port(
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
  end component;

  -- Test signals
  signal X, Y, result: std_logic_vector(15 downto 0);
  signal OP: std_logic_vector(1 downto 0);
  signal ZX, ZY, NO, IsNeg, IsZero: std_logic;

  -- Constants for the ALU operations (assuming they are 2-bit control codes)
  constant ADD    : std_logic_vector(1 downto 0) := "00";
  constant SUB    : std_logic_vector(1 downto 0) := "01";
  constant AND_OP : std_logic_vector(1 downto 0) := "10";
  constant OR_OP  : std_logic_vector(1 downto 0) := "11";

begin
  -- Instantiate the ALU component
  uut: ALU
    port map(
      X => X,
      Y => Y,
      OP => OP,
      ZX => ZX,
      ZY => ZY,
      NO => NO,
      result => result,
      IsNeg => IsNeg,
      IsZero => IsZero
    );

  -- Test process
  process
  begin
    -- Test 1: ADD X and Y
    X <= x"0001";  -- X = 1
    Y <= x"0002";  -- Y = 2
    OP <= ADD;     -- Addition operation
    ZX <= '0';     -- No zeroing of X
    ZY <= '0';     -- No zeroing of Y
    NO <= '0';     -- Normal operation
    wait for 10 ns;

    -- Test 2: SUB X - Y
    OP <= SUB;
    wait for 10 ns;

    -- Test 3: AND X and Y
    OP <= AND_OP;
    wait for 10 ns;

    -- Test 4: OR X and Y
    OP <= OR_OP;
    wait for 10 ns;

    -- Test 5: Zero X and perform ADD
    ZX <= '1';     -- Zero X
    OP <= ADD;
    wait for 10 ns;

    -- Test 6: Zero Y and perform ADD
    ZX <= '0';     -- Restore X
    ZY <= '1';     -- Zero Y
    OP <= ADD;
    wait for 10 ns;

    -- Test 7: Apply NO signal (inverts result)
    NO <= '1';     -- Invert result
    OP <= ADD;
    wait for 10 ns;

    -- Test 8: Negative result (SUB test)
    X <= x"0002";
    Y <= x"0004";
    OP <= SUB;
    NO <= '0';     -- Normal operation
    wait for 10 ns;

    -- End simulation
    wait;
  end process;

end architecture;

