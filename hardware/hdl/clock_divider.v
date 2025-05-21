module clock_divider #(
    parameter integer INPUT_FREQ = 100_000_000,   // Input clock frequency in Hz
    parameter integer TARGET_FREQ = 1            // Target output frequency in Hz
)(
    input wire clk_in,       // Input clock
    output reg clk_out       // Output divided clock
);

    // Calculate the divide count based on input and target frequencies
    localparam integer DIVIDE_COUNT = INPUT_FREQ / (2 * TARGET_FREQ);

    reg [31:0] counter = 0; // 32-bit counter to handle larger divide counts

    initial begin clk_out <= 0; end

    always @(posedge clk_in) begin
        if (counter == DIVIDE_COUNT) begin
            counter <= 0;
            clk_out <= ~clk_out; // Toggle the output clock
        end else begin
            counter <= counter + 1;
        end
    end
endmodule

