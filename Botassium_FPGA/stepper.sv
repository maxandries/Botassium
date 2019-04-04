module stepper(input logic clk,
					output logic dir,
					output logic step);
					
	logic [26:0] counter;
	
	always_ff @(posedge clk) begin
		if (counter == 50000) begin
			step <= ~step;
			counter <= 0;
		end
		else
			counter <= counter + 1;
	end
	
	assign dir = 1'b1;
	
endmodule
