#include <iostream>
#include <string.h>
#include <chrono>
#include <vector>
#include <random>

void BubbleSort(std::vector<int>& arr)
{
	const size_t arrSize = arr.size();
	bool swapped = false;

	for (size_t i = 0; i < arrSize; i++)
	{
		swapped = false;
		for (size_t j = 0; j < arrSize - i - 1; j++)
		{
			if (arr[j] > arr[j + 1])
			{
				int aux = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = aux;

				swapped = true;
			}
		}

		if (swapped == false) {
			break;
		}
	}
}

void InsertionSort(std::vector<int>& arr)
{
	const size_t arrSize = arr.size();

	for (size_t i = 1; i < arrSize; i++) {
		if (arr[i] < arr[i - 1]) {
			int key = arr[i], currPos = i;
			while (key < arr[currPos - 1]) {
				arr[currPos] = arr[currPos - 1];
				currPos--;

				if (currPos == 0) {
					break;
				}
			}
			arr[currPos] = key;
		}
	}
}

void SelectionSort(std::vector<int>& arr)
{
	for (std::vector<int>::iterator it1 = arr.begin(); it1 != arr.end() - 1; it1++) {
		std::vector<int>::iterator minElem = it1;

		for (std::vector<int>::iterator it2 = it1 + 1; it2 != arr.end(); it2++) {
			if (*minElem > *it2) {
				minElem = it2;
			}
		}

		int aux = *minElem;
		*minElem = *it1;
		*it1 = aux;
	}
}

size_t PartitionQuick(const size_t startIndex, const size_t endIndex, std::vector<int>& arr) {
	size_t randomPivotPlace = startIndex + (endIndex - startIndex) / 2;
	int pivot = arr[randomPivotPlace];
	int aux = arr[randomPivotPlace];
	arr[randomPivotPlace] = arr[endIndex];
	arr[endIndex] = aux;

	size_t pivotPlace = startIndex;

	for (size_t i = startIndex; i < endIndex; i++) {
		if (arr[i] < pivot) {
			aux = arr[i];
			arr[i] = arr[pivotPlace];
			arr[pivotPlace++] = aux;
		}
	}

	aux = arr[endIndex];
	arr[endIndex] = arr[pivotPlace];
	arr[pivotPlace] = aux;

	return pivotPlace;
}

void QuickSort(const size_t startIndex, const size_t endIndex, std::vector<int>& arr)
{
	if (startIndex < endIndex) {
		const size_t partitionIndex = PartitionQuick(startIndex, endIndex, arr);

		if (partitionIndex != 0) {
			if (partitionIndex == endIndex) {
				QuickSort(startIndex, partitionIndex - 1, arr);
			}
			else if (endIndex - partitionIndex < partitionIndex - startIndex) {
				QuickSort(startIndex, partitionIndex - 1, arr);
				QuickSort(partitionIndex + 1, endIndex, arr);
			}
			else {
				QuickSort(partitionIndex + 1, endIndex, arr);
				QuickSort(startIndex, partitionIndex - 1, arr);
			}
		}
		else {
			QuickSort(partitionIndex + 1, endIndex, arr);
		}
	}
}

std::pair<std::vector<int>::iterator, std::vector<int>::iterator> Merge(std::vector<int>::iterator itLeft, std::vector<int>::iterator itLeftEnd, std::vector<int>::iterator itRight, std::vector<int>::iterator itRightEnd) {
	std::pair<std::vector<int>::iterator, std::vector<int>::iterator> arrPair(itLeft, itRightEnd - 1);

	std::vector<int> arr;
	while (itLeft != itLeftEnd && itRight != itRightEnd) {
		if (*itLeft < *itRight) {
			arr.push_back(*itLeft);
			itLeft++;
		}
		else {
			arr.push_back(*itRight);
			itRight++;
		}
	}

	for (; itLeft != itLeftEnd; itLeft++) {
		arr.push_back(*itLeft);
	}

	for (; itRight != itRightEnd; itRight++) {
		arr.push_back(*itRight);
	}

	int i = 0;
	for (std::vector<int>::iterator it = arrPair.first; it != itRightEnd; it++, i++) {
		*it = arr[i];
	}

	return arrPair;
}

