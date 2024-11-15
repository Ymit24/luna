module memory_mapped_io(
    input clk,
    input [14:0] addr,
    input write_enable,
    input [15:0] data_in,
    input [15:0] buttons,
    output reg [15:0] leds,
    output reg [15:0] data_out
);

always @(*) begin
    if (addr < 16) begin
        data_out = buttons[addr];
    end else if (addr < 32) begin
        if (write_enable)
            leds[addr-16] = data_in[0];
        data_out = 0;
    end
end

endmodule;
