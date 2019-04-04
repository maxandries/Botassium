	component botassium is
		port (
			clk_clk                : in  std_logic                     := 'X';             -- clk
			reset_reset_n          : in  std_logic                     := 'X';             -- reset_n
			nios_mem_s2_address    : in  std_logic_vector(6 downto 0)  := (others => 'X'); -- address
			nios_mem_s2_chipselect : in  std_logic                     := 'X';             -- chipselect
			nios_mem_s2_clken      : in  std_logic                     := 'X';             -- clken
			nios_mem_s2_write      : in  std_logic                     := 'X';             -- write
			nios_mem_s2_readdata   : out std_logic_vector(31 downto 0);                    -- readdata
			nios_mem_s2_writedata  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- writedata
			nios_mem_s2_byteenable : in  std_logic_vector(3 downto 0)  := (others => 'X')  -- byteenable
		);
	end component botassium;

	u0 : component botassium
		port map (
			clk_clk                => CONNECTED_TO_clk_clk,                --         clk.clk
			reset_reset_n          => CONNECTED_TO_reset_reset_n,          --       reset.reset_n
			nios_mem_s2_address    => CONNECTED_TO_nios_mem_s2_address,    -- nios_mem_s2.address
			nios_mem_s2_chipselect => CONNECTED_TO_nios_mem_s2_chipselect, --            .chipselect
			nios_mem_s2_clken      => CONNECTED_TO_nios_mem_s2_clken,      --            .clken
			nios_mem_s2_write      => CONNECTED_TO_nios_mem_s2_write,      --            .write
			nios_mem_s2_readdata   => CONNECTED_TO_nios_mem_s2_readdata,   --            .readdata
			nios_mem_s2_writedata  => CONNECTED_TO_nios_mem_s2_writedata,  --            .writedata
			nios_mem_s2_byteenable => CONNECTED_TO_nios_mem_s2_byteenable  --            .byteenable
		);

