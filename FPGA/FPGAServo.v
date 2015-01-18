module FPGAServo(
		input  wire	CLOCK_50,
		output wire	[31:0] GPIO_0,
		output wire	[31:0] GPIO_1,
		input  wire	[9:0] switches,
		output wire [7:0] LEDs
	);
	
	qsystem u0 (
		.clk_clk               (CLOCK_50),               //             clk.clk
		.reset_reset_n         (1),         //           reset.reset_n
		.pwm_0_pwmout_ldrive   (GPIO_0[2:0]),   //                .ldrive
		.pwm_0_pwmout_udrive   (GPIO_0[5:3]),   //    pwm_0_pwmout.udrive
		.qei_0_encoderin_encab (GPIO_0[7:6]), // qei_0_encoderin.encab
		.pwm_1_pwmout_ldrive   (GPIO_1[2:0]),   //                .ldrive
		.pwm_1_pwmout_udrive   (GPIO_1[5:3]),   //    pwm_1_pwmout.udrive
		.qei_1_encoderin_encab (GPIO_1[7:6])  // qei_1_encoderin.encab
	);

endmodule
	