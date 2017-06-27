#include "Question.h"

Question::Question(std::string q, std::string a[4], int id)
{
	qId = id;
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
std::string Question::getQuestion() const
{
	return question;
}
std::string Question::getAnswer(int i) const
{
	return answers[i];
}
int Question::correctAnswer() const
{
	return correctId;
}
int Question::getId() const
{
	return qId;
}