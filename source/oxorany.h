/*

@author llxiaoyuan https://github.com/llxiaoyuan/oxorany

MIT License

Copyright (c) 2021 Chase

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef OXORANY_H
#define OXORANY_H

#if _KERNEL_MODE
#include <ntddk.h>
typedef UINT8 _lxy__uint8_t;
typedef UINT32 _lxy__uint32_t;
typedef UINT64 _lxy__uint64_t;
typedef SIZE_T _lxy__size_t;
#else
#include <stdint.h>
typedef uint8_t _lxy__uint8_t;
typedef uint32_t _lxy__uint32_t;
typedef uint64_t _lxy__uint64_t;
typedef size_t _lxy__size_t;
#endif

#if _WIN32 || _WIN64
#if _WIN64
#define OXORANY_ENVIRONMENT64
#else
#define OXORANY_ENVIRONMENT32
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define OXORANY_ENVIRONMENT64
#else
#define OXORANY_ENVIRONMENT32
#endif
#endif

#ifdef _MSC_VER
#define OXORANY_FORCEINLINE __forceinline
#else
#define OXORANY_FORCEINLINE __attribute__((always_inline)) inline
#endif

#ifdef _DEBUG
#define oxorany
#else
#define oxorany(any) _lxy_oxor_any_::oxor_any<decltype(_lxy_oxor_any_::typeofs(any)), _lxy_oxor_any_::array_size(any), __COUNTER__>(any, _lxy_::make_index_sequence<sizeof(decltype(any))>()).get()

#ifdef OXORANY_USE_BIT_CAST
#define oxorany_flt(any) _lxy_::_Bit_cast<decltype(any)>(oxorany((_lxy_::integral_constant<typename _lxy_::type_cast<decltype(any)>::type,_lxy_::_Bit_cast<typename _lxy_::type_cast<decltype(any)>::type>(any)>::value)))
#endif // OXORANY_USE_BIT_CAST

#endif

namespace _lxy_ {

	template<typename T>
	struct type_cast { using type = T; };

	template<>
	struct type_cast<float> { using type = _lxy__uint32_t; };

	template<>
	struct type_cast<double> { using type = _lxy__uint64_t; };

	template <class _Ty, _Ty _Val>
	struct integral_constant {
		static constexpr _Ty value = _Val;

		using value_type = _Ty;
		using type = integral_constant;

		constexpr operator value_type() const noexcept {
			return value;
		}

		/*_NODISCARD*/ constexpr value_type operator()() const noexcept {
			return value;
		}
	};

#ifdef OXORANY_USE_BIT_CAST
	template <class _To, class _From>
	/*_NODISCARD*/ constexpr _To _Bit_cast(const _From& _Val) noexcept {
		return __builtin_bit_cast(_To, _Val);
	}
#endif

	// https://stackoverflow.com/a/32223343/16602611

	template <_lxy__size_t... Ints>
	struct index_sequence {
		using type = index_sequence;
		using value_type = _lxy__size_t;
		static constexpr _lxy__size_t size() noexcept { return sizeof...(Ints); }
	};

	// --------------------------------------------------------------------

	template <class Sequence1, class Sequence2>
	struct _merge_and_renumber;

	template <_lxy__size_t... I1, _lxy__size_t... I2>
	struct _merge_and_renumber<index_sequence<I1...>, index_sequence<I2...>>
		: index_sequence<I1..., (sizeof...(I1) + I2)...>
	{ };

	// --------------------------------------------------------------------

	template <_lxy__size_t N>
	struct make_index_sequence
		: _merge_and_renumber<typename make_index_sequence<N / 2>::type,
		typename make_index_sequence<N - N / 2>::type>
	{ };

	template<> struct make_index_sequence<0> : index_sequence<> { };
	template<> struct make_index_sequence<1> : index_sequence<0> { };
}

namespace _lxy_oxor_any_ {

