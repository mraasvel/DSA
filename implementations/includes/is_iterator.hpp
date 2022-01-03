#pragma once

#include <type_traits>
#include <iterator>

namespace DataStructures {

	namespace Detail {

	template <typename T>
	struct MakeType {
		using type = void;
	};

	template <typename Iterator>
	struct _IsIterator {
		using valid = char[1];
		using invalid = char[2];

		template <typename T>
		static valid& test(
				typename Detail::MakeType<typename T::difference_type>::type*,
				typename Detail::MakeType<typename T::value_type>::type*,
				typename Detail::MakeType<typename T::pointer>::type*,
				typename Detail::MakeType<typename T::reference>::type*,
				typename Detail::MakeType<typename T::iterator_category>::type*);

		template <typename>
		static invalid& test(...);

		static constexpr bool value = sizeof(test<Iterator>(nullptr, nullptr, nullptr, nullptr, nullptr)) == sizeof(valid);
	};

	template <typename T>
	struct _IsIterator<T*> {
		static constexpr bool value = true;
	};

	}

/*
This is how STL checks it: only validates that the category is valid
*/
template<typename InputIterator>
using RequireInputIterator = typename std::enable_if<std::is_convertible<
				typename std::iterator_traits<InputIterator>::iterator_category, std::input_iterator_tag>::value>::type;


template <typename Iterator>
struct IsIterator : public Detail::_IsIterator<typename std::remove_cv<Iterator>::type> {};

}
