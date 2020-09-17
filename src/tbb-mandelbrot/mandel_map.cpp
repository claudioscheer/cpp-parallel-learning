#include "marX2.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <tbb/global_control.h>
#include <tbb/parallel_for.h>

#define DIM 800
#define ITERATION 1024

double diffmsec(struct timeval a, struct timeval b) {
    long sec = (a.tv_sec - b.tv_sec);
    long usec = (a.tv_usec - b.tv_usec);
    if (usec < 0) {
        --sec;
        usec += 1000000;
    }
    return ((double)(sec * 1000) + (double)usec / 1000.0);
}

int main(int argc, char **argv) {
#if !defined(NO_DISPLAY)
    XInitThreads();
#endif
    printf("#map(seq) pattern implementation!\n");
    double init_a = -2.125, init_b = -1.5, range = 3.0;
    int dim = DIM, niter = ITERATION;
    // Stats.
    struct timeval t1, t2;
    int retries = 1;
    double averageTime = 0;
    int max_threads = 12;

    if (argc < 5) {
        printf("Usage: ./map [size of the image] [number of iterations] "
               "[number of retries] [number of threads]\n\n");
    } else {
        dim = atoi(argv[1]);
        niter = atoi(argv[2]);
        retries = atoi(argv[3]);
        max_threads = atoi(argv[4]);
    }
    double *runs = (double *)malloc(retries * sizeof(double));
    unsigned char *M = (unsigned char *)malloc(dim);
    double step = range / ((double)dim);

    printf("Mandelbrot set from (%g+I %g) to (%g+I %g)\n", init_a, init_b,
           init_a + range, init_b + range);
    printf("Resolution %d pixels, Max. n. of iterations %d\n", dim * dim,
           niter);

#if !defined(NO_DISPLAY)
    SetupXWindows(dim, dim, 1, NULL, "Mandelbrot map(seq)");
#endif

    /*
        Limit total number of worker threads that can be active in the task
       scheduler. With max_allowed_parallelism set to 1, global_control enforces
       serial execution of all tasks by the application thread(s), i.e. the task
       scheduler does not allow worker threads to run.

        https://software.intel.com/en-us/node/589744
    */
    tbb::global_control c(tbb::global_control::max_allowed_parallelism,
                          max_threads);

    for (int r = 0; r < retries; r++) {
        // Start time.
        gettimeofday(&t1, NULL);
        // Map each line in parallel.
        for (int i = 0; i < dim; i++) {
            double im = init_b + (step * i);
            tbb::parallel_for(
                tbb::blocked_range<int>(0, dim),
                [M, &im, &niter, &init_a,
                 &step](tbb::blocked_range<int> range) {
                    for (int j = range.begin(); j != range.end(); j++) {
                        double a, cr;
                        a = cr = init_a + step * j;
                        double b = im;
                        int k = 0;
                        for (k = 0; k < niter; k++) {
                            double a2 = a * a;
                            double b2 = b * b;
                            if ((a2 + b2) > 4.0) {
                                break;
                            }
                            b = 2 * a * b + im;
                            a = a2 - b2 + cr;
                        }
                        M[j] = (unsigned char)255 - ((k * 255 / niter));
                    }
                });
#if !defined(NO_DISPLAY)
            ShowLine(M, dim, i);
#endif
        }
        // Stop time.
        gettimeofday(&t2, NULL);
        averageTime += runs[r] = diffmsec(t2, t1);
        printf("Run [%d] DONE, time= %f (ms)\n", r, runs[r]);
    }
    averageTime = averageTime / (double)retries;
    double variance = 0;
    for (int r = 0; r < retries; r++) {
        variance += (runs[r] - averageTime) * (runs[r] - averageTime);
    }
    variance /= retries;
    printf("Average on %d experiments = %f (ms) Std. Dev. %f\n\nPress any "
           "key...\n",
           retries, averageTime, sqrt(variance));

#if !defined(NO_DISPLAY)
    getchar();
    CloseXWindows();
#endif

    return 0;
}
