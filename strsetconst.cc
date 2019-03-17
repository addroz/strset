#include <iostream>
#include <cassert>

#include "strset.h"
#include "strsetconst.h"

using std::cerr;
using std::string;
using std::endl;

namespace jnp1{
	
	typedef unsigned long u_long;
	
	namespace{
        #ifndef NDEBUG
			const bool debug = false;
		#else
			const bool debug = true;
		#endif
		
		enum class msgs{
			BEGIN,
			END
		};
		
		/* Obsługuje logi, zwraca tekst logu przypisanego fladze liczbowej.
		 * */
		const char * messages(msgs i){
			switch(i){
				case msgs::BEGIN : return "Initialization of set42 begin"; break;
				case msgs::END : return "Initialization of set42 end"; break;
			}
			assert(false);
			return nullptr;
		}
        
        
        u_long set42;
        bool is_set42 = false;
        
        bool is_stream = false;
        
        void init_stream(){
			std::ios_base::Init mInitializer;
			is_stream = true;
		}
    }

    u_long strset42(){
		if(!is_stream && !debug) init_stream();
        if(!is_set42){
            if(!debug) cerr << messages(msgs::BEGIN) << endl;
            is_set42 = true;
            set42 = strset_new();
            strset_insert(set42, "42");
            if(!debug) cerr << messages(msgs::END) << endl;
        }
        return set42;
    }
}
