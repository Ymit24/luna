module counter(
    input clk,
    input write_en,
    input [15:0] data_in,
    output reg [15:0] data_out
);

initial data_out = 0;

always@(posedge clk)
begin
    if (write_en)
        data_out <= data_in;
    else
        data_out <= data_out + 1;
end

endmodule;
