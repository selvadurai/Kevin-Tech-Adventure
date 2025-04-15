#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Token types
typedef enum {
    TOKEN_EOF,
    TOKEN_NUMBER,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_ASSIGN,
    TOKEN_OPERATOR,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE
} TokenType;

// Keywords
typedef enum {
    KW_PRINT,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_END,
    KW_TRUE,
    KW_FALSE
} Keyword;

// Token structure
typedef struct {
    TokenType type;
    union {
        double number_value;
        char* string_value;
        Keyword keyword;
    } value;
} Token;

// Lexer structure
typedef struct {
    const char* input;
    int position;
    int length;
    char current_char;
} Lexer;

// Variable structure
typedef struct {
    char* name;
    double value;
} Variable;

// Variable storage
typedef struct {
    Variable* variables;
    int count;
    int capacity;
} VariableStorage;

// Initialize the lexer
void lexer_init(Lexer* lexer, const char* input) {
    lexer->input = input;
    lexer->position = 0;
    lexer->length = strlen(input);
    lexer->current_char = (lexer->length > 0) ? input[0] : '\0';
}

// Move to the next character
void lexer_advance(Lexer* lexer) {
    lexer->position++;
    if (lexer->position < lexer->length) {
        lexer->current_char = lexer->input[lexer->position];
    } else {
        lexer->current_char = '\0';
    }
}

// Skip whitespace
void lexer_skip_whitespace(Lexer* lexer) {
    while (lexer->current_char != '\0' && isspace(lexer->current_char)) {
        lexer_advance(lexer);
    }
}

// Get a number token
Token lexer_get_number(Lexer* lexer) {
    Token token;
    token.type = TOKEN_NUMBER;
    
    char* buffer = (char*)malloc(100);
    int i = 0;
    
    while (lexer->current_char != '\0' && (isdigit(lexer->current_char) || lexer->current_char == '.')) {
        buffer[i++] = lexer->current_char;
        lexer_advance(lexer);
    }
    buffer[i] = '\0';
    
    token.value.number_value = atof(buffer);
    free(buffer);
    
    return token;
}

// Get an identifier or keyword token
Token lexer_get_identifier(Lexer* lexer) {
    Token token;
    
    char* buffer = (char*)malloc(100);
    int i = 0;
    
    while (lexer->current_char != '\0' && (isalnum(lexer->current_char) || lexer->current_char == '_')) {
        buffer[i++] = lexer->current_char;
        lexer_advance(lexer);
    }
    buffer[i] = '\0';
    
    // Check if it's a keyword
    if (strcmp(buffer, "print") == 0) {
        token.type = TOKEN_KEYWORD;
        token.value.keyword = KW_PRINT;
    } else if (strcmp(buffer, "if") == 0) {
        token.type = TOKEN_KEYWORD;
        token.value.keyword = KW_IF;
    } else if (strcmp(buffer, "else") == 0) {
        token.type = TOKEN_KEYWORD;
        token.value.keyword = KW_ELSE;
    } else if (strcmp(buffer, "while") == 0) {
        token.type = TOKEN_KEYWORD;
        token.value.keyword = KW_WHILE;
    } else if (strcmp(buffer, "end") == 0) {
        token.type = TOKEN_KEYWORD;
        token.value.keyword = KW_END;
    } else if (strcmp(buffer, "true") == 0) {
        token.type = TOKEN_KEYWORD;
        token.value.keyword = KW_TRUE;
    } else if (strcmp(buffer, "false") == 0) {
        token.type = TOKEN_KEYWORD;
        token.value.keyword = KW_FALSE;
    } else {
        // It's a regular identifier
        token.type = TOKEN_IDENTIFIER;
        token.value.string_value = strdup(buffer);
    }
    
    free(buffer);
    return token;
}

