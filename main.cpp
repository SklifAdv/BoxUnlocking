#include <iostream>
#include <vector>
#include <random>
#include <time.h>

/*
You are given a locked container represented as a two-dimensional grid of boolean values (true = locked, false = unlocked).
Your task is to write an algorithm that fully unlocks the box, i.e.,
transforms the entire matrix into all false.

Implement the function:
bool openBox(uint32_t y, uint32_t x);
This function should:
	- Use the SecureBox public API (toggle, isLocked, getState).
	- Strategically toggle cells to reach a state where all elements are false.
	- Return true if the box remains locked, false if successfully unlocked.
You are not allowed to path or modify the SecureBox class.

Evaluation Criteria:
	- Functional correctness
	- Computational efficiency
	- Code quality, structure, and comments
	- Algorithmic insight and clarity
*/

class SecureBox
{
private:
	std::vector<std::vector<bool>> box;

public:

	//================================================================================
	// Constructor: SecureBox
	// Description: Initializes the secure box with a given size and 
	//              shuffles its state using a pseudo-random number generator 
	//              seeded with current time.
	//================================================================================
	SecureBox(uint32_t y, uint32_t x) : ySize(y), xSize(x)
	{
		rng.seed(time(0));
		box.resize(y);
		for (auto& it : box)
			it.resize(x);
		shuffle();
	}

	//================================================================================
	// Method: toggle
	// Description: Toggles the state at position (x, y) and also all cells in the
	//              same row above and the same column to the left of it.
	//================================================================================
	void toggle(uint32_t y, uint32_t x)
	{
		box[y][x] = !box[y][x];
		for (uint32_t i = 0; i < xSize; i++)
			box[y][i] = !box[y][i];
		for (uint32_t i = 0; i < ySize; i++)
			box[i][x] = !box[i][x];
	}

	//================================================================================
	// Method: isLocked
	// Description: Returns true if any cell 
	//              in the box is true (locked); false otherwise.
	//================================================================================
	bool isLocked()
	{
		for (uint32_t x = 0; x < xSize; x++)
			for (uint32_t y = 0; y < ySize; y++)
				if (box[y][x])
					return true;

		return false;
	}

	//================================================================================
	// Method: getState
	// Description: Returns a copy of the current state of the box.
	//================================================================================
	std::vector<std::vector<bool>> getState()
	{
		return box;
	}

private:
	std::mt19937_64 rng;
	uint32_t ySize, xSize;

	//================================================================================
	// Method: shuffle
	// Description: Randomly toggles cells in the box to 
	// create an initial locked state.
	//================================================================================
	void shuffle()
	{
		for (uint32_t t = rng() % 1000; t > 0; t--)
			toggle(rng() % ySize, rng() % xSize);
	}
};
bool gauss(std::vector<std::vector<bool>>& A, std::vector<bool>& b, std::vector<bool>& x);
std::vector<bool> elementInfluence(uint32_t y, uint32_t x, uint32_t ySize, uint32_t xSize);
std::vector<bool> boxToVector(uint32_t ySize, uint32_t xSize, SecureBox& box);
//================================================================================
// Function: openBox
// Description: Your task is to implement this function to unlock the SecureBox.
//              Use only the public methods of SecureBox (toggle, getState, isLocked).
//              You must determine the correct sequence of toggle operations to make
//              all values in the box 'false'. The function should return false if
//              the box is successfully unlocked, or true if any cell remains locked.
//================================================================================
bool openBox(uint32_t y, uint32_t x)
{
	SecureBox box(y, x);

	// PLEASE, PROVIDE YOUR SOLUTION HERE

	// Convert current box state (2D) into a flat vector of booleans
	auto stateVector = boxToVector(y, x, box);

	// Build the influence matrix for each possible toggle operation
	std::vector<std::vector<bool>> influenceMatrix;
	for (uint32_t i = 0; i < x; i++)
	{
		for (uint32_t j = 0; j < y; j++)
		{
			auto elementInfluenceVector = elementInfluence(j, i, y, x);
			influenceMatrix.push_back(elementInfluenceVector);
		}
	}

	// Solve the system using Gaussian elimination over GF(2)
	std::vector<bool> solution;
	if (gauss(influenceMatrix, stateVector, solution))
	{
		// Apply the toggle operations based on the solution vector
		for (uint32_t i = 0; i < solution.size(); i++)
		{
			if (solution[i])
			{
				// Convert linear index back to (y, x) coordinates
				box.toggle(i % y, i / y);
			}
		}
	}

	// Return whether the box is still locked after performing all operations
	return box.isLocked();
}

int main(int argc, char* argv[])
{
	uint32_t y = std::atol(argv[1]);
	uint32_t x = std::atol(argv[2]);
	bool state = openBox(y, x);

	if (state)
		std::cout << "BOX: LOCKED!" << std::endl;
	else
		std::cout << "BOX: OPENED!" << std::endl;

	return state;
}

