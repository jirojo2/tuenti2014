/*
 *  Tuenti Challenge #1
 *	José Ignacio Rojo Rivero
 */

#include "util.h"

using namespace std;

class PollEntry
{
public:
	vector<string> names;
	string gender;
	string age;
	string studies;
	string academic_year;

	string printNames()
	{
		string res;

		if (names.size() == 0)
			return "NONE";

		for (string& name : names)
		{
			if (res.empty())
				res = name;
			else
				res += "," + name;
		}
		return res;
	}
};

class Student
{
public:	
	string name;
	string gender;
	string age;
	string studies;
	string academic_year;
};

void processList(vector<Student> & students)
{
	ifstream is ("students", std::ifstream::binary);
	string line;

	do
	{
		getline(is, line);
		
		Student s;
		// name1,gender1,age1,studies1,academic_year1
		vector<string> params = split(line, ',');

		if (params.size() != 5)
		{
			//cout << ":/ line: " << line << endl;
			break;
		}		

		s.name = params[0];
		s.gender = params[1];
		s.age = params[2];
		s.studies = params[3];
		s.academic_year = params[4];

		students.push_back(s);
		
		//cout << "  Student: " << s.name << endl;
	}
	while(is.good());
}

void processInput(vector<PollEntry> & poll)
{
	string line;

	// First line => n of cases
	getline(cin, line);
	int n = stoi(line);

	for (int i = 0; i < n; i++)
	{
		// Get input
		getline(cin, line);

		PollEntry pe;
		// gender,age,studies,academic_year
		vector<string> params = split(line, ',');
		pe.gender = params[0];
		pe.age = params[1];
		pe.studies = params[2];
		pe.academic_year = params[3];

		poll.push_back(pe);
	}
}

bool compareStringNoCase (string & first, string & second)
{
	int i=0;
	while ((i < first.length()) && (i < second.length()))
	{
		if (tolower (first[i]) < tolower (second[i])) return true;
		else if (tolower (first[i]) > tolower (second[i])) return false;
		i++;
	}

	if (first.length() < second.length()) return true;
	else return false;
}

bool sortByName(Student & a, Student & b)
{
	return compareStringNoCase(a.name, b.name);
}

void processData(vector<Student> & students, vector<PollEntry> & poll)
{
	// 1) Sort students list by name
	sort(students.begin(), students.end(), sortByName);

	// 2) Find any matches in the poll data
	for (int i = 0; i < poll.size(); i++)
	{
		PollEntry* pe = &poll[i];

		for (int j = 0; j < students.size(); j++)
		{
			Student* s = &students[j];

			int match = 0;
			if (s->gender.compare(pe->gender) == 0) match++;
			if (s->age.compare(pe->age) == 0) match++;
			if (s->studies.compare(pe->studies) == 0) match++;
			if (s->academic_year.compare(pe->academic_year) == 0) match++;

			if (match == 4)
				pe->names.push_back(s->name);
		}
	}
}

int main(int argc, char** argv)
{
	vector<PollEntry> poll;
	vector<Student> students;

	processList(students);
	processInput(poll);
	processData(students, poll);

	int i = 0;
	for (PollEntry& pe : poll)
	{
		cout << "Case #" << ++i << ": " << pe.printNames() << endl;
	}

	// DEBUG: check sort
	//cout << " " << students[0].name << endl;
	//cout << " " << students[students.size()-1].name << endl;

	return 0;
}

