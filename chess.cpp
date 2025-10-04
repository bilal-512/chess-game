#include "piece.h"
#include <vector>

// Helper function to find piece at position
Piece* FindPieceAt(int row, int col, const std::vector<Piece*>& pieces) {
    for (auto* p : pieces) {
        if (p->GetRow() == row && p->GetCol() == col) {
            return p;
        }
    }
    return nullptr;
}

// Check if a square is under attack by opponent
bool IsSquareUnderAttack(int row, int col, bool byWhite, const std::vector<Piece*>& pieces) {
    for (auto* p : pieces) {
        if (p->IsWhite() == byWhite) {
            if (p->IsMoveValid(row, col, pieces)) {
                return true;
            }
        }
    }
    return false;
}

// Pawn promotion dialog
void ShowPromotionDialog(Piece*& pawn, std::vector<Piece*>& pieces, 
                        Texture2D queen, Texture2D rook, Texture2D bishop, Texture2D knight,
                        int squareSize) {
    bool choosing = true;
    int choice = 0; // 0=Queen, 1=Rook, 2=Bishop, 3=Knight
    
    while (choosing && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Draw semi-transparent overlay
        DrawRectangle(0, 0, 800, 800, Color{0, 0, 0, 150});
        
        // Draw promotion options
        const char* title = "Choose promotion piece:";
        DrawText(title, 250, 250, 24, WHITE);
        
        const char* options[] = {"1 - Queen", "2 - Rook", "3 - Bishop", "4 - Knight"};
        for (int i = 0; i < 4; i++) {
            Color color = (i == choice) ? YELLOW : WHITE;
            DrawText(options[i], 300, 300 + i * 40, 20, color);
        }
        
        EndDrawing();
        
        // Handle input
        if (IsKeyPressed(KEY_ONE)) choice = 0;
        if (IsKeyPressed(KEY_TWO)) choice = 1;
        if (IsKeyPressed(KEY_THREE)) choice = 2;
        if (IsKeyPressed(KEY_FOUR)) choice = 3;
        if (IsKeyPressed(KEY_UP)) choice = (choice - 1 + 4) % 4;
        if (IsKeyPressed(KEY_DOWN)) choice = (choice + 1) % 4;
        
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            choosing = false;
        }
    }
    
    // Replace pawn with chosen piece
    int row = pawn->GetRow();
    int col = pawn->GetCol();
    bool isWhite = pawn->IsWhite();
    
    // Find and remove pawn
    for (int i = 0; i < pieces.size(); i++) {
        if (pieces[i] == pawn) {
            delete pieces[i];
            pieces.erase(pieces.begin() + i);
            break;
        }
    }
    
    // Add new piece
    Piece* newPiece = nullptr;
    switch(choice) {
        case 0: newPiece = new Queen(queen, row, col, isWhite); break;
        case 1: newPiece = new Rook(rook, row, col, isWhite); break;
        case 2: newPiece = new Bishop(bishop, row, col, isWhite); break;
        case 3: newPiece = new Knight(knight, row, col, isWhite); break;
    }
    pieces.push_back(newPiece);
}

