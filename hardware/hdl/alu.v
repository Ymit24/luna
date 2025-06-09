`timescale 1ns/1ns
module alu(
    input  clk,
    input  rst,
    input  [5:0] opcode,
    input  [15:0] x, y, // x = D, y = A or M depending
    output reg [15:0] result
);

always@(posedge clk)
begin
    if (rst) begin
      result <= 16'h0000;
    end else begin
      case (opcode)
        6'b101010: result <= 16'h0000; // 0
        6'b111111: result <= 16'h0001; // 1
        6'b111010: result <= -16'd0001; // -1
        6'b001100: result <= x; // D
        6'b110000: result <= y; // AM
        6'b001101: result <= ~x; // !D, logical not
        6'b110001: result <= ~y; // !AM, logical not
        6'b001111: result <= -x; // arithmatic negative
        6'b110011: result <= -y; // arithmatic negative
        6'b011111: result <= x+16'h0001; // increment D
        6'b110111: result <= y+16'h0001; // increment AM
        6'b001110: result <= x-16'h0001; // decrement D
        6'b110010: result <= y-16'h0001; // decrement AM
        6'b000010: result <= x+y; // D+AM
        6'b010011: result <= x-y; // D-AM
        6'b000111: result <= y-x; // AM-D
        6'b000000: result <= x&y; // D&AM
        6'b010101: result <= x|y; // D|AM
        default: result <= 16'h0000;
      endcase
    end
end

endmodule;
