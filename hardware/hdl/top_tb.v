`timescale 1ns / 1ns
`include "top.v"

module top_tb;
    reg clk;
    wire [15:0] leds;

    top top(
        .clk(clk),
        .leds(leds)
    );

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
