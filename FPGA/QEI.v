// QEI.v

// This file was auto-generated as a prototype implementation of a module
// created in component editor.  It ties off all outputs to ground and
// ignores all inputs.  It needs to be edited to make it do something
// useful.
// 
// This file will not be automatically regenerated.  You should check it in
// to your version control system if you want to keep it.

`timescale 1 ps / 1 ps
module QEI #(
		parameter AUTO_CLOCK_SINK_CLOCK_RATE = "-1"
	) (
		input  wire        clk,          //   clock_sink.clk
		input  wire        reset_n,      //   reset_sink.reset_n
		input  wire        MMS_addr,     // avalon_slave.address
		input  wire        MMS_read,     //             .read
		output wire [31:0] MMS_readdata, //             .readdata
		input  wire [1:0]  encAB         //    EncoderIn.encab
	);

	reg [31:0] count;
	assign MMS_readdata = count;

	wire quadA;
	wire quadB;
	assign quadA = encAB[0];
	assign quadB = encAB[1];

	reg quadA_delayed, quadB_delayed;
	always @(posedge clk) quadA_delayed <= quadA;
	always @(posedge clk) quadB_delayed <= quadB;

	wire count_enable = quadA ^ quadA_delayed ^ quadB ^ quadB_delayed;
	wire count_direction = quadA ^ quadB_delayed;

	always @(posedge clk or negedge reset_n) begin : proc_count
		if(~reset_n) begin
			count <= 32'h00000000;
		end else begin
			if(count_enable) begin
				if(count_direction)
					count<=count+1;
				else
					count<=count-1;
			end
		end
	end

endmodule
