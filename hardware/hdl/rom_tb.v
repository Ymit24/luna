`timescale 1ns/1ns
`include "rom.v"

module rom_tb;
    reg [7:0] address;
    wire [15:0] data;

    rom uut (
        .address(address),
        .data(data)
    );

    initial
    begin
        $dumpfile("rom.vcd");
        $dumpvars(0, rom_tb);

        $display("Checking rom");

        for (address = 0; address < 10; address++) begin
            #100;
            $display("Address %h = %h", address, data);
        end
        $display("Done");

        $finish;
    end
endmodule;
