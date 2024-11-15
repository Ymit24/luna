`timescale 1ns / 1ns
`include "top.v"

module top_tb;
    reg clk;

    top top(.clk(clk));

    always
    begin
        clk = ~clk;
        #5;
    end

    initial begin
        $dumpfile("top.vcd");
        $dumpvars(0, top_tb);
        clk = 0;
        #500
        $finish;
    end
endmodule;
