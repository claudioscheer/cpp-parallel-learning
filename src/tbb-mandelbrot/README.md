## Compilation

To compile all the programs, there is a Makefile. You just need to run the following command.

```bash
make
```


## Recompilation

To recompile the programs, you need to clean the old files and compile again with the following commands.

```bash
make cleanall && make
```


## Display informations
	
When you want to run experiments, you can disable the display by setting the following environment variable and compiling the programs again.

```bash
export MANDEL_NO_DISPLAY=1
```


*When you want to run again without the display, unset the environment variable using the following command.*

```bash
unset MANDEL_NO_DISPLAY
```


## Execution

Parameters:

```bash
./seq [size of the image] [number of iterations] [number of retries]
```

Example:

```bash
./seq 1000 20000 1
```