std::pair<std::vector<int>::iterator, std::vector<int>::iterator> MergeSort(std::vector<int>::iterator itArrBegin, std::vector<int>::iterator itArrEnd) {
	if (itArrEnd - itArrBegin < 1) {
		return std::pair<std::vector<int>::iterator, std::vector<int>::iterator>(itArrBegin, itArrEnd);
	}

	const size_t midPoint = (itArrEnd - itArrBegin) / 2;

	std::vector<int>::iterator leftSideBegin = itArrBegin,
		leftSideEnd = itArrBegin + midPoint,
		rightSideBegin = leftSideEnd + 1,
		rightSideEnd = itArrEnd;

	std::pair<std::vector<int>::iterator, std::vector<int>::iterator> leftSide = MergeSort(leftSideBegin, leftSideEnd),
		rightSide = MergeSort(rightSideBegin, rightSideEnd);

	return Merge(leftSide.first, leftSide.second + 1, rightSide.first, rightSide.second + 1);;
}

void CountingSort(std::vector<int>& arr)
{
	int max = *max_element(arr.begin(), arr.end());
	int min = *min_element(arr.begin(), arr.end());
	int range = max - min + 1;

	std::vector<int> count(range), output(arr.size());
	for (size_t i = 0; i < arr.size(); i++) {
		count[arr[i] - min]++;
	}

	for (size_t i = 1; i < count.size(); i++) {
		count[i] += count[i - 1];
	}

	for (size_t i = arr.size() - 1; i >= 0; i--) {
		output[count[arr[i] - min] - 1] = arr[i];
		count[arr[i] - min]--;

		if (i == 0) {
			break;
		}
	}

	for (size_t i = 0; i < arr.size(); i++) {
		arr[i] = output[i];
	}
}

void RadixSort(std::vector<int>& arr)
{
	int max = arr[0];
	for (size_t i = 1; i < arr.size(); i++) {
		if (max < arr[i]) {
			max = arr[i];
		}
	}

	const size_t arrSize = arr.size();

	std::vector<int> sortedArr; sortedArr.resize(arrSize);
	for (int digitToGet = 1; max / digitToGet > 0; digitToGet *= 10) {
		int digitCount[10] = { 0 };

		for (std::vector<int>::iterator it = arr.begin(); it != arr.end(); it++) {
			digitCount[(*it / digitToGet) % 10]++;
		}

		for (int i = 1; i < 10; i++) {
			digitCount[i] += digitCount[i - 1];
		}

		for (int i = arrSize - 1; i >= 0; i--) {
			sortedArr[--digitCount[(arr[i] / digitToGet) % 10]] = arr[i];
		}

		for (int i = 0; i < arrSize; i++) {
			arr[i] = sortedArr[i];
		}
		sortedArr.clear();
		sortedArr.resize(arrSize);
	}
}

bool CheckSortedCorrectly(std::vector<int>& arr) {
	for (int i = 1; i < arr.size(); i++) {
		if (arr[i - 1] > arr[i]) {
			return false;
		}
	}

	return true;
}

bool Quit() {
	char ans;

	do {
		std::cout << "\nWould you like to quit the program?\n";
		std::cin >> ans;
	} while (!strchr("yYnN", ans));

	return (strchr("yY", ans) != nullptr);
}

void DisplayOnOff(const char* message, bool state) {
	if (state == false) {
		std::cout << message << " - off";
	}
	else {
		std::cout << message << " - on";
	}
}

