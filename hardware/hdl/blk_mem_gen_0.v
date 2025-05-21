module blk_mem_gen_0 (
  input wire clka,
  input wire ena,
  input wire wea,
  input wire [15:0] addra,
  input wire [15:0] dina,
  output reg [15:0] douta,

  input wire clkb,
  input wire enb,
  input wire web,
  input wire [15:0] addrb,
  input wire [15:0] dinb,
  output reg [15:0] doutb
);

reg [15:0] memory [64];

integer i = 0;
initial begin
  for(i = 0; i < 64; i=i+1)
    memory[i] <= 0;
end

always@(posedge clka) begin
  if (ena == 1'b1) begin
    if (wea == 1'b1) begin
      memory[addra] <= dina;
    end
    douta <= memory[addra];
  end
end

always@(posedge clkb) begin
  if (enb == 1'b1) begin
    if (web == 1'b1) begin
      memory[addrb] <= dinb;
    end
    doutb <= memory[addrb];
  end
end

endmodule
