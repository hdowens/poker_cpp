//including standard libs
#include <string.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <unordered_map>
#include <algorithm>

//including local libs
#include "card.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif



/*
DEV NOTES:

12-04-2022
The structure of the showdown needs to change. After reading a lot of the rules & how the showdown works
I've realised that the way it works is that it's the best 5 cards on the table. If the 5 cards in the middle
are a royal flush, then both players can claim that royal flush and the pot is split. This is why I need 
to change it back from recognising if a hand contains a player's card. This is still useful though,
and I was able to refactor a lot of functions by thinking I had to do this. Moving forward, I will 
remove this, and the function will return the best that 5 cards can be on the tab

11-04-2022
Been sidetracked with work and un-motivated to continue, only making small adjustments recently
Finished the card recognising algorith, so that checks to make sure the hand you have contains at 
least one of your cards

Moving forward, need to implement what happens should two players have the same return, i.e.



introduced a much better system for generating the deck and then randomly selecting cards from it.
The next step would be to deal hands to a player, and make sure cards are always going to be unique. i.e.
the cards randomly selected are going to have to go in a new card list and itll have to be cross
referenced when producing the hands.

11-01-2022
The process of the game is very repetitive - bet, deal cards, bet, deal cards, bet, deal cards, bet , showdown
some way to turn betting and dealing would make it a lot easier, needs to be lots of 
error handling. Introduced a player struct, and a player object will be created whenever the game gets
underway. Hopefully this will be able to assist in whats occuring.

Lots of error checking needs to be done.
checking to see if all game actions return a successful error code
check to see if user input is valid and expected
check to see size of deck when drawing cards 1. gets smaller 2. still cards exist

in the future: 
generating hands for players, then allowing them to choice to between folding & calling
money amounts avislee
computerised oponents ( will need to research )
highscores
multiple players
dealer chip moving around
seperating out into seperate files and build executable with cmake
small blinds / big blinds


12-01-2022
Working on the card working algorithm. In essence it checks serially for royal flush through one pair
for a lot of the tests it is repeating so I hope in the future to revist it and make a much more effcient
version as there are things being done twice. Currently for some using a naive huge if statement to check things
like four of a kind but considering changing to hash maps if they are more efficient. Implemented a operator
overloading in the card struct to allow them to be used by the sort function.

//19-01-2022
Implemented the card recognising algorithm, but realised there is a small error with it. It recognises the
cards ok but the it arbritarily realises hands from 7 cards, not taking it into consideration that one of 
the player's cards must be involved with the hand. E.g. if the game cards are 6 ,6 ,2 ,4 ,Q ,9 and the 
player's cards are 3, 8 it will still say twopair because there are two 6's in the 5
*/

//introducing our namespaces
using std::cout; using std::vector;
using std::map; using std::pair;
using std::system; using std::cin;
using std::string; using std::make_pair;
using std::sort; using std::unordered_map;
using std::any_of;

//global variables, for the deck and for assisting printing of the cards
static map<int,string> high_cards;
static map<int,string> suits;

void print_welcome(){
    //yee yee haw
    system("clear");
    cout << NORMT << "-------------------Welcome to Poker-------------------\n";
    cout << NORMT << "Come in out of the cold cowboy, and prepare for a game\n";
    cout << NORMT << "We know it's been a long day. Come on in an play with us.\n";
    cout << NORMT << "You have a few options:\n";
    cout << "\t1. - Play a game of poker against another person beside you\n";
    cout << "\t2. - Play a game of poker against the computer\n";
    cout << "\t3. - Sit back and have a coldie.\n";
    cout << "\t4. - Leave, even though you don't want to." << NORM;
}

void pre_game_betting_msg(){
    system("clear");
    cout << NORMT << "-------------------You vs the Ai-------------------\n";
    cout << NORMT << "Here to take on our finest advancement in technology\n";
    cout << NORMT << "Pre-Flop betting about to take place. lesgoooooooooo\n";
    cout << NORMT << "I dont want to go near you, so lets assume you have $100\n";
    cout << NORMT << "Best of luck.\n";
    
    //cout << "\tAmount of money: ";
    //Add the current pot of the game, current raise in the round, nickname, etc.
    //cout << "\t"
}

void pre_betting_msg(){
    cout << NORMT << "Your options:\n";
    cout << "\t1.Fold\n";
    cout << "\t2.Raise a bet\n";
    cout << "\t3.Call a bet( if one exists )\n";
    cout << "\t4.Go all in!\n";
    cout << "\t5.Check\n";
}

