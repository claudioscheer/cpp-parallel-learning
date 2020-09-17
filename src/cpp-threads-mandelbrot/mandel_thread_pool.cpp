#include "marX2.h"
#include "thread_pool.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include <vector>

#define DIM 800
#define ITERATION 1024

struct Line {
    unsigned char *M;
    int i;
};

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
    double step = range / ((double)dim);

    printf("Mandelbrot set from (%g+I %g) to (%g+I %g)\n", init_a, init_b,
           init_a + range, init_b + range);
    printf("Resolution %d pixels, Max. n. of iterations %d\n", dim * dim,
           niter);

#if !defined(NO_DISPLAY)
    SetupXWindows(dim, dim, 1, NULL, "Mandelbrot map(seq)");
#endif

    ThreadPool thread_pool(max_threads);
    std::vector<std::future<Line *>> lines;

    for (int r = 0; r < retries; r++) {
        // Start time.
        gettimeofday(&t1, NULL);
        for (int i = 0; i < dim; i++) {
            lines.emplace_back(
                thread_pool.enqueue([&dim, &init_a, &init_b, &niter, &step, i] {
                    unsigned char *M = (unsigned char *)malloc(dim);
                    double im = init_b + (step * i);
                    for (int j = 0; j < dim; j++) {
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
                    Line *line = new Line{.M = M, .i = i};
                    return line;
                }));
        }
        for (auto &&line_future : lines) {
            Line *line = line_future.get();
#if !defined(NO_DISPLAY)
            ShowLine(line->M, dim, line->i);
#endif
            free(line->M);
            delete line;
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
