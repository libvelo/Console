#include <iostream>

#include <console/console.hpp>

int main()
{
#ifdef OS_WIN
	//	_setmode(_fileno(stdout), _O_U8TEXT);
//	_setmode(_fileno(stdin), _I_U8TEXT);
//	_setmode(_fileno(stderr), _O_U8TEXT);
 	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif
	console::clear();
	std::cout << "Color manipulation:\n"
	          << console::FontColor::GREEN << "Green text\n"
	          << console::FontColor::BLUE << "Blue text\n"
	          << console::FontColor::BLACK << "Black text\n"
	          << console::FontColor::BROWN << "Brown text\n"
	          << console::FontColor::GRAY << "Gray text\n"
	          << console::FontColor::RED << "Red text\n"
	          << console::FontColor::SEA << "Sea text\n"
	          << console::FontColor::VIOLET << "Violet text\n"
	          << console::FontColor::DEFAULT << "Default text\n";
	std::cout << "\nBackground color manipulation:\n"
	          << console::BackgroundColor::VIOLET << "Violet background" << console::BackgroundColor::DEFAULT << '\n'
	          << console::BackgroundColor::SEA << "Sea background" << console::BackgroundColor::DEFAULT << '\n'
	          << console::BackgroundColor::RED << "Red background" << console::BackgroundColor::DEFAULT << '\n'
	          << console::BackgroundColor::GRAY << "Gray background" << console::BackgroundColor::DEFAULT << '\n'
	          << console::BackgroundColor::BROWN << "Brown background" << console::BackgroundColor::DEFAULT << '\n'
	          << console::BackgroundColor::BLACK << "Black background" << console::BackgroundColor::DEFAULT << '\n'
	          << console::BackgroundColor::BLUE << "Blue background" << console::BackgroundColor::DEFAULT << '\n'
	          << console::BackgroundColor::GREEN << "Green background" << console::BackgroundColor::DEFAULT << '\n'
	          << console::BackgroundColor::DEFAULT << "Default background\n";
	std::cout << "\nReverse colors:\n" << console::FontColor::RED << console::BackgroundColor::GREEN << "Red text on green background ";
	console::reverseColors();
	std::cout << " Green text on red background";
	console::cancelReverseColors();
	std::cout << console::FontColor::DEFAULT << '\n';
	std::cout << "\nFont manipulations:\n"
	          << console::Font::FADED << "Faded text" << console::Font::DEFAULT << '\n'
	          << console::Font::BLINKING << "Blinking text" << console::Font::DEFAULT << '\n'
	          << console::Font::BOLD << "Bold text" << console::Font::DEFAULT << '\n'
	          << console::Font::UNDERLINED << "Underline text" << console::Font::DEFAULT << '\n'
	          << console::Font::DEFAULT << "Default text\n";
	std::cout << "\nEnding of examples" << std::endl;
}
