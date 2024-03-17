#include <data.h>
#include <decl.h>
#include <defs.h>

//return the position of character c
//in string s, or -1 if c not found
static int chrpos(char *s, int c){
    char *p;

    p = strchr(s, c);
    return (p ? p - s : -1);
}

//next char
static int next(void){
    int c;

    if (Putback){ //char put
        c = Putback; //back if there is one
        Putback = 0;
        return c;
    }
    c = fgetc(Infile); //read from input
    if ('\n' == c)
        Line++; //increment line count
    return c; 
}

//put back unwanted char
static void putback (int c){
    Putback = c;
}

static int skip(void){
    int c;

    c = next();
    while (' ' == c || '\t' == c || '\n' == c || '\r' == c || '\f' == c){
        c = next();
    }
    return c;
}
//scan and return an integer literal
//value from the input file
static int scanint(int c){
    int k, val = 0;

    //conver each char int an int value
    while ((k = chrpos("0123456789", c)) >= 0){
        val = val * 10 + k;
        c = next();
    }

    //hit non int char
    putback(c);
    return val;
}


//scan and return next token found in the input
//return 1 if token valid, 0 if no tokens left
int scan(struct token *t){
    int c;

    //skip whitespace
    c = skip();

    //determine the token based on the input char
    switch (c) {
    case EOF:
      return (0);
    case '+':
      t->token = T_PLUS;
      break;
    case '-':
      t->token = T_MINUS;
      break;
    case '*':
      t->token = T_STAR;
      break;
    case '/':
      t->slash = T_SLASH;
      break;  
    default:
      //if its a digit, scan the
      //literal integer value in
      if(isdigit(c)){
        t->intvalue = scanint(c);
        t->token = T_INTLIT;
        break;
      }
    printf("Unrecognised charcater %c on line %d\n", c, Line);
    exit(1);
    }

    //found a token
    return (1);

}