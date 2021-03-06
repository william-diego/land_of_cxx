#include <iostream>
#include <string>
#include <array>
#include <random>
#include <cassert>

enum class Weapon {
    ROCK,
    PAPER,
    SCISSOR,
    LIZARD,
    SPOCK
};

/* Weapon -> stream */
struct BadWeapon {};
std::istream& operator>>(std::istream& is, Weapon& w) {
    char c;
    is >> c;
    switch(c) {
        case 'r': w = Weapon::ROCK; break;
        case 'p': w = Weapon::PAPER; break;
        case 's': w = Weapon::SCISSOR; break;
        case 'l': w = Weapon::LIZARD; break;
        case 'S': w = Weapon::SPOCK; break;
		default:  throw BadWeapon();
    };
    return is;
}
std::ostream& operator<<(std::ostream& os, Weapon w) {
    switch(w) {
        case Weapon::ROCK: return os << "rock";
        case Weapon::PAPER: return os << "paper";
        case Weapon::SCISSOR: return os << "scissor";
        case Weapon::LIZARD: return os << "lizard";
        case Weapon::SPOCK: return os << "Spock";
        default: assert(false && "this should never happen");
    };
}

enum class Status {
    LOSE,
    DRAW,
    WIN
};

std::array<std::array<Status, 5> const, 5> const Matrix = {{
    /* Rock */      { Status::DRAW, Status::LOSE, Status::WIN, Status::WIN, Status::LOSE },
    /* Paper */     { Status::WIN, Status::DRAW, Status::LOSE, Status::LOSE, Status::WIN },
    /* Scissors */  { Status::LOSE, Status::WIN, Status::DRAW, Status::WIN, Status::LOSE },
    /* Lizard */    { Status::LOSE, Status::WIN, Status::LOSE, Status::DRAW, Status::WIN },
    /* Spock */     { Status::WIN, Status::LOSE, Status::WIN, Status::LOSE, Status::DRAW }
}};

class AI {
    std::default_random_engine rengine_;
    std::uniform_int_distribution<int> uniform_dist_;

    public:

    AI() :
        rengine_(std::random_device()()),
        uniform_dist_(0, Matrix.size() - 1)
    {
    }

    Weapon weapon() {
        return static_cast<Weapon>(uniform_dist_(rengine_));
    }
};

static
int usage(char const *progname) {
    std::cerr << "usage: " << progname << " <positive_number>" << std::endl;
    return 1;
}

int main(int argc, char * argv[]) {
    if(argc != 2) {
        return usage(argv[0]);
    }

    /* number of non-tie games */
    int nb_round = std::stoi(argv[1]);
    if(nb_round <= 0) {
        return usage(argv[0]);
    }

    /* main event loop */
    size_t your_score = 0,
           ai_score = 0;

    AI ai;

    while(nb_round) {
        /* do some stuff */
        Weapon your_weapon;
        bool ok = false;
        do {
            try {
                std::cout << "(r)ock-(p)aper-(s)cissors-(l)izard-(S)pock? ";
                std::cin >> your_weapon;
                std::cin.ignore(1);
                ok = true;
            }
            catch(BadWeapon) {
            }
        } while(not ok);

        Weapon ai_weapon = ai.weapon();
        std::cout << "AI: " << ai_weapon << std::endl;

        Status status = Matrix[static_cast<int>(your_weapon)][static_cast<int>(ai_weapon)];

        switch(status) {
            case Status::WIN:
                ++your_score;
                --nb_round;
                break;
            case Status::LOSE:
                ++ai_score;
                --nb_round;
                break;
            case Status::DRAW:
                ;
        }
        std::cout << "Your score: " << your_score << std::endl
                  << "AI score:   " << ai_score << std::endl;
    }
    return 0;
}
