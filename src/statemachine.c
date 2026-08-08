#include "include/statemachine.h"

STATE CURRENT_STATE = ST_BROWSING;

void change_state(STATE state){
    CURRENT_STATE = state;
}

STATE get_current_state(){
    return CURRENT_STATE;
}
