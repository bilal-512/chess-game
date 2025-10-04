#pragma once
#include <raylib.h>
#include <string>
#include <iostream>

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
    virtual bool IsMoveValid(int newRow, int newCol) const = 0;

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

    bool IsMoveValid(int newRow, int newCol) const override {
        int dir = isWhite ? -1 : 1;   // white pawns move up, black pawns move down

        // One step forward
        if (newCol == col && newRow == row + dir) {
            return true;
        }

        // Two steps forward (only if on starting rank and not moved yet)
        int startRow = isWhite ? 6 : 1;  // white pawns start on row 6, black on row 1
        if (!hasMoved && row == startRow && newCol == col && newRow == row + 2 * dir) {
            return true;
        }

        return false;
    }

    void SetPosition(int r, int c)  {
        Piece::SetPosition(r, c);
        hasMoved = true;  // disable double-step after first move
    }
};



class Rook : public Piece {
public:
    Rook(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Rook"; }

    bool IsMoveValid(int newRow, int newCol) const override {
        return (newRow == row || newCol == col);
    }
};

class Knight : public Piece {
public:
    Knight(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Knight"; }

    bool IsMoveValid(int newRow, int newCol) const override {
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

    bool IsMoveValid(int newRow, int newCol) const override {
        return abs(newRow - row) == abs(newCol - col);
    }
};

class Queen : public Piece {
public:
    Queen(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "Queen"; }

    bool IsMoveValid(int newRow, int newCol) const override {
        return (row == newRow || col == newCol ||
                abs(newRow - row) == abs(newCol - col));
    }
};

class King : public Piece {
public:
    King(Texture2D tex, int r, int c, bool white)
        : Piece(tex, r, c, white) {}

    std::string GetName() const override { return "King"; }

    bool IsMoveValid(int newRow, int newCol) const override {
        int dr = abs(newRow - row);
        int dc = abs(newCol - col);
        return (dr <= 1 && dc <= 1);
    }
};
