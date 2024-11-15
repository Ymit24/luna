`timescale 1ns/1ns
`include "control_unit.v"

module control_unit_tb;

    reg clk;
    reg [15:0] instr;
    reg [15:0] reg_a_in, reg_d_in, reg_m_in;
    reg is_negative, is_zero;
    reg [15:0] addr_in;

    wire [15:0] addr_out;
    wire instr_type;
    wire reg_a_en, reg_d_en, reg_m_en;
    wire set_pc;
    wire [15:0] x, y;
    wire [1:0] opcode;

    // Instantiate the UUT
    control_unit uut (
        .clk(clk),
        .instr(instr),
        .reg_a_in(reg_a_in),
        .reg_d_in(reg_d_in),
        .reg_m_in(reg_m_in),
        .is_negative(is_negative),
        .is_zero(is_zero),
        .addr_in(addr_in),
        .addr_out(addr_out),
        .instr_type(instr_type),
        .reg_a_en(reg_a_en),
        .reg_d_en(reg_d_en),
        .reg_m_en(reg_m_en),
        .set_pc(set_pc),
        .x(x),
        .y(y),
        .opcode(opcode)
    );

    always begin
        clk = ~clk;
        #5;
    end

    initial begin
        $dumpfile("control_unit.vcd");
        $dumpvars(0, control_unit_tb);
        $display("Starting control unit test");

        clk = 0;
        instr = 16'b0000000000000000;
        #10;
        instr = 16'b1000000000000011;
        #10;

        $display("done");
        $finish;
    end

endmodule;
