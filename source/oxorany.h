/*

@author llxiaoyuan

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

#include <cstddef>
#include <utility>
#include <cstdint>

#if _WIN32 || _WIN64
#if _WIN64
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#if __GNUC__
#if __x86_64__ || __ppc64__
#define ENVIRONMENT64
#else
#define ENVIRONMENT32
#endif
#endif

#ifdef _MSC_VER
#define OXORANY_FORCEINLINE __forceinline
#else
#define OXORANY_FORCEINLINE __attribute__((always_inline)) inline
#endif

#ifndef _DEBUG
#define oxorany(any) _oxor_any_::oxor_any<decltype(_oxor_any_::typeofs(any)), _oxor_any_::array_size(any), __COUNTER__>(any, std::make_index_sequence<sizeof(decltype(any))>()).get()
#else
#define oxorany(any) (any)
#endif

namespace _oxor_any_ {
	
	size_t& X();

	size_t& Y();

	static constexpr size_t base_key = static_cast<size_t>(
		(__TIME__[7] - '0') +
		(__TIME__[6] - '0') * 10 +
		(__TIME__[4] - '0') * 60 +
		(__TIME__[3] - '0') * 600 +
		(__TIME__[1] - '0') * 3600 +
		(__TIME__[0] - '0') * 36000);

	template<uint32_t s, size_t n>
	class random_constant_32 {
		static constexpr uint32_t x = s ^ (s << 13);
		static constexpr uint32_t y = x ^ (x >> 17);
		static constexpr uint32_t z = y ^ (y << 5);
	public:
		static constexpr uint32_t value = random_constant_32<z, n - 1>::value;
	};

	template<uint32_t s>
	class random_constant_32<s, 0> {
	public:
		static constexpr uint32_t value = s;
	};

	template<uint64_t s, size_t n>
	class random_constant_64 {
		static constexpr uint64_t x = s ^ (s << 13);
		static constexpr uint64_t y = x ^ (x >> 7);
		static constexpr uint64_t z = y ^ (y << 17);
	public:
		static constexpr uint64_t value = random_constant_64<z, n - 1>::value;
	};

	template<uint64_t s>
	class random_constant_64<s, 0> {
	public:
		static constexpr uint64_t value = s;
	};

#ifdef ENVIRONMENT64
#define random_constant random_constant_64
#else
#define random_constant random_constant_32
#endif 

	template<typename T, size_t size>
	static OXORANY_FORCEINLINE constexpr size_t array_size(const T(&a)[size]) { return size; }

	template<typename T>
	static OXORANY_FORCEINLINE constexpr size_t array_size(T a) { return 0; }

	template<typename T, size_t size>
	static inline T typeofs(const T(&)[size]);

	template<typename T>
	static inline T typeofs(T);

	template<size_t key>
	static OXORANY_FORCEINLINE constexpr uint8_t encrypt_byte(uint8_t c, size_t i) {
		return static_cast<uint8_t>(((c + (key * 7)) ^ (i + key)));
	}

	template<size_t key>
	static OXORANY_FORCEINLINE constexpr uint8_t decrypt_byte(uint8_t c, size_t i) {
		//a ^ b == (a + b) - 2 * (a & b)
		size_t a = c;
		size_t b = i + key;
		//size_t a_xor_b = (a + b) - 2 * (a & b);
		size_t a_xor_b = (a + b) - ((a & b) + (b & a));
		return static_cast<uint8_t>((a_xor_b)-(key * 7));
	}

	template<size_t key>
	static OXORANY_FORCEINLINE constexpr size_t limit() {
		constexpr size_t bcf_value[] = { 1,2,3,4,5, 6,8,9,10,16, 32,40,64,66,100, 128,512,1000,1024,4096, 'a','z','A','Z','*' };
		return bcf_value[key % (sizeof(bcf_value) / sizeof(bcf_value[0]))];
	}

	template<typename return_type, size_t key, size_t size>
	static OXORANY_FORCEINLINE const return_type decrypt(uint8_t(&buffer)[size]) {
#ifndef OXORANY_DISABLE_OBFUSCATION

		uint8_t source;
		uint8_t decrypted;
		size_t stack_x;
		size_t stack_y;

	loc_start_1:
		stack_x = X();
		stack_y = Y();
	loc_start_2:
		for (size_t i = 0; i < size; i++) {
			source = buffer[i];
		loc_start_3:
			if (stack_x <= i) {
				if (stack_x < stack_y + limit<key * __COUNTER__>()) {
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);//fake
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
					//unreachable
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);
					goto loc_unreachable_9;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
					//unreachable
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);
					goto loc_unreachable_8;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
					//unreachable
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);
					goto loc_unreachable_7;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
					//unreachable
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);
					goto loc_unreachable_6;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
					//unreachable
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);
					goto loc_unreachable_5;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
					//unreachable
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);
					goto loc_unreachable_4;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
					//unreachable
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);
					goto loc_unreachable_3;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
					//unreachable
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);
					goto loc_unreachable_2;
				}
				else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
					//unreachable
					decrypted = decrypt_byte<key * __COUNTER__>(source, i);
					goto loc_unreachable_1;
				}
			loc_start_4:
				if (stack_y <= i) {
					if (stack_x < stack_y + limit<key * __COUNTER__>()) {
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);//fake
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
						//unreachable
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);
						goto loc_unreachable_1;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
						//unreachable
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);
						goto loc_unreachable_2;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
						//unreachable
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);
						goto loc_unreachable_3;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
						//unreachable
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);
						goto loc_unreachable_4;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
						//unreachable
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);
						goto loc_unreachable_5;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
						//unreachable
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);
						goto loc_unreachable_6;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
						//unreachable
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);
						goto loc_unreachable_7;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
						//unreachable
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);
						goto loc_unreachable_8;
					}
					else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
						//unreachable
						decrypted = decrypt_byte<key * __COUNTER__>(source, i);
						goto loc_unreachable_9;
					}
				loc_start_5:
					if (stack_x + stack_y <= i) {
						if (stack_x < stack_y + limit<key * __COUNTER__>()) {
							decrypted = decrypt_byte<key>(source, i);//real
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
							//unreachable
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							goto loc_unreachable_9;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
							//unreachable
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							goto loc_unreachable_8;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
							//unreachable
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							goto loc_unreachable_7;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
							//unreachable
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							goto loc_unreachable_6;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
							//unreachable
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							goto loc_unreachable_5;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
							//unreachable
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							goto loc_unreachable_4;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
							//unreachable
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							goto loc_unreachable_3;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
							//unreachable
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							goto loc_unreachable_2;
						}
						else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
							//unreachable
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							goto loc_unreachable_1;
						}
					loc_start_6:
						if (stack_x + stack_y != limit<key * __COUNTER__>()) {
							if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_1;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_2;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_3;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_4;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_5;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_6;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_7;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_8;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_9;
							}
						loc_start_7:
							if (stack_x < limit<key * __COUNTER__>()) {
								if (stack_x > stack_y + limit<key * __COUNTER__>()) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 1 + 1) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									goto loc_unreachable_9;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 2 + 1) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									goto loc_unreachable_8;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 3 + 1) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									goto loc_unreachable_7;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 4 + 1) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									goto loc_unreachable_6;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 5 + 1) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									goto loc_unreachable_5;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 6 + 1) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									goto loc_unreachable_4;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 7 + 1) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									goto loc_unreachable_3;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 8 + 1) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									goto loc_unreachable_2;
								}
								else if (stack_x == stack_y + limit<key * __COUNTER__>() % 9 + 1) {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									goto loc_unreachable_1;
								}
							loc_start_8:
								if (stack_y < limit<key * __COUNTER__>()) {
								loc_start_9:
									buffer[i] = decrypted;//assign
								}
								else {
									//unreachable
									decrypted = decrypt_byte<key * __COUNTER__>(source, i);
									decrypted += decrypted;
								loc_unreachable_1:
									buffer[i] = decrypt_byte<key * __COUNTER__>(source, i);
								loc_unreachable_2:
									stack_y++;
								loc_unreachable_3:
									i--;
								}
							}
							else {
								//unreachable
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								decrypted += buffer[i];
							loc_unreachable_4:
								buffer[i] = decrypt_byte<key * __COUNTER__>(source, i);
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
							decrypted = decrypt_byte<key * __COUNTER__>(source, i);
							decrypted -= buffer[i];
						loc_unreachable_7:
							buffer[i] = decrypt_byte<key * __COUNTER__>(source, i);
							stack_y++;
							i -= buffer[i];
							i -= stack_y;
						loc_unreachable_8:
							buffer[i] = decrypt_byte<key * __COUNTER__>(source, i);
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
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_1;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_2;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_3;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_4;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_5;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_6;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_7;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_8;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_9;
							}
							else if (stack_x == stack_y + limit<key * __COUNTER__>()) {
								continue;
							}
							else {
								stack_x = stack_y + limit<key * __COUNTER__>();
								stack_y = stack_x + limit<key * __COUNTER__>();
							}

							if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_start_1;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_start_2;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_start_3;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_start_4;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_start_5;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_start_6;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_start_7;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_start_8;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_start_9;
							}
							else if (stack_x < stack_y + limit<key * __COUNTER__>()) {
								continue;
							}
							else {
								stack_x = stack_y + limit<key * __COUNTER__>();
								stack_y = stack_x + limit<key * __COUNTER__>();
							}

							if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_9;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_8;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_7;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_6;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_5;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_4;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_3;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_2;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								decrypted = decrypt_byte<key * __COUNTER__>(source, i);
								goto loc_unreachable_1;
							}
							else if (stack_x > stack_y + limit<key * __COUNTER__>()) {
								continue;
							}
							else {
								stack_x = stack_y + limit<key * __COUNTER__>();
								stack_y = stack_x + limit<key * __COUNTER__>();
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
		for (size_t i = 0; i < size; i++) {
			buffer[i] = decrypt_byte<key>(buffer[i], i);
		}
#endif // OXORANY_DISABLE_OBFUSCATION
		return reinterpret_cast<return_type>(buffer);
	}

	static OXORANY_FORCEINLINE constexpr size_t align(size_t n, size_t a) {
		return (n + a - 1) & ~(a - 1);
	}

	template<typename any_t, size_t ary_size, size_t counter>
	class oxor_any {

		static constexpr size_t size = align(ary_size * sizeof(any_t), 16)
			+ random_constant<counter^ base_key, (counter^ base_key) % 128>::value % (16 + 1);

		static constexpr size_t key = random_constant<counter^ base_key, (size^ base_key) % 128>::value;

		uint8_t buffer[size];

	public:

		template<size_t... indices>
		OXORANY_FORCEINLINE constexpr oxor_any(const any_t(&any)[ary_size], std::index_sequence<indices...>) :
			buffer{ encrypt_byte<key>(((uint8_t*)&any)[indices], indices)... } {
		}

		OXORANY_FORCEINLINE const any_t* get() { return decrypt<const any_t*, key>(buffer); }
	};

	template<typename any_t, size_t counter>
	class oxor_any<any_t, 0, counter> {

		static constexpr size_t size = align(sizeof(any_t), 16)
			+ random_constant<counter^ base_key, (counter^ base_key) % 128>::value % (16 + 1);

		static constexpr size_t key = random_constant<counter^ base_key, (size^ base_key) % 128>::value;

		uint8_t buffer[size];

	public:

		template<size_t... indices>
		OXORANY_FORCEINLINE constexpr oxor_any(any_t any, std::index_sequence<indices...>) :
			buffer{ encrypt_byte<key>(reinterpret_cast<uint8_t*>(&any)[indices], indices)... } {
		}

		OXORANY_FORCEINLINE const any_t get() { return *decrypt<const any_t*, key>(buffer); }
	};
}
#endif