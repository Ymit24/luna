module control_fsm(
  input clk,
  input rst,

  output reg fetch_clk,
  output reg decode_clk,
  output reg exec_clk,
  output reg wrbk_clk,
  output reg jmp_clk
);
  reg [2:0] counter = 0;

  always @ (posedge clk) begin
    if (rst) begin
      counter <= 0;
      fetch_clk <= 0;
      decode_clk <= 0;
      exec_clk <= 0;
      wrbk_clk <= 0;
      jmp_clk <= 0;
    end else begin
      fetch_clk  <= (counter == 3'b000) ? 1'b1 : 1'b0;
      decode_clk <= (counter == 3'b001) ? 1'b1 : 1'b0;
      exec_clk   <= (counter == 3'b010) ? 1'b1 : 1'b0;
      wrbk_clk   <= (counter == 3'b011) ? 1'b1 : 1'b0;
      jmp_clk    <= (counter == 3'b100) ? 1'b1 : 1'b0;
      if (counter == 3'b100) begin
        counter <= 0;
      end else begin
        counter <= counter + 1;
      end
    end
  end
endmodule;
