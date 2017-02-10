-- cpu.vhd: Simple 8-bit CPU (BrainFuck interpreter)
-- Copyright (C) 2014 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Dávid Mikuš (xmikus15)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(12 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (0) / zapis (1)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

	-- zde dopiste potrebne deklarace signalu
	signal pc_reg : std_logic_vector(12 downto 0);
	signal pc_inc : std_logic;
	signal pc_dec : std_logic;
	 
	signal ptr_reg : std_logic_vector(12 downto 0);
	signal ptr_inc : std_logic;
	signal ptr_dec : std_logic;
	
	signal cnt_reg : std_logic_vector(12 downto 0);
	signal cnt_inc : std_logic;
	signal cnt_dec : std_logic;
	
	-- Instrukcie
	type instructions is (incPtr, decPtr, incValue, decValue, whileB, whileE, putchar, getchar, nop, halt);
	signal ireg_dec : instructions;
	
	-- Automat
	type fsm_state is (sidle, sfetch, sdecode, sIncPtr, sDecPtr, sIncValue1, sDecValue1, 
						sDecValue2,sIncValue2, sPutChar1,sPutChar2,sGetChar1,sGetChar2,
						sWhileB1,sWhileB2,sWhileB3, sWhileB4, sWhileE1, sWhileE2,sWhileE3,sWhileE4,
						sNop, shalt);
	signal presentState : fsm_state;
	signal nextState : fsm_state;
	
	-- Multiplexori
	signal mx1 : std_logic;
	signal mx2 : std_logic_vector (1 downto 0);
	
	
	
	

begin

 -- zde dopiste vlastni VHDL kod
 -- Programovy citac
pc_cntr: process (RESET, CLK)
begin
	if (RESET='1') then
		pc_reg <= (others=>'0');
		-- nabezna hrana
	elsif (CLK'event) and (CLK='1') then
		-- incrementuje programovy citac
		if (pc_inc = '1') then
			pc_reg <= pc_reg + 1;
		end if;
		-- deceremntuje programovy citac
		if(pc_dec = '1') then
			pc_reg <= pc_reg - 1;
		end if;
	end if;
end process;

-- Citac PTR
ptr: process(RESET,CLK)
begin
	if (RESET = '1') then
		ptr_reg <= "1000000000000"; -- 0x1000
	elsif (CLK'event) and (CLK='1') then
		if(ptr_inc = '1') then
			ptr_reg <= ptr_reg + 1;
		end if;
		if(ptr_dec = '1') then
			ptr_reg <= ptr_reg - 1;
		end if;
	end if;
end process;

-- Citac CNT
-- nepouzite
cnt:process(RESET,CLK)
begin
	if (RESET = '1') then
		cnt_reg <= (others => '0');
	elsif (CLK'event) and (CLK='1') then
		if(cnt_inc = '1') then
			cnt_reg <= cnt_reg + 1;
		end if;
		if(cnt_dec = '1') then
			cnt_reg <= cnt_reg - 1;
		end if;
	end if;
end process;

-- Multiplexori
with mx1 select
	DATA_ADDR <= pc_reg when '0',
					ptr_reg when '1',
					(others => 'Z') when others;
with mx2 select
	DATA_WDATA <= IN_DATA when "00",
					DATA_RDATA + 1 when "01",
					DATA_RDATA - 1 when "10",
					(others => 'Z') when others;
					

-- Dekodovanie instrukcii
process (DATA_RDATA)
begin
		case (DATA_RDATA) is
			when X"3E" => ireg_dec <= incPtr;
			when X"3C" => ireg_dec <= decPtr;
			when X"2B" => ireg_dec <= incValue;
			when X"2D" => ireg_dec <= decValue;
			when X"5B" => ireg_dec <= whileB;
			when X"5D" => ireg_dec <= whileE;
			when X"2E" => ireg_dec <= putchar;
			when X"2C" => ireg_dec <= getchar;
			when X"00" => ireg_dec <= halt;
			when others => ireg_dec <= nop;
		end case;
end process;


fsm_pstate: process(CLK,RESET)
begin
	if (RESET = '1') then
		presentState <= sidle;
	elsif (CLK'event) and (CLK='1') then
		if(EN ='1') then
			presentState <= nextState;
		end if;
	end if;
end process;

fsm_nextstate: process(presentState, ireg_dec, OUT_BUSY, DATA_RDATA, IN_VLD, EN, IN_DATA, cnt_reg)
begin
	-- INIT
	mx1 <= '0';
	mx2 <= "00";
	DATA_EN <= '0';	
	pc_inc <= '0';
	pc_dec <= '0';
	ptr_inc <= '0';
	ptr_dec <= '0';
	cnt_inc <= '0';
	cnt_dec <= '0';
	OUT_WE <= '0';
	IN_REQ <= '0';
	nextState <= sfetch;
	case presentState is
		when sidle =>	
			
			nextState <= sfetch;
		when sfetch =>	
			mx1 <= '0'; -- nacitanie pc_reg do DATA_ADDR
			DATA_EN <= '1';
			DATA_RDWR <= '0';
			nextState <= sdecode;
		when sdecode =>
			case ireg_dec is
				when incPtr =>
					nextState <= sIncPtr;
				when decPtr =>
					nextState <= sDecPtr;
				when incValue =>
					nextState <= sIncValue1;
				when decValue =>
					nextState <= sDecValue1;
				when getchar =>
					nextState <= sGetChar1;
				when putchar =>
					nextState <= sPutChar1;
				when whileB =>
					nextState <= sWhileB1;
				when whileE =>
					nextState <= sWhileE1;					
				when nop =>
					nextState <= sNop;
				when halt =>
					nextState <= shalt;
				when others =>
					nextState <= sfetch;
			end case;
		when sIncPtr =>
			pc_inc <= '1';
			pc_dec <= '0';
			ptr_inc <= '1';
			ptr_dec <= '0';
			nextState <= sfetch;
		when sDecPtr =>
			pc_inc <= '1';
			pc_dec <= '0';
			ptr_inc <= '0';
			ptr_dec <= '1';
			nextState <= sfetch;
		when sIncValue1 =>
			DATA_EN <= '1';
			mx1 <= '1';
			DATA_RDWR <= '0';
			nextState <= sIncValue2;
		when sIncValue2 =>
			mx1 <= '1';
			mx2 <= "01";
			DATA_RDWR <= '1';
			DATA_EN <= '1';		
			pc_inc <= '1';
			nextState <= sfetch;
		when sDecValue1 =>
			DATA_EN <= '1';
			mx1 <= '1';
			DATA_RDWR <= '0';
			nextState <= sDecValue2;
		when sDecValue2 =>
			mx1 <= '1';
			mx2 <= "10";
			DATA_RDWR <= '1';
			DATA_EN <= '1';		
			pc_inc <= '1';
			nextState <= sfetch;
		when sGetChar1 =>
			IN_REQ <= '1';
			nextState <= sGetChar2;
		when sGetChar2 =>
			if (IN_VLD = '0') then
				nextState <= sGetChar2;
			else		
				mx1 <= '1';
				mx2 <= "00"; -- citanie zo vstupu
				DATA_RDWR <= '1';
				DATA_EN <= '1';
				pc_inc <= '1';
				nextState <= sfetch;				
			end if;
		when sPutChar1 =>
			DATA_EN <= '1';
			mx1 <= '1';
			DATA_RDWR <= '0';
			nextState <= sPutChar2;
		when sPutChar2 =>
			if (OUT_BUSY = '0') then
			  OUT_DATA <= DATA_RDATA;
			  OUT_WE <= '1';
			  pc_inc <= '1';
			  nextState <= sfetch;
			else
			  nextState <= sPutChar2;
			end if;
		--Cykly
		when sWhileB1 =>
			mx1 <= '1';
			DATA_RDWR <= '0';
			DATA_EN <= '1';
			pc_inc <= '1';
			if (DATA_RDATA = 0) then
				nextState <= sWhileB2;
			else
				nextState <= sfetch;
			end if;
		when sWhileB2 =>
			mx1 <= '0';
			DATA_EN <= '1';
			nextState <= sWhileB3;
		when sWhileB3 =>
			pc_inc <= '1';
			if (ireg_dec = whileE) then			
				nextState <= sfetch;
			else
				nextState <= sWhileB2;
			end if;
		when sWhileE1 =>
			mx1 <= '1';
			DATA_RDWR <= '0';
			DATA_EN <= '1';
			nextState <= sWhileE2;
		when sWhileE2 =>
			if (DATA_RDATA = 0) then
				pc_inc <= '1';
				nextState <= sfetch;
			else
				pc_dec <= '1';
				nextState <= sWhileE3;
			end if;
		when sWhileE3 =>
			mx1<= '0';
			DATA_EN <= '1';
			nextState <= sWhileE4;	
		when sWhileE4 =>
			if (ireg_dec = whileB) then
				nextState <= sfetch;
			else
				pc_dec <= '1';
				nextState <= sWhileE3;
			end if;
		when sNop =>
			pc_inc <= '1';
			nextState <= sidle;
		when shalt =>
			nextState <= shalt;
		when others =>
			nextState <= sNop;
	end case;
	
end process;
end behavioral;
 
