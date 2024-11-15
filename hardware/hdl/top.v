`include "counter.v"
`include "rom.v"
`include "memory.v"
`include "alu.v"
`include "control_unit.v"
`include "memory_mapped_io.v"

module top(
    input clk,
    output [15:0] leds
);

    wire [15:0] alu_out, addr_const;
    wire instr_type, set_pc;
    wire [15:0] x, y, instr, reg_a, reg_d, reg_m;
    wire [1:0] opcode;
    wire zero_x, zero_y, negate_output, is_negative, is_zero;
    wire dest_a, dest_d, dest_m;

    wire [15:0] pc;

    alu alu(
        .clk(clk),
        .x(x),
        .y(y),
        .zero_x(zero_x),
        .zero_y(zero_y),
        .negate_output(negate_output),
        .opcode(opcode),
        .output_result(alu_out),
        .is_zero(is_zero),
        .is_negative(is_negative)
    );

    memory memory(
        .clk(clk),
        .reg_a_en(dest_a),
        .reg_d_en(dest_d),
        .reg_m_en(dest_m),
        .data_in(
            (instr_type) ? addr_const : alu_out
        ),
        .reg_a_out(reg_a),
        .reg_d_out(reg_d),
        .reg_m_out(reg_m),
        .leds(leds)
    );

    control_unit control_unit(
        .instr(instr),
        .reg_a_in(reg_a),
        .reg_d_in(reg_d),
        .reg_m_in(reg_m),
        .is_negative(is_negative),
        .is_zero(is_zero),
        .addr_out(addr_const),
        .instr_type(instr_type),
        .reg_a_en(dest_a),
        .reg_d_en(dest_d),
        .reg_m_en(dest_m),
        .set_pc(set_pc),
        .x(x),
        .y(y),
        .opcode(opcode),
        .negate_output(negate_output),
        .zero_x(zero_x),
        .zero_y(zero_y)
    );

    counter counter(
        .clk(clk),
        .write_en(set_pc),
        .data_in(
            (instr_type) ? addr_const : alu_out
        ),
        .data_out(pc)
    );

    rom rom (
        .address(pc[7:0]),
        .data(instr)
    );

endmodule;
