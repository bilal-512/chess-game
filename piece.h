#pragma once
#include <raylib.h>
#include <string>
#include <iostream>
#include <vector>

class Piece {
protected:
    Texture2D texture;
    int row, col;
    float scale;
    Color tint;
    bool isWhite;

public:
    Piece(Texture2D tex, int r, int c, bool white, float s = 0.5f, Color t = WHITE)
        : texture(tex), row(r), col(c), isWhite(white), scale(s), tint(t) {}

    virtual ~Piece() {}

    virtual std::string GetName() const = 0;  // abstract
    virtual bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const = 0;

    virtual void Draw(int squareSize) const {
        int x = col * squareSize + (squareSize - (texture.width * scale)) / 2;
        int y = row * squareSize + (squareSize - (texture.height * scale)) / 2;
        DrawTextureEx(texture, {(float)x, (float)y}, 0.0f, scale, tint);
    }

    void SetPosition(int r, int c) {
        row = r;
        col = c;
    }

    int GetRow() const { return row; }
    int GetCol() const { return col; }
    bool IsWhite() const { return isWhite; }
};

// ---------------- Subclasses ----------------

class Pawn : public Piece {
private:
    bool hasMoved = false;

public:
    Pawn(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Pawn"; }

    bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const override {
        int dir = isWhite ? -1 : 1;   // white pawns move up, black pawns move down
        int rowDiff = newRow - row;
        int colDiff = newCol - col;

        // Find piece at destination
        Piece* targetPiece = nullptr;
        for (auto* p : allPieces) {
            if (p->GetRow() == newRow && p->GetCol() == newCol) {
                targetPiece = p;
                break;
            }
        }

        // Diagonal capture (one square diagonally)
        if (abs(colDiff) == 1 && rowDiff == dir) {
            // Can only capture if there's an opponent's piece that's NOT a king
            if (targetPiece != nullptr && 
                targetPiece->IsWhite() != isWhite && 
                targetPiece->GetName() != "King") {
                return true;
            }
            return false;
        }

        // Forward movement (must be same column)
        if (colDiff == 0) {
            // One step forward
            if (rowDiff == dir) {
                // Square must be empty
                if (targetPiece == nullptr) {
                    return true;
                }
                return false;
            }

            // Two steps forward (only if on starting rank and not moved yet)
            int startRow = isWhite ? 6 : 1;
            if (!hasMoved && row == startRow && rowDiff == 2 * dir) {
                // Both squares must be empty
                if (targetPiece == nullptr) {
                    // Check intermediate square
                    int intermediateRow = row + dir;
                    for (auto* p : allPieces) {
                        if (p->GetRow() == intermediateRow && p->GetCol() == col) {
                            return false;  // Path is blocked
                        }
                    }
                    return true;
                }
                return false;
            }
        }

        return false;
    }

    void SetPosition(int r, int c) {
        Piece::SetPosition(r, c);
        hasMoved = true;  // disable double-step after first move
    }
};

class Rook : public Piece {
public:
    Rook(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Rook"; }

    bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const override {
        return (newRow == row || newCol == col);
    }
};

class Knight : public Piece {
public:
    Knight(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Knight"; }

    bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const override {
        int dr = abs(newRow - row);
        int dc = abs(newCol - col);
        return (dr == 2 && dc == 1) || (dr == 1 && dc == 2);
    }
};

class Bishop : public Piece {
public:
    Bishop(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Bishop"; }

    bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const override {
        return abs(newRow - row) == abs(newCol - col);
    }
};

class Queen : public Piece {
public:
    Queen(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Queen"; }

    bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const override {
        return (row == newRow || col == newCol ||
                abs(newRow - row) == abs(newCol - col));
    }
};

class King : public Piece {
public:
    King(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "King"; }

    bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const override {
        int dr = abs(newRow - row);
        int dc = abs(newCol - col);
        return (dr <= 1 && dc <= 1);
    }
};