// Name : Taha Ahmed
// Roll Number : 23i-0532
// Section : A
// Course : DSA

#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

const int MAX_WORDS = 1000;
const int MAX_LINE_LENGTH = 1000;
const char* path = new char[100] {"C:\\Users\\ML\\Desktop\\Evaluation 1\\"};
char* temp = new char[500];

// function to copy from source to destination
void copy(const char* src, char* dest, int i = 0)
{

	for (; *(src + i) != '\0'; i++)
	{
		*(dest + i) = *(src + i);
	}
	*(dest + i) = '\0';
}

// function to find the length of the sentence
int findLength(const char* sentence)
{
	int i = 0;
	while (*(sentence + i) != '\0')
	{
		i++;
	}
	return i;
}

// function to find the number of words in a sentence
int findWords(const char* sentence)
{
	int words = 0;
	for (int i = 0; *(sentence + i) != '\0'; i++)
	{
		if (*(sentence + i) == ' ')
		{
			words++;
		}
	}
	return words + 1;
}

// function to compare two sentences
bool cmp(const char* str1, const char* str2)
{
	if (findLength(str1) != findLength(str2))
	{
		return false;
	}

	for (int i = 0; i < findLength(str1); i++)
	{
		if (*(str1 + i) != *(str2 + i))
		{
			return false;
		}
	}
	return true;
}

// function made to check if the stop word was found in the previous logic
bool check(const char* sentence, int start, int end)
{
	bool beforeValid = (start == 0 || *(sentence + start - 1) == ' ');
	bool afterValid = (*(sentence + end) == '\0' || *(sentence + end) == ' ' || *(sentence + end) == '.' || *(sentence + end) == ',');

	return beforeValid && afterValid;
}

// function to convert 1D character pointer to 2D character pointer
char** divide(char* str1, int& n)
{
	int words = findWords(str1);
	int length = findLength(str1);
	char** arr = new char* [words];
	for (int i = 0; i < words; i++) // an array having rows equal to number of words
	{
		*(arr + i) = new char[20];
	}
	int start = 0, end = 0;
	for (int i = 0; i < length + 1; i++)
	{

		if (*(str1 + i) != ' ' && *(str1 + i) != '\0') // if any space or null terminator is encountered, words is changed
		{
			*(*(arr + start) + end++) = *(str1 + i);
		}
		else
		{
			*(*(arr + start++) + end) = '\0';
			end = 0;
		}
	}
	n = start;

	return arr;
}

// function to convert 2D character pointer to 1D character pointer
char* combine(char** arr, int n)
{
	if (!n) // if the number of valid words is zero
	{
		char* temp = new char[1] {""};
		return temp;
	}
	int length = 0;
	for (int i = 0; i < n; i++) // finding length by iterating through each word
	{
		length += findLength(*(arr + i)) + 1;
	}
	char* combined = new char[length]; // combined string
	int index = 0;
	for (int i = 0; i < n && index < length; i++)
	{
		copy(*(arr + i), combined + index); // copying the words into the next continuation index of combined sentence
		index += findLength(*(arr + i));	// incrementing the index according to the copied word
		*(combined + index++) = ' ';		// adding a space to the next index
	}
	*(combined + index - 1) = '\0'; // at the last index, overwriting the space with the null terminator
	return combined;
}

class Unique
{
private:
	char** words;
	int uniqueWords;
	int docNum;
	int** scores;
	double** similarity;

public:
	Unique()
	{
		uniqueWords = 0;
		docNum = 0;
	}

	void setWords(int num, char** myWords)
	{
		uniqueWords = num;

		words = new char* [uniqueWords];
		for (int i = 0; i < uniqueWords; i++)
		{
			*(words + i) = new char[findLength(*(myWords + i)) + 1];
			copy(*(myWords + i), *(words + i));
		}
	}

	int getNumber()
	{
		return uniqueWords;
	}

	char* getUnique(int num)
	{
		return *(words + num);
	}

	void setFrequency(int** numbers, int docs)
	{
		docNum = docs;
		scores = new int* [docs];
		for (int i = 0; i < docs; i++)
		{
			*(scores + i) = new int[uniqueWords];
			for (int j = 0; j < uniqueWords; j++)
			{
				*(*(scores + i) + j) = *(*(numbers + i) + j);
			}
		}
	}

	int getFrequency(char* word, int documentNum) // -1 if not found
	{
		for (int i = 0; i < uniqueWords; i++)
		{
			if (cmp(word, *(words + i)))
			{
				return *(*(scores + documentNum - 1) + i);
			}
		}
		return -1;
	}

