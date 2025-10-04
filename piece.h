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
    bool hasMoved;

public:
    Piece(Texture2D tex, int r, int c, bool white, float s = 0.5f, Color t = WHITE)
        : texture(tex), row(r), col(c), isWhite(white), scale(s), tint(t), hasMoved(false) {}

    virtual ~Piece() {}

    virtual std::string GetName() const = 0;
    virtual bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const = 0;

    virtual void Draw(int squareSize) const {
        int x = col * squareSize + (squareSize - (texture.width * scale)) / 2;
        int y = row * squareSize + (squareSize - (texture.height * scale)) / 2;
        DrawTextureEx(texture, {(float)x, (float)y}, 0.0f, scale, tint);
    }

    virtual void SetPosition(int r, int c) {
        row = r;
        col = c;
        hasMoved = true;
    }

    void SetTexture(Texture2D tex) { texture = tex; }
    int GetRow() const { return row; }
    int GetCol() const { return col; }
    bool IsWhite() const { return isWhite; }
    bool HasMoved() const { return hasMoved; }

    // Check if path is clear between two positions (for sliding pieces)
    bool IsPathClear(int newRow, int newCol, const std::vector<Piece*>& allPieces) const {
        int rowDir = (newRow > row) ? 1 : (newRow < row) ? -1 : 0;
        int colDir = (newCol > col) ? 1 : (newCol < col) ? -1 : 0;

        int checkRow = row + rowDir;
        int checkCol = col + colDir;

        while (checkRow != newRow || checkCol != newCol) {
            for (auto* p : allPieces) {
                if (p->GetRow() == checkRow && p->GetCol() == checkCol) {
                    return false; // Path blocked
                }
            }
            checkRow += rowDir;
            checkCol += colDir;
        }
        return true;
    }
};

// ---------------- Subclasses ----------------

class Pawn : public Piece {
private:
    int lastMoveDoubleStep; // Track which move number this pawn double-stepped

public:
    Pawn(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white), lastMoveDoubleStep(-1) {}

    std::string GetName() const override { return "Pawn"; }

    int GetLastMoveDoubleStep() const { return lastMoveDoubleStep; }

bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const override {
    int dir = isWhite ? -1 : 1;   // White moves up (row decreases), black moves down
    int rowDiff = newRow - row;
    int colDiff = newCol - col;

    Piece* targetPiece = nullptr;
    for (auto* p : allPieces) {
        if (p->GetRow() == newRow && p->GetCol() == newCol) {
            targetPiece = p;
            break;
        }
    }

    // ----- Diagonal capture -----
    if (abs(colDiff) == 1 && rowDiff == dir) {
        if (targetPiece != nullptr && targetPiece->IsWhite() != isWhite && targetPiece->GetName() != "King") {
            return true;
        }
        return false;
    }

    // ----- Forward movement -----
    if (colDiff == 0) {
        // Single-step forward
        if (rowDiff == dir && targetPiece == nullptr) {
            return true;
        }

        // Double-step forward (only from starting rank)
        int startRow = isWhite ? 6 : 1;
        if (row == startRow && rowDiff == 2 * dir && targetPiece == nullptr) {
            int intermediateRow = row + dir;
            // Check if the square in between is empty
            for (auto* p : allPieces) {
                if (p->GetRow() == intermediateRow && p->GetCol() == col) {
                    return false; // Blocked
                }
            }
            return true;
        }
    }

    return false;
}


    void SetPosition(int r, int c) override {
        int rowDiff = abs(r - row);
        if (rowDiff == 2 && !hasMoved) {
            lastMoveDoubleStep = 0; // Will be set by game logic
        }
        Piece::SetPosition(r, c);
    }

    void SetLastMoveDoubleStep(int moveNum) {
        lastMoveDoubleStep = moveNum;
    }
};

class Rook : public Piece {
public:
    Rook(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Rook"; }

    bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const override {
        if (newRow != row && newCol != col) return false;
        return IsPathClear(newRow, newCol, allPieces);
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
        if (abs(newRow - row) != abs(newCol - col)) return false;
        return IsPathClear(newRow, newCol, allPieces);
    }
};

class Queen : public Piece {
public:
    Queen(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Queen"; }

    bool IsMoveValid(int newRow, int newCol, const std::vector<Piece*>& allPieces) const override {
        bool straightLine = (row == newRow || col == newCol);
        bool diagonal = (abs(newRow - row) == abs(newCol - col));
        
        if (!straightLine && !diagonal) return false;
        return IsPathClear(newRow, newCol, allPieces);
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