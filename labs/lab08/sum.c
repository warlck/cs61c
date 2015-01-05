#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif


#define CLOCK_RATE_GHZ 2.26e9

static __inline__ uint64_t RDTSC()
{
    uint32_t hi, lo;
    __asm__ volatile
	(
		"rdtsc"
		: "=a"(lo), "=d"(hi)
	);
	return (((uint64_t)hi) << 32) | ((uint64_t)lo);
}

static int sum_naive(int n, int *a)
{
    int sum = 0;
	for (int i = 0; i < n; i++)  //iterates n times
	{
		sum += a[i];      //adding once so 1 * n =  n instructions
	}
	return sum;
}

static int sum_unrolled(int n, int *a)
{
    int sum = 0;

    // unrolled loop
	for (int i = 0; i < n / 4 * 4; i += 4)
    {
        sum += a[i+0];
        sum += a[i+1];
        sum += a[i+2];
        sum += a[i+3];
    }

    // tail case   (the array minus its first element)
	for (int i = n / 4 * 4; i < n; i++)
	{
		sum += a[i];
	}

    return sum;
}

static int sum_vectorized(int n, int *a)  //so are we unrolling in this procedure
{
    // WRITE YOUR VECTORIZED CODE HERE
	__m128i sum = _mm_setzero_si128();
	int temp[4];
	// 10 / 2 = 2 * 4 = 8 which is the perfect biggest number n is divisible by 4
	for (int i = 0; i < n / 4 * 4; i += 4) { //diving by four since you only need to look at every four values of n * 4 to keep it as a whole number
		__m128i vect = _mm_loadu_si128((__m128i*)(a + i));//a is pointer being shifted by i amount eacht time
		sum = _mm_add_epi32(vect, sum);
	}
	_mm_storeu_si128((__m128i*)temp, sum); //storing values computed in general vector addition that is divisible by four,
	int result = temp[0] + temp[1] + temp[2] + temp[3]; // and storing in temp array to prepare for modification in tail case, four indices since we are iterating over every four values
	for (int i = n / 4 * 4; i < n; i++) {//handle tail cases ex of 10, we still have left over 9 & 10
		result += a[i];
	}
	return result;
}

static int sum_vectorized_unrolled(int n, int *a)  //factor of 2 further increase of performance
{
    // UNROLL YOUR VECTORIZED CODE HERE
    __m128i sum = _mm_setzero_si128();
	int temp[4];
	for (int i = 0; i < n / 16 * 16; i += 16) { //now 16 since we want move four times as fast through indices
		__m128i vect0 = _mm_loadu_si128((__m128i*)(a+i));
        __m128i vect1 = _mm_loadu_si128((__m128i*)(a+i+4));
        __m128i vect2 = _mm_loadu_si128((__m128i*)(a+i+8));
        __m128i vect3 = _mm_loadu_si128((__m128i*)(a+i+12));
		sum = _mm_add_epi32(vect0, sum);
		sum = _mm_add_epi32(vect1, sum);
		sum = _mm_add_epi32(vect2, sum);
		sum = _mm_add_epi32(vect3, sum);
	}
	_mm_storeu_si128((__m128i*) temp, sum);
	int result = temp[0] + temp[1] + temp[2] + temp[3];
	for (int i = n / 16 * 16; i < n; i++) { //tail case still
		result += a[i];
	}
	return result;	
}

void benchmark(int n, int *a, int(*computeSum)(int, int*), char *name)
{
    // warm up cache
    int sum = computeSum(n, a);
    // measure
    uint64_t beginCycle = RDTSC();
    sum += computeSum(n, a);
	uint64_t cycles = RDTSC() - beginCycle;
    
    double microseconds = cycles/CLOCK_RATE_GHZ*1e6;
    
    // print results
	printf("%20s: ", name);
	if (sum == 2 * sum_naive(n, a))
	{
		printf("%.2f microseconds\n", microseconds);
	}
	else
	{
		printf("ERROR!\n");
	}
}

int main(int argc, char **argv)
{
    const int n = 7777;
    
    // initialize the array with random values
	srand48(time(NULL));
	int a[n] __attribute__((aligned(16)));
	for (int i = 0; i < n; i++)
	{
		a[i] = lrand48();
	}
    
    // benchmark series of codes
	benchmark(n, a, sum_naive, "naive");
	benchmark(n, a, sum_unrolled, "unrolled");
	benchmark(n, a, sum_vectorized, "vectorized");
	benchmark(n, a, sum_vectorized_unrolled, "vectorized unrolled");

    return 0;
}