	void calculateAllCosineSimilarities(double**& similarities, int**& documentFrequency)
	{
		for (int i = 0; i < docNum; i++)
		{
			*(documentFrequency + i) = new int[uniqueWords];
			for (int j = 0; j < uniqueWords; j++)
			{
				*(*(documentFrequency + i) + j) = *(*(scores + i) + j);
			}
		}
		double* sims = new double[docNum] {0};
		similarities = new double* [docNum];
		similarity = new double* [docNum];
		for (int i = 0; i < docNum; i++)
		{
			*(similarities + i) = new double[docNum] {0};
			*(similarity + i) = new double[docNum] {0};

			for (int k = 0; k < docNum; k++)
			{
				int dot = 0;
				for (int j = 0; j < uniqueWords; j++)
				{

					dot += *(*(scores + i) + j) * *(*(scores + k) + j);
				}
				*(*(similarities + i) + k) += dot;
				cout << "Dot Product of Document " << i + 1 << " and " << k + 1 << " : " << *(*(similarities + i) + k) << endl;
			}
		}

		for (int i = 0; i < docNum; i++)
		{

			int temp = 0;
			for (int j = 0; j < uniqueWords; j++)
			{
				temp += *(*(scores + i) + j) * *(*(scores + i) + j);
			}
			*(sims + i) = sqrt(temp);
			cout << "\nMagnitude of the Document " << i + 1 << " : " << *(sims + i) << endl;
		}

		for (int i = 0; i < docNum; i++)
		{
			for (int j = 0; j < docNum; j++)
			{
				*(*(similarities + i) + j) /= (*(sims + i) * *(sims + j));
				*(*(similarity + i) + j) = *(*(similarities + i) + j);
				cout << "\nThe Similarity Index of Doc " << i + 1 << " and " << j + 1 << " : " << *(*(similarity + i) + j) * 100 << endl;
			}
		}
		delete[] sims;
	}

	double similarityIn(int documentNum1, int documentNum2)
	{
		cout << "Entered in Function...\n";
		cout << "\nThe Similarity Index of Doc " << documentNum1 << " and " << documentNum1 << " : " << *(*(similarity + documentNum1 - 1) + documentNum2 - 1) * 100 << endl;
		return *(*(similarity + documentNum1 - 1) + documentNum2 - 1) * 100;
	}

	~Unique()
	{
		for (int i = 0; i, docNum; i++)
		{
			delete[] similarity[i];
			delete[] scores[i];
		}
		delete[] similarity;
		delete[] scores;

		for (int i = 0; i < uniqueWords; i++)
		{
			delete[] words[i];
		}
		delete[] words;
	}

} myUnique;

class Documents
{
private:
	int numDocuments = 0;
	char* stopWords;
	char** documents;
	char** documentContent;
	Unique* uneek;

public:
	Documents()
	{
		numDocuments = 0;
		documentContent = new char* [1];
		*(documentContent) = new char{ '\0' };
		documents = new char* [1];
		*(documents) = new char{ '\0' };
		uneek = &myUnique;
	}
	Documents(int numDocuments, char** docNames) : numDocuments(numDocuments)
	{
		documents = new char* [numDocuments];
		for (int i = 0; i < numDocuments; i++)
		{
			copy(*(docNames + i), *(documents + i));
		}
	}
	void setDocuments(int n)
	{
		numDocuments = n;
	}

	void setNames(char** names)
	{
		documents = new char* [numDocuments];
		documentContent = new char* [numDocuments];
		for (int i = 0; i < numDocuments; i++)
		{
			*(documents + i) = new char[200];
			copy(*(names + i), *(documents + i));
		}
	}

	void setName(const char* newDocName, int n)
	{
		delete[] documents[n - 1];
		copy(newDocName, *(documents + n - 1));
	}

	void setStopWords(const char* words)
	{
		stopWords = new char[findLength(words) + 2];
		copy(words, stopWords);
	}

	int getDocuments() const
	{
		return numDocuments;
	}

	char* getDocumentName(int n) const
	{
		return *(documents + n - 1);
	}

	void setContent(const char* dest, int n)
	{
		*(documentContent + n) = new char[MAX_WORDS];
		copy(dest, *(documentContent + n));
	}

	char* getContent(int n)
	{
		return *(documentContent + n);
	}

	char* getText(int documentNumber)
	{
		return (*(documentContent + documentNumber - 1));
	}

