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
		inout  wire BOOT_SEL,
		inout  wire DEV_OE
	);
	
	qsystem u0 (
		.clk_clk               (CLK_50),               //             clk.clk
		.reset_reset_n         (1),         //           reset.reset_n
		.pwm_0_pwmout_ldrive   (GPIO[18:16]),   //                .ldrive
		.pwm_0_pwmout_udrive   (GPIO[5:3]),   //    pwm_0_pwmout.udrive
		.qei_0_encoderin_encab (GPIO[7:6]), // qei_0_encoderin.encab
		.pwm_1_pwmout_ldrive   (GPIO[10:8]),   //                .ldrive
		.pwm_1_pwmout_udrive   (GPIO[13:11]),   //    pwm_1_pwmout.udrive
		.qei_1_encoderin_encab (GPIO[15:14])  // qei_1_encoderin.encab
	);

endmodule
	