//================================================================================
// Function: gauss
// Description: Solves a system of linear equations in GF(2) (modulo 2) using
//              Gaussian elimination. The system is represented by an influence
//              matrix and a state vector. The goal is to find a toggle sequence
//              (unlockSequence) that transforms the current state to a fully
//              unlocked state (all zeros).
//
// Parameters:
//   - influenceMatrix: a binary matrix where each row corresponds to the toggle
//                      effect of a button (i.e., which cells it affects).
//   - stateVector: a binary vector representing the current lock state of the box.
//   - unlockSequence: output vector to be filled with the sequence of toggles
//                     (1 = toggle this cell, 0 = do not toggle).
//
// Returns:
//   - true if a solution exists (box can be unlocked), false otherwise.
//================================================================================
bool gauss(std::vector<std::vector<bool>>& influenceMatrix, std::vector<bool>& stateVector, std::vector<bool>& unlockSequence)
{
	size_t nSize = influenceMatrix.size();		// Number of equations (rows)
	size_t mSize = influenceMatrix[0].size();	// Number of variables (columns)
	unlockSequence.assign(mSize, 0);			// Initialize unlock sequence with all 0s
	size_t row = 0;

	// Forward elimination
	for (size_t col = 0; col < mSize && row < nSize; ++col)
	{
		// Find pivot row with 1 in current column
		for (size_t i = row; i < nSize; ++i)
		{
			if (influenceMatrix[i][col])
			{
				// Swap current row with pivot row
				std::swap(influenceMatrix[i], influenceMatrix[row]);
				bool tmp = stateVector[i];
				stateVector[i] = stateVector[row];
				stateVector[row] = tmp;
				break;
			}
		}

		// If no pivot was found in this column, skip
		if (!influenceMatrix[row][col]) continue;

		// Eliminate the current variable from all other rows
		for (size_t i = 0; i < nSize; ++i)
		{
			if (i != row && influenceMatrix[i][col])
			{
				for (size_t j = col; j < mSize; ++j)
					influenceMatrix[i][j] = influenceMatrix[i][j] ^ influenceMatrix[row][j];

				stateVector[i] = stateVector[i] ^ stateVector[row];
			}
		}
		row++;
	}

	// Check for inconsistency: if 0 = 1 in any remaining row, no solution
	for (size_t i = row; i < nSize; ++i)
		if (stateVector[i]) return false;

	// Back-substitution: construct the unlock sequence from the row-echelon form
	for (size_t i = 0; i < row; ++i)
		for (size_t j = 0; j < mSize; ++j)
			if (influenceMatrix[i][j])
			{
				unlockSequence[j] = stateVector[i];
				break;
			}

	return true;
}

//================================================================================
// Function: boxToVector
// Description: Returns a vector representation for box state.
// 
// Parameters:
//   - ySize: number of rows in the box (height).
//   - xSize: number of columns in the box (width).
//   - box: reference to the SecureBox instance whose state is to be converted.
//
// Returns:
//   - A 1D boolean vector of size ySize * xSize where each element corresponds
//     to a cell in the box (true = locked, false = unlocked), ordered by columns.
//================================================================================
std::vector<bool> boxToVector(uint32_t ySize, uint32_t xSize, SecureBox& box)
{
	std::vector<bool> result(ySize * xSize);
	auto boxState = box.getState();
	for (uint32_t x = 0; x < xSize; x++)
	{
		for (uint32_t y = 0; y < ySize; y++)
		{
			if (boxState[y][x])
			{
				result[x * ySize + y] = true;
			}
		}
	}

	return result;
}

//================================================================================
// Function: elementInfluence
// Description: Returns a vector of boolean values. For element (y, x) in box,
// calculates every element that will change if element (y, x) been toggled and for
// those elements changes value in vector representation of a matrix to "true".
// For example: for matrix 3*3 and element (1, 1), element influence in matrix form
// looks like:
//	
//  |0  1  0|
//  |1  1  1|
//  |0  1  0|
// 
// Function returns vector [0, 1, 0, 1, 1, 1, 0, 1, 0]
// 
// Parameters:
//   - y: Row index of the toggled cell.
//   - x: Column index of the toggled cell.
//   - ySize: Total number of rows in the box.
//   - xSize: Total number of columns in the box.
//
// Returns:
//   - A 1D boolean vector of size ySize * xSize with 'true' at positions affected
//     by the toggle (entire row y and column x), encoded in column-major order.
//================================================================================
std::vector<bool> elementInfluence(uint32_t y, uint32_t x, uint32_t ySize, uint32_t xSize)
{
	std::vector<bool> result(ySize * xSize);

	for (uint32_t i = 0; i < xSize; i++)
		result[i * ySize + y] = true;
	for (uint32_t i = 0; i < ySize; i++)
		result[x * ySize + i] = true;

	return result;
}
