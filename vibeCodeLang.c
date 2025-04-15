// Simple Programming Language Interpreter
// Features: variables, arrays, if statements, for loops, functions

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_STRING,
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULTIPLY, TOKEN_DIVIDE,
    TOKEN_ASSIGN,
    TOKEN_EQUAL, TOKEN_NOT_EQUAL, TOKEN_LESS, TOKEN_GREATER, TOKEN_LESS_EQUAL, TOKEN_GREATER_EQUAL,
    TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_LBRACE, TOKEN_RBRACE,
    TOKEN_LBRACKET, TOKEN_RBRACKET,
    TOKEN_SEMICOLON, TOKEN_COMMA,
    TOKEN_IF, TOKEN_ELSE, TOKEN_FOR, TOKEN_WHILE,
    TOKEN_FUNCTION, TOKEN_RETURN, TOKEN_PRINT
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char* value;
    int line;
} Token;

// Lexer structure
typedef struct {
    const char* source;
    int position;
    int line;
} Lexer;

// Value types
typedef enum {
    VAL_NULL,
    VAL_NUMBER,
    VAL_STRING,
    VAL_ARRAY,
    VAL_FUNCTION
} ValueType;

// Forward declaration for function type
typedef struct Node Node;

// Value structure
typedef struct Value {
    ValueType type;
    union {
        double number;
        char* string;
        struct {
            struct Value* items;
            int count;
            int capacity;
        } array;
        struct {
            Node* node;
            char** param_names;
            int param_count;
        } function;
    } as;
} Value;

// Variable structure
typedef struct {
    char* name;
    Value value;
} Variable;

// Symbol table for variables
typedef struct {
    Variable* variables;
    int count;
    int capacity;
} SymbolTable;

// AST node types
typedef enum {
    NODE_PROGRAM,
    NODE_BLOCK,
    NODE_EXPRESSION,
    NODE_BINARY,
    NODE_UNARY,
    NODE_LITERAL,
    NODE_VARIABLE,
    NODE_ASSIGNMENT,
    NODE_IF,
    NODE_FOR,
    NODE_WHILE,
    NODE_FUNCTION,
    NODE_CALL,
    NODE_RETURN,
    NODE_PRINT,
    NODE_ARRAY,
    NODE_INDEX
} NodeType;

// AST node structure
struct Node {
    NodeType type;
    struct Node* left;
    struct Node* right;
    struct Node* condition;
    struct Node* body;
    struct Node* else_body;
    struct Node* init;
    struct Node* update;
    struct Node* next;
    char* name;
    Value value;
    struct Node** params;
    int param_count;
    struct Node** args;
    int arg_count;
    struct Node** statements;
    int statement_count;
};

// Interpreter structure
typedef struct {
    Token current_token;
    Lexer lexer;
    SymbolTable global_table;
    SymbolTable local_table;
    Value return_value;
    int returning;
} Interpreter;

// Function prototypes
Lexer init_lexer(const char* source);
Token get_next_token(Lexer* lexer);
Node* parse_program(Interpreter* interpreter);
Node* parse_statement(Interpreter* interpreter);
Node* parse_expression(Interpreter* interpreter);
Value evaluate(Interpreter* interpreter, Node* node);
void print_value(Value value);
void free_node(Node* node);
void free_value(Value value);

// Initialize a lexer with source code
Lexer init_lexer(const char* source) {
    Lexer lexer;
    lexer.source = source;
    lexer.position = 0;
    lexer.line = 1;
    return lexer;
}

// Check if character is part of an identifier
int is_alpha(char c) {
    return isalpha(c) || c == '_';
}

// Check if character is part of a number
int is_digit(char c) {
    return isdigit(c);
}

// Skip whitespace and comments
void skip_whitespace_and_comments(Lexer* lexer) {
    while (lexer->source[lexer->position] != '\0') {
        if (isspace(lexer->source[lexer->position])) {
            if (lexer->source[lexer->position] == '\n') {
                lexer->line++;
            }
            lexer->position++;
        } else if (lexer->source[lexer->position] == '/' && lexer->source[lexer->position + 1] == '/') {
            // Skip line comments
            lexer->position += 2;
            while (lexer->source[lexer->position] != '\0' && lexer->source[lexer->position] != '\n') {
                lexer->position++;
            }
        } else {
            break;
        }
    }
}

