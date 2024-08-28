#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


const char* planets[] = {"Mars", "Jupiter", "Saturn"};


typedef struct {
    float temperature;
    float radiation;
    float magnetic_field;
} SpaceProbeData;

typedef struct {
    float fuel;
    float energy;
    float health;
} SpaceProbeStatus;


SpaceProbeData get_planet_data(const char* planet) {
    SpaceProbeData data;
    if (strcmp(planet, "Mars") == 0) {
        data.temperature = -60.0;
        data.radiation = 240.0;
        data.magnetic_field = 0.5;
    } else if (strcmp(planet, "Jupiter") == 0) {
        data.temperature = -108.0;
        data.radiation = 50.0;
        data.magnetic_field = 428.0;
    } else if (strcmp(planet, "Saturn") == 0) {
        data.temperature = -139.0;
        data.radiation = 20.0;
        data.magnetic_field = 21.0;
    }
    return data;
}


void update_probe_status(SpaceProbeStatus *status, float fuel_used, float energy_used) {
    status->fuel -= fuel_used;
    status->energy -= energy_used;
    status->health -= (fuel_used + energy_used) * 0.01; // Health decreases based on fuel and energy usage
}


void travel_to_planets(FILE *file, SpaceProbeStatus *status, const char* planet) {
    fprintf(file, "Travelling to %s...\n", planet);
    sleep(2);
    update_probe_status(status, 5.0, 10.0);
    SpaceProbeData data = get_planet_data(planet);
    fprintf(file, "Arrived at %s\n", planet);
    fprintf(file, "Temperature: %.2f, Radiation: %.2f, Magnetic Field: %.2f\n",
            data.temperature, data.radiation, data.magnetic_field);
    fprintf(file, "Fuel: %.2f, Energy: %.2f, Health: %.2f\n", status->fuel, status->energy, 
    status->health);
    fprintf(file, "-------------------------------------------\n");
    sleep(1);
}


const char* select_planet() {
    int choice;
    printf("Select a planet to visit:\n");
    printf("1: Mars\n");
    printf("2: Jupiter\n");
    printf("3: Saturn\n");
    printf("Choice: ");
    scanf("%d", &choice);
    if (choice >= 1 && choice <= 3) {
        return planets[choice - 1];
    } else {
        printf("Invalid selection. Defaulting to Mars.\n");
        return planets[0];
    }
}


void perform_mission(FILE *file, const char* planet, int task_choice) {
    fprintf(file, "Performing task on %s...\n", planet);
    switch(task_choice) {
        case 1:
            fprintf(file, "Task: Sample Collection\n");
            fprintf(file, "Collected soil samples from %s.\n", planet);
            break;
        case 2:
            fprintf(file, "Task: Photograph Surface\n");
            fprintf(file, "Photographed the surface of %s.\n", planet);
            break;
        case 3:
            fprintf(file, "Task: Analyze Atmosphere\n");
            fprintf(file, "Analyzed the atmosphere of %s.\n", planet);
            break;
        case 4:
            fprintf(file, "Task: Repair Space Probe\n");
            fprintf(file, "Repairing space probe on %s.\n", planet);
            break;
        default:
            fprintf(file, "Invalid task selection. No task performed.\n");
    }
    fprintf(file, "-------------------------------------------\n");
}


void analyze_data(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file for analysis!\n");
        return;
    }

    char line[256];
    float total_temp = 0, total_rad = 0, total_mag = 0;
    float max_temp = -1000, max_rad = -1000, max_mag = -1000;
    float min_temp = 1000, min_rad = 1000, min_mag = 1000;
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "Temperature:", 12) == 0) {
            float temp, rad, mag;
            sscanf(line, "Temperature: %f, Radiation: %f, Magnetic Field: %f", &temp, &rad, 
            &mag);

            total_temp += temp;
            total_rad += rad;
            total_mag += mag;

            if (temp > max_temp) max_temp = temp;
            if (rad > max_rad) max_rad = rad;
            if (mag > max_mag) max_mag = mag;

            if (temp < min_temp) min_temp = temp;
            if (rad < min_rad) min_rad = rad;
            if (mag < min_mag) min_mag = mag;

            count++;
        }
    }

    fclose(file);

    if (count == 0) {
        printf("No data available for analysis.\n");
        return;
    }

    printf("Data Analysis Results:\n");
    printf("Average Temperature: %.2f\n", total_temp / count);
    printf("Average Radiation: %.2f\n", total_rad / count);
    printf("Average Magnetic Field: %.2f\n", total_mag / count);
    printf("Max Temperature: %.2f\n", max_temp);
    printf("Max Radiation: %.2f\n", max_rad);
    printf("Max Magnetic Field: %.2f\n", max_mag);
    printf("Min Temperature: %.2f\n", min_temp);
    printf("Min Radiation: %.2f\n", min_rad);
    printf("Min Magnetic Field: %.2f\n", min_mag);
}



int main(int argc, char *argv[]) {
	
	    FILE *file;
    file = fopen("space_probe_data.txt", "w");
    if (file == NULL) {
        printf("Failed to open file!\n");
        return 1;
    }

    srand(time(NULL));

    SpaceProbeStatus status = {100.0, 100.0, 100.0}; 

    int task_choice;
    printf("Select a task to perform on all planets:\n");
    printf("1: Sample Collection\n");
    printf("2: Photograph Surface\n");
    printf("3: Analyze Atmosphere\n");
    printf("4: Repair Space Probe\n");
    printf("Choice: ");
    scanf("%d", &task_choice);

    for (int i = 0; i < 3; i++) {
        const char* planet = select_planet();
        travel_to_planets(file, &status, planet);
        if (status.health <= 0) {
            printf("Space probe malfunctioned! Mission terminated.\n");
            break;
        }
        perform_mission(file, planet, task_choice);
    }

    fclose(file);
    analyze_data("space_probe_data.txt");
    printf("Travel completed and data saved to space_probe_data.txt.\n");
	
	
	return 0;
}
