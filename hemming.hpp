#ifndef HEMMING_HPP
#define HEMMING_HPP

#include <bitset>
#include <cmath>

namespace hemming {

	template <std::size_t info_len>
	class Hemming
	{
		private:
			constexpr static std::size_t correct_len = 
				int(std::floor(std::log2(info_len))) + 1;
			constexpr static std::size_t all_len = info_len + correct_len;
			typedef std::bitset<sizeof(std::size_t)> bitset_iter;
		public:
			typedef std::bitset<all_len> bitset_all;
			typedef std::bitset<info_len> bitset_info;
			typedef std::bitset<correct_len> symptome_t;
			bitset_all encode(bitset_info input) {
				bitset_all buffer = 0;
				std::size_t iter_all = 0;
				std::size_t iter_info = 0;
				while (iter_all < all_len) {
					if (bitset_iter(iter_all + 1).count() == 1) {
						buffer[iter_all++] = false;
						continue;
					}
					buffer[iter_all++] = input[iter_info++];
				}
				for (std::size_t i = 1; i <= all_len; i <<= 1) {
					std::size_t j = (i + 1) | i;
					//std::cout << bitset_iter(i).to_string() << " " << bitset_iter(j).to_string() << " " << buffer.to_string() << std::endl;
					buffer[i - 1] = buffer[j - 1];
					for (j = (j + 1) | i; j <= all_len; j = (j + 1) | i) {
					//std::cout << bitset_iter(i).to_string() << " " << bitset_iter(j).to_string() << " " << buffer.to_string() << std::endl;
						buffer[i - 1] = buffer[i - 1] ^ buffer[j - 1];
					}
					//std::cout << i << " " << buffer[i-1] << std::endl;
				}
				return buffer;
			}
			symptome_t check(const bitset_all encoded) {
				symptome_t result = 0;
				for (std::size_t i = 1, k = 0; i <= all_len; i <<= 1) {
					std::size_t j = i;
					//std::cout << bitset_iter(i).to_string() << " " << bitset_iter(j).to_string() << " " << result.to_string() << std::endl;
					result[k] = encoded[j - 1];
					for (j = (j + 1) | i; j <= all_len; j = (j + 1) | i) {
						result[k] = result[k] ^ encoded[j - 1];
						//std::cout << bitset_iter(i).to_string() << " " << bitset_iter(j).to_string() << " " << result.to_string() << std::endl;
					}
					++k;
				}
				return result;
			}
			bitset_info decode(bitset_all encoded) {
				auto syndrom = check(encoded).to_ullong();
				bitset_info rez;
				if (syndrom) {
					encoded.flip(syndrom-1);
				}
				for (std::size_t i = 0, k = 0; i < all_len; i++) {
					if (bitset_iter(i + 1).count() == 1) {
						continue;
					}
					rez[k++] = encoded[i];
				}
				return rez;
			}
	};


}

#endif //HEMMING_HPP