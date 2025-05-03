module memory(
    input clk,
    input rst,
    input reg_a_en,
    input reg_d_en,
    input reg_m_en,
    input [15:0] data_in,
    output [15:0] reg_a_out,
    output [15:0] reg_d_out,
    output [15:0] reg_m_out,

    output [15:0] leds
);

reg [15:0] reg_a;
reg [15:0] reg_d;

assign reg_a_out = reg_a;
assign reg_d_out = reg_d;
assign reg_m_out = mem[reg_a];

// Declare memory array with block RAM style
//(* ram_style = "block" *)
reg [15:0] mem [0:8191];

assign leds = mem[10];

always@(posedge clk) begin
  if (rst) begin
    reg_a <= 0;
    reg_d <= 0;
  end else begin
    if (reg_a_en) reg_a <= data_in;
    if (reg_d_en) reg_d <= data_in;
    if (reg_m_en) mem[reg_a] <= data_in;
  end
end

endmodule;
