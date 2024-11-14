library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

library core;
use core.CommonPkg.all;

entity Computer is
  port (
    clk: in std_logic
  );
end entity Computer;

architecture rtl of Computer is
  signal X: Word;
  signal Y: Word;
  signal OP: std_logic_vector(1 downto 0);

  signal aluResult: Word;
  signal isNeg, isZero: std_logic;

  signal addrOut, memoryDataOut: Word;

  signal programCounter: Word;
begin
  Alu: entity core.ALU(rtl) port map (
    X=> X,
    Y=> Y,
    OP=> OP,
    ZX => '0',
    ZY => '0',
    NO => '0',

    result => aluResult,
    IsNeg => isNeg,
    IsZero => isZero
  );

  Memory: entity core.Memory(rtl)
    port map (
      destination_addr => '0',
      destination_data => '0',
      destination_ram =>  '0',
      clk => clk,
      data_in => X,
      addr_out => addrOut,
      data_out => Y,
      ram_out => memoryDataOut
    );

  ProgramMemory: entity core.MemoryBank(rtl)
    generic map (
      ADDR_WIDTH => 16
    )
    port map (
      addr => programCounter,
      data_in => x"0000",
      we=> '0',
      clk => clk,
      data_out => X
    );

end architecture rtl;
