// ==============================================================================
// Module: sram_soc_quad_top
// Project: sram_soc_quad_pure_sdk
// Silicon Target: sky130
// ==============================================================================

module sram_soc_quad_top (
    input  wire        clk,
    input  wire        rst_n,
    input  wire [7:0]  data_in,
    output reg  [7:0]  data_out
);

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            data_out <= 8'h00;
        end else begin
            data_out <= data_in + 8'h01;
        end
    end

endmodule
