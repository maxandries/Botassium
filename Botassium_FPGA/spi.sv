//-------------------------------------------------------------
//
// SPI Module
// 

module spi_slave(
	input  logic 			SPI_CLK,
	input  logic			SPI_CS,
	input  logic 			SPI_MOSI,
	output logic 			SPI_MISO,
	output logic 			Data_WE,
	output logic[6:0] 	Data_Addr,
	output logic[31:0] 	Data_Write,
	input  logic[31:0] 	Data_Read,
	input  logic			Clk
);

	logic [39:0] SPI_reg;

	assign Data_WE   = MOSI_we;
	assign Data_Addr = MOSI_we ? SPI_reg[38:32] : SPI_reg[6:0];
	assign Data_Write = SPI_reg[31:0];
	
//---SPI Sysnchronization -------------------------------------

	logic SPI_CLK_sync;
	logic SPI_CS_sync;

	always_ff @(posedge Clk) begin
		SPI_CLK_sync <= SPI_CLK;
		SPI_CS_sync  <= SPI_CS;
	end
	
//--- SPI FSM -------------------------------------------------

	typedef enum logic [1:0] {S0,S1,S2,S3} statetype;
	statetype state, nextstate;
	
	logic [5:0] SPI_cnt;
	logic 		SPI_cnt_reset, SPI_cnt_inc;
	logic			SPI_reg_reset, SPI_reg_shift, SPI_reg_load;	
	logic 		MISO_we, MISO_reset;
	logic			MOSI_we;
	
// State Register & Bit counter & SPI Register & MISO
	
	always_ff @(posedge Clk) begin
	
		if (SPI_CS_sync)			state <= S0;
		else 							state <= nextstate;
		
		if (SPI_cnt_reset) 	 	SPI_cnt <= 6'b0;
		else if (SPI_cnt_inc) 	SPI_cnt <= SPI_cnt + 6'b1;
		
		if (SPI_reg_reset) 		SPI_reg <= 40'b0;
		else if (SPI_reg_shift)	SPI_reg <= {SPI_reg[38:0], SPI_MOSI};
		else if (SPI_reg_load)	SPI_reg <= {Data_Read, SPI_reg[7:0]};
		
		if (MISO_reset) 			SPI_MISO <= 0;
		else if (SPI_reg_load)	SPI_MISO <= Data_Read[31];
		else if (MISO_we)			SPI_MISO <= SPI_reg[39];
 			
	end
	
// Next State Logic

	always_comb begin
	
		// Default value
		nextstate = state;
		SPI_cnt_reset = 0; SPI_cnt_inc = 0;
		SPI_reg_reset = 0; SPI_reg_shift = 0; SPI_reg_load = 0;
		MISO_we = 0; MISO_reset = 0;
		MOSI_we = 0;
		
		case (state)
			S0 : if (~SPI_CS_sync) begin 			// negedge of SPI_CS
						SPI_cnt_reset = 1;
						SPI_reg_reset = 1;
						MISO_reset    = 1;
						nextstate = S1;
					end
					
			S1	: if (SPI_CLK_sync) begin			// posedge of SPI_CLK
						SPI_reg_shift = 1;
						SPI_cnt_inc   = 1;
						nextstate = S2;
					end
					
			S2 : if (~SPI_CLK_sync) begin			// negedge of SPI_CLK
						MISO_we = 1;
						if (SPI_cnt == 8) SPI_reg_load = 1;
						if (SPI_cnt == 40) begin
							if (SPI_reg[39] == 1) MOSI_we = 1;
							nextstate = S3;
						end
						else nextstate = S1;
					end
					
			S3 : if (SPI_CS_sync) begin 			// posedge of SPI_CS
						nextstate = S0;
					end
		endcase
	end
	
endmodule

//-------------------------------------------------------------
