module control_unit(
    input             decode_clk,
    input             wrbk_clk,
    input             rst,
    input [15:0]      instr,
    input [15:0]      reg_a_in, reg_d_in, reg_m_in,
    input [15:0]      alu_result,

    output reg [15:0] addr_out,
    output reg        instr_type,
    output reg        reg_a_en, reg_d_en, reg_m_en,
    output reg        set_pc,
    output reg [15:0] x, y,
    output reg [5:0]  opcode
);
    wire       a_or_m = instr[12];
    wire [2:0] dest_selects = instr[5:3];
    wire [2:0] jump_condition = instr[2:0];

    // Instruction decoding
    always @ (posedge decode_clk) begin
      instr_type <= instr[15];
      opcode <= instr[11:6];
      if (instr[15] == 1'b0) begin
          addr_out <= { 1'b0, instr[14:0] };
      end

      reg_m_en <= dest_selects[0];
      reg_d_en <= dest_selects[1];
      reg_a_en <= dest_selects[2] | ~instr[15];
    end

    // Set X output
    always @ (posedge decode_clk) begin
        if (rst) begin
            x <= 16'h0;
        end else begin
            x <= reg_d_in;
        end
    end

    // Set Y output
    always @ (posedge decode_clk) begin
        if (rst) begin
            y <= 16'h0;
        end else begin
            if (a_or_m == 1'b0) begin
                y <= reg_a_in;
            end else begin
                y <= reg_m_in;
            end
        end
    end

    wire is_negative = alu_result[15];
    wire is_zero = (alu_result == 16'h0000 ? 1'b1 : 1'b0);

    // Set should jump
    always @ (posedge wrbk_clk) begin
        $display("set should jump");
        if (rst == 1'b1 || instr[15] == 1'b0) begin
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

// timing
// fetch high:
//  rom sampled to set instr
// decode high:
//  instr sampled and control signals set
// execute high:
//  alu activates and computes result
// write back high:
//  memory write(s)
//  determine if we should jump
// jmp:
//  incr pc or jump
