#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 1024
#define MAX_CAMPO 512

/*
Realizado por: Jeisi Rosales, C.I: 29.66.51
*/

typedef enum { T_BRILLA, T_AVANZA, T_CREAR, T_FLUYE, T_UNKNOWN } PromptType;

typedef struct {
    PromptType tipo;
    char campo1[MAX_CAMPO];
    char campo2[MAX_CAMPO];
    char campo3[MAX_CAMPO];
    char campo4[MAX_CAMPO];
    char campo5[MAX_CAMPO];
    char campo6[MAX_CAMPO];
    int linea_inicio; // número de línea donde empieza el prompt
} Prompt;

typedef struct {
    Prompt *arr;
    size_t n;
    size_t cap;
} PromptList;

/* Lista dinámica */
void initList(PromptList *L) { L->arr = NULL; L->n = 0; L->cap = 0; }
void pushList(PromptList *L, Prompt p) {
    if (L->n == L->cap) {
        size_t nc = (L->cap == 0) ? 8 : L->cap * 2;
        Prompt *tmp = realloc(L->arr, nc * sizeof(Prompt));
        if (!tmp) { perror("realloc"); exit(1); }
        L->arr = tmp; L->cap = nc;
    }
    L->arr[L->n++] = p;
}
void freeList(PromptList *L) { free(L->arr); L->arr = NULL; L->n = L->cap = 0; }

/* ---- utilidades de cadenas ---- */

/* Quitar CR, LF y espacios/tabs al final */
void rtrim(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' || s[len-1] == ' ' || s[len-1] == '\t')) {
        s[len-1] = '\0';
        len--;
    }
}

/* Quitar espacios/tabs al inicio */
void ltrim_inplace(char *s) {
    size_t i = 0, j = 0;
    while (s[i] == ' ' || s[i] == '\t') i++;
    if (i == 0) return;
    while (s[i]) s[j++] = s[i++];
    s[j] = '\0';
}

/* Comprueba si una línea está vacía (solo espacios o cero longitud) */
int es_linea_vacia(const char *s) {
    for (const char *p = s; *p; ++p) {
        if (*p != ' ' && *p != '\t' && *p != '\r' && *p != '\n') return 0;
    }
    return 1;
}

/* Compara prefijo considerando la etiqueta exacta (case-sensitive).
   Permite >=1 espacios después de ':' si requireSpace==1.
   Devuelve 1 si coincide y copia el resto
*/
int extraer_campo(const char *line, const char *prefijo, int requireSpace, char *dst, size_t dstcap) {
    size_t p = strlen(prefijo);
    if (strncmp(line, prefijo, p) != 0) return 0;
    const char *rest = line + p;
    if (requireSpace) {
        if (*rest != ' ' && *rest != '\t') return 0;
        // permitir múltiples espacios/tabs
        while (*rest == ' ' || *rest == '\t') rest++;
    }
    // copiar el resto
    strncpy(dst, rest, dstcap-1);
    dst[dstcap-1] = '\0';
    return 1;
}

/* Lista de comandos válidos (exactos, case-sensitive según EBNF) */
const char *COMANDOS[] = { "Describe", "Analiza", "Compara", "Explica como", "Lista" };
const size_t NCOM = sizeof(COMANDOS)/sizeof(COMANDOS[0]);

/* Comprueba si s comienza por un comando válido (de mayor a menor longitud) */
int es_comando_valido(const char *s, char *cmd_out, size_t cap) {
    // buscar coincidencia preferente por longitud (para "Explica como")
    for (size_t i = 0; i < NCOM; ++i) {
        size_t idx = i;
    }
    const char *order[] = { "Explica como", "Describe", "Analiza", "Compara", "Lista" };
    for (size_t k=0;k<sizeof(order)/sizeof(order[0]);++k) {
        const char *cmd = order[k];
        size_t L = strlen(cmd);
        if (strncmp(s, cmd, L) == 0) {
            // aceptamos si sigue el final o un espacio
            if (s[L] == '\0' || s[L] == ' ' || s[L] == '\t') {
                strncpy(cmd_out, cmd, cap-1);
                cmd_out[cap-1] = '\0';
                return 1;
            }
        }
    }
    return 0;
}

