#include "pch.h"
#include "PlagiarismChecker.cpp"
#include <fstream>
#include <string>
#include <math.h>


TEST(PC, FileReading) {
	readInput("input1.txt");
	char expected[] = "^(?=(.{-})$)(?!.*/)(?:[A-E]{,}){-}$^(?=(.{-})$)(?!.*/)(?:[I-O]{,}){-}$^(?=(.{-})$)(?!.*/)(?:[U]{,}){-}$";
	EXPECT_EQ(0, strcmp(getText(1), expected));		//This should return the content of doucment 1
	EXPECT_EQ(2, getNumberOfDocument());
}

TEST(PC, RemovePunctucationMarks)
{
	readInput("input1.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	char expected[] = "aeiou";
	char expected1[] = "vowels aeiou";
	EXPECT_EQ(0, strcmp(getText(1), expected));
	EXPECT_EQ(0, strcmp(getText(2), expected1));
}

TEST(PC, CaseChange) {

	readInput("input.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	char expected[] = "she sell seashells in the sea";
	EXPECT_EQ(0, strcmp(getText(2), expected));		//This should return the content of doucment 2
	EXPECT_EQ(2, getNumberOfDocument());
}


TEST(PC, RemoveStopWords) {

	readInput("input.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	removeStopWords();
	char expected1[] = "she sell seashells sea";
	char expected2[] = "sell shells she sees shells sea";
	EXPECT_EQ(0, strcmp(getText(2), expected1));		//This should return the content of doucment 2
	EXPECT_EQ(0, strcmp(getText(1), expected2));
	EXPECT_EQ(2, getNumberOfDocument());
}


TEST(PC, FrequencyCheck) {

	readInput("input1.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	removeStopWords();
	char** uniqueWords;
	int uniqueCount;
	int** documentFrequency;
	generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
	char name1[] = "vowels";
	char name2[] = "aeiou";
	EXPECT_EQ(getFrequency(name1, 1), 0);
	EXPECT_EQ(getFrequency(name2, 2), 1);
	EXPECT_EQ(uniqueCount, 2);
}

TEST(PC, PlagCheck1) {

	readInput("input.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	removeStopWords();
	char** uniqueWords;
	int uniqueCount;
	int** documentFrequency;
	generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
	double** similarities;
	calculateAllCosineSimilarities(similarities, documentFrequency);

	EXPECT_EQ(similarityIn(1, 1), 100);
	EXPECT_NEAR(similarityIn(2, 1), 53.00, 0.3);
	EXPECT_NEAR(similarityIn(1, 2), 53.00, 0.3);
}

TEST(PC, PlagCheck2) {

	readInput("input1.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	removeStopWords();
	char** uniqueWords;
	int uniqueCount;
	int** documentFrequency;
	generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
	double** similarities;
	calculateAllCosineSimilarities(similarities, documentFrequency);

	EXPECT_EQ(similarityIn(1, 1), 100);
	EXPECT_NEAR(similarityIn(2, 1), 70.71, 0.3);
	EXPECT_NEAR(similarityIn(1, 2), 70.71, 0.3);
}


TEST(PC, PlagCheck3) {

	readInput("input2.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	removeStopWords();
	char** uniqueWords;
	int uniqueCount;
	int** documentFrequency;
	generateFrequencies(uniqueWords, uniqueCount, documentFrequency);
	double** similarities;
	calculateAllCosineSimilarities(similarities, documentFrequency);

	EXPECT_EQ(similarityIn(1, 1), 100);
	EXPECT_EQ(similarityIn(2, 3), 0);
	EXPECT_EQ(similarityIn(1, 2), 100);
}

TEST(PC, MaxfileRead) {

	readInput("input3.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	char expected[] = "aeiou";
	EXPECT_EQ(0, strcmp(getText(1001), expected));		//This should return the content of doucment 1000
	EXPECT_EQ(1002, getNumberOfDocument());
}

TEST(PC, HalfFilesRead) {

	readInput("input4.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	char expected[] = "vowels aeiou";
	EXPECT_EQ(0, strcmp(getText(10), expected));		//This should return the content of doucment 9
	EXPECT_EQ(10, getNumberOfDocument());
}


TEST(PC, lineBreaking_prop) {

	readInput("input5.txt");
	removePunctuationMarks();
	convertUpperToLowerCase();
	char expected[] = "f i n i s h e d";
	EXPECT_EQ(0, strcmp(getText(2), expected));		//This should return the content of doucment 2
	EXPECT_EQ(2, getNumberOfDocument());
}
