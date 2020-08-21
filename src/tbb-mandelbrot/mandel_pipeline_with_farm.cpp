#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <iostream>
#include <tbb/pipeline.h>
#include <tbb/global_control.h>
#include "marX2.h"

#define DIM 800
#define ITERATION 1024

struct Data
{
    int current_i;
    int dim;
    double init_a;
    double init_b;
    int niter;
    double step;
};

struct Line
{
    Data *data;
    unsigned char *M;
    int i;
};

class Input
{
private:
    Data *_data;

public:
    Input(Data *data)
    {
        this->_data = data;
    }

    ~Input(){};

    Line *operator()(tbb::flow_control &fc) const
    {
        if (this->_data->current_i < this->_data->dim)
        {
            // Return the line to be processed.
            int i = this->_data->current_i++;
            Line *line = new Line{.i = i};
            line->data = this->_data;
            return line;
        }
        fc.stop();
        return NULL;
    }
};

class GenerateLine
{
public:
    Line *operator()(Line *line) const
    {
        line->M = (unsigned char *)malloc(line->data->dim);
        double im = line->data->init_b + (line->data->step * line->i);
        for (int j = 0; j < line->data->dim; j++)
        {
            double a, cr;
            a = cr = line->data->init_a + line->data->step * j;
            double b = im;
            int k = 0;
            for (k = 0; k < line->data->niter; k++)
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
            line->M[j] = (unsigned char)255 - ((k * 255 / line->data->niter));
        }
        return line;
    }
};

class Output
{
public:
    void operator()(Line *line) const
    {
#if !defined(NO_DISPLAY)
        ShowLine(line->M, line->data->dim, line->i);
#endif
        free(line->M);
        delete line;
    }
};

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
    printf("#pipeline(seq, farm(seq), seq) pattern implementation!\n");
    double init_a = -2.125, init_b = -1.5, range = 3.0;
    int dim = DIM, niter = ITERATION;
    // Stats.
    struct timeval t1, t2;
    int retries = 1;
    double averageTime = 0;
    int max_threads = 12;

    if (argc < 5)
    {
        printf("Usage: ./pipeline_with_farm [size of the image] [number of iterations] [number of retries] [number of threads]\n\n");
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
    SetupXWindows(dim, dim, 1, NULL, "Mandelbrot pipeline(seq, farm(seq), seq)");
#endif

    /*
        Limit total number of worker threads that can be active in the task scheduler.
        With max_allowed_parallelism set to 1, global_control enforces serial execution of all tasks by the application thread(s), i.e. the task scheduler does not allow worker threads to run.

        https://software.intel.com/en-us/node/589744
    */
    tbb::global_control c(tbb::global_control::max_allowed_parallelism, max_threads);

    for (int r = 0; r < retries; r++)
    {
        // Start time.
        gettimeofday(&t1, NULL);

        Data *data = new Data{.current_i = 0, .dim = dim, .init_a = init_a, .init_b = init_b, .niter = niter, .step = step};
        tbb::parallel_pipeline(max_threads * 4,
                               tbb::make_filter<void, Line *>(
                                   tbb::filter::serial, Input(data)) &
                                   tbb::make_filter<Line *, Line *>(
                                       tbb::filter::parallel, GenerateLine()) &
                                   tbb::make_filter<Line *, void>(
                                       tbb::filter::serial, Output()));

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