/* Detectar tipo por primera línea del bloque (prefijos exactos) */
PromptType detectar_tipo_por_linea(const char *line) {
    if (strncmp(line, "Breve contexto:", 15) == 0) return T_BRILLA;
    if (strncmp(line, "Antecedentes:", 13) == 0) return T_AVANZA;
    if (strncmp(line, "Contexto:", 9) == 0) return T_CREAR;
    if (strncmp(line, "Foco:", 5) == 0) return T_FLUYE;
    return T_UNKNOWN;
}

/* ---- funciones parse para cada tipo ---- */
/* BRILLA: necesita exactamente 6 líneas de campos */
int parse_brilla(char **lines, int start_idx, int nlines, Prompt *out, int linea_inicio) {
    // verificar existencia
    if (start_idx + 5 >= nlines) {
        fprintf(stderr, "Error: BRILLA incompleto en linea %d (faltan líneas)\n", linea_inicio);
        return 0;
    }
    char tmp[MAX_CAMPO];

    if (!extraer_campo(lines[start_idx+0], "Breve contexto:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Breve contexto:' en linea %d\n", linea_inicio);
        return 0;
    }
    strncpy(out->campo1, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+1], "Rol deseado:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Rol deseado:' en linea %d\n", linea_inicio+1);
        return 0;
    }
    if (strncmp(tmp, "Actua como", 10) == 0 && (tmp[10]=='\0' || tmp[10]==' ' || tmp[10]=='\t')) {
        const char *rest = tmp + 10;
        while (*rest == ' ' || *rest == '\t') rest++;
        strncpy(out->campo2, rest, MAX_CAMPO-1);
    } else {
        strncpy(out->campo2, tmp, MAX_CAMPO-1);
    }

    if (!extraer_campo(lines[start_idx+2], "Intención o meta:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Intención o meta:' en linea %d\n", linea_inicio+2);
        return 0;
    }
    strncpy(out->campo3, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+3], "Lenguaje o tono:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Lenguaje o tono:' en linea %d\n", linea_inicio+3);
        return 0;
    }
    strncpy(out->campo4, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+4], "Lógica y estructura:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Lógica y estructura:' en linea %d\n", linea_inicio+4);
        return 0;
    }
    strncpy(out->campo5, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+5], "Aspecto del resultado:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Aspecto del resultado:' en linea %d\n", linea_inicio+5);
        return 0;
    }
    strncpy(out->campo6, tmp, MAX_CAMPO-1);

    out->tipo = T_BRILLA;
    out->linea_inicio = linea_inicio;
    return 6;
}

/* AVANZA: Intención exige Comando */
int parse_avanza(char **lines, int start_idx, int nlines, Prompt *out, int linea_inicio) {
    if (start_idx + 5 >= nlines) {
        fprintf(stderr, "Error: AVANZA incompleto en linea %d\n", linea_inicio);
        return 0;
    }
    char tmp[MAX_CAMPO], cmd[MAX_CAMPO];

    if (!extraer_campo(lines[start_idx+0], "Antecedentes:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Antecedentes:' en linea %d\n", linea_inicio);
        return 0;
    }
    strncpy(out->campo1, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+1], "Visión o propósito:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Visión o propósito:' en linea %d\n", linea_inicio+1);
        return 0;
    }
    strncpy(out->campo2, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+2], "Intención o meta:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Intención o meta:' en linea %d\n", linea_inicio+2);
        return 0;
    }
    strncpy(out->campo3, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+3], "Lenguaje o tono:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Lenguaje o tono:' en linea %d\n", linea_inicio+3);
        return 0;
    }
    strncpy(out->campo4, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+4], "Lógica y estructura:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Lógica y estructura:' en linea %d\n", linea_inicio+4);
        return 0;
    }
    strncpy(out->campo5, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+5], "Aspecto del resultado:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Aspecto del resultado:' en linea %d\n", linea_inicio+5);
        return 0;
    }
    strncpy(out->campo6, tmp, MAX_CAMPO-1);

    out->tipo = T_AVANZA;
    out->linea_inicio = linea_inicio;
    return 6;
}