	void readInput()
	{
		if (numDocuments)
		{
			int length = 0;
			char* temp = new char[200] {'\0'};
			for (int i = 0; *(path + i) != '\0'; i++)
			{

				*(temp + length++) = *(path + i);
			}

			for (int k = 0; k < numDocuments; k++)
			{
				for (int i = 0; *(*(documents + k) + i) != '\0'; i++)
				{
					if (*(*(documents + k) + i) >= 'A' && *(*(documents + k) + i) <= 'Z')
					{
						*(*(documents + k) + i) += 32;
					}
					*(temp + length + i) = *(*(documents + k) + i);
				}
				ifstream doc(temp);
				if (!doc.is_open())
				{
					cout << "failed to Open : " << temp << endl;
					return;
				}
				copy(temp, *(documents + k));
				char* text = new char[MAX_WORDS];

				doc.read(text, MAX_WORDS - 1);
				text[doc.gcount()] = '\0';
				cout << text;
				setContent(text, k);

				doc.close();
				delete[] text;
			}

			delete[] temp;
		}
	}
	void removePunctuationMarks()
	{
		char* junk = new char[35] {'`', '.', '~', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '-', '+', '=', '{', '}', '[', ']', '\'', '|', '\\', '\"', '<', '>', ';', ':', '/', '?', '>', ',', '<'};
		for (int k = 0; k < numDocuments; k++)
		{
			int index = 0;
			for (int i = 0; *(*(documentContent + k) + i) != '\0'; i++)
			{
				bool flag = false;
				for (int j = 0; j < 35; j++)
				{
					if (*(*(documentContent + k) + i) == *(junk + j))
					{
						flag = true;
						break;
					}
				}
				if (index && *(*(documentContent + k) + index - 1) == ' ' && *(*(documentContent + k) + i) == ' ' || (!index && *(*(documentContent + k) + i) == ' '))
				{
					continue;
				}
				if (!flag)
					*(*(documentContent + k) + index++) = *(*(documentContent + k) + i);
			}
			*(*(documentContent + k) + index) = '\0';
			if (index && *(*(documentContent + k) + index - 1) == ' ')
			{
				*(*(documentContent + k) + index - 1) = '\0';
			}
		}
		delete[] junk;
	}

	void convertUpperToLowerCase()
	{
		for (int k = 0; k < numDocuments; k++)
		{
			for (int i = 0; *(*(documentContent + k) + i) != '\0'; i++)
			{
				if (*(*(documentContent + k) + i) >= 'A' && *(*(documentContent + k) + i) <= 'Z')
				{
					*(*(documentContent + k) + i) += 32;
				}
			}
			cout << endl
				<< "Document " << k + 1 << " : " << *(documentContent + k) << endl;
		}
	}

	void removeStopWords()
	{
		int numStopW = 0;
		char** tokenedStop = divide(stopWords, numStopW);
		for (int i = 0; i < numDocuments; i++)
		{
			int valid = 0;
			cout << "Document " << i + 1 << " before cleaning : " << *(documentContent + i) << endl;
			int tempNum = 0;
			char** temp = divide(*(documentContent + i), tempNum);
			for (int j = 0; j < tempNum; j++)
			{
				// cout << "\nHi\n";
				bool yes = false;
				for (int k = 0; k < numStopW; k++)
				{
					if (cmp(temp[j], *(tokenedStop + k)))
					{
						yes = true;
						break;
					}
				}
				if (!yes)
				{
					// cout << "\nHi\n";
					copy(*(temp + j), *(temp + valid++));
				}
				// cout << "\nHello\n";
			}
			char* perfect = combine(temp, valid);
			copy(perfect, *(documentContent + i));
			cout << "Document " << i + 1 << " after cleaning : " << *(documentContent + i) << endl;
		}
		for (int j = 0; j < numStopW; j++)
		{
			delete[] tokenedStop[j];
		}
		delete[] tokenedStop;

		int totalLength = 0;

		for (int i = 0; i < numDocuments; i++)
		{
			int Length = findWords(*(documentContent + i));
			totalLength += Length;
		}
		if (totalLength > 1)
		{
			cout << totalLength << endl;

			char** uniqueness = new char* [totalLength];
			int index = 0;

			for (int i = 0; i < numDocuments; i++)
			{
				int tempWordsCount = 0;
				char** division = divide(*(documentContent + i), tempWordsCount);

				for (int j = 0; j < tempWordsCount; j++)
				{
					*(uniqueness + index) = new char[20];
					copy(*(division + j), *(uniqueness + index));
					index++;
				}

				for (int j = 0; j < tempWordsCount; j++)
				{
					delete[] division[j];
				}
				delete[] division;
			}

			int dups = 0;

			for (int i = 0; i < totalLength - 1; i++)
			{
				for (int j = i + 1; j < totalLength - dups; j++)
				{
					if (cmp(*(uniqueness + i), *(uniqueness + j)))
					{
						dups++;

						for (int l = j; l < totalLength - 1; l++)
						{
							*(uniqueness + l) = *(uniqueness + l + 1);
						}

						j--;
					}
				}
			}

			for (int i = 0; i < totalLength - dups; i++)
			{
				cout << *(uniqueness + i) << endl;
			}
			uneek->setWords(totalLength - dups, uniqueness);
		}
	}

