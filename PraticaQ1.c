#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char nome[41];
    char telefone[16];
    char celular[16];
    char email[41];
    int dia, mes;
} Contato;

typedef struct elemento {
    Contato info;
    struct elemento* prox;
} Elemento;

typedef Elemento* Lista;

/* cria_agenda */
Lista cria_agenda() {
    return NULL;
}

/* insere_contato no final */
void insere_contato(Lista *l, Contato c) {
    Elemento* novo = (Elemento*)malloc(sizeof(Elemento));
    if (!novo) {
        perror("malloc");
        return;
    }
    novo->info = c;
    novo->prox = NULL;
    if (*l == NULL) {
        *l = novo;
    } else {
        Elemento* p = *l;
        while (p->prox) p = p->prox;
        p->prox = novo;
    }
}

/* lista_contatos */
void lista_contatos(Lista l) {
    if (!l) {
        printf("Agenda vazia.\n");
        return;
    }
    int i = 1;
    for (Elemento* p = l; p; p = p->prox, i++) {
        Contato c = p->info;
        printf("Contato %d:\n", i);
        printf(" Nome: %s\n Telefone: %s\n Celular: %s\n Email: %s\n Aniversario: %02d/%02d\n",
               c.nome, c.telefone, c.celular, c.email, c.dia, c.mes);
        printf("-------------------------\n");
    }
}

/* busca_contato por nome (retorna ponteiro para elemento ou NULL) */
Elemento* busca_contato(Lista l, const char* nome) {
    for (Elemento* p = l; p; p = p->prox) {
        if (strcmp(p->info.nome, nome) == 0) return p;
    }
    return NULL;
}

/* remove_contato: remove primeiro contato com o nome informado */
int remove_contato(Lista *l, const char* nome) {
    Elemento* prev = NULL;
    Elemento* cur = *l;
    while (cur) {
        if (strcmp(cur->info.nome, nome) == 0) {
            if (prev) prev->prox = cur->prox;
            else *l = cur->prox;
            free(cur);
            return 1; /* removido */
        }
        prev = cur;
        cur = cur->prox;
    }
    return 0; /* nao encontrado */
}

/* atualiza_contato: busca por nome e atualiza campos conforme entrada do usuario */
int atualiza_contato(Lista l, const char* nome) {
    Elemento* e = busca_contato(l, nome);
    if (!e) return 0;
    Contato *c = &e->info;
    printf("Atualizando contato '%s'. Deixe em branco para manter valor atual.\n", c->nome);

    char buffer[128];

    printf("Novo nome (atual: %s): ", c->nome);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0;
        strncpy(c->nome, buffer, 40); c->nome[40]=0;
    }

    printf("Novo telefone (atual: %s): ", c->telefone);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0; strncpy(c->telefone, buffer, 15); c->telefone[15]=0;
    }

    printf("Novo celular (atual: %s): ", c->celular);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0; strncpy(c->celular, buffer, 15); c->celular[15]=0;
    }

    printf("Novo email (atual: %s): ", c->email);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        buffer[strcspn(buffer, "\n")] = 0; strncpy(c->email, buffer, 40); c->email[40]=0;
    }

    printf("Novo dia de aniversario (atual: %02d) ou 0 para manter: ", c->dia);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        int d = atoi(buffer);
        if (d >=1 && d <=31) c->dia = d;
    }
    printf("Novo mes de aniversario (atual: %02d) ou 0 para manter: ", c->mes);
    fgets(buffer, sizeof(buffer), stdin);
    if (buffer[0] != '\n') {
        int m = atoi(buffer);
        if (m >=1 && m <=12) c->mes = m;
    }
    return 1;
}

/* remover_duplicados — remove nós com nomes repetidos (mantém o primeiro) */
void remover_duplicados(Lista *l) {
    for (Elemento* p = *l; p; p = p->prox) {
        Elemento* prev = p;
        Elemento* q = p->prox;
        while (q) {
            if (strcmp(p->info.nome, q->info.nome) == 0) {
                Elemento* tmp = q;
                prev->prox = q->prox;
                q = q->prox;
                free(tmp);
            } else {
                prev = q;
                q = q->prox;
            }
        }
    }
}

/* libera_agenda */
void libera_agenda(Lista *l) {
    Elemento* p = *l;
    while (p) {
        Elemento* tmp = p;
        p = p->prox;
        free(tmp);
    }
    *l = NULL;
}

/* le um contato do usuario */
Contato ler_contato_interativo() {
    Contato c;
    char buffer[128];

    printf("Nome: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    strncpy(c.nome, buffer, 40); c.nome[40]=0;

    printf("Telefone: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    strncpy(c.telefone, buffer, 15); c.telefone[15]=0;

    printf("Celular: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    strncpy(c.celular, buffer, 15); c.celular[15]=0;

    printf("Email: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    strncpy(c.email, buffer, 40); c.email[40]=0;

    do {
        printf("Dia de aniversario (1-31): ");
        fgets(buffer, sizeof(buffer), stdin);
        c.dia = atoi(buffer);
    } while (c.dia < 1 || c.dia > 31);

    do {
        printf("Mes de aniversario (1-12): ");
        fgets(buffer, sizeof(buffer), stdin);
        c.mes = atoi(buffer);
    } while (c.mes < 1 || c.mes > 12);

    return c;
}

/* menu principal */
int main() {
    Lista agenda = cria_agenda();
    int opc;
    char buffer[128];

    while (1) {
        printf("\n--- AGENDA ---\n");
        printf("1. Inserir Contato\n2. Listar Contatos\n3. Buscar Contato\n4. Editar Contato\n5. Remover Contato\n6. Remover Duplicados\n7. Sair\nEscolha: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        opc = atoi(buffer);

        if (opc == 1) {
            Contato c = ler_contato_interativo();
            insere_contato(&agenda, c);
            printf("Contato inserido.\n");
        } else if (opc == 2) {
            lista_contatos(agenda);
        } else if (opc == 3) {
            printf("Nome para buscar: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            Elemento* e = busca_contato(agenda, buffer);
            if (e) {
                Contato c = e->info;
                printf("Encontrado:\n Nome: %s\n Telefone: %s\n Celular: %s\n Email: %s\n Aniversario: %02d/%02d\n",
                       c.nome, c.telefone, c.celular, c.email, c.dia, c.mes);
            } else printf("Contato nao encontrado.\n");
        } else if (opc == 4) {
            printf("Nome para editar: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if (atualiza_contato(agenda, buffer)) printf("Contato atualizado.\n");
            else printf("Contato nao encontrado.\n");
        } else if (opc == 5) {
            printf("Nome para remover: ");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if (remove_contato(&agenda, buffer)) printf("Contato removido.\n");
            else printf("Contato nao encontrado.\n");
        } else if (opc == 6) {
            remover_duplicados(&agenda);
            printf("Duplicados removidos (se existiam).\n");
        } else if (opc == 7) {
            break;
        } else {
            printf("Opcao invalida.\n");
        }
    }

    libera_agenda(&agenda);
    printf("Saindo.\n");
    return 0;
}
