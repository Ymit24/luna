module control_fsm(
  input clk,
  input rst,

  output reg fetch_clk,
  output reg decode_clk,
  output reg exec_clk,
  output reg wrbk_clk
);
  reg [1:0] counter = 0;

  always @ (posedge clk) begin
    if (rst) begin
      counter <= 0;
      fetch_clk <= 0;
      decode_clk <= 0;
      exec_clk <= 0;
      wrbk_clk <= 0;
    end else begin
      fetch_clk  <= (counter == 2'b00) ? 1'b1 : 1'b0;
      decode_clk <= (counter == 2'b01) ? 1'b1 : 1'b0;
      exec_clk   <= (counter == 2'b10) ? 1'b1 : 1'b0;
      wrbk_clk   <= (counter == 2'b11) ? 1'b1 : 1'b0;
      counter <= counter + 1;
    end
  end
endmodule;