	void generateFrequencies(char**& uniqueWords, int& uniqueCount, int**& documentFrequency)
	{
		uniqueCount = uneek->getNumber();
		cout << uniqueCount << endl;
		uniqueWords = new char* [uniqueCount];
		for (int i = 0; i < uniqueCount; i++)
		{
			*(uniqueWords + i) = new char(findLength(uneek->getUnique(i)) + 1);
			copy(uneek->getUnique(i), *(uniqueWords + i));
		}

		documentFrequency = new int* [numDocuments];
		for (int i = 0; i < numDocuments; i++)
		{
			*(documentFrequency + i) = new int[uniqueCount] {0};
			int tempWordsCount = 0;
			char** division = divide(*(documentContent + i), tempWordsCount);

			for (int j = 0; j < uniqueCount; j++)
			{

				char* uni = uneek->getUnique(j);
				for (int k = 0; k < tempWordsCount; k++)
				{
					if (cmp(uni, *(division + k)))
						(*(*(documentFrequency + i) + j))++;
				}
				cout << endl
					<< uni << " : " << (*(*(documentFrequency + i) + j)) << endl;
			}

			for (int j = 0; j < tempWordsCount; j++)
			{
				delete[] division[j];
			}
			delete[] division;
		}
		myUnique.setFrequency(documentFrequency, numDocuments);
	}

	~Documents()
	{
		delete[] stopWords;
		for (int i = 0; i < numDocuments; i++)
		{
			delete[] documents[i];
			delete[] documentContent[i];
		}

		delete[] documents;
		delete[] documentContent;
		numDocuments = 0;
	}

} myDocs;

class Plagiarism
{
private:
	char* fileToRead;
	Documents* myDocs;

public:
	Plagiarism(Documents* docs)
	{
		myDocs = docs;
	}

	void readInput(const char* pathofInputFile)
	{
		fileToRead = new char[50] {'\0'};
		copy(pathofInputFile, fileToRead);
		ifstream file(fileToRead);
		if (!file.is_open())
		{
			cout << "File was not Opened\n";
			return;
		}

		char* read = new char[500];
		if (file.getline(read, 500))
		{

			int length = 0;
			for (int i = 12; *(read + i) != '\0'; i++)
			{
				length++;
			}

			char* stopWords = new char[length + 1];
			for (int i = 0; i <= length; i++)
			{
				*(stopWords + i) = *(read + i + 12);
			}
			//*(stopWords + length) = '\0';
			myDocs->setStopWords(stopWords);
			cout << "Stop Words : " << stopWords << endl;

			int numDocuments;
			file >> numDocuments;
			myDocs->setDocuments(numDocuments);
			file.ignore(500, '\n');
			char** documents = new char* [numDocuments];
			for (int i = 0; i < numDocuments; i++)
			{
				*(documents + i) = new char[20];
				file.getline(*(documents + i), 20);
			}

			myDocs->setNames(documents);

			file.close();
			delete[] stopWords;
		}
		delete[] read;
	}
	const char* getfile()
	{
		return fileToRead;
	}

	~Plagiarism()
	{
		delete[] fileToRead;
	}

} plag(&myDocs);

// Global Classes and Variables
// Use of classes is must

// Function to read input from a file
static void readInput(const char* pathofInputFile)
{
	plag.readInput(pathofInputFile);
	myDocs.readInput();
}
int getNumberOfDocument()
{
	cout << "Number of Documents : " << myDocs.getDocuments() << endl;
	return myDocs.getDocuments();
}

char* getText(int documentNumber)
{
	return myDocs.getText(documentNumber);
}

// Function to remove punctuation marks from documents
void removePunctuationMarks()
{
	myDocs.removePunctuationMarks();
}

// Function to convert all characters in documents to lowercase
void convertUpperToLowerCase()
{
	myDocs.convertUpperToLowerCase();
}

// Function to remove stopwords from documents
void removeStopWords()
{
	cout << "\nAbout to Remove Stop Words \n";
	myDocs.removeStopWords();
}

// Function to generate frequencies of each unique word in each document
void generateFrequencies(char**& uniqueWords, int& uniqueCount, int**& documentFrequency)
{
	myDocs.generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
}

int getFrequency(char* word, int documentNum) // -1 if not found
{
	return myUnique.getFrequency(word, documentNum);
}

void calculateAllCosineSimilarities(double**& similarities, int** documentFrequency)
{
	myUnique.calculateAllCosineSimilarities(similarities, documentFrequency);
}

double similarityIn(int documentNum1, int documentNum2)
{
	return myUnique.similarityIn(documentNum1, documentNum2);
}
