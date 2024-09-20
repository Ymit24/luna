library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity Memory is
  port (
    destination_addr: in std_logic;
    destination_data: in std_logic;
    destination_ram: in std_logic;

    clk: in std_logic;
    data_in: in std_logic_vector(15 downto 0);

    addr_out: out std_logic_vector(15 downto 0);
    data_out: out std_logic_vector(15 downto 0);
    ram_out: out std_logic_vector(15 downto 0);
  );
end entity Memory;

architecture rtl of Memory is
  signal addr_reg : std_logic;
  signal data_reg : std_logic;
begin
  addr_out <= addr_reg;
  data_out <= data_reg;
end architecture rtl;
