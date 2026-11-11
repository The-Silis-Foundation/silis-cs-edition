// Full Adder Testbench Module
module tb_full_adder;

    // Testbench signals
    logic a;
    logic b;
    logic cin;
    logic sum;
    logic cout;

    // Instantiate the Device Under Test (DUT)
    full_adder dut (
        .a   (a),
        .b   (b),
        .cin (cin),
        .sum (sum),
        .cout(cout)
    );

    // Stimulus generation
    initial begin
        $dumpfile("waveform.vcd");
        $dumpvars(0, tb_full_adder);
        // Display header for console output
        $display("Time\t A B Cin | Cout Sum");
        $display("-----------------------");

        // Loop through all 8 possible input configurations
        for (int i = 0; i < 8; i++) begin
            {a, b, cin} = i[2:0];
            #10; // Wait for 10 time units
            $display("%0t\t %b %b  %b  |  %b    %b", $time, a, b, cin, cout, sum);
        end

        $finish; // End simulation
    end

endmodule