void RunTests(const char algoNames[7][15], const bool* algorithms, const char testCaseNames[5][15], const bool* testCases, const int amount, const int times) {
	std::chrono::time_point<std::chrono::system_clock> startSorting;
	std::chrono::time_point<std::chrono::system_clock> endSorting;
	std::chrono::duration<double> elapsed_seconds;

	for (int i = 0; i < 5; i++) {
		if (*(testCases + i) == true) {
			std::vector<std::vector<int>> arr, copyArr;
			arr.resize(times);

			switch (i) {
			case 0: { // Already Sorted
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					(*it).resize(amount);
					for (int i = 1; i <= amount; i++) {
						(*it)[i - 1] = i;
					}
				}
				break;
			}
			case 1: { // Almost Sorted
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					(*it).resize(amount);
					for (int i = 1; i <= amount; i++) {
						(*it)[i - 1] = i;
					}

					std::mt19937 rand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
					std::uniform_int_distribution<int> numGen(0, amount - 1);

					for (int i = 1; i <= 5; i++) {
						std::swap((*it)[numGen(rand)], (*it)[numGen(rand)]);
					}
				}
				break;
			}
			case 2: { // Random
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					(*it).resize(amount);

					std::mt19937 rand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
					std::uniform_int_distribution<int> numGen(1, amount);

					for (int i = 0; i < amount; i++) {
						(*it)[i] = numGen(rand);
					}
				}
				break;
			}
			case 3: { // Random Flat
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					(*it).resize(amount);
					std::mt19937 rand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
					std::uniform_int_distribution<int> numGen(1, 3);

					for (int i = 0; i < amount; i++) {
						(*it)[i] = numGen(rand);
					}
				}
				break;
			}
			case 4: { // Reverse Sorted
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					(*it).resize(amount);
					for (int i = amount; i > 0; i--) {
						(*it)[amount - i] = i;
					}
				}
				break;
			}
			}

			copyArr = arr;

			std::cout << "\n\n\n" << testCaseNames[i] << " test cases:";

			if (*(algorithms) == true) { // Bubble Sort
				double totalTime = 0.0;
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					startSorting = std::chrono::system_clock::now();

					BubbleSort(*it);

					endSorting = std::chrono::system_clock::now();
					elapsed_seconds = endSorting - startSorting;
					double seconds = elapsed_seconds.count();

					if (!CheckSortedCorrectly(*it)) {
						std::cout << "\nArray is not sorted correctly!";
					}

					totalTime += seconds;
				}

				std::cout << '\n' << algoNames[0] << " took " << totalTime << " seconds!\n";

				arr = copyArr;
			}

			if (*(algorithms + 1) == true) { // Insertion Sort
				double totalTime = 0.0;
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					startSorting = std::chrono::system_clock::now();

					InsertionSort(*it);

					endSorting = std::chrono::system_clock::now();
					elapsed_seconds = endSorting - startSorting;
					double seconds = elapsed_seconds.count();

					if (!CheckSortedCorrectly(*it)) {
						std::cout << "\nArray is not sorted correctly!";
					}

					totalTime += seconds;
				}

				std::cout << '\n' << algoNames[1] << " took " << totalTime << " seconds!\n";

				arr = copyArr;
			}

			if (*(algorithms + 2) == true) { // Selection Sort
				double totalTime = 0.0;
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					startSorting = std::chrono::system_clock::now();

					SelectionSort(*it);

					endSorting = std::chrono::system_clock::now();
					elapsed_seconds = endSorting - startSorting;
					double seconds = elapsed_seconds.count();

					if (!CheckSortedCorrectly(*it)) {
						std::cout << "\nArray is not sorted correctly!";
					}

					totalTime += seconds;
				}

				std::cout << '\n' << algoNames[2] << " took " << totalTime << " seconds!\n";

				arr = copyArr;
			}

			if (*(algorithms + 3) == true) { // Quick Sort
				double totalTime = 0.0;
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					startSorting = std::chrono::system_clock::now();

					QuickSort(0, (*it).size() - 1, *it);

					endSorting = std::chrono::system_clock::now();
					elapsed_seconds = endSorting - startSorting;
					double seconds = elapsed_seconds.count();

					if (!CheckSortedCorrectly(*it)) {
						std::cout << "\nArray is not sorted correctly!";
					}

					totalTime += seconds;
				}

				std::cout << '\n' << algoNames[3] << " took " << totalTime << " seconds!\n";

				arr = copyArr;
			}

			if (*(algorithms + 4) == true) { // Merge Sort
				double totalTime = 0.0;
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					startSorting = std::chrono::system_clock::now();

					MergeSort((*it).begin(), (*it).end() - 1);

					endSorting = std::chrono::system_clock::now();
					elapsed_seconds = endSorting - startSorting;
					double seconds = elapsed_seconds.count();

					if (!CheckSortedCorrectly(*it)) {
						std::cout << "\nArray is not sorted correctly!";
					}

					totalTime += seconds;
				}

				std::cout << '\n' << algoNames[4] << " took " << totalTime << " seconds!\n";

				arr = copyArr;
			}

			if (*(algorithms + 5) == true) { // Counting Sort
				double totalTime = 0.0;
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					startSorting = std::chrono::system_clock::now();

					CountingSort(*it);

					endSorting = std::chrono::system_clock::now();
					elapsed_seconds = endSorting - startSorting;
					double seconds = elapsed_seconds.count();

					if (!CheckSortedCorrectly(*it)) {
						std::cout << "\nArray is not sorted correctly!";
					}

					totalTime += seconds;
				}

				std::cout << '\n' << algoNames[5] << " took " << totalTime << " seconds!\n";

				arr = copyArr;
			}

			if (*(algorithms + 6) == true) { // Radix Sort
				double totalTime = 0.0;
				for (std::vector<std::vector<int>>::iterator it = arr.begin(); it != arr.end(); it++) {
					startSorting = std::chrono::system_clock::now();

					RadixSort(*it);

					endSorting = std::chrono::system_clock::now();
					elapsed_seconds = endSorting - startSorting;
					double seconds = elapsed_seconds.count();

					if (!CheckSortedCorrectly(*it)) {
						std::cout << "\nArray is not sorted correctly!";
					}

					totalTime += seconds;
				}

				std::cout << '\n' << algoNames[6] << " took " << totalTime << " seconds!\n";

				arr = copyArr;
			}
		}
	}
}

