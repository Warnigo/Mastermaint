#include "mastermind.h"

int is_duplicate(char* str, int num){
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == (num + '0')){
            return 1;
        }
    }
    return 0;
}

char* gen_rand() {
    srand(time(0));
    int i = 0;
    char* result = malloc(sizeof(char)*5);
    while( i < 4 ){
        int rand_num = rand() % 9;
        if( is_duplicate(result, rand_num)== 1 ) { continue; }

        result[i] = rand_num + '0';
        i++;
    }
    result[5] = '\0';
    
    return result;
}

int text_leng(char* str){
    int leng = 0;
    for(int i = 0; str[i] != '\0'; i++){
        leng += 1;
    }
    return leng;
}

int str_cmp(char* str, char* needle){
    if(text_leng(str) != text_leng(needle)){
        return 0;
    }
   
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] != needle[i]){
            return 0;
        }
    }
    return 1;
}

char* input_read(){
    char* answer = malloc(8 * sizeof(char)), ch;
    write(STDOUT_FILENO, ">", 1);
    for( int i = 0; read(STDIN_FILENO, &ch, 1) > 0; i++){
        if(ch == '\n' || ch == '\0') { break; }
        answer[i] = ch;
    }
    return answer;
}

int is_num(char* str){
    for(int i = 0; str[i] != '\0'; i++){
        if( str[i] < '0' || str[i] > '8' ){
            return 0;
        }
    }
    return 1;
}

int input_check(char* inp){
    if(text_leng(inp) != 4){
        return 0;
    }
    for(int i = 0; inp[i] != '\0'; i++){
        if(is_duplicate(&inp[i + 1], inp[i] - '0')){
            return 0;
        }
        if(is_num(inp) == 0){
            return 0;
        }
    }
    return 1;
}

int string_index(char* str, char daf){
    for(int i = 0; str[i] != '\0'; i++){
        if (str[i] == daf){
            return i;
        }
    }
    return 0;
}

void check_pos(char* answer, char* code, piece* pieces){
    for (int i = 0; answer[i] != '\0'; i++ ) {
        if(answer[i] == code[i]){
            pieces->wellplcace += 1; 
            continue;
        }
        if(string_index(answer, code[i]) > 0){
            pieces->missplace += 1;
            continue;
        }
    }
    if ( pieces->wellplcace != 4) {
        printf("Well placed pieces: %d\nMisplaced pieces: %d\n", pieces->wellplcace, pieces->missplace);
    }
}

void read_args(char** code, int* attempts, int ac, char** av) {
    for(int i = 1; i < ac; i++){
        if(str_cmp(av[i], "-c") && input_check(av[i + 1])){
            *code = av[++i];
        }

        if (str_cmp(av[i], "-t") && is_num(av[i + 1])) {
            *attempts = atoi(av[++i]);
        }
    }
}

int main(int argc, char** argv) {
    char* secret_code = gen_rand();
    int attempts = 10, round = 0, is_invalid = 0;
    read_args(&secret_code, &attempts, argc, argv);
    printf("Will you find the secret code?\nPlease enter a valid guess\n");
    while ( round <= attempts ) {
        if(is_invalid == 0){
            printf("---\nRound %d\n", round);
        } else { 
            is_invalid = 0; 
        }
        char* answer = input_read();
        piece pieces = { 0, 0 };
        if(input_check(answer) == 0){
            printf("Wrong input!\n");
            is_invalid = 1; continue;
        }
        check_pos(answer, secret_code, &pieces);
        if (pieces.wellplcace == 4){ 
            printf("Congratz! You did it!\n"); break;
        }
        round++;
    }
    return 0;
}