inline const char* toString(int x){
    switch(x){
        case 1: return "HIGHCARD";
        case 2: return "ONEPAIR";
        case 3: return "TWOPAIR";
        case 4: return "THREEOF A KIND";
        case 5: return "STRAIGHT";
        case 6: return "FLUSH";
        case 7: return "FULL HOUSE";
        case 8: return "FOUR OF A KIND";
        case 9: return "STRAIGHT FLUSH";
        case 10: return "ROYAL FLUSH";
        default: return "Unknown hand";
    }
}

//initialis variables required for the game
void init_game(){
    //seed the time 
    srand(time(NULL));


    //init map to help with printing of cards for user assitance
    high_cards.insert(make_pair(11,"Queen"));
    high_cards.insert(make_pair(12,"King"));
    high_cards.insert(make_pair(0,"Jack"));
    high_cards.insert(make_pair(1,"Ace"));

    //init map for suits to be printed 
    suits.insert(make_pair(0, "Spades"));
    suits.insert(make_pair(1, "Hearts"));
    suits.insert(make_pair(2, "Diamonds"));
    suits.insert(make_pair(3, "Clubs"));

  

}

//print card, helper function
void print_card(struct Card &card){
    //used maps here, with the face and suit as the distinguishing key so it 
    //prints the text where there is text
    if(card.face > 10 || card.face < 2){
        cout << "\n\tCard is -> " << high_cards.find(card.face)->second << " of " 
                            << suits.find(card.suit)->second;
    } else {
        cout << "\n\tCard is -> " << card.face << " of "
                            << suits.find(card.suit)->second;
    }

}

//prints a player's hand
void print_hand(struct Hand &hand){
    cout << "\tYour cards this round: ";
    print_card(hand.one), print_card(hand.two);
    cout << NORM;
}

//when 3 cards are on the table
void print_game_table_3(Game_Table &cards){
    print_card(cards.game_cards[0]), print_card(cards.game_cards[1]), print_card(cards.game_cards[2]);
}

//when 4 cards are on the table
void print_game_table_4(Game_Table &cards){
    print_card(cards.game_cards[0]), print_card(cards.game_cards[1]), print_card(cards.game_cards[2]);
    print_card(cards.game_cards[3]);
}

//when 5 cards are on the table
void print_game_table_5(Game_Table &cards){
    print_card(cards.game_cards[0]), print_card(cards.game_cards[1]), print_card(cards.game_cards[2]);
    print_card(cards.game_cards[3]), print_card(cards.game_cards[4]);
}

Card draw_card(vector<Card> &deck){
    Card temp = deck.back();
    deck.pop_back();
    return temp;
}

//generates an arbitrary hand
void generate_hand(struct Hand &hand, vector<Card> &deck){
    hand.one = draw_card(deck);
    hand.two = draw_card(deck);
}

//generates first three table cards
void generate_first_three_cards(Game_Table &cards, vector<Card> &deck){
    cout << "\tBurning the card, you know the rules...\n";
    cout << NORMT << "The flop has drawn: \n";
    deck.pop_back();
    for(int i = 0 ; i < 3 ; i++){
        cards.game_cards[i] = draw_card(deck);
    }
}

void generate_turn_card(struct Game_Table &game_cards, vector<Card> &deck){
    cout << "\tBurning the card, you know the rules...\n";
    deck.pop_back();
    game_cards.game_cards[3] = draw_card(deck);

}

void generate_river_card(struct Game_Table &game_cards, vector<Card> &deck){
    cout << "\tBurning the card, you know the rules...\n";
    deck.pop_back();
     game_cards.game_cards[4] = draw_card(deck);

}

//function to arbitrarily instantiate a player ready for the game
void global_init_player(Player &p){
    p.money = 100;
    p.global_in = 0;
    
}

void round_init_player(Player &p, vector<Card> &d, Hand &h){
    generate_hand(h,d);
    p.round_bet = 0;
    p.in = 0;
}

void init_game(Round &r, int a){
    r.pot = 0;
    r.round_call = 0;
    r.no_of_players = a;
}

void end_game(struct Game &main_game){
    if(main_game.players[0].money != 0)
        cout << NORMT << "You won. Congratulations." << NORM;
    else
        cout << NORMT << "You lost. Loser." << NORM;
    choose_game();
}

