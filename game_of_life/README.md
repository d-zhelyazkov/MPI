Game of Life parallelization
 - implementation with OpenMPI.
Additional GUI is provided for displaying the pattern processing.
 - implementation with SDL

 
Required packages:
- libopenmpi-dev            //OpenMPI
- libsdl2-gfx-dev           //SDL


Required libraries:
- MPI/tools/*


Project build instructions:
SERIAL
    1. execute the command ./buildSerial
    2. a serial.out executable file is produced
PARALLEL
    1. execute the command ./buildParallel
    2. a parallel.out executable file is produced
    
    
Execution commands:
SERIAL
    command: ./serial.out <pattern_file> <iterations>
        where 
            <pattern_file> - file with the initial state of the pattern
            <iterations> - number of interations to process
        example: ./serial.out ./data/breeder1_1000_500 1000
    
    results: 
        - a text file <pattern_file>_<iterations>_serial containing the pattern state after <iterations>
            example ./data/breeder1_1000_500_1000_serial
        - timing results in the command window
    
PARALLEL
    command: mpirun -np <processes> ./parallel.out <pattern_file> <iterations>
        where
            <processes> - number of processes to utilize
                min 2 - one for the GUI and at least one for processing
            <pattern_file>, <iterations> - same as above 
        example: mpirun -np 4 ./parallel.out ./data/breeder1_1000_500 1000
        
    results: 
        - a text file <pattern_file>_<iterations>_parallel - the same as above
            example ./data/breeder1_1000_500_1000_parallel
        - timing results in the command window
        - a GUI window that displays a real-time animation of the pattern processing
        
<pattern_file> requirements:
- is an ASCII text file
- UNIX line endings
- its filename has the following format: <pattern>_<width>_<height>
    where
        <pattern> - the name of the pattern; just a normal text
        <width> - the width of the pattern
        <height> - the height of the pattern
    