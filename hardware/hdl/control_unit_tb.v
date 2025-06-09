`timescale 1ns/1ns
`include "control_unit.v"
`include "control_fsm.v"

module control_unit_tb;
    reg clk = 0;
    reg rst = 0;
    reg [15:0] instr;
    reg [15:0] reg_a_in, reg_d_in, reg_m_in;
    reg [15:0] alu_result;

    wire decode_clk, wrbk_clk;

    wire [15:0] addr_out;
    wire instr_type;
    wire reg_a_en, reg_d_en, reg_m_en;
    wire set_pc;
    wire [15:0] x, y;
    wire [6:0] opcode;

    control_fsm fsm(
      .clk(clk),
      .rst(rst),
      .decode_clk(decode_clk),
      .wrbk_clk(wrbk_clk)
    );

    // Instantiate the UUT
    control_unit uut (
        .decode_clk(decode_clk),
        .wrbk_clk(wrbk_clk),
        .rst(rst),
        .instr(instr),
        .alu_result(alu_result),
        .reg_a_in(reg_a_in),
        .reg_d_in(reg_d_in),
        .reg_m_in(reg_m_in),
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

        alu_result = 16'h0000;

        reg_a_in = 16'd2;
        reg_d_in = 16'd3;
        reg_m_in = 16'd4;

        rst = 1;
        #40;
        rst = 0;
        #40;

        instr = 16'b0000000000000000;
        #80;
        instr = 16'b0000000000000011;
        #80;

        alu_result = -16'd5;

        instr = 16'b1110_000010_110_000; // D+A
        #80;

        alu_result = 16'd2;
        #80;

        alu_result = 16'd0;

        instr = 16'b1111_000010_110_000; // D+M
        #80;

        instr = 16'b1110_010101_000_111; // 0; JMP
        #80;

        $display("done");
        $finish;
    end

endmodule;
