module control_unit_fsm(
  input clk,
  input rst,
  output reg [1:0] state
);

always@(posedge clk) begin
  if (rst) begin
    state <= STATE_FETCH;
  end else begin
    state <= state + 1;
  end
end

endmodule
