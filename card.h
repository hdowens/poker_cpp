#ifndef GAME_H
#define GAME_H

using std::vector; 


enum hands{HIGHCARD=1, ONEPAIR, TWOPAIR, TOFAKIND, STRAIGHT, FLUSH, FHOUSE, FOFAKIND, STRAFL, ROYFL};


//card struct definition
struct Card {
   int face, suit;
   bool operator < (Card &x){
       return face < x.face;
   }  
};

//hand struct definition
struct Hand {
    Card one,two;
};

struct Player{
    Hand card;
    int money, in, global_in, round_bet;
};

struct Game{
    int no_of_players, pot, round_call;
};


//struct to represent the five cards which will be drawn on the table
struct Game_Table{
    vector<Card> game_cards{5};
};

//macros
#define NORMT "\n\n\n\t"
#define NORM "\n\n\n"

//misc
void spin_timer();

//helper functions
void print_hand(struct Hand &hand);
void print_card(struct Card& card);
void print_game_table_3(Game_Table &cards);
void print_game_table_4(Game_Table &cards);
void print_game_table_5(Game_Table &cards);
void print_showdown_return(hands s);
//prints for each round
void pre_game_betting_msg();
void pre_betting_msg();
void pre_betting_msg();

//init functions and welcome messages
void print_welcome();
void init_game();

//game functions
void generate_hand(struct Hand &hand, vector<Card> &deck);
void play_game(int no_of_players, int no_of_ai, vector<Card> &deck);
int shuffle_deck(vector<Card> &arr);
void generate_deck(vector<Card> &arr);
Card draw_card(vector<Card> &deck);
void start_vs_computer();
void choose_game();
void generate_first_three_cards(struct Game_Table &game_cards, vector<Card> &deck);
void manage_bet_choice(int choice, Player &player,  Game &game);
void init_player(Player &p, vector<Card> &d, Hand &h);
void init_game(Game &g, int a);
void generate_turn_card(struct Game_Table &game_cards, vector<Card> &deck);
void generate_river_card(struct Game_Table &game_cards, vector<Card> &deck);
int analyse_hand(struct Hand &hand, struct Game_Table &cards);


//card analysis functions
int test_number_of(vector<Card> &cards, int face_req, int suit_req);
int straight_test(vector<Card> &cards);
int flush_test(vector<Card> &cards);
int full_house_test(vector<Card> &cards);
int two_pair_test(vector<Card> &cards);
int strfl_test(vector<Card> &cards);
int royfl_test(vector<int> &arr);


//main func
int main();


#endif // CARD_H