#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <iostream>
#include <thread>
#include <vector>
#include "marX2.h"
#include "queue.h"

#define DIM 800
#define ITERATION 1024

struct Line
{
    unsigned char *M;
    int i;
};

struct Data
{
    int current_i;
    int dim;
    double init_a;
    double init_b;
    int niter;
    double step;
    int max_threads;
};

void stage_1(Data *data, utils::queue<Line *> *queue_out)
{
    while (data->current_i < data->dim)
    {
        // Return the line to be processed.
        int i = data->current_i++;
        Line *line = new Line{.i = i};
        queue_out->push(line);
    }
    for (int i = 0; i < data->max_threads; i++)
    {
        Line *line = new Line{.i = -1};
        queue_out->push(line);
    }
}

void stage_2(Data *data, utils::queue<Line *> *queue_in, utils::queue<Line *> *queue_out)
{
    while (true)
    {
        Line *line = queue_in->pop();
        if (line->i == -1)
        {
            queue_out->push(line);
            break;
        }

        line->M = (unsigned char *)malloc(data->dim);

        double im = data->init_b + (data->step * line->i);
        for (int j = 0; j < data->dim; j++)
        {
            double a, cr;
            a = cr = data->init_a + data->step * j;
            double b = im;
            int k = 0;
            for (k = 0; k < data->niter; k++)
            {
                double a2 = a * a;
                double b2 = b * b;
                if ((a2 + b2) > 4.0)
                {
                    break;
                }
                b = 2 * a * b + im;
                a = a2 - b2 + cr;
            }
            line->M[j] = (unsigned char)255 - ((k * 255 / data->niter));
        }

        queue_out->push(line);
    }
}

void stage_3(Data *data, utils::queue<Line *> *queue_in)
{
    int threads_finished = 0;
    while (true)
    {
        Line *line = queue_in->pop();
        if (line->i == -1)
        {
            threads_finished++;
            if (threads_finished == data->max_threads)
            {
                break;
            }
        }
        else
        {
#if !defined(NO_DISPLAY)
            ShowLine(line->M, data->dim, line->i);
#endif
            free(line->M);
        }
        delete line;
    }
}

double diffmsec(struct timeval a, struct timeval b)
{
    long sec = (a.tv_sec - b.tv_sec);
    long usec = (a.tv_usec - b.tv_usec);
    if (usec < 0)
    {
        --sec;
        usec += 1000000;
    }
    return ((double)(sec * 1000) + (double)usec / 1000.0);
}

int main(int argc, char **argv)
{
#if !defined(NO_DISPLAY)
    XInitThreads();
#endif
    printf("#farm(E(seq), W(seq), C(seq)) pattern implementation!\n");
    double init_a = -2.125, init_b = -1.5, range = 3.0;
    int dim = DIM, niter = ITERATION;
    // Stats.
    struct timeval t1, t2;
    int retries = 1;
    double averageTime = 0;
    int max_threads = 12;

    if (argc < 5)
    {
        printf("Usage: ./farm [size of the image] [number of iterations] [number of retries] [number of threads]\n\n");
    }
    else
    {
        dim = atoi(argv[1]);
        niter = atoi(argv[2]);
        retries = atoi(argv[3]);
        max_threads = atoi(argv[4]);
    }
    double *runs = (double *)malloc(retries * sizeof(double));
    double step = range / ((double)dim);

    printf("Mandelbrot set from (%g+I %g) to (%g+I %g)\n",
           init_a, init_b, init_a + range, init_b + range);
    printf("Resolution %d pixels, Max. n. of iterations %d\n", dim * dim, niter);

#if !defined(NO_DISPLAY)
    SetupXWindows(dim, dim, 1, NULL, "Mandelbrot farm(E(seq), W(seq), C(seq))");
#endif

    Data *data = new Data{
        .current_i = 0,
        .dim = dim,
        .init_a = init_a,
        .init_b = init_b,
        .niter = niter,
        .step = step,
        .max_threads = max_threads,
    };

    for (int r = 0; r < retries; r++)
    {
        std::vector<std::thread> threads;
        utils::queue<Line *> queue1;
        utils::queue<Line *> queue2;

        // Start time.
        gettimeofday(&t1, NULL);
        // Generate the work.
        threads.emplace_back(stage_1, data, &queue1);
        // Create a thread farm.
        for (int t = 0; t < max_threads; t++)
        {
            threads.emplace_back(stage_2, data, &queue1, &queue2);
        }
        // Show the lines.
        threads.emplace_back(stage_3, data, &queue2);
        for (auto &t : threads)
        {
            t.join();
        }
        // Stop time.
        gettimeofday(&t2, NULL);
        averageTime += runs[r] = diffmsec(t2, t1);
        printf("Run [%d] DONE, time= %f (ms)\n", r, runs[r]);
    }
    averageTime = averageTime / (double)retries;
    double variance = 0;
    for (int r = 0; r < retries; r++)
    {
        variance += (runs[r] - averageTime) * (runs[r] - averageTime);
    }
    variance /= retries;
    printf("Average on %d experiments = %f (ms) Std. Dev. %f\n\nPress any key...\n", retries, averageTime, sqrt(variance));

#if !defined(NO_DISPLAY)
    getchar();
    CloseXWindows();
#endif

    return 0;
}