// Read an identifier or keyword
Token read_identifier(Lexer* lexer) {
    int start = lexer->position;
    while (is_alpha(lexer->source[lexer->position]) || is_digit(lexer->source[lexer->position])) {
        lexer->position++;
    }
    
    int length = lexer->position - start;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start, length);
    value[length] = '\0';
    
    Token token;
    token.line = lexer->line;
    token.value = value;
    
    // Check for keywords
    if (strcmp(value, "if") == 0) token.type = TOKEN_IF;
    else if (strcmp(value, "else") == 0) token.type = TOKEN_ELSE;
    else if (strcmp(value, "for") == 0) token.type = TOKEN_FOR;
    else if (strcmp(value, "while") == 0) token.type = TOKEN_WHILE;
    else if (strcmp(value, "function") == 0) token.type = TOKEN_FUNCTION;
    else if (strcmp(value, "return") == 0) token.type = TOKEN_RETURN;
    else if (strcmp(value, "print") == 0) token.type = TOKEN_PRINT;
    else token.type = TOKEN_IDENTIFIER;
    
    return token;
}

// Read a number
Token read_number(Lexer* lexer) {
    int start = lexer->position;
    while (is_digit(lexer->source[lexer->position])) {
        lexer->position++;
    }
    
    // Handle decimal part
    if (lexer->source[lexer->position] == '.' && is_digit(lexer->source[lexer->position + 1])) {
        lexer->position++; // Skip the dot
        while (is_digit(lexer->source[lexer->position])) {
            lexer->position++;
        }
    }
    
    int length = lexer->position - start;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start, length);
    value[length] = '\0';
    
    Token token;
    token.type = TOKEN_NUMBER;
    token.value = value;
    token.line = lexer->line;
    return token;
}

// Read a string
Token read_string(Lexer* lexer) {
    lexer->position++; // Skip opening quote
    int start = lexer->position;
    
    while (lexer->source[lexer->position] != '\0' && lexer->source[lexer->position] != '"') {
        if (lexer->source[lexer->position] == '\n') lexer->line++;
        lexer->position++;
    }
    
    int length = lexer->position - start;
    char* value = malloc(length + 1);
    strncpy(value, lexer->source + start, length);
    value[length] = '\0';
    
    if (lexer->source[lexer->position] == '"') {
        lexer->position++; // Skip closing quote
    }
    
    Token token;
    token.type = TOKEN_STRING;
    token.value = value;
    token.line = lexer->line;
    return token;
}

// Get the next token from the source code
Token get_next_token(Lexer* lexer) {
    skip_whitespace_and_comments(lexer);
    
    Token token;
    token.line = lexer->line;
    token.value = NULL;
    
    if (lexer->source[lexer->position] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }
    
    char c = lexer->source[lexer->position];
    
    if (is_alpha(c)) {
        return read_identifier(lexer);
    }
    
    if (is_digit(c)) {
        return read_number(lexer);
    }
    
    if (c == '"') {
        return read_string(lexer);
    }
    
    lexer->position++;
    
    switch (c) {
        case '+': token.type = TOKEN_PLUS; break;
        case '-': token.type = TOKEN_MINUS; break;
        case '*': token.type = TOKEN_MULTIPLY; break;
        case '/': token.type = TOKEN_DIVIDE; break;
        case '(': token.type = TOKEN_LPAREN; break;
        case ')': token.type = TOKEN_RPAREN; break;
        case '{': token.type = TOKEN_LBRACE; break;
        case '}': token.type = TOKEN_RBRACE; break;
        case '[': token.type = TOKEN_LBRACKET; break;
        case ']': token.type = TOKEN_RBRACKET; break;
        case ';': token.type = TOKEN_SEMICOLON; break;
        case ',': token.type = TOKEN_COMMA; break;
        
        case '=':
            if (lexer->source[lexer->position] == '=') {
                lexer->position++;
                token.type = TOKEN_EQUAL;
            } else {
                token.type = TOKEN_ASSIGN;
            }
            break;
            
        case '!':
            if (lexer->source[lexer->position] == '=') {
                lexer->position++;
                token.type = TOKEN_NOT_EQUAL;
            } else {
                // Handle unary not if needed
                printf("Error at line %d: Unexpected token '!'\n", lexer->line);
                token.type = TOKEN_EOF;
            }
            break;
            
        case '<':
            if (lexer->source[lexer->position] == '=') {
                lexer->position++;
                token.type = TOKEN_LESS_EQUAL;
            } else {
                token.type = TOKEN_LESS;
            }
            break;
            
        case '>':
            if (lexer->source[lexer->position] == '=') {
                lexer->position++;
                token.type = TOKEN_GREATER_EQUAL;
            } else {
                token.type = TOKEN_GREATER;
            }
            break;
            
        default:
            // Unrecognized character
            printf("Error at line %d: Unrecognized character '%c'\n", lexer->line, c);
            token.type = TOKEN_EOF;
    }
    
    return token;
}

