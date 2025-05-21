`timescale 1ns/1ns
`include "alu.v"

module alu_tb;
    reg [15:0] x, y;
    reg zero_x, zero_y, negate_output;
    reg clk = 0;
    reg rst = 0;
    reg [1:0] opcode;
    wire [15:0] output_result;
    wire is_zero, is_negative;

    luna_alu uut(
        .x(x),
        .y(y),
        .zero_x(zero_x),
        .zero_y(zero_y),
        .negate_output(negate_output),
        .is_zero(is_zero),
        .is_negative(is_negative),
        .clk(clk),
        .rst(rst),
        .opcode(opcode),
        .output_result(output_result)
    );

    always begin
        clk = ~clk;
        #5;
    end

    initial begin
        $dumpfile("alu_out.vcd");
        $dumpvars(0, luna_alu_tb);

        rst = 1;
        #40;
        rst = 0;
        #10;

        x = 16'h0002;
        y = 16'h0005;

        zero_x = 0;
        zero_y = 0;
        negate_output = 0;
        opcode = 2'h2;

        #30;

        $display("X= %d, Y=%d, o=%d", x, y, output_result);
        x = 16'd10;
        #40;

        $display("X= %d, Y=%d, o=%d", x, y, output_result);
        zero_x=1;

        #50;
        $display("X= %d, Y=%d, o=%d", x, y, output_result);

        y = 16'h0000;
        #50;
        x = 16'h0000;
        #50;
        negate_output = 1;
        #50;

        $finish;
    end
endmodule;
