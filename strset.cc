#include <iostream>
#include <set>
#include <map>
#include <string>
#include <cassert>

#include "strset.h"
#include "strsetconst.h"

using std::string;
using std::cerr;
using std::endl;
using std::set;
using std::map;

namespace jnp1{
	
	typedef unsigned long u_long;
	typedef set<string> strings;
    typedef map<u_long, strings> sets;

    namespace{
		
		#ifndef NDEBUG
			const bool debug = false;
		#else
			const bool debug = true;
		#endif
		
		enum class msgs{
			NEW,
			DELETE,
			SIZE,
			INSERT,
			REMOVE,
			TEST,
			CLEAR,
			COMP,
			RESULT,
			SUCCESS,
			SET42_FAIL,
			NO_SET_FAIL,
			NO_ELEMENT_FAIL,
			ELEMENT_FAIL,
			NULL_VAULE_FAIL
		};
		
		/* Obsługuje logi, zwraca tekst logu przypisanego fladze liczbowej.
		 * */
		const char * messages(msgs i){
			switch(i){
				case msgs::NEW : return "strset_new ";
				case msgs::DELETE : return "strset_delete ";
				case msgs::SIZE : return "strset_size ";
				case msgs::INSERT : return "strset_insert ";
				case msgs::REMOVE : return "strset_remove ";
				case msgs::TEST : return "strset_test ";
				case msgs::CLEAR : return "strset_clear ";
				case msgs::COMP : return "strset_comp ";

				case msgs::RESULT : return "result: ";
				case msgs::SUCCESS : return "success ";
				case msgs::SET42_FAIL : return "attempt to change set42, fail ";
				case msgs::NO_SET_FAIL : return "no set with specific id, fail ";
				case msgs::NO_ELEMENT_FAIL : return "no such element, fail ";
				case msgs::ELEMENT_FAIL : return "element already in the set, fail ";
				case msgs::NULL_VAULE_FAIL : return "element has a value of NULL, fail ";
			}
			assert(false);
			return nullptr;
		}
		
		/* Zmienne i funkcje obsługujące inicjalizację głównej mapy,
		 * setu42 i strumieni wejścia/wyjścia
		 * */
        bool is_set42 = false;
        u_long set42;
        bool is_stream = false;
        
        void init_stream(){
			std::ios_base::Init mInitializer;
			is_stream = true;
		}
        
        u_long get_set42(){
			if(!is_set42){
				set42 = strset42();
				is_set42 = true;
			}
			return set42;
		}
        
        sets& a(){
			static sets ab;
			return ab;
		}
		
		
		/* Zwraca najmniejszą liczbę, która nie jest kluczem żadnego elementu
		 * zadanej mapy.
		 * */
        u_long get_first_free(){
            if(a().empty()) return 0;
            sets::iterator it = a().begin();
            u_long last = it->first;
            ++it;
            while(it != a().end()){
                u_long i = it->first;
                if(i > last + 1) break;
                last = i;
                ++it;
            }
            return last + 1;
        }
    }

    u_long strset_new(){
		if(!is_stream && !debug) init_stream();
        if(!debug) cerr << messages(msgs::NEW) << endl;	
        u_long result = get_first_free();
        set<string> b;
        a().emplace(result, b);
        if(!debug) cerr << messages(msgs::RESULT) << result << endl;
        return result;
    }
	
    void strset_delete(u_long id){
		if(!is_stream && !debug) init_stream();
        if(!debug) cerr << messages(msgs::DELETE) << id << endl;
        if(id == get_set42()) {
            if(!debug) cerr << messages(msgs::SET42_FAIL) << endl;
            return;
        }
        a().erase(id);
        if(!debug) cerr << messages(msgs::SUCCESS) << endl;
    }

    size_t strset_size(u_long id){
		if(!is_stream && !debug) init_stream();
        if(!debug) cerr << messages(msgs::SIZE) << id << endl;
        if(a().count(id) == 0) {
            if(!debug) cerr << messages(msgs::NO_SET_FAIL) << endl;
            return 0;
        }
        size_t result = (a().find(id))->second.size();
        if(!debug) cerr << messages(msgs::RESULT) << result << endl;
        return result;
    }