// Create a new AST node
Node* create_node(NodeType type) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->condition = NULL;
    node->body = NULL;
    node->else_body = NULL;
    node->init = NULL;
    node->update = NULL;
    node->next = NULL;
    node->name = NULL;
    node->value.type = VAL_NULL;
    node->params = NULL;
    node->param_count = 0;
    node->args = NULL;
    node->arg_count = 0;
    node->statements = NULL;
    node->statement_count = 0;
    return node;
}

// Compare token type with expected type, advance if matches
int match(Interpreter* interpreter, TokenType type) {
    if (interpreter->current_token.type == type) {
        interpreter->current_token = get_next_token(&interpreter->lexer);
        return 1;
    }
    return 0;
}

// Consume expected token type or report error
void consume(Interpreter* interpreter, TokenType type, const char* error_message) {
    if (interpreter->current_token.type == type) {
        interpreter->current_token = get_next_token(&interpreter->lexer);
    } else {
        printf("Error at line %d: %s\n", interpreter->current_token.line, error_message);
        exit(1);
    }
}

// Forward declarations for recursive parsing
Node* parse_expression(Interpreter* interpreter);
Node* parse_statement(Interpreter* interpreter);

// Parse a primary expression (literals, variables, etc.)
Node* parse_primary(Interpreter* interpreter) {
    Node* node = NULL;
    
    if (interpreter->current_token.type == TOKEN_NUMBER) {
        node = create_node(NODE_LITERAL);
        node->value.type = VAL_NUMBER;
        node->value.as.number = atof(interpreter->current_token.value);
        free(interpreter->current_token.value);
        consume(interpreter, TOKEN_NUMBER, "Expected number");
    } else if (interpreter->current_token.type == TOKEN_STRING) {
        node = create_node(NODE_LITERAL);
        node->value.type = VAL_STRING;
        node->value.as.string = interpreter->current_token.value;
        consume(interpreter, TOKEN_STRING, "Expected string");
    } else if (interpreter->current_token.type == TOKEN_IDENTIFIER) {
        char* name = interpreter->current_token.value;
        consume(interpreter, TOKEN_IDENTIFIER, "Expected identifier");
        
        if (interpreter->current_token.type == TOKEN_LPAREN) {
            // Function call
            Node* call = create_node(NODE_CALL);
            call->name = name;
            consume(interpreter, TOKEN_LPAREN, "Expected '('");
            
            // Parse arguments
            call->args = NULL;
            call->arg_count = 0;
            
            if (interpreter->current_token.type != TOKEN_RPAREN) {
                do {
                    call->arg_count++;
                    call->args = realloc(call->args, sizeof(Node*) * call->arg_count);
                    call->args[call->arg_count - 1] = parse_expression(interpreter);
                } while (match(interpreter, TOKEN_COMMA));
            }
            
            consume(interpreter, TOKEN_RPAREN, "Expected ')'");
            node = call;
        } else if (interpreter->current_token.type == TOKEN_LBRACKET) {
            // Array indexing
            Node* index = create_node(NODE_INDEX);
            index->name = name;
            consume(interpreter, TOKEN_LBRACKET, "Expected '['");
            index->right = parse_expression(interpreter);
            consume(interpreter, TOKEN_RBRACKET, "Expected ']'");
            node = index;
        } else {
            // Simple variable
            node = create_node(NODE_VARIABLE);
            node->name = name;
        }
    } else if (interpreter->current_token.type == TOKEN_LPAREN) {
        consume(interpreter, TOKEN_LPAREN, "Expected '('");
        node = parse_expression(interpreter);
        consume(interpreter, TOKEN_RPAREN, "Expected ')'");
    } else if (interpreter->current_token.type == TOKEN_LBRACKET) {
        // Array literal
        node = create_node(NODE_ARRAY);
        consume(interpreter, TOKEN_LBRACKET, "Expected '['");
        
        node->args = NULL;
        node->arg_count = 0;
        
        if (interpreter->current_token.type != TOKEN_RBRACKET) {
            do {
                node->arg_count++;
                node->args = realloc(node->args, sizeof(Node*) * node->arg_count);
                node->args[node->arg_count - 1] = parse_expression(interpreter);
            } while (match(interpreter, TOKEN_COMMA));
        }
        
        consume(interpreter, TOKEN_RBRACKET, "Expected ']'");
    } else {
        printf("Error at line %d: Unexpected token\n", interpreter->current_token.line);
        exit(1);
    }
    
    return node;
}

