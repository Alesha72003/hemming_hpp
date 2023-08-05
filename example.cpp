#include <iostream>
#include "hemming.hpp"

const int len = 4;
const int message = 0b0101;

hemming::Hemming<len> hem;
const auto encoded_message = hem.encode(message);
const int size_all = encoded_message.size();
int statistic_all[size_all];
int statistic_incorrect[size_all];

int main() {
	std::cout <<"orginal: " << encoded_message.to_string() << std::endl;
	for (std::size_t err = 1;;++err) {
		hemming::Hemming<len>::bitset_all err_bitset(err);
		int i = err_bitset.count();
		auto corrupted_message = encoded_message ^ err_bitset;
		if (hem.check(corrupted_message).any())
			++statistic_incorrect[i-1];
		++statistic_all[i-1];
		if (i == size_all) break;
	}

	std::cout << "Кол-во ошибок\tКоличество комбинаций\tКоличество найденых ошибок\t% найденных ошибок" << std::endl;
	for (int i = 0; i < size_all; i++) {
		std::cout << (i + 1) << " " << statistic_all[i] << " " << statistic_incorrect[i]  << " " << statistic_incorrect[i] * 100 / statistic_all[i] << std::endl;
	}
}