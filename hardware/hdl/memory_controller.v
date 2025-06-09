module memory_controller(
    input             clk,
    input             wrbk_clk,
    input             rst,
    input             reg_a_en,
    input             reg_d_en,
    input             reg_m_en,
    input      [15:0] data_in,

    input [15:0] program_counter,

    output     [15:0] reg_a_out,
    output     [15:0] reg_d_out,
    output reg [15:0] reg_m_out,

    // BRAM
    input      [15:0] bram_douta,
    output reg        bram_wea,
    output reg [15:0] bram_dina,
    output reg [15:0] bram_addra
);

reg [15:0] reg_a = 0;
reg [15:0] reg_d = 0;

assign reg_a_out = reg_a;
assign reg_d_out = reg_d;

always@(posedge wrbk_clk) begin
    if (rst) begin
        reg_a <= 0;
        reg_d <= 0;
        bram_wea <= 1'b0;
        bram_dina <= 1'b0;
    end else begin
        if (reg_a_en) begin
            reg_a <= data_in;
            bram_addra <= data_in; // Set memory addr
        end
        if (reg_d_en) reg_d <= data_in;

        bram_wea <= reg_m_en;
        bram_dina <= data_in;
    end
end

always@(posedge clk) begin
    if (rst) begin
        reg_m_out <= 16'h0000;
    end else begin
      if (bram_addra == 16'h0001) begin
        reg_m_out <= program_counter;
      end else begin
        reg_m_out <= bram_douta;
      end
    end
end

endmodule;