// Parse a multiplicative expression (*, /)
Node* parse_multiplicative(Interpreter* interpreter) {
    Node* left = parse_primary(interpreter);
    
    while (interpreter->current_token.type == TOKEN_MULTIPLY || interpreter->current_token.type == TOKEN_DIVIDE) {
        TokenType op = interpreter->current_token.type;
        match(interpreter, op);
        
        Node* node = create_node(NODE_BINARY);
        node->left = left;
        node->right = parse_primary(interpreter);
        
        if (op == TOKEN_MULTIPLY) {
            node->name = strdup("*");
        } else {
            node->name = strdup("/");
        }
        
        left = node;
    }
    
    return left;
}

// Parse an additive expression (+, -)
Node* parse_additive(Interpreter* interpreter) {
    Node* left = parse_multiplicative(interpreter);
    
    while (interpreter->current_token.type == TOKEN_PLUS || interpreter->current_token.type == TOKEN_MINUS) {
        TokenType op = interpreter->current_token.type;
        match(interpreter, op);
        
        Node* node = create_node(NODE_BINARY);
        node->left = left;
        node->right = parse_multiplicative(interpreter);
        
        if (op == TOKEN_PLUS) {
            node->name = strdup("+");
        } else {
            node->name = strdup("-");
        }
        
        left = node;
    }
    
    return left;
}

// Parse a comparative expression (<, >, <=, >=)
Node* parse_comparative(Interpreter* interpreter) {
    Node* left = parse_additive(interpreter);
    
    while (interpreter->current_token.type == TOKEN_LESS || interpreter->current_token.type == TOKEN_GREATER ||
           interpreter->current_token.type == TOKEN_LESS_EQUAL || interpreter->current_token.type == TOKEN_GREATER_EQUAL) {
        TokenType op = interpreter->current_token.type;
        match(interpreter, op);
        
        Node* node = create_node(NODE_BINARY);
        node->left = left;
        node->right = parse_additive(interpreter);
        
        switch (op) {
            case TOKEN_LESS: node->name = strdup("<"); break;
            case TOKEN_GREATER: node->name = strdup(">"); break;
            case TOKEN_LESS_EQUAL: node->name = strdup("<="); break;
            case TOKEN_GREATER_EQUAL: node->name = strdup(">="); break;
            default: break;
        }
        
        left = node;
    }
    
    return left;
}

// Parse an equality expression (==, !=)
Node* parse_equality(Interpreter* interpreter) {
    Node* left = parse_comparative(interpreter);
    
    while (interpreter->current_token.type == TOKEN_EQUAL || interpreter->current_token.type == TOKEN_NOT_EQUAL) {
        TokenType op = interpreter->current_token.type;
        match(interpreter, op);
        
        Node* node = create_node(NODE_BINARY);
        node->left = left;
        node->right = parse_comparative(interpreter);
        
        if (op == TOKEN_EQUAL) {
            node->name = strdup("==");
        } else {
            node->name = strdup("!=");
        }
        
        left = node;
    }
    
    return left;
}

// Parse an assignment expression
Node* parse_assignment(Interpreter* interpreter) {
    Node* left = parse_equality(interpreter);
    
    if (interpreter->current_token.type == TOKEN_ASSIGN) {
        consume(interpreter, TOKEN_ASSIGN, "Expected '='");
        
        Node* node = create_node(NODE_ASSIGNMENT);
        node->left = left;
        node->right = parse_expression(interpreter);
        
        return node;
    }
    
    return left;
}

// Parse a general expression
Node* parse_expression(Interpreter* interpreter) {
    return parse_assignment(interpreter);
}

// Parse a block of statements
Node* parse_block(Interpreter* interpreter) {
    Node* block = create_node(NODE_BLOCK);
    block->statements = NULL;
    block->statement_count = 0;
    
    consume(interpreter, TOKEN_LBRACE, "Expected '{'");
    
    while (interpreter->current_token.type != TOKEN_RBRACE && interpreter->current_token.type != TOKEN_EOF) {
        Node* statement = parse_statement(interpreter);
        if (statement != NULL) {
            block->statement_count++;
            block->statements = realloc(block->statements, sizeof(Node*) * block->statement_count);
            block->statements[block->statement_count - 1] = statement;
        }
    }
    
    consume(interpreter, TOKEN_RBRACE, "Expected '}'");
    
    return block;
}

