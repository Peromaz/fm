typedef enum {
    ST_BROWSING,
    ST_ASCENDING,
    ST_DESCENDING,
    ST_TERMINAL_TOO_SMALL,
    ST_OPTIONS_MENU_OPEN,
}STATE;

void change_state(STATE state);
STATE get_current_state();
