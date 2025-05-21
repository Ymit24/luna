module vga(
    input wire clk,
    input wire rst,
    output wire [3:0] red,
    output wire [3:0] green,
    output wire [3:0] blue,
    output wire hsync,
    output wire vsync,
    
    input wire [15:0] bram_doutb,
    output reg [15:0] bram_addrb
);

// VGA 640x480 @ 60Hz timing parameters
localparam H_VISIBLE    = 640;
localparam H_FRONT_PORCH = 16;
localparam H_SYNC_START = H_VISIBLE + H_FRONT_PORCH;       // 656
localparam H_SYNC_END   = H_SYNC_START + 96;               // 752
localparam H_TOTAL      = H_SYNC_END + 48;                 // 800

localparam V_VISIBLE    = 480;
localparam V_FRONT_PORCH = 10;
localparam V_SYNC_START = V_VISIBLE + V_FRONT_PORCH;       // 490
localparam V_SYNC_END   = V_SYNC_START + 2;                // 492
localparam V_TOTAL      = V_SYNC_END + 33;                 // 525

// Registers for counters and clock divider
reg [9:0] h_counter = 10'd0;
reg [9:0] v_counter = 10'd0;

// Clock divider and counter logic with synchronous reset
always @(posedge clk) begin
    bram_addrb <= 16'd1;
    if (rst) begin
        h_counter <= 10'd0;
        v_counter <= 10'd0;
    end else begin
        if (h_counter == H_TOTAL - 1) begin
            h_counter <= 10'd0;
            if (v_counter == V_TOTAL - 1) begin
                v_counter <= 10'd0;
            end else begin
                v_counter <= v_counter + 1;
            end
        end else begin
            h_counter <= h_counter + 1;
        end
    end
end

// Generate sync signals (active low)
assign hsync = (h_counter >= H_SYNC_START && h_counter < H_SYNC_END) ? 1'b0 : 1'b1;
assign vsync = (v_counter >= V_SYNC_START && v_counter < V_SYNC_END) ? 1'b0 : 1'b1;

// Generate RGB signals (white during visible area, off during blanking)
assign red   = (h_counter < H_VISIBLE && v_counter < V_VISIBLE) ? bram_doutb[3:0] : 4'b0000;
assign green = (h_counter < H_VISIBLE && v_counter < V_VISIBLE) ? 4'b0110 : 4'b0000;
assign blue  = (h_counter < H_VISIBLE && v_counter < V_VISIBLE) ? 4'b1111 : 4'b0000;

endmodule

