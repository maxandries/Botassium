
module botassium (
	clk_clk,
	reset_reset_n,
	nios_mem_s2_address,
	nios_mem_s2_chipselect,
	nios_mem_s2_clken,
	nios_mem_s2_write,
	nios_mem_s2_readdata,
	nios_mem_s2_writedata,
	nios_mem_s2_byteenable);	

	input		clk_clk;
	input		reset_reset_n;
	input	[6:0]	nios_mem_s2_address;
	input		nios_mem_s2_chipselect;
	input		nios_mem_s2_clken;
	input		nios_mem_s2_write;
	output	[31:0]	nios_mem_s2_readdata;
	input	[31:0]	nios_mem_s2_writedata;
	input	[3:0]	nios_mem_s2_byteenable;
endmodule