void play_game(struct Game &main_game){
    for(auto& i : main_game.players){
        global_init_player(i);
    }
    bool play = true;

    while(play){
        play_round(main_game);

        for(auto& i : main_game.players){
            if(i.money == 0)
                play = false;
        }
    }

    end_game(main_game);
}


//function that plays the game
void play_round(struct Game &main_game){
    
    static vector<Card> deck(52);
    generate_deck(deck);


    int pre_flop = 0;
    int pre_turn = 0;
    int pre_river = 0;

    Hand player, computer;

    Game_Table game_cards;

    Round round;
    //int sum = no_of_ai + no_of_players;

    round_init_player(main_game.players[0],deck,player);
    round_init_player(main_game.players[1],deck,computer);
    
    init_game(round, (main_game.players.size()));
    pre_game_betting_msg();

    

    //flop
    //TODO:
    //make flop , turn, river, showdown all into their seperate functions
    pre_betting_msg();
    print_hand(player);
    cout << NORMT << "What'll it be then: ";
    cin >> pre_flop;
    manage_bet_choice(pre_flop, main_game.players[0], round);
    //cout << NORM;
    //generate first three cards
    generate_first_three_cards(game_cards, deck);
    //cout << NORMT << "The cards below are the 3 cards on the table. What'll be your play? "; 
    print_game_table_3(game_cards);
    
    //turn
    pre_betting_msg();
    cout << NORM;
    print_hand(player);
    cout << NORMT << "What'll it be then: ";
    cin >> pre_turn;
    manage_bet_choice(pre_turn, main_game.players[0], round);
    generate_turn_card(game_cards, deck);
    print_game_table_4(game_cards);
    cout << NORM;

    //river    
    pre_betting_msg();
    cout << NORM;
    print_hand(player);
    cout << NORMT << "What'll it be then: ";
    cin >> pre_river;
    manage_bet_choice(pre_river,main_game.players[0], round);
    generate_river_card(game_cards, deck);
    print_game_table_5(game_cards);
    cout << NORM;

    int player_ret = analyse_hand(player, game_cards);
    int comp_ret = analyse_hand(computer,game_cards);

    if(player_ret > comp_ret){
        cout << NORMT << "Player wins with : " << toString(player_ret) << NORM;
    } else if (player_ret < comp_ret) {
        cout << NORMT << "Computer wins with : " << toString(comp_ret) << NORM;
    } else if(player_ret == comp_ret){
        //check who has a better hand function
    } else {
        exit(EXIT_FAILURE);
    }
    
    


    //cout << NORMT << "Size of deck after generating hands + table: " << deck.size() << NORM;
}


int royfl_test(vector<int> &arr){
    std::unordered_map<int,int> m;
    for(int i = 0 ; i < arr.size() ; i++){
        m[arr[i]]++;
    }

    std::unordered_map<int,int>::iterator it = m.begin();
    while(it != m.end()){
        if(it->second == 5){
            cout << "\n\tA royal flush!\n\n";
            return ROYFL;
        }
        it++;
    }
    return 1;
}


//tests for all straight flushes not in the form 10,J,Q,K,A (as that would be a royal flush anyway)
int strfl_test(vector<Card> &cards){
    for(int i = 0; i < 3 ; i++){
        if(cards[i+1].face == cards[i].face+1 && cards[i+2].face == cards[i+1].face+1 && cards[i+3].face == cards[i+2].face+1 
            && cards[i+4].face == cards[i+3].face+1){
                if(cards[i].suit == cards[i+1].suit == cards[i+2].suit == cards[i+3].suit == cards[i+4].suit){
                    cout << "\n\n\tstraight flush found\n" ;
                    return STRAFL;
                }
        }
    }
    return 1;
}



//function to test for flushes
int flush_test(vector<Card> &cards){
    std::unordered_map<int,int> suits;
    for(int i = 0 ; i < cards.size() ; i++){
        suits[cards[i].suit]++;
    }
    std::unordered_map<int,int>::iterator it = suits.begin();
    while(it != suits.end()){
        if(it->second == 5){
            cout << "\n\tA flush!\n\n";
            return FLUSH;
        }
        it++;
    }
    return 1;
}

//prototype of the eventual function
//royal flush, flush, straight flush, four of a kind, 3 of a kind
//all use a function identical below except they are all hard coded
//in the future I would like to modularise it a bit better


