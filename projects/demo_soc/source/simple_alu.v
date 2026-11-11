/* verilator lint_off UNUSEDSIGNAL */
/* verilator lint_off UNDRIVEN */
(* blackbox *)
module simple_alu (
    input  wire clk,
    input  wire rst_n,
    input  wire a_in,
    input  wire b_in,
    input  wire op_sel,
    output wire res_out,
    output wire flag_zero
);
    assign res_out   = op_sel ? (a_in & b_in) : (a_in ^ b_in);
    assign flag_zero = ~res_out;
endmodule
/* verilator lint_on UNUSEDSIGNAL */
/* verilator lint_on UNDRIVEN */
