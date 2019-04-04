//-------------------------------------------------------------
//
// SPI Module
// 

module spi_slave(
	input  logic 			SPI_CLK, //Enable only  when CE = 0
	input  logic			SPI_CS,
	input  logic 			SPI_MOSI,
	output logic 			SPI_MISO,
	input  logic 			Data_WE,
	input  logic[3:0] 	Data_Addr,
	input  logic[31:0] 	Data_Write,
	output logic[31:0] 	Data_Read,
	input  logic			Clk //System clock
);

	logic [63:0] SPI_reg; //need to be change to [63:0] because 8 bytes
	logic [3:0] address;


//---Input Registers -(from PI to ARM) ------------------------

	logic [31:0] mosiRAM[15:0]; //16 registers of 32 bits
	logic 		 mosiRAM_we;
    
	assign Data_Read = mosiRAM[Data_Addr]; //read from mosi
	
	always_ff @(posedge Clk) begin
		if (mosiRAM_we) mosiRAM[address] <= SPI_reg[31:0]; //write to mosi SPI_reg[31:0] = data from the pi to arm
	end
	
//--- Output Registers - (from ARM to PI)----------------------

	logic [31:0] misoRAM[15:0]; //16 registers of 32 bits
	logic [31:0] misoRAM_read;

	assign misoRAM_read = misoRAM[address]; //read SPI_reg[3:0] = the register of miso (4bit because there are 16 registers)
	
	always_ff @(posedge Clk) begin
		if (Data_WE) misoRAM[Data_Addr] <= Data_Write; //write
	end
	
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
	
	logic [6:0] SPI_cnt; //!!check if enough for 64 bits !!! (minibot comment only)
	logic 		SPI_cnt_reset, SPI_cnt_inc;
	logic		SPI_reg_reset, SPI_reg_shift, SPI_reg_load;	
	logic 		MISO_we, MISO_reset;
	logic 		addressR, dataR;
	
// State Register & Bit counter & SPI Register & MISO
	
	always_ff @(posedge Clk) begin
	
		if (SPI_CS_sync)			state <= S0;
		else 							state <= nextstate;
		
		if (SPI_cnt_reset) 	 	SPI_cnt <= 6'b0;
		else if (SPI_cnt_inc) 	SPI_cnt <= SPI_cnt + 6'b1;
		
		if (SPI_reg_reset) 		SPI_reg <= 64'b0;
		else if (SPI_reg_shift)	SPI_reg <= {SPI_reg[62:0], SPI_MOSI};
		else if (SPI_reg_load)	SPI_reg <= {SPI_reg[63:39],misoRAM_read,SPI_reg[7:0]};
		
		if (MISO_reset) 		SPI_MISO <= 0;
		else if (SPI_reg_load)	SPI_MISO <= misoRAM_read[31];
		else if (MISO_we)		SPI_MISO <= SPI_reg[63];
		
		if(addressR)         address <= SPI_reg[3:0];
 			
	end
	
// Next State Logic

	always_comb begin
	
	
		// Default value
		nextstate = state;
		SPI_cnt_reset = 0; 
		SPI_cnt_inc = 0;
		SPI_reg_reset = 0; 
		SPI_reg_shift = 0; 
		SPI_reg_load = 0;
		MISO_we = 0; 
		MISO_reset = 0;
		mosiRAM_we = 0;
		addressR = 0;
		dataR = 0;
		
		case (state)
			S0 : if (~SPI_CS_sync) begin			// negedge of SPI_CS no transfer until SPI_CS_sync = 0
						SPI_cnt_reset = 1;
						SPI_reg_reset = 1;
						MISO_reset    = 1;
						nextstate = S1;
					end
					
			S1	: if (SPI_CLK_sync) begin			// posedge of SPI_CLK SPI Transfer state
						SPI_reg_shift = 1;
						SPI_cnt_inc   = 1;
						nextstate = S2;
					end
					
			S2 : if (~SPI_CLK_sync) begin			// negedge of SPI_CLK 
						MISO_we = 1;
						if (SPI_cnt == 8) SPI_reg_load = 1;
						if (SPI_cnt == 64) begin
							if (SPI_reg[36] == 1) begin
								addressR = 1;
							end
							else if (SPI_reg[37] == 1) begin
								dataR = 1;
								if (SPI_reg[63] == 1) mosiRAM_we = 1;
							end
							nextstate = S3;
						end
						else nextstate = S1;
					end
					
			S3 : if (SPI_CS_sync) begin 			// posedge of SPI_CS idle state, wait for the CS to go back to 0 then back to S1
						nextstate = S0;
						addressR =0;
						dataR = 0;
					end
		endcase
	end
	
endmodule

//-------------------------------------------------------------