    void strset_insert(u_long id, const char* value){
		if(!is_stream && !debug) init_stream();
		if(value == nullptr){
			if(!debug) cerr << messages(msgs::NULL_VAULE_FAIL) << endl;
			return;
		}
        string val(value);
        if(!debug) cerr << messages(msgs::INSERT) << id << " " << val << endl;
        if(id == get_set42() && (val != "42" || strset_size(id) != 0)){
            if(!debug) cerr << messages(msgs::SET42_FAIL) << endl;
            return;
        }
        if(a().count(id) == 0) {
            if(!debug) cerr << messages(msgs::NO_SET_FAIL) << endl;
            return;
        }
        strings& object = (a().find(id))->second;
        if(object.count(val) > 0){
            if(!debug) cerr << messages(msgs::ELEMENT_FAIL) << endl;
            return;
        }
        object.insert(val);
        if(!debug) cerr << messages(msgs::SUCCESS) << endl;
    }

    void strset_remove(u_long id, const char* value){
		if(!is_stream && !debug) init_stream();
		if(value == nullptr){
			if(!debug) cerr << messages(msgs::NULL_VAULE_FAIL) << endl;
			return;
		}
        string val(value);
        if(!debug) cerr << messages(msgs::REMOVE) << id << " " << val << endl;
        if(id == get_set42()) {
            if(!debug) cerr << messages(msgs::SET42_FAIL) << endl;
            return;
        }
        if(a().count(id) == 0) {
            if(!debug) cerr << messages(msgs::NO_SET_FAIL) << endl;
            return;
        }
        strings& object = (a().find(id))->second;
        if(object.count(val) == 0){
            if(!debug) cerr << messages(msgs::NO_ELEMENT_FAIL) << endl;
            return;
        }
        object.erase(value);
        if(!debug) cerr << messages(msgs::SUCCESS) << endl;
    }

    int strset_test(u_long id, const char* value){
		if(!is_stream && !debug) init_stream();
		if(value == nullptr){
			if(!debug) cerr << messages(msgs::NULL_VAULE_FAIL) << endl;
			return 0;
		}
        string val(value);
        if(!debug) cerr << messages(msgs::TEST) << id << " " << val << endl;
        if(a().count(id) == 0) {
            if(!debug) cerr << messages(msgs::NO_SET_FAIL) << endl;
            return 0;
        }
        if((a().find(id))->second.count(value) == 0){
            if(!debug) cerr << messages(msgs::RESULT) << 0 << endl;
            return 0;
        }
        if(!debug) cerr << messages(msgs::RESULT) << 1 << endl;
        return 1;
    }

    void strset_clear(u_long id){
		if(!is_stream && !debug) init_stream();
        if(!debug) cerr << messages(msgs::CLEAR) << id << endl;
        if(id == get_set42()) {
            if(!debug) cerr << messages(msgs::SET42_FAIL) << endl;
            return;
        }
        if(a().count(id) == 0) {
            if(!debug) cerr << messages(msgs::NO_SET_FAIL) << endl;
            return;
        }
        (a().find(id))->second.clear();
        if(!debug) cerr << messages(msgs::SUCCESS) << endl;
    }

    int strset_comp(u_long id1, u_long id2){
		if(!is_stream && !debug) init_stream();
        if(!debug) cerr << messages(msgs::COMP) << id1 << " " << id2 << endl;
        if(a().count(id1) == 0 && a().count(id2) == 0) {
            if(!debug) cerr << messages(msgs::RESULT) << 0 << endl;
            return 0;
        }
        if(a().count(id1) == 0) {
            if(!debug) cerr << messages(msgs::RESULT) << -1 << endl;
            return -1;
        }
        if(a().count(id2) == 0) {
            if(!debug) cerr << messages(msgs::RESULT) << 1 << endl;
            return 1;
        }
        strings& first = a().find(id1)->second;
        strings& second = a().find(id2)->second;
        strings::iterator it1 = first.begin();
        strings::iterator it2 = second.begin();
        while(it1 != first.end() && it2 != second.end()){
            if(it1->compare(*it2) < 0) {
                if(!debug) cerr << messages(msgs::RESULT) << -1 << endl;
                return -1;
            }
            if(it1->compare(*it2) > 0)  {
                if(!debug) cerr << messages(msgs::RESULT) << 1 << endl;
                return 1;
            }
            it1++;
            it2++;
        }
        if(it1 != first.end()) {
            if(!debug) cerr << messages(msgs::RESULT) << 1 << endl;
            return 1;
        }
        if(it2 != second.end()) {
            if(!debug) cerr << messages(msgs::RESULT) << -1 << endl;
            return -1;
        }
        if(!debug) cerr << messages(msgs::RESULT) << 0 << endl;
        return 0;
    }
}
