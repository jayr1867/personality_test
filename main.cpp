//
// Project 2: Personality Tests
//
// Author: Jay Raval
//
// Spring 2022
//
#include <set>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "driver.h"
using namespace std;

/*
  fact();
  reads the OCEAN charachters and their values form the question file.
*/

void fact(char &f_char1, int &q_map1, char &f_char2,
  int &q_map2, string &p, bool &access) {
	if (p.size() == 1) {
		if (isdigit(p[0])) {
			q_map2 = 1;
		} else {
			f_char1 = p[0];
		}
	}
	if (p.size() == 2) {
		if (p[0] == '-') {
			q_map2 = -1;
		}
		if ((isalpha(p[1]))) {
			f_char1 = p[1];
		}
	}
	if (p.size() == 3 && isdigit(p[0])) {
		q_map1 = 1;
		f_char2 = p[2];
		access = true;
	}
	if (p.size() == 4 && isdigit(p[1])) {
		q_map1 = -1;
		f_char2 = p[3];
		access = true;
	}
}


/*
  q_read();
  reads the question file and stores the data in the set of Question struct.
*/

void q_read(set<Question> &Q) {

	ifstream infile;
	infile.open("questions.txt");

	string temp0;
	getline(infile, temp0);

	while (!infile.eof()) {
		Question temp;
		stringstream parser(temp0);
		string que, factor, p;;
		getline(parser, que, '.');
		temp.questionText = que;

		getline(parser, factor, '.');

		bool access = false;
		int q_map1 = 9, q_map2 = -9;
		char f_char1, f_char2;
		stringstream parsed(factor);
		while (getline(parsed, p, ':')) {
			fact(f_char1, q_map1, f_char2, q_map2, p, access);
			if (!access) {
				temp.factors[f_char1] = q_map2;
			} else {
				temp.factors[f_char1] = q_map1;
				temp.factors[f_char2] = q_map2;
			}
		}

		Q.insert(temp);
		getline(infile, temp0);
	}
}


/*
  print_opt();
  print the options for user to answer.
*/

void print_opt() {
	cout << endl;
	cout << "1. Strongly disagree" << endl <<
		"2. Disagree" << endl <<
		"3. Neutral" << endl <<
		"4. Agree" << endl <<
		"5. Strongly agree" << endl;
	cout << endl;
}


/*
  questions();
  prints the questions randomly from the question set and stores the user
  answers in a map.
*/

void questions(set<Question> &Q, map<char, int> &score) {
	auto it = randomQuestionFrom(Q);
	map<Question, int> answer;
	map<char, int> temp;
	int ans = 0;
	cout << endl << "How much do you agree with this statement?" << endl;
	cout << "\"" << it.questionText << ".\"" << endl;
	print_opt();
	cout << "Enter your answer here (1-5): ";
	cin >> ans;
	while(ans < 1 || ans > 5) {
		cout << "Enter your answer here (ONLY 1-5): ";
		cin >> ans;
	}
	answer[it] = ans;

	temp = scoresFrom(answer);
	for (auto itr = temp.begin(); itr != temp.end(); ++itr) {
		score[(*itr).first] += (*itr).second;
	}
}


/*
  p_read();
  reads the data from the ".people" files and stores the data in the set of
  Person struct.
*/

void p_read(string filename, set<Person> &P) {
	P.clear();
	ifstream infile;
	infile.open(filename);

	string temps;
	getline(infile, temps, '.');

	while (!infile.eof()) {
		Person temp;
		stringstream parser(temps);
		string nam, score, ind, olo;

		temp.name = temps;

		int q_map1 = 9, i = 0;
		char f_char1, f_char2;

		while (i < 5) {
			if (infile.eof()) {
				break;
			}
			infile >> f_char1;
			infile >> f_char2;

			if (i == 4) {
				getline(infile, olo);
				q_map1 = stoi(olo);
			} else {
				infile >> q_map1;
			}

			temp.scores[f_char1] = q_map1;
			++i;
		}

		P.insert(temp);
		getline(infile, temps, '.');
	}
}


/*
  test_file();
  takes the input from the user as a number and returns the file name
  corresponding to that given number.
*/

string test_file(int num) {
	if (num == 1) {
		return "BabyAnimals.people";
	} else if (num == 2) {
		return "Brooklyn99.people";
	} else if (num == 3) {
		return "Disney.people";
	} else if (num == 4) {
		return "Hogwarts.people";
	} else if (num == 5) {
		return "MyersBriggs.people";
	} else if (num == 6) {
		return "SesameStreet.people";
	} else if (num == 7) {
		return "StarWars.people";
	} else if (num == 8) {
		return "Vegetables.people";
	} else if (num == 9) {
		return "mine.people";
	}

	throw runtime_error("no point of this");
}


/*
  person();
  takes set of Person and a map as an input and returns the most similar person
  to the user from the ".people" file they selected.
*/
void person(set<Person> &P, map<char, int> &score) {
	cout << "You got " << (mostSimilarTo(score, P)).name << "!" << endl;
}


/*
  print_tf();
  prints the test files.
*/

void print_tf() {
	cout << endl;
	cout << "1. BabyAnimals" << endl <<
		"2. Brooklyn99" << endl <<
		"3. Disney" << endl <<
		"4. Hogwarts" << endl <<
		"5. MyersBriggs" << endl <<
		"6. SesameStreet" << endl <<
		"7. StarWars" << endl <<
		"8. Vegetables" << endl <<
		"9. mine" << endl <<
		"0. To end program." << endl <<
		endl;
}


/*
  menu();
  takes the set of Question and the set of Person as an input and outputs the
  whole program. In other words, this is the whole UI for the program.
*/

void menu(set<Question> &Q, set<Person> &P) {
	int num_que = 0;
	cout << "Welcome to the Personality Quiz!" << endl;

	cout << endl << "Choose number of questions: ";
	cin >> num_que;
	cout << endl;

	q_read(Q);
	map<char, int> score;

	for (int i = 0; i < num_que; ++i) {
		questions(Q, score);
	}

	int test_num = -1;
	print_tf();

	cout << "Choose test number (1-9, or 0 to end): ";
	cin >> test_num;

	while (test_num != 0) {
		p_read(test_file(test_num), P);
		person(P, score);

		print_tf();
		cout << "Choose test number (1-9, or 0 to end): ";
		cin >> test_num;
	}
	cout << "Goodbye!" << endl;
}

int main() {
	set<Question> questions;
	set<Person> people;
	menu(questions, people);

	return 0;
}
