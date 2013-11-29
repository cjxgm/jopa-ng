// vim: noet ts=4 sw=4 sts=0
#pragma once
#include <deque>
using namespace std;

namespace jopang
{
	class Buffer
	{
		deque<float> buf;

	public:
		void put(float data[], unsigned int len)
		{
			for (unsigned int i=0; i<len; i++)
				buf.push_back(data[i]);
			printf("+ -> %d\n", buf.size());
		}

		bool get(float data[], unsigned int len)
		{
			if (len > buf.size()) return true;	// underflow
			for (unsigned int i=0; i<len; i++) {
				data[i] = buf.front();
				buf.pop_front();
			}
			printf("- -> %d\n", buf.size());
			return false;
		}

		void put_stereo(float L[], float R[], unsigned int len)
		{
			for (unsigned int i=0; i<len; i++) {
				buf.push_back(L[i]);
				buf.push_back(R[i]);
			}
			printf("+2-> %d\n", buf.size());
		}

		bool get_stereo(float L[], float R[], unsigned int len)
		{
			if (len*2 > buf.size()) return true;	// underflow
			for (unsigned int i=0; i<len; i++) {
				L[i] = buf.front(); buf.pop_front();
				R[i] = buf.front(); buf.pop_front();
			}
			printf("-2-> %d\n", buf.size());
			return false;
		}

		// helper functions
		template <class T, unsigned int N>
		inline void put(T (&data)[N]) { put(data, N); }

		template <class T, unsigned int N>
		inline bool get(T (&data)[N]) { return get(data, N); }

		template <class T, unsigned int N>
		inline void put_stereo(T (&L)[N], T (&R)[N]) { put_stereo(L, R, N); }

		template <class T, unsigned int N>
		inline bool get_stereo(T (&L)[N], T (&R)[N]) { return get_stereo(L, R, N); }
	};
};

