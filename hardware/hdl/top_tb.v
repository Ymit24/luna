`timescale 1ns / 1ns
`include "top.v"

module top_tb;
    reg clk=0, rst=0;
    wire hsync, vsync;
    wire [15:0] leds;
    wire [3:0] red, green, blue;

    top top(
        .raw_clk(clk),
        .rst(rst),
        .leds(leds),
        .hsync(hsync),
        .vsync(vsync),
        .red(red),
        .green(green),
        .blue(blue)
    );

    always
    begin
        clk = ~clk;
        #5;
    end

    initial begin
        $dumpfile("top.vcd");
        $dumpvars(0, top_tb);
        $display("test 1");
        rst = 1;
        #50
        rst = 0;
        #8000
        $finish;
    end
endmodule;
