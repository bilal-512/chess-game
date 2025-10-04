#include "piece.h"
#include <vector>

int main() {
    const int width = 800;
    const int height = 800;
    const int squareSize = width / 8;

    InitWindow(width, height, "Two-Player Chess");

    // Load black piece textures
    Texture2D b_pawn   = LoadTexture("./Images/b_pawn_png_128px.png");
    Texture2D b_rook   = LoadTexture("./Images/b_rook_png_128px.png");
    Texture2D b_knight = LoadTexture("./Images/b_knight_png_128px.png");
    Texture2D b_bishop = LoadTexture("./Images/b_bishop_png_128px.png");
    Texture2D b_queen  = LoadTexture("./Images/b_queen_png_128px.png");
    Texture2D b_king   = LoadTexture("./Images/b_king_png_128px.png");

    // Load white piece textures
    Texture2D w_pawn   = LoadTexture("./Images/w_pawn_png_128px.png");
    Texture2D w_rook   = LoadTexture("./Images/w_rook_png_128px.png");
    Texture2D w_knight = LoadTexture("./Images/w_knight_png_128px.png");
    Texture2D w_bishop = LoadTexture("./Images/w_bishop_png_128px.png");
    Texture2D w_queen  = LoadTexture("./Images/w_queen_png_128px.png");
    Texture2D w_king   = LoadTexture("./Images/w_king_png_128px.png");

    // Vector of pieces
    std::vector<Piece*> pieces;

    // Black pieces (top of board)
    // Black pawns
    for (int col = 0; col < 8; col++) {
        pieces.push_back(new Pawn(b_pawn, 1, col, false));
    }
    // Black back rank
    pieces.push_back(new Rook(b_rook, 0, 0, false));
    pieces.push_back(new Knight(b_knight, 0, 1, false));
    pieces.push_back(new Bishop(b_bishop, 0, 2, false));
    pieces.push_back(new Queen(b_queen, 0, 3, false));
    pieces.push_back(new King(b_king, 0, 4, false));
    pieces.push_back(new Bishop(b_bishop, 0, 5, false));
    pieces.push_back(new Knight(b_knight, 0, 6, false));
    pieces.push_back(new Rook(b_rook, 0, 7, false));

    // White pieces (bottom of board)
    // White pawns
    for (int col = 0; col < 8; col++) {
        pieces.push_back(new Pawn(w_pawn, 6, col, true));
    }
    // White back rank
    pieces.push_back(new Rook(w_rook, 7, 0, true));
    pieces.push_back(new Knight(w_knight, 7, 1, true));
    pieces.push_back(new Bishop(w_bishop, 7, 2, true));
    pieces.push_back(new Queen(w_queen, 7, 3, true));
    pieces.push_back(new King(w_king, 7, 4, true));
    pieces.push_back(new Bishop(w_bishop, 7, 5, true));
    pieces.push_back(new Knight(w_knight, 7, 6, true));
    pieces.push_back(new Rook(w_rook, 7, 7, true));

    Piece* selectedPiece = nullptr;
    int selectedRow = -1, selectedCol = -1;
    bool whiteTurn = true; // White starts

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
                        // Only allow selecting pieces of the current player's color
                        if (p->IsWhite() == whiteTurn) {
                            selectedPiece = p;
                            selectedRow = row;
                            selectedCol = col;
                        }
                        break;
                    }
                }
            } else {
                // Try to move piece
                if (selectedPiece->IsMoveValid(row, col)) {
                    Piece* target = nullptr;
                    int targetIndex = -1;
                    
                    // Find if there's a piece at the destination
                    for (int i = 0; i < pieces.size(); i++) {
                        if (pieces[i]->GetRow() == row && pieces[i]->GetCol() == col) {
                            target = pieces[i];
                            targetIndex = i;
                            break;
                        }
                    }

                    if (target) {
                        if (target->IsWhite() == selectedPiece->IsWhite()) {
                            // Same color → invalid move, deselect
                            selectedPiece = nullptr;
                            selectedRow = selectedCol = -1;
                        } else {
                            // Capture opponent's piece
                            delete pieces[targetIndex];
                            pieces.erase(pieces.begin() + targetIndex);
                            
                            // Move to destination
                            selectedPiece->SetPosition(row, col);
                            
                            // Switch turns
                            whiteTurn = !whiteTurn;
                            
                            // Deselect
                            selectedPiece = nullptr;
                            selectedRow = selectedCol = -1;
                        }
                    } else {
                        // Empty square, move piece
                        selectedPiece->SetPosition(row, col);
                        
                        // Switch turns
                        whiteTurn = !whiteTurn;
                        
                        // Deselect
                        selectedPiece = nullptr;
                        selectedRow = selectedCol = -1;
                    }
                } else {
                    // Invalid move, deselect
                    selectedPiece = nullptr;
                    selectedRow = selectedCol = -1;
                }
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

        // Display whose turn it is
        const char* turnText = whiteTurn ? "White's Turn" : "Black's Turn";
        DrawText(turnText, 10, 10, 20, whiteTurn ? WHITE : BLACK);

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
    
    UnloadTexture(w_pawn);
    UnloadTexture(w_rook);
    UnloadTexture(w_knight);
    UnloadTexture(w_bishop);
    UnloadTexture(w_queen);
    UnloadTexture(w_king);

    CloseWindow();
    return 0;
}