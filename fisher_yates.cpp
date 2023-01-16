#include <string.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <valarray>
#include <bits/stdc++.h>
#include <set>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif


typedef std::vector<int>::iterator vec_iter;

using std::cout; using std::vector;
using std::map; using std::string;
using std::make_pair; using std::sort;
using std::endl; using std::set;

enum hands{HIGHCARD=1, ONEPAIR, TWOPAIR, TOFAKIND, STRAIGHT, FLUSH, FHOUSE, FOFAKIND, STRAFL, ROYFL};

static map<int,string> high_cards;
static map<int,string> suits;

struct Card {
   int face, suit;
   bool operator < (Card &x){
       return face < x.face;
   } 
};

struct Hand {
    Card one,two;
};

struct Game_Table{
    vector<Card> game_cards{5};
};

void print_card(struct Card &card){
    //used maps here, with the face and suit as the distinguishing key so it 
    //prints the text where there is text
    if(card.face > 10 || card.face < 2){
        cout << "\n\tCard is -> ";
        cout << high_cards.find(card.face)->second;
        cout << " of " << suits.find(card.suit)->second;
    } else {
        cout << "\n\tCard is -> " << card.face << " of "
                            << suits.find(card.suit)->second;
    }

}

vector<int> slice(const vector<int>& v, int start=0, int end=-1) {
    int oldlen = v.size();
    int newlen;

    if (end == -1 or end >= oldlen){
        newlen = oldlen-start;
    } else {
        newlen = end-start;
    }

    vector<int> nv(newlen);

    for (int i=0; i<newlen; i++) {
        nv[i] = v[start+i];
    }
    return nv;
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

void print_hand(struct Hand &hand){
    cout << "\tYour cards this round: ";
    print_card(hand.one), print_card(hand.two);
}


//tests for all straight flushes not in the form 10,J,Q,K,A (as that would be a royal flush anyway)
int strfl_test(vector<Card> &cards, Hand &hand){
      
    bool contains_player_card = false;

    //print_hand(hand);
    for(int i = 0 ; i < 3; i++){
        if(cards[i+1].face == cards[i].face+1 && cards[i+2].face == cards[i+1].face+1 && cards[i+3].face == cards[i+2].face+1 
        && cards[i+4].face == cards[i+3].face+1){
            if(cards[i].suit == cards[i+1].suit && cards[i+1].suit == cards[i+2].suit && cards[i+2].suit == cards[i+3].suit && cards[i+3].suit == cards[i+4].suit){
                if((cards[i].suit == hand.one.suit && cards[i].face == hand.one.face) || (cards[i].suit == hand.two.suit && cards[i].face == hand.two.face)){
                    contains_player_card = true;
                }
                else if ((cards[i+1].suit == hand.one.suit && cards[i+1].face == hand.one.face) || (cards[i+1].suit == hand.two.suit && cards[i+1].face == hand.two.face)){
                    contains_player_card = true;
                }
                else if ((cards[i+2].suit == hand.one.suit && cards[i+2].face == hand.one.face) || (cards[i+2].suit == hand.two.suit && cards[i+2].face == hand.two.face)){
                    contains_player_card = true;
                }
                else if ((cards[i+3].suit == hand.one.suit && cards[i+3].face == hand.one.face) || (cards[i+3].suit == hand.two.suit && cards[i+3].face == hand.two.face)){
                    contains_player_card = true;
                }
                else if ((cards[i+4].suit == hand.one.suit && cards[i+4].face == hand.one.face) || (cards[i+4].suit == hand.two.suit && cards[i+4].face == hand.two.face)){
                    contains_player_card = true;
                }
               
            }
        }   
    }

    if(contains_player_card){
        cout << "\n\t Straight flush found!";
        return STRAFL;
    }    
    return 1;
}

int no_kind_test(vector<Card> &cards, Hand &hand, int x){

    //vars needed for func
    bool contains_player_card = false;
    std::unordered_map<int,int> faces;
    int check_no;

    for(int i = 0 ; i < cards.size() ; i++){
        faces[cards[i].face]++;
    }

    std::unordered_map<int,int>::iterator it = faces.begin();
    while(it != faces.end()){
        if(it->second == x){
             if(hand.one.face == it->first || hand.two.face == it->first) {contains_player_card = true;}
        }
        it++;
    }
    
    if(contains_player_card){
        if(x == 4){
            cout << "\n\t Found a four of a kind ! " << endl;
            return FOFAKIND;
        } else if (x == 3){
            cout << "\n\t Found a three of a kind ! " << endl;
            return TOFAKIND;
        }
        
    }

    return 1;
}



//function to test for flushes
int flush_test(vector<Card> &cards, Hand &hand){
    bool contains_player_card = false;
    std::unordered_map<int,int> suits;
    for(int i = 0 ; i < cards.size() ; i++){
        suits[cards[i].suit]++;
    }
    std::unordered_map<int,int>::iterator it = suits.begin();
    while(it != suits.end()){
        if(it->second == 5)
            if(hand.one.suit == it->first || hand.two.suit == it->first){contains_player_card = true;}
        it++;
    }

    if(contains_player_card){
        cout << "\n\tFound a flush\n";
        return FLUSH;
    }
    return 1;
}

//prototype of the eventual function
//royal flush, flush, straight flush, four of a kind, 3 of a kind
//all use a function identical below except they are all hard coded
//in the future I would like to modularise it a bit better

//11-03-2021
//needs to be scrapped as it arbritrarily looks. I had forgotten this when
//first writingthis func.

/*
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
*/

// the most challenging to write due to how I generated the original cards.
// jack is a 0 and ace is 1 or beyond king (14), so the faces need to be altered before writing
// my naive test of testing to see if the next 5 cards are all in
// consecutive order
int straight_test(vector<Card> &cards, Hand &hand){

    vector<int> faces;
    set<int> set_faces;
    bool contains_player_card = false;

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
        set_faces.insert(item);
    }

    vector<int> unique_faces(set_faces.size());
    for(const auto &it : set_faces){
        unique_faces.push_back(it);
    }

    //lazy copy - bad
   //vector<int> unique_faces = vector<int>(set_faces.begin(), set_faces.end());

    for(int i = 0 ; i < unique_faces.size()-4 ; i++){
        if(unique_faces[i+1] == unique_faces[i]+1 && unique_faces[i+2] == unique_faces[i+1]+1 && unique_faces[i+3] == unique_faces[i+2]+1 
            && unique_faces[i+4] == unique_faces[i+3]+1){
                if(unique_faces[i] == hand.one.face || unique_faces[i] == hand.two.face){contains_player_card = true;}
                else if(unique_faces[i+1] == hand.one.face || unique_faces[i+1] == hand.two.face){contains_player_card = true;}
                else if(unique_faces[i+2] == hand.one.face || unique_faces[i+2] == hand.two.face){contains_player_card = true;}
                else if(unique_faces[i+3] == hand.one.face || unique_faces[i+3] == hand.two.face){contains_player_card = true;}
                else if(unique_faces[i+4] == hand.one.face || unique_faces[i+4] == hand.two.face){contains_player_card = true;}
            }
    }

    if(contains_player_card){
        cout << "\n\tstraight found " << endl;
        return STRAIGHT;
    }
    return 1;
}

