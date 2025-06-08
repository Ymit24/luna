module rom(
    input clk,
    input rst,
    input wire [7:0] address,
    output reg [15:0] data
);
    reg [15:0] rom_memory [0:255];

    always @ (posedge clk) begin
      if (rst) begin
        data <= 16'h0000;
      end else begin
        data <= rom_memory[address];
      end
    end

    initial $readmemh("program.txt", rom_memory);
endmodule;
