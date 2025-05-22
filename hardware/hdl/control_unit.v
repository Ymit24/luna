`include "control_unit_fsm.v"

module control_unit(
    input             clk,
    input             rst,
    input [15:0]      instr,
    input [15:0]      reg_a_in, reg_d_in, reg_m_in,
    input             is_negative, is_zero,

    output reg [15:0] addr_out,
    output reg        instr_type,
    output reg        reg_a_en, reg_d_en, reg_m_en,
    output reg        set_pc,
    output reg [15:0] x, y,
    output reg [1:0]  opcode,
    output reg        negate_output, zero_x, zero_y
);
  localparam STATE_FETCH  = 2'd0;
  localparam STATE_DECODE  = 2'd1;
  localparam STATE_EXECUTE  = 2'd2;
  localparam STATE_STORE  = 2'd3;

  wire [2:0] dest_selects   = instr[14:12];
  wire [3:0] reg_selects    = instr[11:8];
  wire [2:0] flags          = instr[7:5];
  wire [2:0] jump_condition = instr[2:0];

  wire [1:0] state;

  control_unit_fsm fsm(
    .clk(clk),
    .rst(rst),
    .state(state)
  );

  // instruction decoding
  always@(posedge clk) begin
    if (rst) begin
      reg_a_en <= 1'd0;
      reg_d_en <= 1'd0;
      reg_m_en <= 1'd0;

      instr_type <= 1'd0;
      opcode <= 2'd0;
      addr_out <= 16'd0;
      negate_output <= 1'd0;

      zero_y <= 0;
      zero_x <= 0;
    end else begin
      if (state == STATE_DECODE) begin
        reg_a_en <= dest_selects[2] | instr[15];
        reg_d_en <= dest_selects[1];
        reg_m_en <= dest_selects[0];

        instr_type <= instr[15];
        opcode <= instr[4:3];
        addr_out <= { 1'b0, instr[14:0] };
        negate_output <= flags[0];
        zero_y <= flags[1];
        zero_x <= flags[2];
      end
    end
  end

  // Set X output
  always @ (posedge clk) begin
    if (rst) begin
      x <= 16'h0;
    end else begin
      if (state == STATE_DECODE) begin
        case (reg_selects[1:0])
          2'h0: x <= reg_a_in;
          2'h1: x <= reg_d_in;
          2'h2: x <= reg_m_in;
          default: x <= 16'h0001;
        endcase
      end
    end
  end

  // Set Y output
  always @ (posedge clk) begin
    if (rst) begin
        y <= 16'h0;
    end else begin
      if (state == STATE_DECODE) begin
        case (reg_selects[3:2])
          2'h0: y <= reg_a_in;
          2'h1: y <= reg_d_in;
          2'h2: y <= reg_m_in;
          default: y <= 16'h0001;
        endcase
      end
    end
  end

  // Set should jump
  always @ (posedge clk) begin
    $display("set should jump");
    if (rst == 1'b1 || instr_type == 1'b1 || state != STATE_FETCH) begin
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