int test_number_of(vector<Card> &cards, int face_req, int suit_req){
    std::unordered_map<int,int> faces;
    std::unordered_map<int,int> suits;
    for(int i = 0 ; i < cards.size() ; i++){
        faces[cards[i].face]++;
        suits[cards[i].suit]++;
    }
    if(face_req > 0){
        std::unordered_map<int,int>::iterator it = faces.begin();
        while(it != faces.end()){
            if(it->second == face_req){
                cout << "\n\tFound!\n\n";
                return 0;
            }
            it++;
        }
    }
    if(suit_req > 0){
        std::unordered_map<int,int>::iterator it = suits.begin();
        while(it != suits.end()){
            if(it->second == suit_req){
                cout << "\n\tFound!\n\n";
                return 0;
            }
            it++;
        }
    }
    
    return 1;
}


// the most challenging to write due to how I generated the original cards.
// jack is a 0 and ace is 1, so the faces need to be altered before writing
// my naive test of testing to see if the next 5 cards are all in
// consecutive order
int straight_test(vector<Card> &cards){
    vector<int> faces;
    for(int i = 0 ; i < cards.size() ; i++){
        if(cards[i].face == 11){
            //cards[i].face = 12;
            faces.push_back(12);
        }
            
        if(cards[i].face == 12){
            //cards[i].face == 13;
            faces.push_back(13);
        }
            
        if(cards[i].face == 0){
            //cards[i].face == 11;
            faces.push_back(11);
        }
           
        if(cards[i].face == 1){
            faces.push_back(14);
        }
        if(cards[i].face > 0 && cards[i].face < 11){
            faces.push_back(cards[i].face);
        }
    }
    cout << "\n\n\t";
    sort(faces.begin(),faces.end());
    for(auto item : faces){
        cout << ' ' << item;
    }

    for(int i = 0 ; i < faces.size()-4 ; i++){
        if(faces[i+1] == faces[i]+1 && faces[i+2] == faces[i+1]+1 && faces[i+3] == faces[i+2]+1 
            && faces[i+4] == faces[i+3]+1){
                cout << "\n\n\tstraight found\n\n";
                return STRAIGHT;
            }
    }
    return 1;
}

int full_house_test(vector<Card> &cards){
    bool thr = 0;
    bool tw = 0;
    std::unordered_map<int,int> faces;
    for(int i = 0 ; i < cards.size() ; i++){
        faces[cards[i].face]++;
    }

    std::unordered_map<int,int>::iterator it = faces.begin();
    while(it != faces.end()){
        if(it->second == 3){
            thr = true;
        }
        if(it->second == 2){
            tw = true;
        }
        it++;
    }

    if(tw && thr){
        cout << "Full ouse"; 
        return FHOUSE;
    }
        
    return 1;
}

int two_pair_test(vector<Card> &cards){
    int count = 0;
    std::unordered_map<int,int> faces;
    for(int i = 0 ; i < cards.size() ; i++){
        faces[cards[i].face]++;
    }

    std::unordered_map<int,int>::iterator it = faces.begin();
    while(it != faces.end()){
        if(it->second == 2  ){
            count++;
        }
        it++;
    }

    if(count == 2){
        cout << "\n\n\ttwopair found!\n\n";
        return TWOPAIR;
    } else if (count == 1){
        cout << "\n\n\tonepair found!\n\n";
        return ONEPAIR;
    }
        
    return 1;
}


