# BoxUnlocking
Solution for unlocking box represented by binary matrix
-------------------------------------------------------------------------------------------
To build and run the project, follow these steps:
1. Ensure that you have a C++ compiler (such as GCC or Clang) and CMake installed on your system.
2. Copy files into folder
3. Create a build directory within the project folder via command line:
	mkdir build
	cd build
	
4. Run CMake to configure the project and generate build files.
   From the build directory, run the following:
	cmake ..
   
   This will use the CMakeLists.txt file in the parent directory to generate the appropriate build system files for your platform.
5. Once CMake has configured the project, build it using the following command:
   (On Linux/macOS)
	make
	
   (On Windows using Visual Studio)
   Open the project in Visual Studio and build it using the IDE, or run the following command in the build directory:
	cmake --build . --config Release
	
6. Running the project: To run the program, use the following command in your terminal (in Release folder):
	./BoxUnlocking <x_size> <y_size>
	
	This will generate box (x_size)*(y_size), lock it and try to unlock (result message will be printed in terminal):
	
-------------------------------------------------------------------------------------------
Techniques Used and Why the Solution Works

This solution is based on linear algebra over the Galois Field GF(2) (also known as modulo 2 arithmetic),
a natural fit for problems involving binary states (locked/unlocked represented as true/false).
The main challenge lies in determining the correct sequence of toggle operations that unlocks all cells of a grid,
where each toggle affects an entire row and column.

Techniques Used:

- Matrix Representation of Influence:
Each toggle operation has a predictable effect: flipping all cells in a specific row and column. 
This influence is captured as a binary vector, and all possible toggle positions form a binary influence matrix.

- Flattened Boolean Vector State:
The current state of the SecureBox is represented as a single flat vector of booleans, allowing us to easily model and manipulate it in matrix form.

- Gaussian Elimination over GF(2):
To solve the system of equations representing the transformation from toggle actions to resulting locked state, 
we use Gaussian elimination tailored for binary values (XOR instead of addition/subtraction). 
This yields a toggle sequence that, when applied, results in all cells being false (unlocked).

- Toggle Execution Based on Solution Vector:
Once a valid solution is found, the code iterates through the solution vector and toggles only the required positions.

Why the Solution Works:

- The toggle behavior of the box is deterministic and linear in the binary sense: each toggle flips fixed positions regardless of previous actions.
- The entire problem reduces to solving a linear system of equations where toggles are variables, and the final unlocked state is the target outcome (a zero vector).
- Gaussian elimination over GF(2) is a proven, efficient method to solve such systems and detect whether a solution exists.
- If a solution exists, executing the corresponding toggles guarantees that the entire box will be unlocked.

-------------------------------------------------------------------------------------------
