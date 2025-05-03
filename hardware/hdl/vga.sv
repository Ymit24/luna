module vga_sync_driver (
  input  logic clk, rst,
  output logic hsync, vsync,
  output logic display,
);

localparam H_VA = 640;
localparam H_FP = 16;
localparam H_SP = 96;
localparam H_BP = 48;

localparam V_VA = 480;
localparam V_FP = 10;
localparam V_SP = 2;
localparam V_BP = 33;

logic [15:0] x, y;

local_counter #(WIDTH=800, HEIGHT=640) pixel_counter(clk, rst, x, y);

assign display = hsync & vsync;

always_ff @(posedge clk) begin
  if (x > H_SP) begin
    hsync <= 1;
  end else begin
    hsync <= 0;
  end

  if (y > V_SP) begin
    vsync <= 1;
  end else begin
    vsync <= 0;
  end

end

endmodule

module local_counter #(
  parameter WIDTH = 640,
  parameter HEIGHT = 480
) (
  input  logic clk, rst,
  output logic [$clog2(WIDTH):0] x,
  output logic [$clog2(HEIGHT):0] y,
);

always_ff @(posedge clk) begin
  if (rst) begin
    x <= 0;
    y <= 0;
  end else begin
    if (x < WIDTH) begin
      x <= x + 1;
    end else begin
      x <= 0;
      if (y < HEIGHT) begin
        y <= y + 1;
      end else begin
        y <= 0;
      end
    end
  end
end
  
endmodule
