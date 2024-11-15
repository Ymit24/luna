module memory(
    input clk,
    input reg_a_en,
    input reg_d_en,
    input reg_m_en,
    input [15:0] data_in,
    output [15:0] reg_a_out,
    output [15:0] reg_d_out,
    output [15:0] reg_m_out
);

reg [15:0] reg_a;
reg [15:0] reg_d;

assign reg_a_out = reg_a;
assign reg_d_out = reg_d;
assign reg_m_out = mem[reg_a];

// Declare memory array with block RAM style
(* ram_style = "block" *) reg [15:0] mem [0:8191];

always@(posedge clk) begin
    if (reg_a_en)
        reg_a <= data_in;
    else
        reg_a <= reg_a;

    if (reg_d_en)
        reg_d <= data_in;
    else
        reg_d <= reg_d;

    if (reg_m_en)
        mem[reg_a] <= data_in;
    else
        mem[reg_a] <= mem[reg_a];
end

endmodule;