	/*
	template <_lxy__size_t ...>
	struct indexSequence {
	};

	template <_lxy__size_t N, _lxy__size_t ... Next>
	struct indexSequenceHelper : public indexSequenceHelper<N - 1U, N - 1U, Next...> {
	};

	template <_lxy__size_t ... Next>
	struct indexSequenceHelper<0U, Next ... > {
		using type = indexSequence<Next ... >;
	};

	template <_lxy__size_t N>
	using makeIndexSequence = typename indexSequenceHelper<N>::type;
	*/

	_lxy__size_t& X();

	_lxy__size_t& Y();

	static constexpr _lxy__size_t base_key = static_cast<_lxy__size_t>(
		(__TIME__[7] - '0') +
		(__TIME__[6] - '0') * 10 +
		(__TIME__[4] - '0') * 60 +
		(__TIME__[3] - '0') * 600 +
		(__TIME__[1] - '0') * 3600 +
		(__TIME__[0] - '0') * 36000);

	template<_lxy__uint32_t s, _lxy__size_t n>
	class random_constant_32 {
		static constexpr _lxy__uint32_t x = s ^ (s << 13);
		static constexpr _lxy__uint32_t y = x ^ (x >> 17);
		static constexpr _lxy__uint32_t z = y ^ (y << 5);
	public:
		static constexpr _lxy__uint32_t value = random_constant_32<z, n - 1>::value;
	};

	template<_lxy__uint32_t s>
	class random_constant_32<s, 0> {
	public:
		static constexpr _lxy__uint32_t value = s;
	};

	template<_lxy__uint64_t s, _lxy__size_t n>
	class random_constant_64 {
		static constexpr _lxy__uint64_t x = s ^ (s << 13);
		static constexpr _lxy__uint64_t y = x ^ (x >> 7);
		static constexpr _lxy__uint64_t z = y ^ (y << 17);
	public:
		static constexpr _lxy__uint64_t value = random_constant_64<z, n - 1>::value;
	};

	template<_lxy__uint64_t s>
	class random_constant_64<s, 0> {
	public:
		static constexpr _lxy__uint64_t value = s;
	};

#ifdef OXORANY_ENVIRONMENT64
#define random_constant random_constant_64
#else
#define random_constant random_constant_32
#endif 

	template<typename T, _lxy__size_t size>
	static OXORANY_FORCEINLINE constexpr _lxy__size_t array_size(const T(&)[size]) { return size; }

	template<typename T>
	static OXORANY_FORCEINLINE constexpr _lxy__size_t array_size(T) { return 0; }

	template<typename T, _lxy__size_t size>
	static inline T typeofs(const T(&)[size]);

	template<typename T>
	static inline T typeofs(T);

	template<_lxy__size_t key>
	static OXORANY_FORCEINLINE constexpr _lxy__uint8_t encrypt_byte(_lxy__uint8_t c, _lxy__size_t i) {
		return static_cast<_lxy__uint8_t>(((c + (key * 7)) ^ (i + key)));
	}

	template<_lxy__size_t key>
	static OXORANY_FORCEINLINE constexpr _lxy__uint8_t decrypt_byte(_lxy__uint8_t c, _lxy__size_t i) {
		//a ^ b == (a + b) - 2 * (a & b)
		_lxy__size_t a = c;
		_lxy__size_t b = i + key;
		//_lxy__size_t a_xor_b = (a + b) - 2 * (a & b);
		_lxy__size_t a_xor_b = (a + b) - ((a & b) + (b & a));
		return static_cast<_lxy__uint8_t>((a_xor_b)-(key * 7));
	}

	template<_lxy__size_t key>
	static OXORANY_FORCEINLINE constexpr _lxy__size_t limit() {
		constexpr _lxy__size_t bcf_value[] = { 1,2,3,4,5, 6,8,9,10,16, 32,40,64,66,100, 128,512,1000,1024,4096, 'a','z','A','Z','*' };
		return bcf_value[key % (sizeof(bcf_value) / sizeof(bcf_value[0]))];
	}