/* CREAR: Acción requiere comando */
int parse_crear(char **lines, int start_idx, int nlines, Prompt *out, int linea_inicio) {
    if (start_idx + 4 >= nlines) {
        fprintf(stderr, "Error: CREAR incompleto en linea %d\n", linea_inicio);
        return 0;
    }
    char tmp[MAX_CAMPO], cmd[MAX_CAMPO];

    if (!extraer_campo(lines[start_idx+0], "Contexto:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Contexto:' en linea %d\n", linea_inicio);
        return 0;
    }
    strncpy(out->campo1, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+1], "Rol:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Rol:' en linea %d\n", linea_inicio+1);
        return 0;
    }
    if (strncmp(tmp, "Actua como", 10) == 0 && (tmp[10]=='\0' || tmp[10]==' ' || tmp[10]=='\t')) {
        const char *rest = tmp + 10;
        while (*rest == ' ' || *rest == '\t') rest++;
        strncpy(out->campo2, rest, MAX_CAMPO-1);
    } else {
        strncpy(out->campo2, tmp, MAX_CAMPO-1);
    }

    if (!extraer_campo(lines[start_idx+2], "Enfoque:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Enfoque:' en linea %d\n", linea_inicio+2);
        return 0;
    }
    strncpy(out->campo3, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+3], "Acción:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Acción:' en linea %d\n", linea_inicio+3);
        return 0;
    }
    if (!es_comando_valido(tmp, cmd, sizeof(cmd))) {
        fprintf(stderr, "Error semántico: comando inválido en 'Acción:' linea %d\n", linea_inicio+3);
        return 0;
    }
    strncpy(out->campo4, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+4], "Resultado:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Resultado:' en linea %d\n", linea_inicio+4);
        return 0;
    }
    strncpy(out->campo5, tmp, MAX_CAMPO-1);

    out->tipo = T_CREAR;
    out->linea_inicio = linea_inicio;
    return 5;
}

/* FLUYE: 5 líneas campo */
int parse_fluye(char **lines, int start_idx, int nlines, Prompt *out, int linea_inicio) {
    if (start_idx + 4 >= nlines) {
        fprintf(stderr, "Error: FLUYE incompleto en linea %d\n", linea_inicio);
        return 0;
    }
    char tmp[MAX_CAMPO];

    if (!extraer_campo(lines[start_idx+0], "Foco:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Foco:' en linea %d\n", linea_inicio);
        return 0;
    }
    strncpy(out->campo1, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+1], "Lugar:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Lugar:' en linea %d\n", linea_inicio+1);
        return 0;
    }
    strncpy(out->campo2, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+2], "Usuario:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Usuario:' en linea %d\n", linea_inicio+2);
        return 0;
    }
    strncpy(out->campo3, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+3], "Yo Interior:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Yo Interior:' en linea %d\n", linea_inicio+3);
        return 0;
    }
    strncpy(out->campo4, tmp, MAX_CAMPO-1);

    if (!extraer_campo(lines[start_idx+4], "Ejercicio:", 1, tmp, sizeof(tmp))) {
        fprintf(stderr, "Error lexico: se esperaba 'Ejercicio:' en linea %d\n", linea_inicio+4);
        return 0;
    }
    strncpy(out->campo5, tmp, MAX_CAMPO-1);

    out->tipo = T_FLUYE;
    out->linea_inicio = linea_inicio;
    return 5;
}

/* ---- procesamiento de bloques ---- */

