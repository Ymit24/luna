`timescale 1ns/1ns
`include "counter.v"

module counter_tb;
    reg clk, write_en;
    reg [15:0] data_in;
    wire [15:0] data_out;

    counter uut(
        .clk(clk),
        .write_en(write_en),
        .data_in(data_in),
        .data_out(data_out)
    );

    always begin
        clk = ~clk;
        #5;
    end

    initial
    begin
        $dumpfile("counter.vcd");
        $dumpvars(0, counter_tb);

        clk = 0;
        data_in = 0;
        write_en = 0;
        #80;

        data_in = 15'h20;
        #20;
        write_en = 1;
        #50;
        write_en = 0;
        #200
        data_in = 15'h02;
        write_en = 1;
        #20;
        write_en = 0;
        #200;

        $finish;
    end

endmodule;
