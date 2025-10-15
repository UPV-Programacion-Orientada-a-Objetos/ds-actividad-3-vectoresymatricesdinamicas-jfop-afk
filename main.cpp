#include <iostream>

#define NUM_PRODUCTOS_FIJOS 5
#define NUM_MP_FIJOS 4

float** planificacion_matriz = NULL;
float* costos_unitarios = NULL;
float* inventario_mp_inicial = NULL;
float* costos_adquisicion_mp = NULL;

int num_semanas_actual = 0;
int num_productos_actual = NUM_PRODUCTOS_FIJOS;
int requerimientos_estaticos[NUM_PRODUCTOS_FIJOS][NUM_MP_FIJOS] = {
    {10, 5, 2, 0},
    {8, 6, 0, 1},
    {15, 0, 4, 3},
    {5, 1, 1, 0},
    {2, 2, 2, 2}
};

// ------------------------------------------------------------------------------------------------

void inicializar_datos();
void liberar_memoria();
void ver_plan_produccion();
void agregar_nueva_semana();
void modificar_produccion();
void calcular_cogs_inventario();
void calcular_consumo_mp(int semana_index, float consumo_mp_result[]);
void mostrar_menu();
bool validar_indice(int indice, int max, const char* tipo);

// ------------------------------------------------------------------------------------------------

void inicializar_datos() {
    num_semanas_actual = 4;

    std::cout << "-> Asignando memoria inicial para Matriz de Planificacion (5x" << num_semanas_actual << ")..." << std::endl;

    planificacion_matriz = new float*[num_productos_actual];

    for (int i = 0; i < num_productos_actual; ++i) {
        planificacion_matriz[i] = new float[num_semanas_actual];
    }

    std::cout << "-> Asignando memoria para Costos Unitarios y Inventario MP..." << std::endl;

    costos_unitarios = new float[num_productos_actual];
    inventario_mp_inicial = new float[NUM_MP_FIJOS];
    costos_adquisicion_mp = new float[NUM_MP_FIJOS];

    for (int i = 0; i < num_productos_actual; ++i) {
        for (int j = 0; j < num_semanas_actual; ++j) {
            planificacion_matriz[i][j] = (float)((i + 1) * 100 + (j * 50));
        }
    }

    costos_unitarios[0] = 50.0f;
    costos_unitarios[1] = 65.0f;
    costos_unitarios[2] = 40.0f;
    costos_unitarios[3] = 20.0f;
    costos_unitarios[4] = 35.0f;

    inventario_mp_inicial[0] = 5000.0f;
    inventario_mp_inicial[1] = 4000.0f;
    inventario_mp_inicial[2] = 3000.0f;
    inventario_mp_inicial[3] = 2000.0f;

    costos_adquisicion_mp[0] = 5.0f;
    costos_adquisicion_mp[1] = 3.5f;
    costos_adquisicion_mp[2] = 10.0f;
    costos_adquisicion_mp[3] = 8.0f;

    std::cout << "Inicialización exitosa. Matriz de Planificación: " << num_productos_actual << " Productos x " << num_semanas_actual << " Semanas." << std::endl;
}

void liberar_memoria() {
    std::cout << "\nLiberando memoria de Matriz y Vectores Dinámicos..." << std::endl;

    if (planificacion_matriz != NULL) {
        for (int i = 0; i < num_productos_actual; ++i) {
            if (planificacion_matriz[i] != NULL) {
                delete[] planificacion_matriz[i];
                planificacion_matriz[i] = NULL;
            }
        }
        delete[] planificacion_matriz;
        planificacion_matriz = NULL;
    }

    if (costos_unitarios != NULL) {
        delete[] costos_unitarios;
        costos_unitarios = NULL;
    }

    if (inventario_mp_inicial != NULL) {
        delete[] inventario_mp_inicial;
        inventario_mp_inicial = NULL;
    }

    if (costos_adquisicion_mp != NULL) {
        delete[] costos_adquisicion_mp;
        costos_adquisicion_mp = NULL;
    }

    std::cout << "Memoria liberada con éxito." << std::endl;
}

// ------------------------------------------------------------------------------------------------

void ver_plan_produccion() {
    std::cout << "\n--- Plan de Producción Actual (" << num_productos_actual << "x" << num_semanas_actual << ") ---" << std::endl;
    std::cout << "Prod/Sem |";
    for (int j = 0; j < num_semanas_actual; ++j) {
        std::cout.width(10);
        std::cout << "Semana " << j;
    }
    std::cout << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;

    for (int i = 0; i < num_productos_actual; ++i) {
        std::cout << "Prod " << i << "    |";
        for (int j = 0; j < num_semanas_actual; ++j) {
            std::cout.width(10);
            std::cout << planificacion_matriz[i][j];
        }
        std::cout << std::endl;
    }
}

// ------------------------------------------------------------------------------------------------

bool validar_indice(int indice, int max, const char* tipo) {
    if (indice < 0 || indice >= max) {
        std::cout << "ERROR: El indice de " << tipo << " (" << indice << ") esta fuera de los limites permitidos (0 a " << max - 1 << ")." << std::endl;
        return false;
    }
    return true;
}

// ------------------------------------------------------------------------------------------------

