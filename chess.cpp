#include <raylib.h>
#include <iostream>

int main() {
    const int width = 800;
    const int height = 800;
    const int squareSize = width / 8;

    InitWindow(width, height, "Chess is not for beginners");
    Texture2D b_pawn   = LoadTexture("./Images/b_pawn_png_128px.png");
    Texture2D b_rook   = LoadTexture("./Images/b_rook_png_128px.png");
    Texture2D b_knight = LoadTexture("./Images/b_knight_png_128px.png");
    Texture2D b_bishop = LoadTexture("./Images/b_bishop_png_128px.png");
    Texture2D b_queen  = LoadTexture("./Images/b_queen_png_128px.png");
    Texture2D b_king   = LoadTexture("./Images/b_king_png_128px.png");

    int selectedRow = -1;
    int selectedCol = -1;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                int x = col * squareSize;
                int y = row * squareSize;

                if ((row + col) % 2 == 0) {
                    DrawRectangle(x, y, squareSize, squareSize, LIGHTGRAY);
                } else {
                    DrawRectangle(x, y, squareSize, squareSize, DARKGREEN);
                }
            }
        }
        for (int col = 0; col < 8; col++) {
            int x = col * squareSize + (squareSize - (b_pawn.width * 0.5f)) / 2;
            int y = 1 * squareSize + (squareSize - (b_pawn.height * 0.5f)) / 2;
            DrawTextureEx(b_pawn, {(float)x, (float)y}, 0.0f, 0.5f, WHITE);
        }





    
        Vector2 mousePos = GetMousePosition();
        int col = mousePos.x / squareSize;
        int row = mousePos.y / squareSize;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedRow = row;
            selectedCol = col;
        }

        if (selectedRow != -1 && selectedCol != -1) {
           
            DrawRectangle(selectedCol * squareSize, selectedRow * squareSize,
                        squareSize, squareSize, Color{255, 215, 0, 60}); 

            
            Rectangle rect = {
                (float)(selectedCol * squareSize),
                (float)(selectedRow * squareSize),
                (float)squareSize,
                (float)squareSize
            };
            DrawRectangleLinesEx(rect, 3, GOLD); 
        }



        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
