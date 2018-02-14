


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.bus_multiplexer_pkg.all;
use ieee.math_real.all;

entity sd_loop is
generic(
		Col		:integer := 60;
		Lig 	:integer := 80;
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



Architecture behav of sd_loop is

	signal ITaddrin, MTaddrin, VTaddrin 	: std_logic_vector(integer(ceil(LOG2(REAL(Col*8*Lig/N)))) downto 0);
	signal Etaddrin 						: std_logic_vector(integer(ceil(LOG2(real(COL*LIG/N)))) downto 0);
	signal MTaddrout, VTaddrout				: std_logic_vector(integer(ceil(LOG2(REAL(Col*8*Lig/N)))) downto 0);
	signal ITin 							: std_logic_vector(N*8-1 downto 0) ;
	signal ITwe, MTwe, VTwe, ETwe 			: std_logic;
	signal state 							: unsigned(7 downto 0) := to_unsigned(0,8);
	signal count1 							: unsigned(7 downto 0) := to_unsigned(0,8);
	signal count2 							: unsigned(7 downto 0) := to_unsigned(0,8);
	signal buff 							: std_logic_vector(Col*8-1 downto 0);

	signal ITint,MTintin, VTintin, MTintout, VTintout 	: bus_array(P - 1 downto 0)((N*8-1) downto 0);
	signal ETint			: bus_array(P - 1 downto 0)((N-1) downto 0);
	signal ETintout 		: std_logic_vector(N-1 downto 0);


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
	I4: entity work.ET(behav) generic map(P=>P, N =>N, Col => Col, Lig => Lig) port map(
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
		variable bufff 	: std_logic_vector(Col*8-1 downto 0) := buff;
		variable iter  	: unsigned(7 downto 0) := count1;
		variable pos  	: unsigned(7 downto 0) := count2;
		variable address: integer :=0;
	begin

		if(rising_edge (CLK)) then
			ITwe <= '0';
			MTwe <= '0';
			VTwe <= '0';
		end if;

		if(RESET = '1') then
			iter := X"00";
			pos  := X"00";
			status := X"00";
		elsif(status = 1) then-- reception des images
			if(rising_edge(CLK) and we = '1') then
				address := ((to_integer(iter)+4)*8-1);
				bufff(address downto to_integer(iter)) :=  imagein;
				iter := iter +4;
				if(iter = Col) then
					iter := X"00";
					ITwe <= '1';
					ITaddrin <= std_logic_vector(pos);
					ITin <= bufff;
					pos := pos +1;
					if(pos = Lig) then
						pos := X"00";
						status := X"02";
					end if;
				end if;
			end if;
		elsif(status = 2) then -- traitement
			iter := iter+1;
			if(rising_edge(CLK)) then
				if iter > 8 then
					ITaddrin <= std_logic_vector(to_unsigned((to_integer(iter) - 9),integer(ceil(LOG2(REAL(Col*8*Lig/N))))));
					MTaddrin <= std_logic_vector(to_unsigned((to_integer(iter) - 9),integer(ceil(LOG2(REAL(Col*8*Lig/N))))));
					VTaddrin <= std_logic_vector(to_unsigned((to_integer(iter) - 9),integer(ceil(LOG2(REAL(Col*8*Lig/N))))));
					Etaddrin <= std_logic_vector(to_unsigned((to_integer(iter) - 9),integer(ceil(LOG2(REAL(Col*8*Lig/N))))));
					MTwe <='1';
					VTwe <='1';
					ETwe <='1';
				end if;
				if pos < Col then
					ITaddrin <= std_logic_vector(pos);
					MTaddrout <= std_logic_vector(pos);
					VTaddrout <= std_logic_vector(pos);
				end if;
			end if;
		elsif (status = 3) then
			if (pos <Col) then
				if(iter = "00") then
					Etaddrin <= std_logic_vector(pos);
					bufff (N-1 downto 0) := ETintout; --ATTRENTION A LA VALEUT DE N
					pos := pos +1;
					iter := iter + 1;
				else
					bufff(N*to_integer(iter)-1 downto 0) := ETintout;
					imageout <= bufff;
					iter :=  X"00";
				end if;
			else
				pos :=X"00";
				status := X"00";
			end if;
		end if;
		state <= status;
		buff <= bufff;
		count1 <= iter;
		count2 <= pos;
	end process;

	process(we)
	begin
		if we = '1' and state = "00" then
			state <= X"01";
		end if;
	end process;
	process(count1)
	begin
		if(state = X"02" and count1 > Lig+8) then
			state <= X"03";
			count1 <= X"00";
			count2 <= X"00";
		end if;
		if(count1 > 8 and state = X"02") then
			ITwe <= '1';
			MTwe <= '1';
			VTwe <= '1';
		end if;
	end process;
end behav;