// Get the next token
Token lexer_get_next_token(Lexer* lexer) {
    Token token;
    
    while (lexer->current_char != '\0') {
        // Skip whitespace
        if (isspace(lexer->current_char)) {
            lexer_skip_whitespace(lexer);
            continue;
        }
        
        // Numbers
        if (isdigit(lexer->current_char)) {
            return lexer_get_number(lexer);
        }
        
        // Identifiers and keywords
        if (isalpha(lexer->current_char) || lexer->current_char == '_') {
            return lexer_get_identifier(lexer);
        }
        
        // Assignment and equality operator
        if (lexer->current_char == '=') {
            lexer_advance(lexer);
            if (lexer->current_char == '=') {
                token.type = TOKEN_OPERATOR;
                token.value.string_value = strdup("==");
                lexer_advance(lexer);
                return token;
            } else {
                token.type = TOKEN_ASSIGN;
                token.value.string_value = strdup("=");
                return token;
            }
        }
        
        // Operators
        if (lexer->current_char == '+' || lexer->current_char == '-' || 
            lexer->current_char == '*' || lexer->current_char == '/' ||
            lexer->current_char == '<' || lexer->current_char == '>' ||
            lexer->current_char == '!') {
            
            char op[3] = {lexer->current_char, '\0', '\0'};
            lexer_advance(lexer);
            
            // Check for two-character operators like !=, <=, >=
            if (lexer->current_char == '=' && (op[0] == '!' || op[0] == '<' || op[0] == '>')) {
                op[1] = '=';
                lexer_advance(lexer);
            }
            
            token.type = TOKEN_OPERATOR;
            token.value.string_value = strdup(op);
            return token;
        }
        
        // Parentheses and braces
        if (lexer->current_char == '(') {
            token.type = TOKEN_LPAREN;
            lexer_advance(lexer);
            return token;
        }
        
        if (lexer->current_char == ')') {
            token.type = TOKEN_RPAREN;
            lexer_advance(lexer);
            return token;
        }
        
        if (lexer->current_char == '{') {
            token.type = TOKEN_LBRACE;
            lexer_advance(lexer);
            return token;
        }
        
        if (lexer->current_char == '}') {
            token.type = TOKEN_RBRACE;
            lexer_advance(lexer);
            return token;
        }
        
        // Semicolon
        if (lexer->current_char == ';') {
            token.type = TOKEN_SEMICOLON;
            lexer_advance(lexer);
            return token;
        }
        
        // Unknown character
        printf("Error: Unknown character '%c'\n", lexer->current_char);
        exit(1);
    }
    
    // End of file
    token.type = TOKEN_EOF;
    return token;
}

// Initialize variable storage
void var_storage_init(VariableStorage* storage) {
    storage->capacity = 10;
    storage->count = 0;
    storage->variables = (Variable*)malloc(sizeof(Variable) * storage->capacity);
}

// Find a variable by name
int var_storage_find(VariableStorage* storage, const char* name) {
    for (int i = 0; i < storage->count; i++) {
        if (strcmp(storage->variables[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Set variable value
void var_storage_set(VariableStorage* storage, const char* name, double value) {
    int index = var_storage_find(storage, name);
    
    if (index != -1) {
        // Update existing variable
        storage->variables[index].value = value;
    } else {
        // Create new variable
        if (storage->count >= storage->capacity) {
            // Resize if needed
            storage->capacity *= 2;
            storage->variables = (Variable*)realloc(storage->variables, sizeof(Variable) * storage->capacity);
        }
        
        storage->variables[storage->count].name = strdup(name);
        storage->variables[storage->count].value = value;
        storage->count++;
    }
}

// Get variable value
double var_storage_get(VariableStorage* storage, const char* name) {
    int index = var_storage_find(storage, name);
    
    if (index != -1) {
        return storage->variables[index].value;
    } else {
        printf("Error: Variable '%s' not defined\n", name);
        exit(1);
    }
}

// Free variable storage
void var_storage_free(VariableStorage* storage) {
    for (int i = 0; i < storage->count; i++) {
        free(storage->variables[i].name);
    }
    free(storage->variables);
}

// A simple interpreter for demonstration
void interpret(const char* source) {
    Lexer lexer;
    lexer_init(&lexer, source);
    
    VariableStorage vars;
    var_storage_init(&vars);
    
    Token token = lexer_get_next_token(&lexer);
    
    while (token.type != TOKEN_EOF) {
        // Handle variable assignment
        if (token.type == TOKEN_IDENTIFIER) {
            char* var_name = strdup(token.value.string_value);
            
            token = lexer_get_next_token(&lexer);
            if (token.type != TOKEN_ASSIGN) {
                printf("Error: Expected '=' after identifier\n");
                free(var_name);
                exit(1);
            }
            
            token = lexer_get_next_token(&lexer);
            if (token.type != TOKEN_NUMBER) {
                printf("Error: Expected a number after '='\n");
                free(var_name);
                exit(1);
            }
            
            var_storage_set(&vars, var_name, token.value.number_value);
            free(var_name);
        }
        
        // Handle print statement
        else if (token.type == TOKEN_KEYWORD && token.value.keyword == KW_PRINT) {
            token = lexer_get_next_token(&lexer);
            
            if (token.type == TOKEN_IDENTIFIER) {
                double value = var_storage_get(&vars, token.value.string_value);
                printf("%f\n", value);
            } else if (token.type == TOKEN_NUMBER) {
                printf("%f\n", token.value.number_value);
            } else {
                printf("Error: Expected identifier or number after 'print'\n");
                exit(1);
            }
        }
        
        // Get the next token
        token = lexer_get_next_token(&lexer);
    }
    
    var_storage_free(&vars);
}

int main() {
    // Example program
    const char* program = "x = 10; y = 20; print x; print y;";
    printf("Running program: %s\n", program);
    interpret(program);
    
    return 0;
}
