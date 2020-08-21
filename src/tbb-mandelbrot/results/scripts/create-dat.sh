rm data.dat

parallel_patterns_header=(Sequencial Pipeline Map Pipeline-Farm Pipeline-Map Pipeline-Farm-Map)
parallel_patterns=(seq pipeline map pipeline_with_farm pipeline_with_map pipeline_with_farm_map)
threads=($(seq 1 1 24))

printf "Threads\t" >> data.dat
for pp in "${parallel_patterns_header[@]}"; do
    printf "$pp\t" >> data.dat
    printf "$pp-std-dev\t" >> data.dat
done

printf "\n" >> data.dat

for t in "${threads[@]}"; do
    printf "$t\t" >> data.dat
    for pp in "${parallel_patterns[@]}"; do
        output=$(cat raw/$pp-$t.txt | grep "Average on 5 experiments =" | grep -Po "[0-9]*[.][0-9]+")
        for i in $output; do
            printf "$i\t" >> data.dat
        done
    done
    printf "\n" >> data.dat
done