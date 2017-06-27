#pragma once

#include <string>

class Question
{
public:
	Question(std::string q, std::string a[4], int id);
	// Get question text
	std::string getQuestion() const;
	// Get an answer's text. assume that 0<=i<=3
	std::string getAnswer(int i) const;
	// Get the index of the correct answer
	int correctAnswer() const;
	// Get the question's Id in the DataBase.
	int getId() const;
private:
	std::string question;
	std::string answers[4];
	int correctId;
	int qId;
};