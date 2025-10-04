#include "piece.h"
#include <vector>

int main() {
    const int width = 800;
    const int height = 800;
    const int squareSize = width / 8;

    InitWindow(width, height, "OOP Chess with Movement");

    // Load textures
    Texture2D b_pawn   = LoadTexture("./Images/b_pawn_png_128px.png");
    Texture2D b_rook   = LoadTexture("./Images/b_rook_png_128px.png");
    Texture2D b_knight = LoadTexture("./Images/b_knight_png_128px.png");
    Texture2D b_bishop = LoadTexture("./Images/b_bishop_png_128px.png");
    Texture2D b_queen  = LoadTexture("./Images/b_queen_png_128px.png");
    Texture2D b_king   = LoadTexture("./Images/b_king_png_128px.png");

    // Vector of pieces
    std::vector<Piece*> pieces;

    // Pawns
    for (int col = 0; col < 8; col++) {
        pieces.push_back(new Pawn(b_pawn, 1, col, false));
    }

    // Back rank
    pieces.push_back(new Rook(b_rook, 0, 0, false));
    pieces.push_back(new Knight(b_knight, 0, 1, false));
    pieces.push_back(new Bishop(b_bishop, 0, 2, false));
    pieces.push_back(new Queen(b_queen, 0, 3, false));
    pieces.push_back(new King(b_king, 0, 4, false));
    pieces.push_back(new Bishop(b_bishop, 0, 5, false));
    pieces.push_back(new Knight(b_knight, 0, 6, false));
    pieces.push_back(new Rook(b_rook, 0, 7, false));

    Piece* selectedPiece = nullptr;
    int selectedRow = -1, selectedCol = -1;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw chessboard
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                Color c = ((row + col) % 2 == 0) ? LIGHTGRAY : DARKGREEN;
                DrawRectangle(col * squareSize, row * squareSize, squareSize, squareSize, c);
            }
        }

        // Draw all pieces
        for (auto* p : pieces) {
            p->Draw(squareSize);
        }

        // Mouse handling
        Vector2 mousePos = GetMousePosition();
        int col = mousePos.x / squareSize;
        int row = mousePos.y / squareSize;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (selectedPiece == nullptr) {
                // Select piece at this square
                for (auto* p : pieces) {
                    if (p->GetRow() == row && p->GetCol() == col) {
                        selectedPiece = p;
                        selectedRow = row;
                        selectedCol = col;
                        break;
                    }
                }
            } else {
                // Try to move piece
// Try to move piece
if (selectedPiece->IsMoveValid(row, col)) {
    Piece* target = nullptr;
    for (auto* p : pieces) {
        if (p->GetRow() == row && p->GetCol() == col) {
            target = p;
            break;
        }
    }

    if (target) {
        if (target->IsWhite() == selectedPiece->IsWhite()) {
            // Same color → invalid move
            selectedPiece = nullptr;
            selectedRow = selectedCol = -1;
            continue;
        } else {
          
        }
    }

    // Move to destination
    selectedPiece->SetPosition(row, col);
}

                // Deselect in either case
                selectedPiece = nullptr;
                selectedRow = selectedCol = -1;
            }
        }

        // Draw highlight if selected
        if (selectedPiece != nullptr) {
            DrawRectangle(selectedPiece->GetCol() * squareSize,
                          selectedPiece->GetRow() * squareSize,
                          squareSize, squareSize, Color{255, 215, 0, 60});

            Rectangle rect = {
                (float)(selectedPiece->GetCol() * squareSize),
                (float)(selectedPiece->GetRow() * squareSize),
                (float)squareSize, (float)squareSize};
            DrawRectangleLinesEx(rect, 3, GOLD);
        }

        EndDrawing();
    }

    // Cleanup
    for (auto* p : pieces) delete p;

    UnloadTexture(b_pawn);
    UnloadTexture(b_rook);
    UnloadTexture(b_knight);
    UnloadTexture(b_bishop);
    UnloadTexture(b_queen);
    UnloadTexture(b_king);

    CloseWindow();
    return 0;
}
