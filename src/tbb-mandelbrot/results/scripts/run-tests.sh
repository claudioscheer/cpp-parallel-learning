#!/bin/bash

export MANDEL_NO_DISPLAY=1
make cleanall && make

image_dimensions=2048
mandelbrot_iterations=10240
retries=5
threads=($(seq 1 1 24))

for t in "${threads[@]}"; do
    echo "Testing 'map' with $t threads..."
    ./map $image_dimensions $mandelbrot_iterations $retries $t > results/raw/map-$t.txt
    echo "Testing 'pipeline_with_farm' with $t threads..."
    ./pipeline_with_farm $image_dimensions $mandelbrot_iterations $retries $t > results/raw/pipeline_with_farm-$t.txt
    echo "Testing 'pipeline_with_map' with $t threads..."
    ./pipeline_with_map $image_dimensions $mandelbrot_iterations $retries $t > results/raw/pipeline_with_map-$t.txt
    echo "Testing 'pipeline_with_farm_map' with $t threads..."
    ./pipeline_with_farm_map $image_dimensions $mandelbrot_iterations $retries $t > results/raw/pipeline_with_farm_map-$t.txt
done

echo "Testing 'seq'..."
./seq $image_dimensions $mandelbrot_iterations $retries > results/raw/seq-1.txt
echo "Testing 'pipeline'..."
./pipeline $image_dimensions $mandelbrot_iterations $retries > results/raw/pipeline-1.txt

threads=($(seq 2 1 24))
for t in "${threads[@]}"; do
    echo "Average on 5 experiments = 0.0 (ms) Std. Dev. 0.0" > results/raw/seq-$t.txt
    echo "Average on 5 experiments = 0.0 (ms) Std. Dev. 0.0" > results/raw/pipeline-$t.txt
done