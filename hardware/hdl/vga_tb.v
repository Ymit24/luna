`timescale 1ns/1ps
`include "vga.v"

module vga_tb;
  // Clock and reset
  reg clk = 0;
  reg rst = 1;

  // VGA outputs
  wire [3:0] red;
  wire [3:0] green;
  wire [3:0] blue;
  wire hsync;
  wire vsync;

  // BRAM interface
  reg [15:0] bram_doutb = 16'h0000;
  wire [15:0] bram_addrb;

  // Instantiate DUT
  vga uut (
    .clk(clk),
    .rst(rst),
    .red(red),
    .green(green),
    .blue(blue),
    .hsync(hsync),
    .vsync(vsync),
    .bram_doutb(bram_doutb),
    .bram_addrb(bram_addrb)
  );

  // 25 MHz pixel clock generator (T = 40 ns)
  always #20 clk = ~clk;

  // Reset pulse
  initial begin
    #100;
    rst = 0;
  end

  // Simple BRAM read model: for visibility, echo the address as data
  always @(posedge clk) begin
    if (rst)
      bram_doutb <= 16'h0000;
    else
      bram_doutb <= bram_addrb;
  end

  // Dump waves
  initial begin
    $dumpfile("vga_tb.vcd");
    $dumpvars(0, vga_tb);
  end

  // Run for a finite number of cycles and finish
  initial begin
    // simulate ~100,000 pixel clocks (~4 ms)
    # (100_000 * 160);
    $display("Simulation complete at time %t", $time);
    $finish;
  end

endmodule
