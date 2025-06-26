#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "audio_player.h"


#define Nombre 64

int main(int argc, char const *argv[])
{       
    FILE *archivo = fopen("C:\\Users\\cacho\\Documents\\Parcial\\audio_list.raw", "rb");

    if(!archivo)
    {
        perror("\nNo se pudo abrir el archivo");
        return 1;
    }

    uint32_t count_of_sounds;
    fread(&count_of_sounds, sizeof(uint32_t), 1, archivo);

    uint32_t * sample_rates = malloc(sizeof(uint32_t) * count_of_sounds);
    uint32_t * sample_counts = malloc(sizeof(uint32_t) * count_of_sounds);
    char(*sound_names)[Nombre] = malloc(sizeof(char[Nombre]) * count_of_sounds);
    float ** samples = malloc(sizeof(float*) * count_of_sounds);


    if (!sample_rates || !sample_counts || !sound_names || !samples) {
        printf("No se pudo reservar memoria.\n");
        fclose(archivo);
        return 1;
    }

    // aca leemos lso encabezados
     for (uint32_t i = 0; i < count_of_sounds; i++) {
        fread(&sample_rates[i], sizeof(uint32_t), 1, archivo);

        fread(&sample_counts[i], sizeof(uint32_t), 1, archivo);

        fread(sound_names[i], sizeof(char), Nombre, archivo);

        samples[i] = NULL;
    }


    //leemos las muestras

    for (uint32_t i = 0; i < count_of_sounds; i++)
    {
        samples[i] = malloc(sizeof(float) * sample_counts[i]);
        if(!samples[i])
        {
            printf("No se pudo reservar memoria para la pista %d\n", i);
            fclose(archivo);
            return 1;

        }
        fread(samples[i], sizeof(float), sample_counts[i], archivo);
        /* code */
    }

    fclose(archivo);
    //ahora trabajamos con los datos que tenbemos

    //mostramos los tittulos
    
    printf("pistas disponibles :\n");
    for (uint32_t i = 0; i < count_of_sounds; i++)
    {
        printf("[%d] %s", i, sound_names[i] );
        /* code */
    }


    // Selección y acción
    int seleccion = -1;
    printf("\nElegi una pista (0 a %d): ", count_of_sounds - 1);
    scanf("%d", &seleccion);

    if (seleccion < 0 || seleccion >= (int)count_of_sounds)
    {
        printf("Esa pista no existe, capo\n");
        /* code */
    }else
    {
        int accion;
        printf("Que queres hacer \n1) reproducir \n2) guardar en el archivo.raw ");
        scanf("%d", &accion);
        if (accion == 1)
        {
            play_audio(sample_rates[seleccion] , sample_counts[seleccion] ,samples[seleccion]);

            /* code */
        }else if (accion == 2)
        {
            FILE* out = fopen("sound.raw", "wb");
            if (!out) {
                printf("No se pudo crear el archivo sound.raw");
            } else {
                fwrite(samples[seleccion], sizeof(float), sample_counts[seleccion], out);
                fclose(out);
                printf("Pista guardada como sound.raw\n");
            }
            /* code */
        }else
        {
            printf("Opción no válida.\n");


        }
        

    }
    



    // Liberamos la  memoria
    for (uint32_t i = 0; i < count_of_sounds; i++) 
    {
        free(samples[i]);
    }
    free(samples);
    free(sample_rates);
    free(sample_counts);
    free(sound_names);




    return 0;
}
