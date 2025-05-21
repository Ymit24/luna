module led_driver(
    input clk,
    input rst,

    output reg led,

    // BRAM 
    input [15:0] bram_doutb,
    output reg [15:0] bram_addrb
);

reg [3:0] addr;

always@(posedge clk) begin
    if (rst) begin
        bram_addrb <= 16'h0000;
        led <= 0;
        addr <= 0;
    end else begin
        bram_addrb <= addr;
        addr <= addr + 1;
        led <= bram_doutb[0];
    end
end

endmodule
