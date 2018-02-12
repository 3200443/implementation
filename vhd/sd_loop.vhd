Library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;



package bus_multiplexer_pkg is
        type bus_array is array(natural range <>) of std_logic_vector;
end package;




library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.bus_multiplexer_pkg.all;

entity sd_loop is
generic(
		Col		:integer := 80;
		Lig 	:integer := 60;
		P 		:integer := 4;
		N 		:integer := 16
);
port(
  imagein	: in std_logic_vector(31 downto 0);
  we 		: in std_logic;
  RESET 	: in std_logic;
  CLK 		: in std_logic;
  imageout	: out std_logic_vector(31 downto 0)
);

end entity ;



Architecture behav of sd_3pixel is

	signal ITaddrin, MTaddrin, VTaddrin,Etaddrin: std_logic_vector(LOG2(REAL(Col*8*Lig/N)) downto 0);
	signal MTaddrout, VTaddrout: std_logic_vector(LOG2(REAL(Col*8*Lig/N)) downto 0);
	signal ITin : std_logic_vector(N*8-1 downto 0) ;
	signal ITwe, MTwe, VTwe, ETwe : std_logic;
	signal state 	: unsigned(7 downto 0) := to_unsigned(0,8);
	signal count1 	: unsigned(7 downto 0) := to_unsigned(0,8);
	signal count2 	: unsigned(7 downto 0) := to_unsigned(0,8);
	signal buff 	: std_logic_vector(Col*8-1 downto 0);

	signal ITint,MTintin, VTintin, MTintout, VTintout 	: bus_array(P - 1 downto 0)((N*8-1) downto 0);
	signal ETint			: bus_array(P - 1 downto 0)((N-1) downto 0);
	signal ETintout (N-1 downto 0);


begin

	I0: entity work.sd_3pixel generic map(P=>P, N=>N) port map( 
		itin 		=> ITint,
		mtmoinsun	=> MTintin,
		CLK 		=> CLK,
		vtmoinsun	=> VTintin,
		mtout		=> MTintout,
		vt			=> VTintout,
		et 			=> ETint
	);
	I1: entity work.VT(behav) generic map(P=>P, N =>N, Col => Col, Lig => Lig) port map(
		reset	 	=> RESET, 
		CLK 		=> CLK, 
		we 			=> VTwe, 
		addrin 		=> VTaddrin, 
		addrout 	=> VTaddrout,
		inpout 		=> VTintout,
		outpout 	=> VTintin
	);
	I2: entity work.MT(behav) generic map(P=>P, N =>N, Col => Col, Lig => Lig) port map(
		reset	 	=> RESET, 
		CLK 		=> CLK, 
		we 			=> MTwe, 
		addrin 		=> MTaddrin, 
		addrout 	=> MTaddrout,
		inpout 		=> MTintout,
		outpout 	=> MTintin
	);
	I3: entity work.IT(behav)generic map(P=>P, N =>N, Col => Col, Lig => Lig) port map(
		reset	 	=> RESET, 
		CLK 		=> CLK, 
		we 			=> ITwe, 
		addr 		=> ITaddrin, 
		inpout 		=> ITin,
		outpout 	=> ITint
	);
	I0: entity work.ET(behav) generic map(P=>P, N =>N, Col => Col, Lig => Lig) port map(
		reset	 	=> RESET, 
		CLK 		=> CLK, 
		we 			=> ETwe, 
		addr 		=> Etaddrin, 
		inpout 		=> ETint,
		outpout 	=> ETintout
	);
	--process(imagein)
	-- 	variable status : unsigned(7 downto 0) := unsigned(state);
	--begin
	--	if(status = 0) then
	--		status = 1;
	--	end if
	--end process

	process(CLK, RESET)
		variable status : unsigned(7 downto 0) := unsigned(state);
		variable bufff 	: std_logic_vector := buff;
		variable iter  	: unsigned(7 downto 0) := count1;
		variable pos  	: unsigned(7 downto 0) := count2;
	begin

		if(rising_edge (CLK)) then
			ITwe <= "0";
			MTwe <= "0";
			VTwe <= "0";
		end if;

		if(RESET = '1') then
			count1 <= X"00";
			count2 <= X"00";
			status <= X"00";
		else if(status = 1) -- reception des images
			if(rising_edge(CLK) & we) then
				bufff(((iter+4)*8-1) downto iter) <=  imagein;
				iter := iter +4;
				if(iter = Col) then
					iter := 0;
					ITwe <= "1";
					ITaddrin <= count2;
					ITin <= bufff;
					count2 := count2 +1;
					if(count2 = Lig) then
						count2 := 0;
						status := 2;
					end if;
				end if;
			end if;
		else if(status = 2) then -- traitement
			iter := iter+1;
			if(rising_edge(CLK)) then
				if iter > 8 then
					ITaddrin := iter - 9;
					MTaddrin := iter - 9;
					VTaddrin := iter - 9;
					Etaddrin := iter - 9;
					MTwe <=1;
					VTwe <=1;
					ETwe <=1;
				end if;
				if pos < Col then
					ITaddrout <= pos;
					MTaddrout <= pos;
					VTaddrout <= pos;
				end if;
			end if;
		else if (status = 3)
			if (pos <Col) then
				if(iter = "00") then
					Etaddrin <= pos;
					bufff (N-1 downto 0) <= ETintout; --ATTRENTION A LA VALEUT DE N
					pos := pos +1;
					iter := iter + 1;
				else
					bufff(N*iter-1 downto 0) <= ETintout;
					imageout <= bufff;
					iter =  "00";
				end if;
			else
				pos :=X"0";
				status := "0";
			end if;
		end if;
		state <= status;
		buff <= bufff;
		count1 <= iter;
		count2 <= pos;
	end process;

	process(we)
		if we = "1" & status = "0" then
			state = 1;
		end if;
	end process;
	process(count1)
	begin
		if(status = 2 & count1 > Lig+8) then
			status = 3;
			iter <= X"00";
			pos <= X"00";
		end if;
		if(count1 > 8 & status = 2) then
			ITwe <= "1";
			MTwe <= "1";
			VTwe <= "1";
		end if;
	end process;
end behav;
