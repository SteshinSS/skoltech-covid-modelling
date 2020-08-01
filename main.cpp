#include <iostream>
#include <vector>
#include <math.h>
#include <random>

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

    std::random_device rd{};
    std::mt19937 rng{rd()};
    std::bernoulli_distribution distribution(infected_fraction);


    double x = 0.0;
    for (int i = 0; i < size; ++i) {
        queue.push_back(Human{x, 0.0, distribution(rng)});
        x += social_distance;
    }
    return queue;
}

std::vector<Human> GenerateCrowd(double social_distance, int size, double infected_fraction) {
    std::vector<Human> result;

    std::random_device rd{};
    std::mt19937 rng{rd()};
    std::bernoulli_distribution distribution(infected_fraction);

    // Generate crowd as a square grid
    // So if it's side equal N, then there will be NxN people
    int side = std::sqrt(size);

    for (int i = 0; i < side; ++i) {
        for (int j = 0; j < side; ++j) {
            result.push_back(Human{i*social_distance, j*social_distance, distribution(rng)});
        }
    }

    // We generated side x side people. Let's add what left
    int total_left = size - side * side;
    for (int i = 0; i < total_left; ++i) {
        result.push_back(Human{side*social_distance, (side + i)*social_distance, distribution(rng)});
    }

    return result;
}

double ScoreFunctionFirst(double distance) {
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
    do {
        std::cout << "Choose type of a crowd." << std::endl;
        std::cout << "Enter \'G\' for grid" << std::endl;
        std::cout << "Enter \'Q\' for queue" << std::endl;
        std::cout << "> ";
        char crowd_type;
        std::cin >> crowd_type;
        if (crowd_type != 'Q' && crowd_type != 'G') {
            std::cout << "Please enter Q or G." << std::endl;
            continue;
        }

        std::cout << "Enter number of people" << std::endl;
        std::cout << "> ";
        int size;
        std::cin >> size;

        std::cout << "Enter average distance between people" << std::endl;
        if (crowd_type == 'G') {
            std::cout << "In case of grid crowd, enter length of grid's cell" << std::endl;
        }
        std::cout << "> ";
        double distance;
        std::cin >> distance;

        std::cout << "Enter fraction of already infected people" << std::endl;
        std::cout << "> ";
        double infected_fraction;
        std::cin >> infected_fraction;

        std::cout << std::endl;
        std::cout << "Calculating score..." << std::endl;

        std::vector<Human> crowd;
        if (crowd_type == 'G') {
            crowd = GenerateCrowd(distance, size, infected_fraction);
        } else if (crowd_type == 'Q') {
            crowd = GenerateQueue(distance, size, infected_fraction);
        }

        double score = CalculateScore(crowd, ScoreFunctionFirst);
        std::cout << "Infection score: " << score << std::endl << std::endl;
        std::cout << "Change parameters? (Y/N)" << std::endl;
        std::cout << "> " << std::endl;
        char change_parameters;
        std::cin >> change_parameters;
        if (change_parameters != 'Y') {
            break;
        }
    } while (true);

    return 0;
}
