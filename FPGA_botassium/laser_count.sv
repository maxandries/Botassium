module laser_count(
	input logic clk,
	input logic Opto,
	input logic codeA, 
	input logic codeB,
	input logic Laser,
	output logic[31:0] count);
	
	
	logic direction;
	logic phase;
	logic [15:0] RisingEdge, FallingEdge;
	logic [15:0] counter;
	logic invertLaser;
	logic [31:0] timer;

	always_comb begin
		phase <=  Atrue | Btrue;
	end
	always_ff @(posedge phase) begin
		if(codeA)
				direction <= 0;
		else if(codeB)
			direction <= 1;
		else
			direction <=0;
	end
	logic A1, A2, Atrue;
	logic B1, B2, Btrue;
	
	always_ff @(posedge clk) begin
		if(Atrue & ~(codeA|A1|A2))
			Atrue <= 0;
		else if (~Atrue & A1 && A2 && codeA)
			Atrue <= 1;
		if(Btrue & ~(codeB|B1|B2))
			Btrue <= 0;
		else if (~Btrue & B1 && B2 && codeB)
			Btrue <= 1;
		A2 <= A1;
		A1 <= codeA;
	
		B2 <= B1;
		B1 <= codeB;
	end
	logic [15:0] RisingEdgeOld, FallingEdgeOld;
	always_ff @(posedge Laser, posedge Opto) begin
		FallingEdgeOld <= FallingEdge;
		if(Opto) begin
			FallingEdge <= 0;
		end
		else if (Laser & FallingEdge == 0) begin
			FallingEdge <= counter;
		end
	end

	always_ff @(posedge Opto, negedge Laser) begin
		RisingEdgeOld <= RisingEdge;
		if (Opto) begin
			RisingEdge <= 0;
		end
		else if (!Laser & RisingEdge == 0) begin
			RisingEdge <= counter;
		end
	end

		
	always_ff @(posedge clk) begin
		timer <= timer +1;
		if(timer == 100000) begin
			count <= {RisingEdgeOld, FallingEdgeOld};
			timer <= 0;
		end
	end
	always_ff @(posedge Opto, posedge Atrue) begin
		if(Opto)
			counter <= 0;
		else if(Atrue)
			if(!direction)
				counter <= counter+1;
			else
				counter <= counter-1;
	end
endmodule
