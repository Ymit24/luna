module rom(
    input wire [7:0] address,
    output [15:0] data
);
    reg [15:0] rom_memory [0:255];
    assign data = rom_memory[address];

    initial $readmemh("program.txt", rom_memory);
endmodule;
