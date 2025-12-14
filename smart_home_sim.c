#/*******************************************************
 * smart_home_sim.c
 * Simple Smart Home simulation (console-based)
 *
 * Author: Joseph (please update author/contact)
 * License: MIT
 *
 * Build: gcc -std=c11 -Wall -Wextra -O2 -o smart_home_sim smart_home_sim.c
 ******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#define SLEEP_MS(ms) Sleep(ms)
#else
#include <unistd.h>
#define SLEEP_MS(ms) usleep((ms) * 1000)
#endif

typedef struct {
    int room_light;
    int kitchen_light;
    int hall_light;
    int fan_speed; // 0 = OFF, 1 = LOW, 2 = MEDIUM, 3 = HIGH
    float last_temp;
} HomeState;

HomeState state = {0, 0, 0, 0, 0.0f};

/* Forward declarations for functions used by tests */
void saveToEEPROM(void);
void loadFromEEPROM(void);

// ------------------------- Menu UI -------------------------
void showMainMenu() {
    printf("\n===== Smart Home Control Panel =====\n");
    printf("1. Toggle Lights\n");
    printf("2. Fan Speed Control\n");
    printf("3. Door Lock (Password Protected)\n");
    printf("4. Simulate Temperature Sensor\n");
    printf("5. Save Settings (EEPROM)\n");
    printf("6. Load Settings (EEPROM)\n");
    printf("0. Exit\n");
    printf("Enter your choice: ");
}

// ------------------------- 1. Lights -------------------------
void toggleLights() {
    int ch;
    printf("\n--- Toggle Lights ---\n");
    printf("1. Room Light (%s)\n", state.room_light ? "ON" : "OFF");
    printf("2. Kitchen Light (%s)\n", state.kitchen_light ? "ON" : "OFF");
    printf("3. Hall Light (%s)\n", state.hall_light ? "ON" : "OFF");
    printf("Enter Light Number to Toggle: ");
    scanf("%d", &ch);
    switch (ch) {
        case 1: state.room_light = !state.room_light; break;
        case 2: state.kitchen_light = !state.kitchen_light; break;
        case 3: state.hall_light = !state.hall_light; break;
        default: printf("Invalid choice.\n");
    }
    printf("Light state updated.\n");
}

// ------------------------- 2. Fan Control -------------------------
void fanControl() {
    printf("\n--- Fan Speed Control ---\n");
    printf("0. OFF\n1. LOW\n2. MEDIUM\n3. HIGH\n");
    printf("Enter Fan Speed: ");
    scanf("%d", &state.fan_speed);
    if (state.fan_speed < 0 || state.fan_speed > 3) {
        printf("Invalid fan speed. Reset to OFF.\n");
        state.fan_speed = 0;
    } else {
        printf("Fan speed set successfully.\n");
    }
}

// ------------------------- 3. Door Lock -------------------------
void doorLock() {
    char password[20];
    int attempts = 3;
    const char correct[] = "1234";

    while (attempts--) {
        printf("Enter password: ");
        scanf("%s", password);
        if (strcmp(password, correct) == 0) {
            printf("Access Granted. Door Unlocked!\n");
            return;
        } else {
            printf("Wrong Password. Attempts left: %d\n", attempts);
        }
    }
    printf("Access Denied. Door remains locked.\n");
}

// ------------------------- 4. Simulate Temperature -------------------------
void simulateTemperature() {
    int adc_val;
    printf("Enter simulated ADC value (0-1023): ");
    scanf("%d", &adc_val);
    if (adc_val < 0 || adc_val > 1023) {
        printf("Invalid ADC value.\n");
        return;
    }
    state.last_temp = (adc_val / 1023.0f) * 5.0f * 100.0f;
    printf("Temperature: %.2f °C\n", state.last_temp);
}

/* Helper: convert ADC reading to temperature (same formula used above) */
float adc_to_temp(int adc_val) {
    if (adc_val < 0) adc_val = 0;
    if (adc_val > 1023) adc_val = 1023;
    return (adc_val / 1023.0f) * 5.0f * 100.0f;
}

/* Self-test: non-interactive checks to verify basic functionality. */
int run_self_test(void) {
    HomeState saved = {1, 0, 1, 2, 0.0f};
    float expected_temp = adc_to_temp(512);

    // set known state and save
    state = saved;
    saveToEEPROM();

    // clear, then load
    state.room_light = state.kitchen_light = state.hall_light = state.fan_speed = 0;
    state.last_temp = 0.0f;
    loadFromEEPROM();

    // validate
    if (state.room_light != saved.room_light) return 2;
    if (state.kitchen_light != saved.kitchen_light) return 3;
    if (state.hall_light != saved.hall_light) return 4;
    if (state.fan_speed != saved.fan_speed) return 5;

    // test ADC conversion
    float got = adc_to_temp(512);
    if (fabsf(got - expected_temp) > 0.001f) return 6;

    printf("Self-test passed.\n");
    return 0;
}

// ------------------------- 5. Save to EEPROM (file) -------------------------
void saveToEEPROM() {
    FILE *fp = fopen("eeprom.bin", "wb");
    if (!fp) {
        printf("Failed to open EEPROM file.\n");
        return;
    }
    fwrite(&state, sizeof(HomeState), 1, fp);
    fclose(fp);
    printf("Settings saved to EEPROM.\n");
}

// ------------------------- 6. Load from EEPROM (file) -------------------------
void loadFromEEPROM() {
    FILE *fp = fopen("eeprom.bin", "rb");
    if (!fp) {
        printf("EEPROM file not found.\n");
        return;
    }
    fread(&state, sizeof(HomeState), 1, fp);
    fclose(fp);
    printf("Settings loaded from EEPROM.\n");
}

// ------------------------- Main -------------------------
int main(int argc, char **argv) {
    int choice;

    // Non-interactive test mode
    if (argc > 1 && (strcmp(argv[1], "--test") == 0 || strcmp(argv[1], "-t") == 0)) {
        int rc = run_self_test();
        return rc;
    }

    while (1) {
        showMainMenu();
        if (scanf("%d", &choice) != 1) {
            // clear input and continue
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
            case 1: toggleLights(); break;
            case 2: fanControl(); break;
            case 3: doorLock(); break;
            case 4: simulateTemperature(); break;
            case 5: saveToEEPROM(); break;
            case 6: loadFromEEPROM(); break;
            case 0: printf("Exiting Smart Home System...\n"); return 0;
            default: printf("Invalid choice.\n");
        }
        SLEEP_MS(1000); // Small delay for effect (portable)
    }
}
