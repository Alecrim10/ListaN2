#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    double val;
    struct Node* next;
} Node;

typedef struct {
    Node* top;
} Stack;

/* Stack operations */
Stack* createStack() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    if (!s) { perror("malloc"); return NULL; }
    s->top = NULL;
    return s;
}

void push(Stack* stack, double value) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) { perror("malloc"); exit(1); }
    n->val = value;
    n->next = stack->top;
    stack->top = n;
}

double pop(Stack* stack) {
    if (!stack->top) {
        fprintf(stderr, "Erro: pop em pilha vazia.\n");
        exit(1);
    }
    Node* n = stack->top;
    double v = n->val;
    stack->top = n->next;
    free(n);
    return v;
}

int isEmpty(Stack* stack) {
    return stack->top == NULL;
}

void freeStack(Stack* stack) {
    while (stack->top) {
        Node* tmp = stack->top;
        stack->top = tmp->next;
        free(tmp);
    }
    free(stack);
}

/* helper: verifica se token eh numero (considera sinal e ponto decimal) */
int isNumber(const char* token) {
    if (!token || *token == '\0') return 0;
    char* p = (char*)token;
    int hasDigit = 0;
    int hasDot = 0;
    if (*p == '+' || *p == '-') p++;
    while (*p) {
        if (isdigit((unsigned char)*p)) hasDigit = 1;
        else if (*p == '.' && !hasDot) hasDot = 1;
        else return 0;
        p++;
    }
    return hasDigit;
}

/* evaluateRPN: retorna resultado e seta errcode:
   0 = ok
   1 = expressao malformada (operandos insuficientes ou sobrando)
   2 = divisao por zero
   3 = token invalido
*/
double evaluateRPN(char* expression, int* errcode) {
    *errcode = 0;
    Stack* s = createStack();
    if (!s) { *errcode = 3; return 0; }

    /* duplicar string para usar strtok */
    char* expr = strdup(expression);
    if (!expr) { freeStack(s); *errcode = 3; return 0; }

    const char* delim = " \t\n";
    char* token = strtok(expr, delim);

    while (token) {
        if (isNumber(token)) {
            double v = atof(token);
            push(s, v);
        } else if (strlen(token) == 1 && strchr("+-*/", token[0])) {
            if (isEmpty(s)) { *errcode = 1; free(expr); freeStack(s); return 0; }
            double val2 = pop(s);
            if (isEmpty(s)) { *errcode = 1; free(expr); freeStack(s); return 0; }
            double val1 = pop(s);
            double res;
            switch (token[0]) {
                case '+': res = val1 + val2; break;
                case '-': res = val1 - val2; break;
                case '*': res = val1 * val2; break;
                case '/':
                    if (val2 == 0.0) { *errcode = 2; free(expr); freeStack(s); return 0; }
                    res = val1 / val2;
                    break;
                default:
                    *errcode = 3; free(expr); freeStack(s); return 0;
            }
            push(s, res);
        } else {
            /* token inválido */
            *errcode = 3;
            free(expr);
            freeStack(s);
            return 0;
        }
        token = strtok(NULL, delim);
    }

    /* Ao final, deve ter exatamente 1 valor na pilha */
    if (isEmpty(s)) { *errcode = 1; free(expr); freeStack(s); return 0; }
    double resultado = pop(s);
    if (!isEmpty(s)) {
        *errcode = 1;
        free(expr);
        freeStack(s);
        return 0;
    }

    free(expr);
    freeStack(s);
    *errcode = 0;
    return resultado;
}

/* main */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s \"expressao_em_RPN\"\n", argv[0]);
        printf("Ex: %s \"10 5 +\"\n", argv[0]);
        return 1;
    }
    int err;
    double res = evaluateRPN(argv[1], &err);
    if (err == 0) {
        printf("Resultado: %.2f\n", res);
    } else if (err == 1) {
        printf("Erro: Expressao malformada (operandos insuficientes ou sobrando).\n");
    } else if (err == 2) {
        printf("Erro: Tentativa de divisao por zero.\n");
    } else {
        printf("Erro: Token invalido na expressao.\n");
    }
    return (err == 0) ? 0 : 1;
}