void manage_bet_choice(int choice, Player &player, Round &round){
    int amount;
    while(choice > 0 && choice < 6){
        if (choice == 1){
            cout << NORMT << "Skipping the go I see." << NORM;
            player.in = 1;
            choice = 10;
            
        }
        else if (choice == 2){
            cout << NORMT << "I see you want to make a bet.\n";
            cout << "\n\tWhat do you wanna raise it by: ";
            cin >> amount;
            if(amount < player.money){
                if(round.round_call == 0){
                    round.round_call += amount;
                    player.money -= amount;
                    player.round_bet += amount;
                    round.pot += amount;
                }
                cout << NORMT << "Bet Raised, game on! " << NORM;
                
            }
            else if (player.money == amount){
                 cout << NORMT << "You do realise this means you're going all in?\n";
            }
            
             else {
                cout <<  NORMT << " No money you madman." << NORM;
            }
            choice = 10;
        }
        else if (choice == 3){
            if(round.round_call > 0) {
                if(player.money > round.round_call){
                    player.money -= round.round_call;
                    round.pot += round.round_call;
                    player.round_bet += round.round_call;
                    cout << NORMT << "Bet called!" << NORM;
                }
                else if(player.money == round.round_call){
                    cout << NORMT << "You do realise this means you're going all in?\n";
                }
                else if (player.money < round.round_call){
                    cout << NORMT << "You don't have enough to call the bet." << NORM;
                }
            }
            else{
                cout << NORMT << "No call exists." << NORM;
            }
            choice = 10;
        }

        else if (choice == 4) {
            if(player.money > 0){
                round.pot += player.money;
                player.round_bet += player.money;
                player.money = 0;
                cout << NORMT <<  "Youre all in. Better get lucky!" << NORM;
            }
            choice = 10;
        }

        else if (choice == 5){
            if(round.round_call == 0){
                cout << NORMT << "Player it safe I see. Live a little" << NORM;
            }
            choice = 10;
        }
        
        else {
            cout << NORMT << "You didn't pick an available option. You are automatically folded. Idiot." << NORM;
            player.in = 1;
        }
    }
    cout << "\tComputer is choosing.... " << NORM;
    
    spin_timer();
    
} 

int analyse_hand(Hand &hand, Game_Table &cards){
    //since cards can be classed as numerous types, test from best to worst
    //royal flush, then straight flush.... two pair
    vector<Card> rem_cards {7};
    vector<int> suits;
    vector<int> faces;
    for(int i = 0 ; i < 5 ; i++){
        rem_cards[i] = cards.game_cards[i];
    }
    rem_cards[5] = hand.one;
    rem_cards[6]= hand.two;

    sort(rem_cards.begin(), rem_cards.end());
    for(auto item : rem_cards){
        print_card(item);
    }


    //get all data about the cards
    for(auto item : rem_cards){
        if(item.face > 9 || item.face < 2 ){
            suits.push_back(item.suit);
        }  
    }

    if(royfl_test(suits) != 1){
        return ROYFL;
    } 

    if(strfl_test(rem_cards) != 1){
        return STRAFL;
    }

    if(test_number_of(rem_cards,4,0) != 1){
        return FOFAKIND;
    }

    if(full_house_test(rem_cards) != 1){
        return FHOUSE;
    }

    if(test_number_of(rem_cards,0,5) != 1){
        return FLUSH;
    }

    if(straight_test(rem_cards) != 1){
        return STRAIGHT;
    }
    
     if(test_number_of(rem_cards,3,0) != 1){
        return TOFAKIND;
    }

    int ret = two_pair_test(rem_cards);
    if(ret != 1)
        return ret;

    return HIGHCARD;
}

void spin_timer(){
    int glo = 2;
    cout << "\t";
    int i = 0;
    while(i < glo)
    {
        for (int counter = 0; counter < 4; counter++)
        {
            printf("\b%c", "|/-\\"[counter]);
            fflush(stdout);
            sleep(1);
        }
        i++;
    }
    cout << NORM;
}

//fisher-yates shuffling algorithm
int shuffle_deck(vector<Card> &arr){
    int i = arr.size()-1;
    do
    {   
        if( i == 0 ) break;
        int j = rand() % i;
        Card temp = arr[j];
        arr[j] = arr[i];
        arr[i] = temp;
    } while ( i-- );
    return 0;
}

void generate_deck(vector<Card> &arr){
    for ( int i = 0 ; i < 52 ; ++i){
        arr[i].face = i % 13;
        arr[i].suit = i % 4;
    }
    shuffle_deck(arr);

}

void start_vs_computer(){
    struct Game main_game;
    vector<Player> players(2);
    main_game.players = players;
    play_game(main_game);
       
}


void choose_game(){
    print_welcome();
    int ret;
    cout << NORMT << "What'll it be then: ";
    cin >> ret;
    if(ret > 2){
        cout << NORMT << "Alrighty then, fuck off" << NORM;
        exit(1);
    } else {
        if ( ret == 1 ){
            cout << NORMT << "Sorry not ready yet." << NORM;
        }
        else if ( ret == 2) {
            cout << NORMT << "Now you're talking. Here we go" << NORM;
            start_vs_computer();
        }

    }
    
}

int main(){
    init_game();
    choose_game();
    return 0;
}


