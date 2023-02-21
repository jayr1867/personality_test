//
// Project 2: Personality Tests
//
// Author: Jay Raval
//
// Spring 2022
//

#include <math.h>
#include <limits>
#include <string>
#include <map>
#include <set>
#include <iostream>
#include "myrandom.h"

using namespace std;

constexpr double lowest_double = std::numeric_limits<double>::lowest();

/* Type: Question
 *
 * Type representing a personality quiz question.
 */
struct Question {
    string questionText;  // Text of the question
    map<char, int> factors;   // Map from factors to +1 or -1
    friend bool operator< (const Question& lhs, const Question& rhs) {
        return lhs.questionText < rhs.questionText;
    }
    friend bool operator== (const Question& lhs, const Question& rhs) {
        return lhs.questionText == rhs.questionText;
    }
    friend bool operator!= (const Question& lhs, const Question& rhs) {
        return lhs.questionText != rhs.questionText;
    }
};

/* Type: Person
 *
 * Type representing a person, used to represent people when determining
 * who's the closest match to the user.
 */
struct Person {
    string name;      // Name of the person
    map<char, int> scores;  // Map from factors to +1 or -1
    friend bool operator< (const Person& lhs,   const Person& rhs) {
        return lhs.name < rhs.name;
    }
    friend bool operator== (const Person& lhs, const Person& rhs) {
        return lhs.name == rhs.name;
    }
    friend bool operator!= (const Person& lhs, const Person& rhs) {
        return lhs.name != rhs.name;
    }
};

/* randomElement
 *
 * This function selects, at random, a Question from the inputted questions set
 * and returns the question.  Note, this function does not remove the randomly
 * selected question from the set.
*/
Question randomElement(set<Question>& questions) {
    int ind = randomInteger(0, (int)questions.size()-1);
    int i = 0;
    for (auto e : questions) {
        if (i == ind) {
            return e;
        }
        i++;
    }
    return {};
}


/*
 * This function selects a random question from the set, removes it from the set,
 * and returns it.
 *
*/
Question randomQuestionFrom(set<Question>& questions) {
  if (questions.size() == 0) {
    throw runtime_error("questions: empty");
  }
  auto que = randomElement(questions);
  auto it = questions.find(que);
  questions.erase(it);

  return que;
}


/*
 * This function takes a map of answers given by the user, adds the value in a
 * different map and returns it.
 *
*/
map<char, int> scoresFrom(map<Question, int>& answers) {
    map<char, int> scores;
    for (auto it = answers.begin(); it !=answers.end(); ++it) {
      for (auto iter = (*it).first.factors.begin();
      iter != (*it).first.factors.end(); ++iter) {
        if ((*it).second == 5) {
          scores[((*iter).first)] += (2 * (*iter).second);
        } else if ((*it).second == 4) {
          scores[((*iter).first)] += (1 * (*iter).second);
        } else if ((*it).second == 3) {
          scores[((*iter).first)] += (0 * (*iter).second);
        } else if ((*it).second == 2) {
          scores[((*iter).first)] += ((-1) * (*iter).second);
        } else if ((*it).second == 1) {
          scores[((*iter).first)] += ((-2) * (*iter).second);
        }
      }
    }
    return scores;
}


/*
  This function takes a ma of scores as an input and the normalize it by taking
  the square root of the sum of the squares of all scores and then dividing
  that square root with all the scores and adding it to another map and
  returning it.
*/
map<char, double> normalize(map<char, int>& scores) {
    map<char, double> normal;
    bool non_zero = false;
    size_t total = 0;
    double sqr;

    for (auto it = scores.begin(); it != scores.end(); ++it) {
      total += pow((*it).second, 2);
      if ((*it).second != 0) {
        non_zero = true;
      }
    }

    if (!non_zero) {
      throw runtime_error("input map: no non_zero value");
    }

    sqr = sqrt((double)total);

    for (auto it = scores.begin(); it != scores.end(); ++it) {
      normal[(*it).first] = ((*it).second / sqr);
    }
    return normal;
}


/*
  This function takes two normalized maps of two different users and finds the
  similarity between those two users using the cosine similarity.
*/
double cosineSimilarityOf(const map<char, double>& lhs,
  const map<char, double>& rhs) {
  double cs_sim = 0.0;
  for (auto it = lhs.begin(); it != lhs.end(); ++it) {
    auto itr = rhs.find((*it).first);
    cs_sim += ((*it).second * (*itr).second);
  }
  return cs_sim;
}




/*
  takes a score map and a set of Person as an input and returns an iterator to
  the set of Person who has the most similarity to the user.
*/
Person mostSimilarTo(map<char, int>& scores, set<Person>& people) {
  if (people.size() == 0) {
    throw runtime_error("people: empty");
  }

  double base = lowest_double;

  auto max_sim = people.begin();

  map<char, double> normal = normalize(scores);

  for (auto it = people.begin(); it != people.end(); ++it) {
    map<char, int> temp = (*it).scores;

    map<char, double> peeps = normalize(temp);

    if (cosineSimilarityOf(normal, peeps) > base) {
      max_sim = it;
      base = cosineSimilarityOf(normal, peeps);
    }
  }

  return *max_sim;
}
