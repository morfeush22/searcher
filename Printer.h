#ifndef PRINTER_H_
#define PRINTER_H_

#include <iostream>

class Printer {
public:
	Printer() = default;
	virtual ~Printer() = default;

	virtual void Print(const std::string &path, std::string str) {
		std::cout << path << str << "\n";
	}
};

#endif /* PRINTER_H_ */
