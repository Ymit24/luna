use work.CommonPkg.all;

entity MemoryBank is
  generic (ADDR_WIDTH: integer := 16)
  port (
    addr: in std_logic_vector(ADDR_WIDTH - 1 downto 0);
    data_in: in Word;
    we: in std_logic;
    clk: in std_logic;

    data_out: out Word;
  );
end entity MemoryBank;

architecture rtl of MemoryBank is
  type ram_t is array (0 to 2 ** ( ADDR_WIDTH-1 ) of Word);
  signal ram: ram_t;
begin
  main: process(clk)
  begin
    if (clk'event and clk = '1') then
      if (we = '1') then
        ram(conv_integer(addr)) <= data_in;
      end if;
      data_out <= ram(conv_integer(addr));
    end if;
  end process main;
end architecture rtl;
