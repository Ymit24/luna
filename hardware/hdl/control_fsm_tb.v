`timescale 1ns/1ns
`include "control_fsm.v"

module control_fsm_tb;
    reg clk = 0;
    reg rst = 0;
    wire fetch_clk, decode_clk, exec_clk, wrbk_clk;

    control_fsm uut(
        .clk(clk),
        .rst(rst),
        .fetch_clk(fetch_clk),
        .decode_clk(decode_clk),
        .exec_clk(exec_clk),
        .wrbk_clk(wrbk_clk)
    );

    always begin
        clk = ~clk;
        #5;
    end

    initial begin
        $dumpfile("control_fsm_out.vcd");
        $dumpvars(0, control_fsm_tb);

        rst = 1;
        #20;
        rst = 0;
        #20;

        #500;

        $finish;
    end
endmodule;

