module SSD_Controller (
    input clk,                    // Clock input
    input [15:0] ssd_in,          // 32-bit input for the four digits
    output reg [6:0] seg,         // Segment output (a to g)
    output reg [3:0] an           // Anode signals for the four digits
);
    reg [1:0] digit_select;       // 2-bit counter for digit selection
    reg [3:0] current_digit;      // Current digit value (4 bits)
    
    always @(posedge clk) begin
        digit_select <= digit_select + 1; // Increment digit select counter
    end

    always @(posedge clk) begin
        // Mux to select the current digit based on digit_select
        case (digit_select)
            2'b00: begin
                an = 4'b0111;    // Activate digit 0
                current_digit = ssd_in[15:11];
            end
            2'b01: begin
                an = 4'b1011;    // Activate digit 1
                current_digit = ssd_in[10:8];
            end
            2'b10: begin
                an = 4'b1101;    // Activate digit 2
                current_digit = ssd_in[7:4];
            end
            2'b11: begin
                an = 4'b1110;    // Activate digit 3
                current_digit = ssd_in[3:0];
            end
        endcase
    end

    always @* begin
        // Seven-segment decoder
        case (current_digit)
        4'b0000: seg = 7'b0000001; // "0"     
        4'b0001: seg = 7'b1001111; // "1" 
        4'b0010: seg = 7'b0010010; // "2" 
        4'b0011: seg = 7'b0000110; // "3" 
        4'b0100: seg = 7'b1001100; // "4" 
        4'b0101: seg = 7'b0100100; // "5" 
        4'b0110: seg = 7'b0100000; // "6" 
        4'b0111: seg = 7'b0001111; // "7" 
        4'b1000: seg = 7'b0000000; // "8"     
        4'b1001: seg = 7'b0000100; // "9" 
        default: seg = 7'b0000001; // "0"

        endcase
    end
endmodule