	template<typename return_type, _lxy__size_t key, _lxy__size_t size>
	static OXORANY_FORCEINLINE const return_type decrypt(_lxy__uint8_t(&buffer)[size]) {
#ifndef OXORANY_DISABLE_OBFUSCATION

		_lxy__uint8_t source;
		_lxy__uint8_t decrypted; //do not assign initial value
		_lxy__size_t stack_x;
		_lxy__size_t stack_y;

	loc_start_1:
		stack_x = X();
		stack_y = Y();
	loc_start_2:
		for (_lxy__size_t i = 0; i < size; i++) {
			source = buffer[i];
		loc_start_3:
			if (stack_x <= i) {
				if (stack_x < stack_y + limit<key * __COUNTER__>()) {
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);//fake
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
					//unreachable
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);
					goto loc_unreachable_9;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
					//unreachable
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);
					goto loc_unreachable_8;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
					//unreachable
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);
					goto loc_unreachable_7;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
					//unreachable
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);
					goto loc_unreachable_6;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
					//unreachable
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);
					goto loc_unreachable_5;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
					//unreachable
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);
					goto loc_unreachable_4;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
					//unreachable
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);
					goto loc_unreachable_3;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
					//unreachable
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);
					goto loc_unreachable_2;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
					//unreachable
					decrypted = decrypt_byte<key* __COUNTER__>(source, i);
					goto loc_unreachable_1;
				}
			loc_start_4:
				if (stack_y <= i) {
					if (stack_x < stack_y + limit<key * __COUNTER__>()) {
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);//fake
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
						//unreachable
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);
						goto loc_unreachable_1;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
						//unreachable
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);
						goto loc_unreachable_2;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
						//unreachable
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);
						goto loc_unreachable_3;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
						//unreachable
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);
						goto loc_unreachable_4;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
						//unreachable
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);
						goto loc_unreachable_5;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
						//unreachable
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);
						goto loc_unreachable_6;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
						//unreachable
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);
						goto loc_unreachable_7;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
						//unreachable
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);
						goto loc_unreachable_8;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
						//unreachable
						decrypted = decrypt_byte<key* __COUNTER__>(source, i);
						goto loc_unreachable_9;
					}
				loc_start_5:
					if (stack_x + stack_y <= i) {
						if (stack_x < stack_y + limit<key * __COUNTER__>()) {
							decrypted = decrypt_byte<key>(source, i);//real
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							goto loc_unreachable_9;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							goto loc_unreachable_8;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							goto loc_unreachable_7;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							goto loc_unreachable_6;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							goto loc_unreachable_5;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							goto loc_unreachable_4;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							goto loc_unreachable_3;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							goto loc_unreachable_2;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							goto loc_unreachable_1;
						}
					loc_start_6:
						if (stack_x + stack_y != limit<key * __COUNTER__>()) {
							if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_1;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_2;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_3;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_4;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_5;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_6;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_7;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_8;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_9;
							}
						loc_start_7:
							if (stack_x < limit<key * __COUNTER__>()) {
								if (stack_x > stack_y + limit<key * __COUNTER__>()) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									goto loc_unreachable_9;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									goto loc_unreachable_8;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									goto loc_unreachable_7;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									goto loc_unreachable_6;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									goto loc_unreachable_5;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									goto loc_unreachable_4;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									goto loc_unreachable_3;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									goto loc_unreachable_2;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									goto loc_unreachable_1;
								}
							loc_start_8:
								if (stack_y < limit<key * __COUNTER__>()) {
								loc_start_9:
									buffer[i] = decrypted;//assign
								}
								else {
									//unreachable
									decrypted = decrypt_byte<key* __COUNTER__>(source, i);
									decrypted += decrypted;
								loc_unreachable_1:
									buffer[i] = decrypt_byte<key* __COUNTER__>(source, i);
								loc_unreachable_2:
									stack_y++;
								loc_unreachable_3:
									i--;
								}
							}
							else {
								//unreachable
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								decrypted += buffer[i];
							loc_unreachable_4:
								buffer[i] = decrypt_byte<key* __COUNTER__>(source, i);
								buffer[i] += decrypted;
							loc_unreachable_5:
								stack_x += stack_y;
							loc_unreachable_6:
								i--;
								i -= decrypted;
							}
						}
						else {
							//unreachable
							decrypted = decrypt_byte<key* __COUNTER__>(source, i);
							decrypted -= buffer[i];
						loc_unreachable_7:
							buffer[i] = decrypt_byte<key* __COUNTER__>(source, i);
							stack_y++;
							i -= buffer[i];
							i -= stack_y;
						loc_unreachable_8:
							buffer[i] = decrypt_byte<key* __COUNTER__>(source, i);
							stack_x++;
							i--;
							i -= stack_x;
						loc_unreachable_9:
							i += buffer[i];
							i += stack_y;
							continue;
						}
					}
					else {
						//unreachable
						while (true) {
							if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_1;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_2;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_3;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_4;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_5;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_6;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_7;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_8;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_9;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								continue;
							}
							else {
								stack_x = stack_y + limit<key* __COUNTER__>();
								stack_y = stack_x + limit<key* __COUNTER__>();
							}

							if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_start_1;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_start_2;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_start_3;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_start_4;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_start_5;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_start_6;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_start_7;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_start_8;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_start_9;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								continue;
							}
							else {
								stack_x = stack_y + limit<key* __COUNTER__>();
								stack_y = stack_x + limit<key* __COUNTER__>();
							}

							if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_9;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_8;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_7;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_6;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_5;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_4;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_3;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_2;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key* __COUNTER__>(source, i);
								goto loc_unreachable_1;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								continue;
							}
							else {
								stack_x = stack_y + limit<key* __COUNTER__>();
								stack_y = stack_x + limit<key* __COUNTER__>();
							}
						}
					}
				}
				else {
					//unreachable
					//Ooops, Decompilation failure:
					//401000: stack frame is too big
					return reinterpret_cast<return_type>(buffer + ((key * __COUNTER__) % 0x400000 + 0x1400000));
				}
			}
			else {
				//unreachable
				//Ooops, Decompilation failure:
				//401000: stack frame is too big
				return reinterpret_cast<return_type>(buffer + ((key * __COUNTER__) % 0x1400000 + 0x400000));
			}
		}

