`timescale 1ns/1ns
`include "alu.v"

module alu_tb;
    reg clk = 0;
    reg rst = 0;
    reg [6:0] opcode;
    reg [15:0] x, y;
    wire [15:0] result;

    alu uut(
        .x(x),
        .y(y),
        .clk(clk),
        .rst(rst),
        .opcode(opcode),
        .result(result)
    );

    always begin
        clk = ~clk;
        #5;
    end

    initial begin
        $dumpfile("alu_out.vcd");
        $dumpvars(0, alu_tb);

        rst = 1;
        #40;
        rst = 0;
        #10;

        x = 16'h0002;
        y = 16'h0005;

        opcode = 6'b101010; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b111111; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b111010; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b001100; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b110000; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b001101; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b110001; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b001111; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b110011; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b011111; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b110111; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b001110; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b110010; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b000010; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b010011; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b000111; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b000000; #30; $display("X= %d, Y=%d, o=%d", x, y, result);
        opcode = 6'b010101; #30; $display("X= %d, Y=%d, o=%d", x, y, result);

        #50;

        $finish;
    end
endmodule;
