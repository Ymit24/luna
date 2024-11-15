`timescale 1ns / 1ps
`include "memory.v"

module memory_tb;

    // Testbench signals
    reg clk;
    reg reg_a_en;
    reg reg_d_en;
    reg reg_m_en;
    reg [15:0] data_in;
    wire [15:0] reg_a_out;
    wire [15:0] reg_d_out;
    wire [15:0] reg_m_out;

    // Instantiate the memory module
    memory uut (
        .clk(clk),
        .reg_a_en(reg_a_en),
        .reg_d_en(reg_d_en),
        .reg_m_en(reg_m_en),
        .data_in(data_in),
        .reg_a_out(reg_a_out),
        .reg_d_out(reg_d_out),
        .reg_m_out(reg_m_out)
    );

    // Clock generation
    always begin
        clk = ~clk;
        #5;
    end
    // Test sequence
    initial begin
        $dumpfile("memory.vcd");
        $dumpvars(0, memory_tb);
        $display("Starting");
        // Initialize inputs
        clk = 0;
        reg_a_en = 0;
        reg_d_en = 0;
        reg_m_en = 0;
        data_in = 0;

        // Display initial values
        $display("Starting test...");

        // Test write to reg_a
        #10;
        data_in = 16'h1234;
        reg_a_en = 1;
        #10;
        reg_a_en = 0;
        #10;
        $display("reg_a_out = %h (expected 1234)", reg_a_out);

        // Test write to reg_d
        #10;
        data_in = 16'h5678;
        reg_d_en = 1;
        #10;
        reg_d_en = 0;
        #10;
        $display("reg_d_out = %h (expected 5678)", reg_d_out);

        // Test write to memory (mem array) at address 0
        #10;
        data_in = 16'h9ABC;
        reg_m_en = 1;
        #10;
        reg_m_en = 0;
        #10;
        $display("reg_m_out at addr 1 = %h (expected 9ABC)", reg_m_out);

        // Test read from memory (mem array) at address 1
        #10;
        $display("Read reg_m_out at addr 1 = %h (expected 9ABC)", reg_m_out);

        // Additional tests if desired...

        $display("Test completed.");
        $finish;
    end

endmodule
