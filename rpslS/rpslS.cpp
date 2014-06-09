#include <iostream>
#include <string>
#include <array>
#include <random>

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
    };
}

enum class Status {
    LOOSE,
    DRAW,
    WIN
};

std::array< std::array< Status, 5>, 5> Matrix {
    {
        /* Rock */      { Status::DRAW, Status::LOOSE, Status::WIN, Status::WIN, Status::LOOSE },
        /* Paper */     { Status::WIN, Status::DRAW, Status::LOOSE, Status::LOOSE, Status::WIN },
        /* Scissors */  { Status::LOOSE, Status::WIN, Status::DRAW, Status::WIN, Status::LOOSE },
        /* Lizard */    { Status::LOOSE, Status::WIN, Status::LOOSE, Status::DRAW, Status::WIN },
        /* Spock */     { Status::WIN, Status::LOOSE, Status::WIN, Status::LOOSE, Status::DRAW }
    }
};

class AI {
    std::default_random_engine _rengine;
    std::uniform_int_distribution<int> _uniform_dist;

    public:

    AI() :
        _rengine(std::random_device()()),
        _uniform_dist(0,4)
    {
    }

    Weapon weapon() {
        return static_cast<Weapon>(_uniform_dist(_rengine));
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
            case Status::LOOSE:
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
