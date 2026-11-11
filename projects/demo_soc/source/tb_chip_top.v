`timescale 1ns/1ps
module tb_chip_top;
    reg clk_pad = 0;
    reg rst_n_pad = 0;
    reg a_pad = 0;
    reg b_pad = 0;
    reg op_pad = 0;
    wire res_pad;
    wire zero_pad;
    wire [3:0] status_leds;

    chip_top dut (
        .clk_pad(clk_pad),
        .rst_n_pad(rst_n_pad),
        .a_pad(a_pad),
        .b_pad(b_pad),
        .op_pad(op_pad),
        .res_pad(res_pad),
        .zero_pad(zero_pad),
        .status_leds(status_leds)
    );

    always #5 clk_pad = ~clk_pad;

    initial begin
        $dumpfile("waveform.vcd");
        $dumpvars(0, tb_chip_top);
        #15 rst_n_pad = 1;
        #10 a_pad = 1; b_pad = 1; op_pad = 0;
        #20 a_pad = 1; b_pad = 0; op_pad = 1;
        #20 a_pad = 0; b_pad = 0; op_pad = 0;
        #50 $finish;
    end
endmodule
