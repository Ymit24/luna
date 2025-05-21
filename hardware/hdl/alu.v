`timescale 1ns/1ns
module alu(
    input clk,
    input rst,
    input [15:0] x, y,
    input zero_x, zero_y, negate_output,
    input [1:0] opcode,
    output wire [15:0] output_result,
    output is_zero, is_negative
);

wire [15:0] effective_x, effective_y;

reg [15:0] internal_output;

assign effective_x = (zero_x) ? 16'h0000 : x;
assign effective_y = (zero_y) ? 16'h0000 : y;

assign output_result = (negate_output) ? ~internal_output : internal_output;
assign is_zero = (internal_output == 16'h0000) ? 1 : 0;
assign is_negative = output_result[15];

localparam OPCODE_OR  = 2'h0;
localparam OPCODE_AND = 2'h1;
localparam OPCODE_ADD = 2'h2;
localparam OPCODE_SUB = 2'h3;

always@(posedge clk or posedge rst)
begin
    if (rst) begin
      internal_output <= 0;
    end else begin
      case (opcode)
        OPCODE_OR:  internal_output <= effective_x | effective_y;
        OPCODE_AND: internal_output <= effective_x & effective_y;
        OPCODE_ADD: internal_output <= effective_x + effective_y;
        OPCODE_SUB: internal_output <= effective_x - effective_y;
        default: internal_output <= 16'h00;
      endcase
    end
end

endmodule;
