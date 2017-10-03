#ifndef PRINTER_H_
#define PRINTER_H_

#include <iostream>
#include <string>

class Printer {
public:
	Printer() = default;
	virtual ~Printer() = default;

	virtual void Print(const std::string &path, std::string str) const {
		ReplaceCharacterWithCharacter(&str, kTab, "\\t");
		ReplaceCharacterWithCharacter(&str, kNewLine, "\\n");
		std::cout << path << str << "\n";
	}

private:
	static void ReplaceCharacterWithCharacter(std::string *str, const std::string &c, const std::string &cc) {
		std::string::size_type pos = 0;
		while ((pos = str->find(c, pos)) != std::string::npos) {
			str->replace(pos, 1, cc);
			++pos;
		}
	}

	const std::string kTab {9};				//ascii tab
	const std::string kNewLine {10};	//ascii new line
};

#endif /* PRINTER_H_ */
