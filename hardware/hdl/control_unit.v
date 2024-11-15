module control_unit(
    input clk,
    input [15:0] instr,
    input [15:0] reg_a_in, reg_d_in, reg_m_in,
    input is_negative, is_zero,
    input [15:0] addr_in,
    output [15:0] addr_out,
    output instr_type,
    output reg_a_en, reg_d_en, reg_m_en,
    output set_pc,
    output reg [15:0] x, y,
    output [1:0] opcode
);
    wire [3:0] reg_selects;
    wire [2:0] dest_selects;
    wire [2:0] flags;
    wire [2:0] jump_condition;

    // instruction decoding
    assign jump_condition = instr[2:0];
    assign opcode = instr[4:3];
    assign flags = instr[7:5];
    assign reg_selects = instr[11:8];
    assign dest_selects = instr[14:12];
    assign instr_type = instr[15];
    assign addr_out = { 0, instr[14:0] };

    assign reg_m_en = dest_selects[0];
    assign reg_d_en = dest_selects[1];
    assign reg_a_en = dest_selects[2] | instr_type;

    assign set_pc = ~instr_type & (
        (jump_condition == 3'h0) & 0 |
        (jump_condition == 3'h1) & ~is_negative |
        (jump_condition == 3'h2) & is_zero |
        (jump_condition == 3'h3) & (~is_negative | is_zero) |
        (jump_condition == 3'h4) & (is_negative) |
        (jump_condition == 3'h5) & (~is_zero) |
        (jump_condition == 3'h6) & (is_zero | is_negative) |
        (jump_condition == 3'h7) & 1
    );

    always@(*)
    begin
        case (reg_selects[1:0])
             4'h0: x = addr_in;
             4'h1: x = reg_d_in;
             4'h2: x = reg_m_in;
             4'h3: x = 1;
        endcase
        case (reg_selects[3:2])
             4'h0: y = addr_in;
             4'h1: y = reg_d_in;
             4'h2: y = reg_m_in;
             4'h3: y = 1;
        endcase
    end
endmodule;