#else
		for (volatile _lxy__size_t i = 0; i < size; i++) {
			buffer[i] = decrypt_byte<key>(buffer[i], i);
		}
#endif // OXORANY_DISABLE_OBFUSCATION
		return reinterpret_cast<return_type>(buffer);
	}

	static OXORANY_FORCEINLINE constexpr _lxy__size_t align(_lxy__size_t n, _lxy__size_t a) {
		return (n + a - 1) & ~(a - 1);
	}

	template<typename any_t, _lxy__size_t ary_size, _lxy__size_t counter>
	class oxor_any {

		static constexpr _lxy__size_t size = align(ary_size * sizeof(any_t), 16)
			+ random_constant<counter^ base_key, (counter^ base_key) % 128>::value % (16 + 1);

		static constexpr _lxy__size_t key = random_constant<counter^ base_key, (size^ base_key) % 128>::value;

		_lxy__uint8_t buffer[size];

	public:

		template<_lxy__size_t... indices>
		OXORANY_FORCEINLINE constexpr oxor_any(const any_t(&any)[ary_size], _lxy_::index_sequence<indices...>) :
			buffer{ encrypt_byte<key>(((_lxy__uint8_t*)&any)[indices], indices)... } {
		}

		OXORANY_FORCEINLINE const any_t* get() { return decrypt<const any_t*, key>(buffer); }
	};

	template<typename any_t, _lxy__size_t counter>
	class oxor_any<any_t, 0, counter> {

		static constexpr _lxy__size_t size = align(sizeof(any_t), 16)
			+ random_constant<counter^ base_key, (counter^ base_key) % 128>::value % (16 + 1);

		static constexpr _lxy__size_t key = random_constant<counter^ base_key, (size^ base_key) % 128>::value;

		_lxy__uint8_t buffer[size];

	public:

		template<_lxy__size_t... indices>
		OXORANY_FORCEINLINE constexpr oxor_any(any_t any, _lxy_::index_sequence<indices...>) :
			buffer{ encrypt_byte<key>(reinterpret_cast<_lxy__uint8_t*>(&any)[indices], indices)... } {
		}

		OXORANY_FORCEINLINE const any_t get() { return *decrypt<const any_t*, key>(buffer); }
	};
}

#endif // OXORANY_H