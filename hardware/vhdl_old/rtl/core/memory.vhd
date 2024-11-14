library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

library core;
use core.CommonPkg.all;

entity Memory is
  port (
    destination_addr: in std_logic;
    destination_data: in std_logic;
    destination_ram: in std_logic;

    clk: in std_logic;
    data_in: in std_logic_vector(15 downto 0);

    addr_out: out std_logic_vector(15 downto 0);
    data_out: out std_logic_vector(15 downto 0);
    ram_out: out std_logic_vector(15 downto 0)
  );
end entity Memory;

architecture rtl of Memory is
  signal addr_reg : Word;
  signal data_reg : Word;
begin
  RAM: entity core.MemoryBank(rtl)
    generic map(ADDR_WIDTH => 15)
    port map(
      addr => addr_reg,
      data_in => data_in,
      data_out => ram_out,
      we => destination_ram,
      clk => clk
    );

  process(clk)
  begin
    if (rising_edge(clk)) then
      addr_out <= addr_reg;
      data_out <= data_reg;
    end if;
  end process;
end architecture rtl;
