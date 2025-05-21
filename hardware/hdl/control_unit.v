module control_unit(
    input             clk,
    input             rst,
    input [15:0]      instr,
    input [15:0]      reg_a_in, reg_d_in, reg_m_in,
    input             is_negative, is_zero,

    output [15:0]     addr_out,
    output            instr_type,
    output            reg_a_en, reg_d_en, reg_m_en,
    output reg        set_pc,
    output reg [15:0] x, y,
    output [1:0]      opcode,
    output            negate_output, zero_x, zero_y
);
    wire [2:0] dest_selects   = instr[14:12];
    wire [3:0] reg_selects    = instr[11:8];
    wire [2:0] flags          = instr[7:5];
    wire [2:0] jump_condition = instr[2:0];

    // instruction decoding
    assign instr_type = instr[15];
    assign opcode = instr[4:3];
    assign addr_out = { 1'b0, instr[14:0] };

    assign reg_m_en = dest_selects[0];
    assign reg_d_en = dest_selects[1];
    assign reg_a_en = dest_selects[2] | instr_type;

    assign negate_output = flags[0];
    assign zero_y = flags[1];
    assign zero_x = flags[2];

/*

program

@2  => 1 000 0000 000 000 010
D=A => 0 010 0000 000 000 000
@3  => 1 000 0000 000 000 011
M=2 => 0 100 0000 000 000 000
@4  => 1 000 0000 000 000 100
M=3 => 0 100 0000 000 000 000

*/
    // Set X output
    always @ (posedge clk) begin
        if (rst) begin
            x <= 16'h0;
        end else begin
            case (reg_selects[1:0])
                2'h0: x <= reg_a_in;
                2'h1: x <= reg_d_in;
                2'h2: x <= reg_m_in;
                default: x <= 16'h0001;
            endcase
        end
    end

    // Set Y output
    always @ (posedge clk) begin
        if (rst) begin
            y <= 16'h0;
        end else begin
            case (reg_selects[3:2])
                2'h0: y <= reg_a_in;
                2'h1: y <= reg_d_in;
                2'h2: y <= reg_m_in;
                default: y <= 16'h0001;
            endcase
        end
    end

    initial begin
        $display("test 2");
    end

    // Set should jump
    always @ (posedge clk) begin
        $display("set should jump");
        if (rst == 1'b1 || instr_type == 1'b1) begin
            set_pc <= 1'h0;
        end else begin
            case(jump_condition)
                3'h0: set_pc <= 1'b0;
                3'h1: set_pc <= ~is_negative;
                3'h2: set_pc <= is_zero;
                3'h3: set_pc <= (~is_negative) | is_zero;
                3'h4: set_pc <= is_negative;
                3'h5: set_pc <= ~is_zero;
                3'h6: set_pc <= is_zero | is_negative;
                3'h7: set_pc <= 1'b1;
                default: set_pc <= 1'b0;
            endcase
        end
    end
endmodule;
