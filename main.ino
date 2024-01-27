extern bool enableSimulation = false;

void startSimulation(String hexString) {
    init_pin();
    set_matrix_initial_state(hexString);
    enableSimulation = true;
}

void resetSimulation() {
    enableSimulation = false;
    reset_matrix_state();
    init_pin();
}

void setup() {
    setupHttpServer();
}

void loop() {
    server.handleClient();
    if (enableSimulation) {
        unsigned display_start = millis();
        while (millis() - display_start < 120)
            display(matrix);
        update_matrix();
    }
}