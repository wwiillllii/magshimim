#pragma once

#include <string>

class Question
{
public:
	Question(std::string q, std::string a[4]);
	std::string getQuestion();
	std::string getAnswer(int i);
	int correctAnswer();
private:
	std::string question;
	std::string answers[4];
	int correctId;
};