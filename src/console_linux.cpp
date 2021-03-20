#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>

#include "console/console.hpp"

//	\033[#A передвинуть курсор вверх на # строк
//	\033[#B передвинуть курсор вниз на # строк
//	\033[#С передвинуть курсор вправо на # столбцов
//	\033[#D передвинуть курсор влево на # столбцов
//	\033[#E передвинуть курсор вниз на # строк и поставить в начало строки
//	\033[#F передвинуть курсор вверх на # строк и поставить в начало строки
//	\033[#G переместить курсор в указанный столбец текущей строки
//	\033[#;#H задает абсолютные координаты курсора (строка, столбец)

static struct winsize w;

inline bool is_atty(const std::ostream& stream)
{
	FILE* std_stream = nullptr;
	if(&stream == &std::cout)
		std_stream = stdout;
	else
		if((&stream == &std::cerr) || (&stream == &std::clog))
			std_stream = stderr;

	return static_cast<bool>(::isatty(fileno(std_stream)));
}

inline void console_helper(FILE* ostream, int value, int shift = 0)
{
	fprintf(ostream,
	        "\033[%dm",
	        (value == 0 ? 0 : value + shift));
}

void findSize() { ioctl(STDOUT_FILENO, TIOCGWINSZ, &w); }

std::ostream& console::operator<<(std::ostream& os, console::FontColor color)
{
	if(is_atty(os))
		console_helper(stdout, static_cast<int>(color), 30);

	return os;
}

std::ostream& console::operator<<(std::ostream& os, console::Font value)
{
	int font = 0;
	switch(value)
	{
		case console::Font::BOLD: font = 1;
			break;
		case console::Font::NON_DOLD: font = 22;
			break;
		case console::Font::NON_UNDERLINED: font = 24;
			break;
		case console::Font::NON_BLINKING: font = 25;
			break;
		case console::Font::FADED: font = 2;
			break;
		case console::Font::UNDERLINED: font = 4;
			break;
		case console::Font::BLINKING: font = 5;
			break;
		case console::Font::DEFAULT:
		default: font = 0;
	}

	os << "\033[" << (font) << 'm';

	return os;
}

std::ostream& console::operator<<(std::ostream& os, console::BackgroundColor color)
{
	if(is_atty(os))
		console_helper(stdout, static_cast<int>(color), 40);
	return os;
}

void console::reverseColors() { console_helper(stdout, 7); }

void console::cancelReverseColors() { console_helper(stdout, 27); }

void console::clear() { printf("\033[2J\033[H"); }

const console::dimension& console::getSize()
{
	if(w.ws_col == 0)
		findSize();
	return reinterpret_cast<const console::dimension&>(w);
}

int console::getWidth()
{
	if(w.ws_col == 0)
		findSize();
	return w.ws_col;
}

int console::getHeight()
{
	if(w.ws_row == 0)
		findSize();
	return w.ws_row;
}

void console::moveTo(int x, int y) { printf("\033[%d;%dH", x, y); }

void console::writeToPosition(int x, int y, std::string_view str)
{
	moveTo(x, y);
	printf("%.*s", static_cast<int>(str.size()), str.data());
}

char console::getKeyPress()
{
	char buf = 0;
	struct termios old = {0};

	if(tcgetattr(0, &old) < 0)
		perror("tcsetattr()");

	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;

	if(tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if(read(0, &buf, 1) < 0)
		perror("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if(tcsetattr(0, TCSADRAIN, &old) < 0)
		perror("tcsetattr ~ICANON");
	return (buf);
}