void SelectAlgorithms(const char names[7][15], bool* algorithms) {
	int caseNum;

	do {
		system("cls");
		for (int i = 0; i < 7; i++) {
			std::cout << '\n' << i + 1 << ". "; DisplayOnOff(names[i], *(algorithms + i));
		}
		std::cout << "\n8.Continue the program."; std::cin >> caseNum;

		if (caseNum >= 1 && caseNum <= 7) {
			*(algorithms + caseNum - 1) = !(*(algorithms + caseNum - 1));
		}
	} while (caseNum != 8);
}

void SelectTestCases(const char names[6][15], bool* testCases, int &amount, int &times) {
	int caseNum;

	do {
		for (int i = 0; i < 5; i++) {
			std::cout << '\n' << i + 1 << ". "; DisplayOnOff(names[i], *(testCases + i));
		}
		std::cout << "\n6.Continue the program."; std::cin >> caseNum;

		if (caseNum >= 1 && caseNum <= 5) {
			*(testCases + caseNum - 1) = !(*(testCases + caseNum - 1));
		}
	} while (caseNum != 6);

	do {
		std::cout << "\nHow many elements should the program run on?\n";
		std::cin >> amount;
	} while (amount < 2);

	do {
		std::cout << "\nHow many times should each sorting algorithm run?\n";
		std::cin >> times;
	} while (times < 1);
}

int main()
{
	const char algoNames[7][15] = {"Bubble Sort", "Insertion Sort", "Selection Sort", "Quick Sort", "Merge Sort", 
		"Counting Sort", "Radix Sort"};
	const char testCaseNames[5][15] = { "Already Sorted", "Almost Sorted", "Random", "Random Flat", "Reverse Sorted" };

	bool algorithms[7] = { true, true, true, true, true, true, true};
	bool testCases[5] = { true, true, true, true, true };

	int amount = 10, times = 10;

	do {
		int caseNum = 0;

		do {
			system("cls");
			std::cout << "\nPlease select your option:";
			std::cout << "\n1. Run tests.";
			std::cout << "\n2. Select sorting algorithms. (Default all algorithms on)";
			std::cout << "\n3. Select test cases, times and amount. (Default all cases on 10 unsigned ints 10 times)";
			std::cin >> caseNum;
		} while (caseNum < 1 || caseNum > 3);

		switch (caseNum) {
		case 1: {
			if (times == 0) {
				std::cout << "\nPlease select your desired test cases, amount and times!";
			}
			else {
				RunTests(algoNames, algorithms, testCaseNames, testCases, amount, times);
			}
			break;
		}

		case 2: {
			SelectAlgorithms(algoNames, algorithms);
			break;
		}

		case 3: {
			SelectTestCases(testCaseNames, testCases, amount, times);
			break;
		}
		default:
			std::cout << "\nERROR: wrong caseNum!";
		}
	} while (!Quit());
	return 0;
}