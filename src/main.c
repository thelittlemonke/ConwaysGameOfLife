#include "raylib.h"
#include "raymath.h"
#include "tilemap.h"
#include "constants.h"

//Engine Data
typedef struct {
	Camera2D* camera;
	//current iteration
	unsigned int generation;
	//how fast will rem_time decrement (-1 is tick asap)
	unsigned int simulation_speed;
	//remaining time until next iteration
	float rem_time;
} EData;

void draw(EData* data);
void update(EData* data);
void tick(EData* data);

int main(void) {
	SetConfigFlags(FLAG_MSAA_4X_HINT); //*better vision*
	SetConfigFlags(FLAG_VSYNC_HINT);

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Conway's game of life");
	SetWindowMonitor(0);
	SetExitKey(KEY_NULL);

	tilemap_initialise();

	//Setup data
	Camera2D camera = { 0 };
	camera.zoom = 1.0f;

	EData data;
	data.camera = &camera;
	data.generation = 0;
	data.simulation_speed = 0;	
	data.rem_time = 1.0f;

	while (!WindowShouldClose()) {
		update(&data);
		draw(&data);
	}

	CloseWindow();
}

void tick(EData* data) {
	//this is kinda fishy, but it is what it is
	bool temp_tilemap[COLUMNS][ROWS];
	for (int i = 0; i < COLUMNS; i++) {
		for (int j = 0; j < ROWS; j++) {
			temp_tilemap[i][j] = get_tile(i, j);
		}
	}

	for (int x = 0; x < COLUMNS; x++) {
		for (int y = 0; y < ROWS; y++) {
			bool tile = temp_tilemap[x][y];

			unsigned short count = get_neighbors(x, y, temp_tilemap);
			if ((count < 2 || count > 3) && tile)
				set_tile(x, y, false);
			else if (count == 3 && !tile)
				set_tile(x, y, true);
		}
	}

	data->generation++;
}

void update(EData* data) {
	float dt = GetFrameTime();
	data->rem_time -= dt * data->simulation_speed;
	if (data->rem_time <= 0.0f || data->simulation_speed == -1) {
		data->rem_time = 1.0f;
		tick(data);
	}

	if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
		Vector2 delta = GetMouseDelta();
		delta = Vector2Scale(delta, -1.0f / data->camera->zoom);
		data->camera->target = Vector2Add(data->camera->target, delta);
	}
	else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
		data->simulation_speed = 0;

		int x, y;
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *data->camera);
		y = mousePos.x / TILE_SIZE;
		x = mousePos.y / TILE_SIZE;
		set_tile(x, y, !get_tile(x, y));
	}

	float wheel = GetMouseWheelMove();
	if (wheel != 0) {
		Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *data->camera);
		data->camera->offset = GetMousePosition();
		data->camera->target = mousePos;

		float scaleFactor = 1.0f + (0.25f * fabsf(wheel));
		if (wheel < 0)
			scaleFactor = 1.0f / scaleFactor;
		data->camera->zoom = Clamp(data->camera->zoom * scaleFactor, MIN_ZOOM, MAX_ZOOM);
	}

	if (IsKeyPressed(KEY_SPACE)) {
		tick(data);
		data->simulation_speed = 0;
	}
	else if (IsKeyPressed(KEY_R)) {
		tilemap_randomize();
		data->simulation_speed = 0;
    data->generation = 0;
	}
  else if(IsKeyPressed(KEY_C)){
    tilemap_initialise();
    data->generation = 0;
    data->simulation_speed = 0;
  }
	else if(IsKeyPressed(KEY_ONE)){
		data->simulation_speed = 1;
	}
	else if (IsKeyPressed(KEY_TWO)) {
		data->simulation_speed = 2;
	}
	else if (IsKeyPressed(KEY_THREE)) {
		data->simulation_speed = 5;
	}
	else if (IsKeyPressed(KEY_FOUR)) {
		data->simulation_speed = -1;
	}
}

void draw(EData* data) {
	BeginDrawing();
	ClearBackground(WHITE);
	BeginMode2D(*data->camera);
	for (int i = 0; i < COLUMNS; i++) {
		for (int j = 0; j < ROWS; j++) {
			DrawRectangleLines(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
			//If there's a tile and alive
			if (get_tile(i, j)) {
				DrawRectangle(j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE, RED);
			}
		}
	}
	Rectangle rect = {
		-1,
		-1,
		TILE_SIZE * ROWS + 1,
		TILE_SIZE * COLUMNS + 1,
	};
	DrawRectangleLinesEx(rect, 2, BLACK);
	EndMode2D();

	int x, y;
	Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), *data->camera);
	y = mousePos.x / TILE_SIZE;
	x = mousePos.y / TILE_SIZE;
	DrawText(TextFormat("X: %d\nY: %d", y, x), 5, 5, 20, BLACK);
	DrawText(TextFormat("Generation: %d", data->generation), 5, 45, 20, BLACK);
	DrawText(TextFormat("Speed: %d", data->simulation_speed), 5, 65, 20, BLACK);
	EndDrawing();
}
