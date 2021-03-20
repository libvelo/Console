#include<iostream>

#include <Windows.h>
#include <io.h>

#include "console/console.hpp"

static COORD current_dimension = {0, 0};
static HANDLE consoleHandle = INVALID_HANDLE_VALUE;

//FontColor
//			DEFAULT = -1,
//			GRAY = 0,
//			RED = FOREGROUND_RED,
//			BLUE = FOREGROUND_BLUE,
//			GREEN = FOREGROUND_GREEN,
//			INTENSITY = FOREGROUND_INTENSITY,
//			YELLOW = FOREGROUND_GREEN | FOREGROUND_RED,
//			MAGENTA = FOREGROUND_BLUE | FOREGROUND_RED,
//			CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
//			WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED

//background color
//			RED = BACKGROUND_RED,
//			BLUE = BACKGROUND_BLUE,
//			GREEN = BACKGROUND_GREEN,
//			INTENSITY = BACKGROUND_INTENSITY

inline bool is_atty(const std::ostream& stream)
{
	FILE* std_stream = nullptr;
	if(&stream == &std::cout)
		std_stream = stdout;
	else
		if((&stream == &std::cerr) || (&stream == &std::clog))
			std_stream = stderr;

	return static_cast<bool>(::_isatty(_fileno(std_stream)));
}

inline bool handleCheck(std::ostream& stream)
{
	if(consoleHandle == INVALID_HANDLE_VALUE)
	{
		if(&stream == &std::cout)
			consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		else
			if(&stream == &std::cerr)
				consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
		if(consoleHandle == INVALID_HANDLE_VALUE)
		{
			std::cerr << "Error while getting input handle\n";
			return false;
		}
	}
	return true;
}

template<typename T>
inline bool win_change_attributes(std::ostream& stream, T value)
{
	if constexpr (!std::is_same_v<T, console::FontColor> || !std::is_same_v<T, console::Font> ||
					  !std::is_same_v<T, console::BackgroundColor>)
		return false;

	HANDLE hTerminal = INVALID_HANDLE_VALUE;
	if(&stream == &std::cout)
		hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
	else
		if(&stream == &std::cerr)
			hTerminal = GetStdHandle(STD_ERROR_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO info;

	if(!GetConsoleScreenBufferInfo(hTerminal, &info))
		return false;

	if(value == T::DEFAULT)
		return static_cast<bool>(SetConsoleTextAttribute(hTerminal, info.wAttributes));

	if constexpr(std::is_same_v<T, console::FontColor>)
		info.wAttributes &= ~(info.wAttributes & 0x0F);

	if constexpr(std::is_same_v<T, console::BackgroundColor>)
		info.wAttributes &= ~(info.wAttributes & 0xF0);

	info.wAttributes |= static_cast<WORD>(value);

	return static_cast<bool>(SetConsoleTextAttribute(hTerminal, info.wAttributes));
}

std::ostream& console::operator<<(std::ostream& os, console::FontColor value)
{
	if(!is_atty(os) || !handleCheck(os))
		return os;
	SetConsoleTextAttribute(consoleHandle, static_cast<WORD>(value));
	return os;
}

std::ostream& console::operator<<(std::ostream& os, console::Font value)
{
	if(!is_atty(os) || !handleCheck(os))
		return os;
	SetConsoleTextAttribute(consoleHandle, static_cast<WORD>(value));
	return os;
}

std::ostream& console::operator<<(std::ostream& os, console::BackgroundColor value)
{
	if(!is_atty(os) || !handleCheck(os))
		return os;
	SetConsoleTextAttribute(consoleHandle, static_cast<WORD>(value));
	return os;
}

void console::reverseColors(){}

void console::cancelReverseColors(){}

void console::clear()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD tl = {0, 0};
	CONSOLE_SCREEN_BUFFER_INFO s;
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = static_cast<DWORD>(s.dwSize.X) * s.dwSize.Y;
	FillConsoleOutputCharacter(console, ' ', cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

const console::dimension& console::getSize(){ return reinterpret_cast<console::dimension&>(current_dimension); }

int console::getWidth(){ return current_dimension.X; }

int console::getHeight(){ return current_dimension.Y; }

void console::moveTo(int x, int y)
{
	SetConsoleCursorPosition(consoleHandle, {static_cast<SHORT>(x), static_cast<SHORT>(y)});
}

void console::writeToPosition(int, int, std::string_view){}

char console::getKeyPress(){ return 0; }