int main() {
    const int width = 800;
    const int height = 800;
    const int squareSize = width / 8;

    InitWindow(width, height, "Two-Player Chess");
    SetTargetFPS(60);

    // Load black piece textures
    Texture2D b_pawn = LoadTexture("./Images/b_pawn_png_128px.png");
    Texture2D b_rook = LoadTexture("./Images/b_rook_png_128px.png");
    Texture2D b_knight = LoadTexture("./Images/b_knight_png_128px.png");
    Texture2D b_bishop = LoadTexture("./Images/b_bishop_png_128px.png");
    Texture2D b_queen = LoadTexture("./Images/b_queen_png_128px.png");
    Texture2D b_king = LoadTexture("./Images/b_king_png_128px.png");

    // Load white piece textures
    Texture2D w_pawn = LoadTexture("./Images/w_pawn_png_128px.png");
    Texture2D w_rook = LoadTexture("./Images/w_rook_png_128px.png");
    Texture2D w_knight = LoadTexture("./Images/w_knight_png_128px.png");
    Texture2D w_bishop = LoadTexture("./Images/w_bishop_png_128px.png");
    Texture2D w_queen = LoadTexture("./Images/w_queen_png_128px.png");
    Texture2D w_king = LoadTexture("./Images/w_king_png_128px.png");

    std::vector<Piece*> pieces;

    // Black pieces (top of board)
    for (int col = 0; col < 8; col++) {
        pieces.push_back(new Pawn(b_pawn, 1, col, false));
    }
    pieces.push_back(new Rook(b_rook, 0, 0, false));
    pieces.push_back(new Knight(b_knight, 0, 1, false));
    pieces.push_back(new Bishop(b_bishop, 0, 2, false));
    pieces.push_back(new Queen(b_queen, 0, 3, false));
    pieces.push_back(new King(b_king, 0, 4, false));
    pieces.push_back(new Bishop(b_bishop, 0, 5, false));
    pieces.push_back(new Knight(b_knight, 0, 6, false));
    pieces.push_back(new Rook(b_rook, 0, 7, false));

    // White pieces (bottom of board)
    for (int col = 0; col < 8; col++) {
        pieces.push_back(new Pawn(w_pawn, 6, col, true));
    }
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
    bool whiteTurn = true;
    int moveCounter = 0;
    Pawn* lastDoubleStepPawn = nullptr;

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
                // Select piece
                for (auto* p : pieces) {
                    if (p->GetRow() == row && p->GetCol() == col) {
                        if (p->IsWhite() == whiteTurn) {
                            selectedPiece = p;
                            selectedRow = row;
                            selectedCol = col;
                        }
                        break;
                    }
                }
            } else {
                bool moveSuccessful = false;
                
                // Check for castling (King moves 2 squares horizontally)
                if (selectedPiece->GetName() == "King" && !selectedPiece->HasMoved() &&
                    row == selectedRow && abs(col - selectedCol) == 2) {
                    
                    int rookCol = (col > selectedCol) ? 7 : 0;
                    Piece* rook = FindPieceAt(row, rookCol, pieces);
                    
                    if (rook && rook->GetName() == "Rook" && !rook->HasMoved()) {
                        // Check if path is clear
                        int start = (rookCol == 0) ? 1 : 5;
                        int end = (rookCol == 0) ? 4 : 7;
                        bool pathClear = true;
                        
                        for (int c = start; c < end; c++) {
                            if (FindPieceAt(row, c, pieces) != nullptr) {
                                pathClear = false;
                                break;
                            }
                        }
                        
                        // Check if king passes through or ends in check
                        if (pathClear) {
                            int checkCols[] = {selectedCol, (selectedCol + col) / 2, col};
                            bool safe = true;
                            for (int c : checkCols) {
                                if (IsSquareUnderAttack(row, c, !whiteTurn, pieces)) {
                                    safe = false;
                                    break;
                                }
                            }
                            
                            if (safe) {
                                // Perform castling
                                selectedPiece->SetPosition(row, col);
                                int newRookCol = (col > selectedCol) ? col - 1 : col + 1;
                                rook->SetPosition(row, newRookCol);
                                moveSuccessful = true;
                            }
                        }
                    }
                }
                // Check for en passant
                else if (selectedPiece->GetName() == "Pawn" && 
                         abs(col - selectedCol) == 1 && 
                         FindPieceAt(row, col, pieces) == nullptr) {
                    
                    int captureRow = selectedRow;
                    Piece* target = FindPieceAt(captureRow, col, pieces);
                    
                    if (target && target->GetName() == "Pawn" && 
                        target->IsWhite() != whiteTurn) {
                        
                        Pawn* targetPawn = dynamic_cast<Pawn*>(target);
                        if (targetPawn && targetPawn == lastDoubleStepPawn &&
                            targetPawn->GetLastMoveDoubleStep() == moveCounter - 1) {
                            
                            // Perform en passant
                            selectedPiece->SetPosition(row, col);
                            
                            // Remove captured pawn
                            for (int i = 0; i < pieces.size(); i++) {
                                if (pieces[i] == target) {
                                    delete pieces[i];
                                    pieces.erase(pieces.begin() + i);
                                    break;
                                }
                            }
                            moveSuccessful = true;
                        }
                    }
                }
                // Normal move
                else if (selectedPiece->IsMoveValid(row, col, pieces)) {
                    Piece* target = FindPieceAt(row, col, pieces);
                    
                    if (target) {
                        if (target->IsWhite() == selectedPiece->IsWhite()) {
                            // Same color - deselect
                            selectedPiece = nullptr;
                            selectedRow = selectedCol = -1;
                        } else {
                            // Capture
                            for (int i = 0; i < pieces.size(); i++) {
                                if (pieces[i] == target) {
                                    delete pieces[i];
                                    pieces.erase(pieces.begin() + i);
                                    break;
                                }
                            }
                            selectedPiece->SetPosition(row, col);
                            moveSuccessful = true;
                        }
                    } else {
                        // Empty square
                        selectedPiece->SetPosition(row, col);
                        moveSuccessful = true;
                    }
                }
                
                if (moveSuccessful) {
                    // Track double-step pawns for en passant
                    lastDoubleStepPawn = nullptr;
                    if (selectedPiece->GetName() == "Pawn") {
                        Pawn* pawn = dynamic_cast<Pawn*>(selectedPiece);
                        if (abs(row - selectedRow) == 2) {
                            pawn->SetLastMoveDoubleStep(moveCounter);
                            lastDoubleStepPawn = pawn;
                        }
                        
                        // Check for promotion
                        int promotionRow = whiteTurn ? 0 : 7;
                        if (row == promotionRow) {
                            Texture2D promQueen = whiteTurn ? w_queen : b_queen;
                            Texture2D promRook = whiteTurn ? w_rook : b_rook;
                            Texture2D promBishop = whiteTurn ? w_bishop : b_bishop;
                            Texture2D promKnight = whiteTurn ? w_knight : b_knight;
                            
                            ShowPromotionDialog(selectedPiece, pieces, 
                                              promQueen, promRook, promBishop, promKnight,
                                              squareSize);
                        }
                    }
                    
                    whiteTurn = !whiteTurn;
                    moveCounter++;
                }
                
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

        // Display whose turn
        const char* turnText = whiteTurn ? "White's Turn" : "Black's Turn";
        DrawText(turnText, 10, 10, 20, whiteTurn ? WHITE : BLACK);
        
        // Display move counter
        DrawText(TextFormat("Move: %d", moveCounter), 10, 35, 16, GRAY);

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