// Parse an if statement
Node* parse_if_statement(Interpreter* interpreter) {
    consume(interpreter, TOKEN_IF, "Expected 'if'");
    consume(interpreter, TOKEN_LPAREN, "Expected '('");
    
    Node* node = create_node(NODE_IF);
    node->condition = parse_expression(interpreter);
    
    consume(interpreter, TOKEN_RPAREN, "Expected ')'");
    
    node->body = parse_statement(interpreter);
    
    if (interpreter->current_token.type == TOKEN_ELSE) {
        consume(interpreter, TOKEN_ELSE, "Expected 'else'");
        node->else_body = parse_statement(interpreter);
    }
    
    return node;
}

// Parse a for loop
Node* parse_for_statement(Interpreter* interpreter) {
    consume(interpreter, TOKEN_FOR, "Expected 'for'");
    consume(interpreter, TOKEN_LPAREN, "Expected '('");
    
    Node* node = create_node(NODE_FOR);
    
    // Initialization
    if (interpreter->current_token.type != TOKEN_SEMICOLON) {
        node->init = parse_expression(interpreter);
    }
    consume(interpreter, TOKEN_SEMICOLON, "Expected ';'");
    
    // Condition
    if (interpreter->current_token.type != TOKEN_SEMICOLON) {
        node->condition = parse_expression(interpreter);
    }
    consume(interpreter, TOKEN_SEMICOLON, "Expected ';'");
    
    // Update
    if (interpreter->current_token.type != TOKEN_RPAREN) {
        node->update = parse_expression(interpreter);
    }
    consume(interpreter, TOKEN_RPAREN, "Expected ')'");
    
    // Body
    node->body = parse_statement(interpreter);
    
    return node;
}

// Parse a while loop
Node* parse_while_statement(Interpreter* interpreter) {
    consume(interpreter, TOKEN_WHILE, "Expected 'while'");
    consume(interpreter, TOKEN_LPAREN, "Expected '('");
    
    Node* node = create_node(NODE_WHILE);
    node->condition = parse_expression(interpreter);
    
    consume(interpreter, TOKEN_RPAREN, "Expected ')'");
    
    node->body = parse_statement(interpreter);
    
    return node;
}

// Parse a function declaration
Node* parse_function_declaration(Interpreter* interpreter) {
    consume(interpreter, TOKEN_FUNCTION, "Expected 'function'");
    
    Node* node = create_node(NODE_FUNCTION);
    
    if (interpreter->current_token.type == TOKEN_IDENTIFIER) {
        node->name = strdup(interpreter->current_token.value);
        free(interpreter->current_token.value);
        consume(interpreter, TOKEN_IDENTIFIER, "Expected identifier");
    } else {
        node->name = NULL; // Anonymous function
    }
    
    consume(interpreter, TOKEN_LPAREN, "Expected '('");
    
    // Parse parameters
    node->params = NULL;
    node->param_count = 0;
    
    if (interpreter->current_token.type != TOKEN_RPAREN) {
        do {
            if (interpreter->current_token.type == TOKEN_IDENTIFIER) {
                node->param_count++;
                node->params = realloc(node->params, sizeof(Node*) * node->param_count);
                
                Node* param = create_node(NODE_VARIABLE);
                param->name = strdup(interpreter->current_token.value);
                free(interpreter->current_token.value);
                
                node->params[node->param_count - 1] = param;
                consume(interpreter, TOKEN_IDENTIFIER, "Expected identifier");
            } else {
                printf("Error at line %d: Expected parameter name\n", interpreter->current_token.line);
                exit(1);
            }
        } while (match(interpreter, TOKEN_COMMA));
    }
    
    consume(interpreter, TOKEN_RPAREN, "Expected ')'");
    
    // Parse function body
    node->body = parse_block(interpreter);
    
    return node;
}

// Parse a return statement
Node* parse_return_statement(Interpreter* interpreter) {
    consume(interpreter, TOKEN_RETURN, "Expected 'return'");
    
    Node* node = create_node(NODE_RETURN);
    
    if (interpreter->current_token.type != TOKEN_SEMICOLON) {
        node->right = parse_expression(interpreter);
    }
    
    consume(interpreter, TOKEN_SEMICOLON, "Expected ';'");
    
    return node;
}

