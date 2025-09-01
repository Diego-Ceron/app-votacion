#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct direccion {
    int calle;
    int numero_exterior;
    char *cruzamientos;
    char *colonia;
    char *ciudad;
    char *estado;
    int codigo_postal;
};

struct dependiente {
    char *primer_nombre;
    char *segundo_nombre;
    char *apellido_paterno;
    char *apellido_materno;
    char *curp;
    int edad;
};

struct fecha {
    int dia;
    int mes;
    int anio;
};

struct ciudadano {
    char *primer_nombre;
    char *segundo_nombre;
    char *apellido_paterno;
    char *apellido_materno;
    char *curp;
    char *email;
    char *telefono;
    struct direccion direccion;
    int distrito;
    int num_dependientes;
    struct dependiente *dependientes;
    int edad;
};

int convertir_dos_digitos(char decena, char unidad) {
    return (decena - '0') * 10 + (unidad - '0');
}

void obtener_fecha_nacimiento(const char *curp, struct fecha *nacimiento) {
    int anio = convertir_dos_digitos(curp[4], curp[5]);
    nacimiento->mes = convertir_dos_digitos(curp[6], curp[7]);
    nacimiento->dia = convertir_dos_digitos(curp[8], curp[9]);
    nacimiento->anio = (anio <= 22) ? 2000 + anio : 1900 + anio;
}

int calcular_edad(struct fecha nacimiento, struct fecha votacion) {
    int edad = votacion.anio - nacimiento.anio;
    if (votacion.mes < nacimiento.mes || 
       (votacion.mes == nacimiento.mes && votacion.dia < nacimiento.dia)) {
        edad--;
    }
    return edad;
}

void leer_linea(char *destino, int tamano, const char *mensaje) {
    int ch;

    // Limpiar buffer de entrada
    while ((ch = getchar()) != '\n' && ch != EOF);

    printf("%s", mensaje);

    // Leer la línea
    if (fgets(destino, tamano, stdin) != NULL) {
        destino[strcspn(destino, "\n")] = '\0';
    } else {
        destino[0] = '\0';
    }
}

void datos_ciudadano(struct ciudadano *c, struct fecha votacion) {
    struct fecha nacimiento;

    c->primer_nombre = malloc(100 * sizeof(char));
    printf("Primer nombre: ");
    scanf("%99s", c->primer_nombre);

    c->segundo_nombre = malloc(100 * sizeof(char));
    printf("Segundo nombre (o 'N/A'): ");
    scanf("%99s", c->segundo_nombre);

    c->apellido_paterno = malloc(100 * sizeof(char));
    printf("Apellido paterno: ");
    scanf("%99s", c->apellido_paterno);

    c->apellido_materno = malloc(100 * sizeof(char));
    printf("Apellido materno: ");
    scanf("%99s", c->apellido_materno);

    c->curp = malloc(19 * sizeof(char));
    printf("CURP: ");
    scanf("%18s", c->curp);

    c->email = malloc(100 * sizeof(char));
    printf("Email: ");
    scanf("%99s", c->email);

    c->telefono = malloc(20 * sizeof(char));
    printf("Telefono: ");
    scanf("%19s", c->telefono);

    // Dirección
    printf("Direccion:\n");
    printf("  Calle: ");
    scanf("%d", &c->direccion.calle);
    printf("  Numero exterior: ");
    scanf("%d", &c->direccion.numero_exterior);

    c->direccion.cruzamientos = malloc(100 * sizeof(char));
    leer_linea(c->direccion.cruzamientos, 100, "  Cruzamientos: ");

    c->direccion.colonia = malloc(100 * sizeof(char));
    leer_linea(c->direccion.colonia, 100, "  Colonia: ");

    c->direccion.ciudad = malloc(100 * sizeof(char));
    printf("  Ciudad: ");
    scanf("%99s", c->direccion.ciudad);

    c->direccion.estado = malloc(100 * sizeof(char));
    printf("  Estado: ");
    scanf("%99s", c->direccion.estado);

    printf("  Codigo postal: ");
    scanf("%d", &c->direccion.codigo_postal);

    printf("Distrito: ");
    scanf("%d", &c->distrito);

    obtener_fecha_nacimiento(c->curp, &nacimiento);
    c->edad = calcular_edad(nacimiento, votacion);

}

void escribir_archivo(const struct ciudadano *c, const char *nombre_archivo) {
    FILE *archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return;
    }

    fprintf(archivo, "Nombre completo: %s %s %s %s\n",
            c->primer_nombre,
            strcmp(c->segundo_nombre, "N/A") == 0 ? "" : c->segundo_nombre,
            c->apellido_paterno,
            c->apellido_materno);
    fprintf(archivo, "CURP: %s\n", c->curp);
    fprintf(archivo, "Email: %s\n", c->email);
    fprintf(archivo, "Telefono: %s\n", c->telefono);
    fprintf(archivo, "Edad: %d\n", c->edad);
    fprintf(archivo, "Distrito: %d\n", c->distrito);
    fprintf(archivo, "Direccion:\n");
    fprintf(archivo, "  Calle: %d\n", c->direccion.calle);
    fprintf(archivo, "  Numero exterior: %d\n", c->direccion.numero_exterior);
    fprintf(archivo, "  Cruzamientos: %s\n", c->direccion.cruzamientos);
    fprintf(archivo, "  Colonia: %s\n", c->direccion.colonia);
    fprintf(archivo, "  Ciudad: %s\n", c->direccion.ciudad);
    fprintf(archivo, "  Estado: %s\n", c->direccion.estado);
    fprintf(archivo, "  Codigo postal: %d\n", c->direccion.codigo_postal);

    fclose(archivo);
}

void liberar_memoria(struct ciudadano *c) {
    free(c->primer_nombre);
    free(c->segundo_nombre);
    free(c->apellido_paterno);
    free(c->apellido_materno);
    free(c->curp);
    free(c->email);
    free(c->telefono);
    free(c->direccion.cruzamientos);
    free(c->direccion.colonia);
    free(c->direccion.ciudad);
    free(c->direccion.estado);
}

int main() {
    struct fecha votacion;
    struct ciudadano ciudadano;

    printf("Fecha de votacion (dd mm aaaa): ");
    scanf("%d %d %d", &votacion.dia, &votacion.mes, &votacion.anio);

    datos_ciudadano(&ciudadano, votacion);

    if (ciudadano.edad >= 18) {
        printf("%s %s %s %s puede votar.\n",
               ciudadano.primer_nombre,
               strcmp(ciudadano.segundo_nombre, "N/A") == 0 ? "" : ciudadano.segundo_nombre,
               ciudadano.apellido_paterno,
               ciudadano.apellido_materno);
               escribir_archivo(&ciudadano, "datos_ciudadano.txt");
    } else {
        printf("%s %s %s %s NO puede votar.\n",
               ciudadano.primer_nombre,
               strcmp(ciudadano.segundo_nombre, "N/A") == 0 ? "" : ciudadano.segundo_nombre,
               ciudadano.apellido_paterno,
               ciudadano.apellido_materno);
    }

    liberar_memoria(&ciudadano);
    return 0;
}
