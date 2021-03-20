#ifndef VELO_CONSOLE_HPP 
#define VELO_CONSOLE_HPP 

#include <ostream>
#include <string_view>

namespace console
{
	enum class FontColor : int16_t { DEFAULT, BLACK, RED, GREEN, BROWN, BLUE, VIOLET, SEA, GRAY };

	std::ostream& operator<<(std::ostream&, FontColor);

	enum class Font { DEFAULT, BOLD, FADED, UNDERLINED, BLINKING, NON_DOLD, NON_UNDERLINED, NON_BLINKING };

	std::ostream& operator<<(std::ostream&, Font);

	enum class BackgroundColor : std::int16_t { DEFAULT, BLACK, RED, GREEN, BROWN, BLUE, VIOLET, SEA, GRAY };

	std::ostream& operator<<(std::ostream&, BackgroundColor);

	struct dimension
	{
		uint16_t row_count;
		uint16_t column_count;
	};

	void reverseColors();
	void cancelReverseColors();

	void clear();

	const dimension& getSize();

	int getWidth();

	int getHeight();

	void moveTo(int, int);

	/**
	 *  row
	 *  col
	 *  string
	 */
	void writeToPosition(int, int, std::string_view);

	char getKeyPress();
} // namespace console

#endif