// Parse a print statement
Node* parse_print_statement(Interpreter* interpreter) {
    consume(interpreter, TOKEN_PRINT, "Expected 'print'");
    
    Node* node = create_node(NODE_PRINT);
    node->right = parse_expression(interpreter);
    
    consume(interpreter, TOKEN_SEMICOLON, "Expected ';'");
    
    return node;
}

// Parse a single statement
Node* parse_statement(Interpreter* interpreter) {
    if (interpreter->current_token.type == TOKEN_IF) {
        return parse_if_statement(interpreter);
    } else if (interpreter->current_token.type == TOKEN_FOR) {
        return parse_for_statement(interpreter);
    } else if (interpreter->current_token.type == TOKEN_WHILE) {
        return parse_while_statement(interpreter);
    } else if (interpreter->current_token.type == TOKEN_FUNCTION) {
        return parse_function_declaration(interpreter);
    } else if (interpreter->current_token.type == TOKEN_RETURN) {
        return parse_return_statement(interpreter);
    } else if (interpreter->current_token.type == TOKEN_PRINT) {
        return parse_print_statement(interpreter);
    } else if (interpreter->current_token.type == TOKEN_LBRACE) {
        return parse_block(interpreter);
    } else {
        // Expression statement
        Node* node = parse_expression(interpreter);
        consume(interpreter, TOKEN_SEMICOLON, "Expected ';'");
        return node;
    }
}

// Parse a complete program
Node* parse_program(Interpreter* interpreter) {
    Node* program = create_node(NODE_PROGRAM);
    program->statements = NULL;
    program->statement_count = 0;
    
    while (interpreter->current_token.type != TOKEN_EOF) {
        Node* statement = parse_statement(interpreter);
        if (statement != NULL) {
            program->statement_count++;
            program->statements = realloc(program->statements, sizeof(Node*) * program->statement_count);
            program->statements[program->statement_count - 1] = statement;
        }
    }
    
    return program;
}

// Find a variable in the symbol table
Variable* find_variable(SymbolTable* table, const char* name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->variables[i].name, name) == 0) {
            return &table->variables[i];
        }
    }
    return NULL;
}

// Create a new variable in the symbol table
Variable* create_variable(SymbolTable* table, const char* name) {
    // Check if we need to resize the table
    if (table->count >= table->capacity) {
        table->capacity = table->capacity == 0 ? 8 : table->capacity * 2;
        table->variables = realloc(table->variables, sizeof(Variable) * table->capacity);
    }
    
    // Create the new variable
    Variable* var = &table->variables[table->count++];
    var->name = strdup(name);
    var->value.type = VAL_NULL; // Default value is null
    
    return var;
}

// Make a copy of a value
Value copy_value(Value value) {
    Value copy = value;
    
    // Deep copy for strings and arrays
    if (value.type == VAL_STRING && value.as.string != NULL) {
        copy.as.string = strdup(value.as.string);
    } else if (value.type == VAL_ARRAY) {
        copy.as.array.items = malloc(sizeof(Value) * value.as.array.count);
        copy.as.array.capacity = value.as.array.count;
        copy.as.array.count = value.as.array.count;
        
        for (int i = 0; i < value.as.array.count; i++) {
            copy.as.array.items[i] = copy_value(value.as.array.items[i]);
        }
    } else if (value.type == VAL_FUNCTION) {
        if (value.as.function.param_names) {
            copy.as.function.param_names = malloc(sizeof(char*) * value.as.function.param_count);
            for (int i = 0; i < value.as.function.param_count; i++) {
                copy.as.function.param_names[i] = strdup(value.as.function.param_names[i]);
            }
        }
        // Note: We don't copy the node since it's just a reference
    }
    
    return copy;
}

// Free memory used by a value
void free_value(Value value) {
    if (value.type == VAL_STRING) {
        free(value.as.string);
    } else if (value.type == VAL_ARRAY) {
        for (int i = 0; i < value.as.array.count; i++) {
            free_value(value.as.array.items[i]);
        }
        free(value.as.array.items);
    } else if (value.type == VAL_FUNCTION) {
        if (value.as.function.param_names) {
            for (int i = 0; i < value.as.function.param_count; i++) {
                free(value.as.function.param_names[i]);
            }
            free(value.as.function.param_names);
        }
        // Note: We don't free the node since it's just a reference and will be freed elsewhere
    }
}

// Get the value of a variable
Value get_variable(Interpreter* interpreter, const char* name) {
    // Look in local scope first
    Variable* local = find_variable(&interpreter->local_table, name);
    if (local != NULL)
