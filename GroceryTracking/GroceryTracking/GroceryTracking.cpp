#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char *procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();	
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen: 
		You sent me: Test
Return:
	100 is returned to the C++	
*/
int callIntFunc(string proc, string param)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char *paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject *pName, *pModule, *pDict, *pFunc, *pValue = nullptr, *presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char *procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject *pName, *pModule, *pDict, *pFunc, *pValue = nullptr, *presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}

// Prints the menu options as specified
void PrintMenu()
{
	cout << "1: Print frequency of all items" << endl;
	cout << "2: Print frequency of a specific item" << endl;
	cout << "3: Print a histogram of all items" << endl;
	cout << "4: Exit" << endl;
}

// This function just calls the corresponding Python function
void AllItemFreq()
{
	CallProcedure("CountAll");
}

void OneItemFreq()
{
	// We need to read in which item to look for
	string item = "";

	do
	{
		// Prompt the user for an item
		cout << "Please enter the item to count" << endl;
		getline(cin, item);
	// Loop until we get input
	} while (item.empty());

	// Pass the item to the corresponding Python function
	int freq = callIntFunc("CountOne", item);

	// If the item was found, print it and its frequency
	if (freq >= 0)
		cout << item << ": " << freq << endl;
}

// This function works between the two languages to make a histogram
void Histogram()
{
	// Call the python function
	// This creates the file "frequency.dat" with the data we'll make the histogram out of
	CallProcedure("MakeHistogram");

	// Open the file we wrote to in Python
	ifstream in_file; 
	in_file.open("frequency.dat");

	// If it opened successfully
	if (in_file.is_open())
	{
		// Read in the numbers at the beginning of the file
		// The first is the total number of items
		// This tells us how many times we should read in
		// It makes it so that we dont have to mess around with eof or anything
		int num_items = 0;
		// The second is the length of the longest item name
		// This lets us format our output to line everything up
		int longest_name = 0;
		in_file >> num_items;
		in_file >> longest_name;

		// Variables to read our items into
		string item = "";
		int frequency = 0;

		// Loop the same number of times as there are grocery items
		for (int i = 0; i < num_items; ++i)
		{
			// Read in the item and its frequency
			in_file >> item;
			in_file >> frequency;

			// Print out the item name formatted like "   item:"
			cout << setw(longest_name) << item << ": ";

			// Print out the frequency with one * per occurrence
			cout << setw(frequency) << setfill('*') << "" << endl;
			cout << setfill(' ');
		}

		// We're done, so close the file
		in_file.close();
	}

}

void HandleInput(string in)
{
	// Switch over the first char in the user input
	switch (in.at(0))
	{
	// They want the frequency of all items
	case '1':
		AllItemFreq();
		break;
	// They want the frequency of one item
	case '2':
		OneItemFreq();
		break;
	// They want the frequency of all items as a histogram
	case '3':
		Histogram();
		break;
	// They want to exit
	case '4':
		return;
	// They didn't enter a valid option
	default:
		cout << "Please enter a valid option." << endl;
		break;
	}
}

void main()
{
	// A non empty string to avoid errors on while check
	const string not_empty = " ";
	// A string to store user input
	string input = not_empty;
	// The user can enter 4 to quit
	const char exit = '4';

	// While the user hasn't chosen to quit
	while (input.at(0) != exit)
	{
		// Print the menu
		PrintMenu();

		// Get input from the user
		getline(cin, input);
		
		// If the user didn't enter anything
		if (input.empty())
		{
			// Prompt them for input and go again
			cout << "Please select an option." << endl;
			input = not_empty;
			continue;
		}

		// Otherwise, pass their input to HandleInput
		HandleInput(input);
	}
}