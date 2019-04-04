module wheel_count(
	input logic clk,
	input logic rst,
	input logic codeA, 
	input logic codeB,
	output logic[31:0] count);
	
	
	logic direction;
	logic phase;
	logic [31:0] counter, counterOld, timer;

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
	
		if (timer == 100000) begin
			count <= counter - counterOld;
			counterOld <= counter;
			timer <= 0;
		end
		else
			timer <= timer+1;
	
	end
	
	always_ff @(negedge rst, posedge Atrue) begin
		if(!rst)
			counter <= 0;
		else if(Atrue)
			if(direction)
				counter <= counter+1;
			else
				counter <= counter-1;
	end
endmodule
