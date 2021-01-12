#include <eosio/eosio.hpp>

using namespace eosio;

CONTRACT tictactoe : public contract{
    public:
        using contract::contract;
        TABLE games {
            name host;
            name challenger;
            auto primary_key() const {return host.value;}
        };
         
        typedef multi_index<name( "gameshelf" ), games> games_table;

        ACTION welcome(eosio::name host, eosio::name challenger){
            print("Welcome, challengers ", host ," and ",challenger,"!");    
        };

        ACTION create( const name &challenger, name &host ){
            require_auth(host);
            check(host != challenger, "Challenger should not be the same as the host.");

            games_table existingGames( get_self(), get_self().value );
            auto itr = existingGames.find(challenger.value);
            check(itr == existingGames.end(), "Game already exists.");
            existingGames.emplace(host, [&](auto &game) {
                game.challenger = challenger;
                game.host = host;
            });
        };


        ACTION close( const name &challenger, const name &host ){
            check(has_auth(host), "Only the host can close the game.");
            require_auth(host);

            games_table existingGames(get_self(), get_self().value);
            auto itr = existingGames.find(challenger.value);
            check(itr != existingGames.end(), "Game does not exist.");
            existingGames.erase(itr);        
        };
};