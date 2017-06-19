#include "Question.h"

Question::Question(std::string q, std::string a[4])
{
	question = q;
	int count, i;
	for (count = 0; count < 4; count++)
	{
		i = rand() & 3;
		while (answers[i] != "")
			i = (i + 1) & 3;
		answers[i] = a[count];
		if (!count) correctId = i;
	}
}
std::string Question::getQuestion()
{
	return question;
}
std::string Question::getAnswer(int i)
{
	return answers[i];
}
int Question::correctAnswer()
{
	return correctId;
}