void agregar_nueva_semana() {
    int nueva_num_semanas = num_semanas_actual + 1;
    float** nueva_matriz = NULL;

    std::cout << "\n-> Agregando Semana " << num_semanas_actual << "..." << std::endl;

    nueva_matriz = new float*[num_productos_actual];

    for (int i = 0; i < num_productos_actual; ++i) {
        nueva_matriz[i] = new float[nueva_num_semanas];
    }

    for (int i = 0; i < num_productos_actual; ++i) {
        for (int j = 0; j < num_semanas_actual; ++j) {
            nueva_matriz[i][j] = planificacion_matriz[i][j];
        }
        nueva_matriz[i][num_semanas_actual] = 0.0f;
    }

    for (int i = 0; i < num_productos_actual; ++i) {
        delete[] planificacion_matriz[i];
    }
    delete[] planificacion_matriz;

    planificacion_matriz = nueva_matriz;
    num_semanas_actual = nueva_num_semanas;

    std::cout << "Matriz redimensionada a " << num_productos_actual << "x" << num_semanas_actual << ". ¡Memoria gestionada con éxito!" << std::endl;
}

// ------------------------------------------------------------------------------------------------

void modificar_produccion() {
    int producto_idx, semana_idx;
    float cantidad;

    std::cout << "\n--- Modificar Produccion Planificada ---" << std::endl;

    std::cout << "Ingrese Producto (0 a " << num_productos_actual - 1 << "): ";
    std::cin >> producto_idx;

    if (!validar_indice(producto_idx, num_productos_actual, "Producto")) {
        return;
    }

    std::cout << "Ingrese Semana (0 a " << num_semanas_actual - 1 << "): ";
    std::cin >> semana_idx;

    if (!validar_indice(semana_idx, num_semanas_actual, "Semana")) {
        return;
    }

    std::cout << "Cantidad a producir: ";
    std::cin >> cantidad;

    if (cantidad < 0) {
        std::cout << "ERROR: Cantidad ingresada invalida." << std::endl;
        return;
    }

    planificacion_matriz[producto_idx][semana_idx] = cantidad;
    std::cout << "Produccion actualizada: Producto " << producto_idx << ", Semana " << semana_idx << " = " << cantidad << " unidades." << std::endl;
}

// ------------------------------------------------------------------------------------------------


void calcular_consumo_mp(int semana_index, float consumo_mp_result[]) {


    for(int j = 0; j < NUM_MP_FIJOS; ++j) {
        consumo_mp_result[j] = 0.0f;
    }

    for (int i = 0; i < num_productos_actual; ++i) {
        float produccion_prod_i = planificacion_matriz[i][semana_index];

        for (int j = 0; j < NUM_MP_FIJOS; ++j) {
            consumo_mp_result[j] += produccion_prod_i * requerimientos_estaticos[i][j];
        }
    }
}

// ------------------------------------------------------------------------------------------------

void calcular_cogs_inventario() {
    int semana_idx;
    float costo_produccion_total = 0.0f;
    float produccion_unidades_total = 0.0f;
    float valor_inventario_final = 0.0f;

    std::cout << "\n--- Calculo de Costos (COGS e Inventario Final) ---" << std::endl;
    std::cout << "Ingrese Semana para el calculo de costos (0 a " << num_semanas_actual - 1 << "): ";
    std::cin >> semana_idx;

    if (!validar_indice(semana_idx, num_semanas_actual, "Semana")) {
        return;
    }

    float consumo_mp_semanal[NUM_MP_FIJOS];


    calcular_consumo_mp(semana_idx, consumo_mp_semanal);

    for (int i = 0; i < num_productos_actual; ++i) {
        float unidades = planificacion_matriz[i][semana_idx];
        float costo_unitario = costos_unitarios[i];

        costo_produccion_total += unidades * costo_unitario;
        produccion_unidades_total += unidades;
    }

    std::cout.precision(2);
    std::cout << std::fixed;

    std::cout << "\n--- Reporte de Costos (Semana " << semana_idx << ") ---" << std::endl;
    std::cout << "Produccion Total (Semana " << semana_idx << "): " << produccion_unidades_total << " unidades." << std::endl;
    std::cout << "Costo Total de Produccion (COGS): $" << costo_produccion_total << std::endl;

    std::cout << "Consumo de Materia Prima:" << std::endl;
    for (int j = 0; j < NUM_MP_FIJOS; ++j) {
        float mp_restante = inventario_mp_inicial[j] - consumo_mp_semanal[j];
        float valor_mp = mp_restante * costos_adquisicion_mp[j];

        valor_inventario_final += valor_mp;

        std::cout << "  - MP-0" << j + 1 << ": Consumido (" << consumo_mp_semanal[j] << "), Restante (" << mp_restante << ")";
        std::cout << ", Valor Restante: $" << valor_mp << std::endl;
    }

    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "Valor del Inventario Final: $" << valor_inventario_final << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
}

// ------------------------------------------------------------------------------------------------

void mostrar_menu() {
    std::cout << "\n--- Sistema de Planificacion y Costos (COGS) ---" << std::endl;
    std::cout << "1. Ver Plan de Produccion" << std::endl;
    std::cout << "2. Agregar Nueva Semana" << std::endl;
    std::cout << "3. Modificar Produccion" << std::endl;
    std::cout << "4. Calcular COGS y Final Inventory" << std::endl;
    std::cout << "5. Salir" << std::endl;
    std::cout << "Opcion seleccionada: ";
}

int main() {
    inicializar_datos();

    int opcion = 0;

    while (opcion != 5) {
        mostrar_menu();

        std::cin >> opcion;

        switch (opcion) {
            case 1:
                ver_plan_produccion();
                break;
            case 2:
                agregar_nueva_semana();
                break;
            case 3:
                modificar_produccion();
                break;
            case 4:
                calcular_cogs_inventario();
                break;
            case 5:
                break;
            default:
                std::cout << "\nOpcion no valida. Intente de nuevo." << std::endl;
                break;
        }
    }
    liberar_memoria();

    return 0;
}
