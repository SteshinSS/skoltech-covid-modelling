#include <iostream>
#include <vector>
#include <math.h>

struct Human {
    double x;
    double y;
    bool is_infected;
};

double GetDistance(const Human &lhs, const Human &rhs) {
    return std::sqrt((lhs.x - rhs.x)*(lhs.x - rhs.x) + (lhs.y - rhs.y)*(lhs.y - rhs.y));
}

std::vector<Human> GenerateQueue(double social_distance, int size, double infected_fraction) {
    std::vector<Human> queue;
    double x = 0.0;
    for (int i = 0; i < size; ++i) {
        queue.push_back(Human{x, 0.0});
        x += social_distance;
    }
    return queue;
}

std::vector<Human> GenerateCrowd(double social_distance, int size) {
    std::vector<Human> result;

    // Generate crowd as a square grid
    // So if it's side equal N, then there will be NxN people
    int side = std::sqrt(size);

    for (int i = 0; i < side; ++i) {
        for (int j = 0; j < side; ++j) {
            result.push_back(Human{i*social_distance, j*social_distance});
        }
    }

    // We generated side x side people. Let's add what left
    int total_left = size - side * side;
    for (int i = 0; i < total_left; ++i) {
        result.push_back(Human{side*social_distance, (side + i)*social_distance});
    }

    return result;
}

double ScoreFunction1(double distance) {
    if (distance > 5.0) {
        return 0;
    }

    if (distance < 0.1) {
        return 1;
    }
    return (0.01)/(distance * distance);
}

template<typename Func>
double CalculateScore(const std::vector<Human> &people, Func ScoreFunction) {
    double score = 0.0;

    for (const Human &person : people) {
        if (person.is_infected) {
            for (const Human &other_person : people) {
                if (!other_person.is_infected) {
                    score += ScoreFunction(GetDistance(person, other_person));
                }
            }
        }
    }

    return score;
}

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
