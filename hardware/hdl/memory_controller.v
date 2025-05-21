module memory_controller(
    input             clk,
    input             rst,
    input             reg_a_en,
    input             reg_d_en,
    input             reg_m_en,
    input      [15:0] data_in,
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

always@(posedge clk) begin
    if (rst) begin
        reg_a <= 0;
        reg_d <= 0;
    end else begin
        if (reg_a_en) begin
            reg_a <= data_in;
            bram_addra <= data_in; // Set memory addr
        end
        if (reg_d_en) reg_d <= data_in;
    end
end

always@(posedge clk) begin
    if (!rst) begin
        bram_wea <= reg_m_en;
        bram_dina <= data_in;
        reg_m_out <= bram_douta;
    end
end

endmodule;

// module vga(
//     input clk,
//     input rst,
//     output hsync,
//     output vsync,
//     output [11:0] rgb,

//     output bram_addrb,
//     input bram_doutb
// );

// wire [9:0] x, y;
// wire y_en;

// horiz_counter hc(
//     .clk(clk),
//     .rst(rst),
//     .value(x),
//     .overflow(y_en)
// );

// vert_counter vc(
//     .clk(clk),
//     .rst(rst),
//     .value(y),
//     .en(y_en)
// );

// //pixel_gen pg(
// //    .clk(clk),
// //    .rst(rst),
// //    .x(x),
// //    .y(y),
// //    .bram_addrb(bram_addrb),
// //    .bram_doutb(bram_doutb),
// //    .rgb(rgb)
// //);

// assign hsync = (x >= 656 && x < 752) ? 1'h0 : 1'h1;
// assign vsync = (y >= 490 && y < 492) ? 1'h0 : 1'h1;

// assign rgb = (x < 640 && y < 480) ? 12'hffffff : 12'h000000;

// endmodule

// module pixel_gen(
//     input clk,
//     input rst,
//     input [9:0] x,
//     input [9:0] y,
//     input [15:0] bram_doutb,
//     output reg [15:0] bram_addrb,
//     output reg [11:0] rgb
// );

// localparam WIDTH = 80;
// localparam HEIGHT = 60;

// // 640x480 => 320x240 => 160x120 => 80x60

// always @(posedge clk) begin
//     if (rst) begin
//         bram_addrb <= 0;
//         rgb <= 0;
//     end else begin
//         bram_addrb <= (x/8) + (y/8) * 10'd60;
//         if (x < 10'd640 && y < 10'd480) begin
//             if (y < 50) begin
//                 rgb <= bram_doutb[12:0];
//             end else begin
//                 rgb <= x;
//             end
//         end else begin
//             rgb <= 0;
//         end
//     end
// end

// endmodule

// module horiz_counter(
//     input clk,
//     input rst,
//     output reg [9:0] value,
//     output reg overflow
// );
//     always@(posedge clk) begin
//         if (rst) begin
//             value <= 10'h0;
//             overflow <= 1'h0;
//         end else begin
//             if (value < 800) begin
//                 value <= value + 1;
//                 overflow <= 1'h0;
//             end else begin
//                 value <= 0;
//                 overflow <= 1'h1;
//             end
//         end
//     end
// endmodule

// module vert_counter(
//     input clk,
//     input rst,
//     input en,
//     output reg [9:0] value
// );
//     always@(posedge clk) begin
//         if (rst) begin
//             value <= 10'h0;
//         end else begin
//             if (value < 525 and en) begin
//                 value <= value + 1;
//             end else begin
//                 value <= 0;
//             end
//         end
//     end
// endmodule