int full_house_test(vector<Card> &cards, Hand &hand){
    bool thr = 0;
    bool tw = 0;

    bool contains_player_card = false;

    std::unordered_map<int,int> faces;
    for(int i = 0 ; i < cards.size() ; i++){
        faces[cards[i].face]++;
    }

    std::unordered_map<int,int>::iterator it = faces.begin();
    while(it != faces.end()){
        if(it->second == 3){
            if(it->first == hand.one.face || it->first == hand.two.face){contains_player_card = true;}
            thr = true;
        }
        if(it->second == 2){
            if(it->first == hand.one.face || it->first == hand.two.face){contains_player_card = true;}
            tw = true;
        }
        it++;
    }

    if(tw && thr && contains_player_card){
        cout << "\n\tFull ouse"; 
        return FHOUSE;
    }
        
    return 1;
}

int two_pair_test(vector<Card> &cards, Hand &hand){

    int count = 0;
    std::unordered_map<int,int> faces;

    for(int i = 0 ; i < cards.size() ; i++){
        faces[cards[i].face]++;
    }

    std::unordered_map<int,int>::iterator it = faces.begin();
    while(it != faces.end()){
        if(it->second == 2  ){
            //we actually dont need to check this. If there is a twopair on the table, it still counts to
            //being a twopair, and will still be counted .If it is the best hand the person with the 
            //highest kicker will win - its the best 5 cards on the table that counts
            //if(it->first == hand.one.face || it->first == hand.two.face){contains_player_card = true;}
            count++;
        }
        it++;
    }

    if(count == 2 ){
        cout << "\n\n\ttwopair found!\n\n";
        return TWOPAIR;
    } else if (count == 1){
        cout << "\n\n\tonepair found!\n\n";
        return ONEPAIR;
    }
        
    return 1;
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
    
    //this is a naive check to see if one or both cards are involved in the final hand
    //not actually too sure on the rules, if one/both of players cards can have to be involved
    //in the royal flush. Currently it checks for just one. IF I find out it is two the middle OR
    //just needs to be changed to an AND.
    //note this check is in every func. can easily change all of them.

    if((hand.one.face > 9 || hand.one.face < 2) || (hand.two.face > 9 || hand.two.face < 2)){
        if(royfl_test(suits) != 1){
            return ROYFL;
        }   
    }

    if(strfl_test(rem_cards, hand) != 1){
        return STRAFL;
    }

    if(no_kind_test(rem_cards, hand, 4) != 1){
        return FOFAKIND;
    }

    if(full_house_test(rem_cards, hand) != 1){
        return FHOUSE;
    }

    if(flush_test(rem_cards, hand) != 1){
        return FLUSH;
    }

    if(straight_test(rem_cards, hand) != 1){
        return STRAIGHT;
    }
    
     if(no_kind_test(rem_cards, hand, 3) != 1){
        return TOFAKIND;
    }

    int ret = two_pair_test(rem_cards, hand);
    if(ret != 1)
        return ret;

    return HIGHCARD;
}
    

