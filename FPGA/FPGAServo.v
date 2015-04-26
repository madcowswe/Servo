module FPGAServo(
		input  wire	CLK_50,
		inout  wire	[38:1] GPIO,
		input  wire M0_ENC_A,
		input  wire M0_ENC_B,
		input  wire M0_ENC_Z,
		output wire M0_LEDB,
		output wire M0_LEDG,
		output wire M0_LEDR,
		output wire M0_PWM_AH,
		output wire M0_PWM_AL,
		output wire M0_PWM_BH,
		output wire M0_PWM_BL,
		output wire M0_PWM_CH,
		output wire M0_PWM_CL,
		input  wire M1_ENC_A,
		input  wire M1_ENC_B,
		input  wire M1_ENC_Z,
		output wire M1_LEDB,
		output wire M1_LEDG,
		output wire M1_LEDR,
		output wire M1_PWM_AH,
		output wire M1_PWM_AL,
		output wire M1_PWM_BH,
		output wire M1_PWM_BL,
		output wire M1_PWM_CH,
		output wire M1_PWM_CL,
		input  wire M2_ENC_A,
		input  wire M2_ENC_B,
		input  wire M2_ENC_Z,
		output wire M2_LEDB,
		output wire M2_LEDG,
		output wire M2_LEDR,
		output wire M2_PWM_AH,
		output wire M2_PWM_AL,
		output wire M2_PWM_BH,
		output wire M2_PWM_BL,
		output wire M2_PWM_CH,
		output wire M2_PWM_CL,
		input  wire STATUSLED_B,
		input  wire STATUSLED_R,
		inout  wire DEV_OE
	);
	
	qsystem u0 (
		.clk_clk               (CLK_50),               //             clk.clk
		.reset_reset_n         (1),         //           reset.reset_n
		.pwm_0_pwmout_ldrive   ({M0_PWM_AL, M0_PWM_BL, M0_PWM_CL}),   //                .ldrive
		.pwm_0_pwmout_udrive   ({M0_PWM_AU, M0_PWM_BU, M0_PWM_CU}),   //    pwm_0_pwmout.udrive
		.qei_0_encoderin_encab ({M0_ENC_A, M0_ENC_B}) // qei_0_encoderin.encab
	);

endmodule
	