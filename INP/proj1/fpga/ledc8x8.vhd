library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
	port(
		SMCLK : in std_logic;
		RESET : in std_logic;
		ROW   : out std_logic_vector(0 to 7);
		LED   : out std_logic_vector(0 to 7)	
	);
end entity ledc8x8;

architecture behavioral of ledc8x8 is
	-- 5 bitovy citac
	signal citac : std_logic_vector(4 downto 0) := (others => '0');
	signal clock_enable : std_logic;

	signal row_cnt : std_logic_vector(2 downto 0) := (others => '0');
begin
	-- Delic frekvencie
	citac_gen : process(SMCLK, RESET)
	begin
		if RESET = '1' then
			citac <= (others => '0');
		elsif SMCLK'event and SMCLK = '1' then
			citac <= citac + 1;
		end if;
	end process;

	-- Paralelne priradenie
	clock_enable <= '1' when citac = "11111" else '0';

	---------------------------------------------------
	-- Citac aktualneho riadku, inkrementuje sa len ked
	-- delic frekvencie prave preteka
	row_cnt_gen : process(SMCLK, RESET)
	begin
		if RESET = '1' then
			row_cnt <= (others => '0');
		elsif SMCLK'event and SMCLK = '1' then
			if clock_enable = '1' then
				row_cnt <= row_cnt + 1;
			end if;
		end if;
	end process;
	
	---------------------------------------------------
	-- one-hot convertor
	with row_cnt select
		ROW <= "10000000" when "000",
		       "01000000" when "001",
		       "00100000" when "010",
		       "00010000" when "011",
		       "00001000" when "100",
		       "00000100" when "101",
		       "00000010" when "110",
		       "00000001" when "111",
		       "XXXXXXXX" when others;

	-- samotne rozsvietenie diody podla aktualneho riadku
	-- invertovane !
	with row_cnt select
		LED <=	"00011111" when "000",
			"01101111" when "001",
			"01101111" when "010",
			"01101110" when "011",
			"00000100" when "100",
			"11101010" when "101",
			"11101110" when "110",
			"11101110" when "111",
			"XXXXXXXX" when others;

end architecture behavioral;
