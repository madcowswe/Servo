module FPGAServo(
		input wire CLK_50,
		input wire ENC_A1,
		input wire ENC_A2,
		input wire ENC_A3,
		input wire ENC_B1,
		input wire ENC_B2,
		input wire ENC_B3,
		input wire ENC_Z1,
		input wire ENC_Z2,
		input wire ENC_Z3,
		inout wire [30:5] GPIO,
		//output wire LEDB1,
		//output wire LEDB2,
		//output wire LEDB3,
		//output wire LEDG1,
		//output wire LEDG2,
		//output wire LEDG3,
		//output wire LEDR1,
		//output wire LEDR2,
		//output wire LEDR3,
		//output wire PWM_AH1,
		//output wire PWM_AH2,
		//output wire PWM_AH3,
		//output wire PWM_AL1,
		//output wire PWM_AL2,
		//output wire PWM_AL3,
		//output wire PWM_BH1,
		//output wire PWM_BH2,
		//output wire PWM_BH3,
		//output wire PWM_BL1,
		//output wire PWM_BL2,
		//output wire PWM_BL3,
		//output wire PWM_CH1,
		//output wire PWM_CH2,
		//output wire PWM_CH3,
		//output wire PWM_CL1,
		//output wire PWM_CL2,
		//output wire PWM_CL3,
		input wire RXD,
		inout wire [36:31] SGPIO,
		//output wire STATUSLED_B,
		//output wire STATUSLED_R,
		//output wire TXD,
		//output wire CTS_n,
		input wire RTS_n
	);
	
	//force -freeze sim:/qsystem_tb/qsystem_inst/altpll_0/areset St0 0
	//force -freeze sim:/qsystem_tb/qsystem_inst/altpll_0/read St0 0
	//force -freeze sim:/qsystem_tb/qsystem_inst/altpll_0/write St0 0

	 qsystem u0 (
		.clk_clk               (CLK_50),               //             clk.clk
		.reset_reset_n         (1),         //           reset.reset_n
		.pwm_0_pwmout_ldrive   ({GPIO[5], GPIO[7], GPIO[9]}),   //                .ldrive
		.pwm_0_pwmout_udrive   ({GPIO[6], GPIO[8], GPIO[10]}),   //    pwm_0_pwmout.udrive
		.qei_0_encoderin_encab ({ENC_A2, ENC_B2})
	 );


//	reg [32:0] ctr = 0;
//	reg blinkystate = 0;
//	always @(posedge CLK_50) begin : proc_blinky
//		ctr <= ctr + 1;
//		if(ctr == 25000000) begin
//			blinkystate <= ~blinkystate;
//			ctr <= 0;
//		end
//	end
//
//	assign LEDB1 = blinkystate;

endmodule
	