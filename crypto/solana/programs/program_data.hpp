#pragma once
#include <cstdint>
#include <iterator>
#include <vector>

namespace program {

	template<typename t>
	void emit_data(std::vector<std::uint8_t>& buffer, const t v) {
		for (auto i = 0u; i < sizeof(v); ++i) {
			buffer.emplace_back(static_cast<std::uint8_t>((v >> (i * 8u)) & 0xFF));
		}
		return;
	}
	__inline void emit_data(std::vector<std::uint8_t>& buffer, const std::vector<std::uint8_t>& v) {
		buffer.insert(buffer.end(), v.begin(), v.end());
		return;
	}
	template <class it, std::enable_if_t<std::_Is_iterator_v<it>, int> = 0>
	__inline void emit_data(std::vector<std::uint8_t>& buffer, const it& begin, const it& end) {
		for (auto i = begin; i != end; ++i) {
			buffer.emplace_back(*i);
		}
		return;
	}

}