/* procesar bloque: detecta tipo y llama a la función parse correspondiente.
   Retorna número de líneas consumidas (sin contar las líneas vacías de separación),
   o 0 en caso de error. */
int procesar_bloque(char **lines, int start_idx, int nlines, PromptList *list, int linea_inicio) {
    PromptType tipo = detectar_tipo_por_linea(lines[start_idx]);
    Prompt p;
    memset(&p, 0, sizeof(p));
    int consumed = 0;
    switch (tipo) {
        case T_BRILLA:
            consumed = parse_brilla(lines, start_idx, nlines, &p, linea_inicio);
            break;
        case T_AVANZA:
            consumed = parse_avanza(lines, start_idx, nlines, &p, linea_inicio);
            break;
        case T_CREAR:
            consumed = parse_crear(lines, start_idx, nlines, &p, linea_inicio);
            break;
        case T_FLUYE:
            consumed = parse_fluye(lines, start_idx, nlines, &p, linea_inicio);
            break;
        default:
            fprintf(stderr, "Error: primer prefijo no reconocido en linea %d: '%s'\n", linea_inicio, lines[start_idx]);
            return 0;
    }
    if (consumed > 0) {
        pushList(list, p);
        return consumed;
    }
    return 0;
}

/* ---- main: lectura y control ---- */
int main(int argc, char **argv) {
    FILE *f = fopen("prompts.pmt", "rb");
    if (!f) { perror("fopen"); return 1; }

    /* Leer todas las líneas en memoria */
    char **lines = NULL;
    size_t cap = 0, nlines = 0;
    char buffer[MAX_LINE];
    int lineno = 0;
    while (fgets(buffer, sizeof(buffer), f)) {
        lineno++;
        rtrim(buffer); // quita CR/LF y espacios finales
        // reservar
        if (nlines == cap) {
            size_t nc = (cap == 0) ? 512 : cap * 2;
            char **tmp = realloc(lines, nc * sizeof(char*));
            if (!tmp) { perror("realloc"); fclose(f); return 1; }
            lines = tmp; cap = nc;
        }
        lines[nlines] = malloc(MAX_LINE);
        if (!lines[nlines]) { perror("malloc"); fclose(f); return 1; }
        strcpy(lines[nlines], buffer);
        nlines++;
    }
    fclose(f);

    /* Procesar bloques */
    PromptList lista;
    initList(&lista);

    size_t i = 0;
    while (i < nlines) {
        // saltar líneas vacías
        if (es_linea_vacia(lines[i])) { i++; continue; }
        int inicio_linea = (int)i + 1;
        // procesar bloque en i
        int consumed = procesar_bloque(lines, (int)i, (int)nlines, &lista, inicio_linea);
        if (consumed == 0) {
            // intentar avanzar hasta encontrar una línea vacía (siguiente bloque)
            size_t j = i+1;
            while (j < nlines && !es_linea_vacia(lines[j])) j++;
            i = j;
            continue;
        }
        // avanzar i consumiendo las líneas del bloque
        i += consumed;
        // ahora saltar 1+ líneas vacías para llegar al siguiente bloque
        while (i < nlines && es_linea_vacia(lines[i])) i++;
    }

    /* Imprimir resultados */
    printf("\nParseo completado. Prompts encontrados: %zu\n\n", lista.n);
    for (size_t k=0;k<lista.n;k++) {
        Prompt *p = &lista.arr[k];
        printf("Prompt %zu (linea %d) - Tipo: ", k+1, p->linea_inicio);
        switch (p->tipo) {
            case T_BRILLA: printf("BRILLA\n"); break;
            case T_AVANZA: printf("AVANZA\n"); break;
            case T_CREAR:  printf("CREAR\n"); break;
            case T_FLUYE:  printf("FLUYE\n"); break;
            default: printf("DESCONOCIDO\n"); break;
        }
    }

    /* liberar memoria */
    for (size_t j=0;j<nlines;j++) free(lines[j]);
    free(lines);
    freeList(&lista);
    return 0;
}