int shuffle_ve(std::vector<Card> &arr){
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

void generate_deck(std::vector<Card> &arr){
    for ( int i = 0 ; i < 52 ; ++i){
        arr[i].face = i % 13;
        arr[i].suit = i % 4;
    }
}

int main(){
    srand(time(NULL));

    std::cout << "\n";
    //static std::vector<Card> deck(52);
    //generate_deck(deck);

    high_cards.insert(make_pair(11,"Queen"));
    high_cards.insert(make_pair(12,"King"));
    high_cards.insert(make_pair(0,"Jack"));
    high_cards.insert(make_pair(1,"Ace"));

    //init map for suits to be printed 
    suits.insert(make_pair(0, "Spades"));
    suits.insert(make_pair(1, "Hearts"));
    suits.insert(make_pair(2, "Diamonds"));
    suits.insert(make_pair(3, "Clubs"));

    Hand test_hand;
    test_hand.one.face = 8;
    test_hand.one.suit = 2;
    test_hand.two.face = 4;
    test_hand.two.suit = 0;

    Game_Table test_game;

    test_game.game_cards[0].face = 1;
    test_game.game_cards[0].suit = 0;

    test_game.game_cards[1].face = 9;
    test_game.game_cards[1].suit = 1;

    test_game.game_cards[2].face = 2;
    test_game.game_cards[2].suit = 3;

    test_game.game_cards[3].face = 7;
    test_game.game_cards[3].suit = 0;

    test_game.game_cards[4].face = 8;
    test_game.game_cards[4].suit = 2;

    int ret = analyse_hand(test_hand, test_game);
    cout << "\n\n";
    return 0;
}


