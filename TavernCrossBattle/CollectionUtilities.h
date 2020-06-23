#pragma once

#include <algorithm>

namespace battle {
	namespace utilities {
		template<typename Iterator, typename Pred>
		inline bool find_if_exists(Iterator first, Iterator last, Pred closure) {
			typedef typename std::iterator_traits<Iterator>::reference Tref;
			return std::find_if(first, last, [&](const Tref x) {
				return closure(x);
				}) != last;
		}
	}
}