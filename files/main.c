//small things to implement at end: en pessant


#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <wchar.h>
#include <ctype.h>

void displayBoard(wchar_t board[8][8]);
int moveChoice(wchar_t board[8][8]);
int checkIfMoveOnBoard(char mF1, char mFA, char mT1, char mTA);
int pieceChosen(char fromX, char fromY, wchar_t board[8][8]);
void convertToCo(char originalMoveList[4], int newMoveList[4]);
int takingOwnPiece(int toX, int toY, wchar_t board[8][8]);
int ownsPieceMoving(int fromX, int fromY, wchar_t board[8][8]);
int differentSquaresChosen(int fromX, int fromY, int toX, int toY);
char whichPiece(int x, int y, wchar_t board[8][8]);
int ifMoveDoable(char piece, int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
int pawnMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
int queenMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
int knightMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
int bishopMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
int rookMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
int kingMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
void movePiecesToCastle(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
int checkForCheck(int x, int y, wchar_t board[8][8]);
int checkAfterMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
void movePieces(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
char promotion(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]);
void resign();
void draw();


int turnCounter = 0;
int whiteCanCastle = 1;
int blackCanCastle = 1;
int WLrook = 1;
int WRrook = 1;
int BLrook = 1;
int BRrook = 1;
int whiteCastledThisTurn = 0;
int blackCastledThisTurn = 0;
int testing = 0;
int kingMoving = 0;
int enPassantWhiteX = 9;
int enPassantWhiteY = 9;
int enPassantBlackX = 9;
int enPassantBlackY = 9;

wchar_t chessboard[8][8] = {
        {L'♖', L'♘', L'♗', L'♕', L'♔', L'♗', L'♘', L'♖'},
        {L'♙', L'♙', L'♙', L'♙', L'♙', L'♙', L'♙', L'♙'},
        {L'-', L'-', L'-', L'-', L'-', L'-', L'-', L'-'},
        {L'-', L'-', L'-', L'-', L'-', L'-', L'-', L'-'},
        {L'-', L'-', L'-', L'-', L'-', L'-', L'-', L'-'},
        {L'-', L'-', L'-', L'-', L'-', L'-', L'-', L'-'},
        {L'♟', L'♟', L'♟', L'♟', L'♟', L'♟', L'♟', L'♟'},
        {L'♜', L'♞', L'♝', L'♛', L'♚', L'♝', L'♞', L'♜'}
    };


int main(void){

    /* Note: 0,0 is a8 on a normal board */

    /* This allows UNICODE to be used */
    setlocale(LC_ALL, "");


    displayBoard(chessboard);
    wprintf(L"DRAW to ask for a draw\n");
    wprintf(L"RESIGN to resign\n");

    while(1){
        moveChoice(chessboard);
        displayBoard(chessboard);
    }        

    return 0;
}

void displayBoard(wchar_t board[8][8]){
    wchar_t lazyList[8] = {L'8', L'7', L'6', L'5', L'4', L'3', L'2', L'1'};
    wprintf(L"\n\n\n\n\n");
    for (int i = 0; i < 8; i++){
        wprintf(L"%lc", lazyList[i]);
        for (int j=0; j < 8; j++){
            wprintf(L"   %lc", board[i][j]);
        }
        wprintf(L"   %lc\n", lazyList[i]);
    }
    wprintf(L"    A   B   C   D   E   F   G   H\n");
    return;
}

int moveChoice(wchar_t board[8][8]){
    char moveFromA, moveFrom1, moveToA, moveTo1;
    int validMove = 0;

    //Resetting en passant values to unimportant value
    if (turnCounter % 2 == 0){
        enPassantWhiteX = 9;
        enPassantWhiteY = 9;
    }
    else{
        enPassantBlackX = 9;
        enPassantBlackY = 9;
    }
    
    

    while (!validMove){
        if (turnCounter % 2==0){
            wprintf(L"White Choose A Move (e.g A2 A3): ");
            scanf(" %c %c %c %c", &moveFromA,&moveFrom1,&moveToA,&moveTo1);
        }
        else{
            wprintf(L"Black Choose A Move (e.g A7 A6): ");
            scanf(" %c %c %c %c", &moveFromA,&moveFrom1,&moveToA,&moveTo1);
        }

        moveFromA = toupper(moveFromA);
        moveToA = toupper(moveToA);
        if (isalpha(moveFrom1)){
            moveFrom1 = toupper(moveFrom1);
        }
        if (isalpha(moveTo1)){
            moveTo1 = toupper(moveTo1);
        }

        //Checking for resignation
        if (moveFromA == 'R' && moveFrom1 == 'E' && moveToA == 'S' && moveTo1 == 'I'){
            resign();
        }

        //Asking for draw
        if (moveFromA == 'D' && moveFrom1 == 'R' && moveToA == 'A' && moveTo1 == 'W'){
            draw();
        }

        /* Checking if move is valid before anything happens with the move choices */

        /* First check is if the entered spaces even exist on the board */

        if (!checkIfMoveOnBoard(moveFrom1, moveFromA, moveTo1, moveToA)){
            wprintf(L"Not a valid move\n");
            continue;
        }


        /* Convert input to co-ordinates */
        char originalCo[4] = {moveFrom1, moveFromA, moveTo1, moveToA};
        int newCo[4];
        convertToCo(originalCo, newCo);

        /* Convert back from list to singular co-ordinates */
        int fromX, fromY, toX, toY;
        fromX = newCo[0];
        fromY = newCo[1];
        toX = newCo[2];
        toY = newCo[3];


        /* Check if the same square is entered twice */

        if (!differentSquaresChosen(fromX, fromY, toX, toY)){
            wprintf(L"You have to move a piece\n");
            continue;
        }

        /* Check if there is a piece on the chosen square */

        if(!pieceChosen(fromX, fromY, board)){
            wprintf(L"No piece chosen\n");
            continue;
        }

        /* Check is a piece of yours is on the square you are moving to */

        if(!takingOwnPiece(toX, toY, board)){
            wprintf(L"You cannot take your own piece\n");
            continue;
        }

        /* Check if you own the piece you are moving */

        if(!ownsPieceMoving(fromX, fromY, board)){
            wprintf(L"You dont own the piece you are moving\n");
            continue;
        }

        /* See what piece is being moved */
        char pieceChoice;
        pieceChoice = whichPiece(fromX, fromY, board);

        /* See if that piece can make the move */
        if(!ifMoveDoable(pieceChoice, fromX, fromY, toX, toY, board)){
            wprintf(L"You can't move there\n");
            continue;
        }


        //Check if king is in check after move choice first
        //Have to create a sandbox board where I make the move and then check if king would be in check, if yes dont let the move happen
        if (checkAfterMove(fromX, fromY, toX, toY, board)){
            wprintf(L"King in check\n");
            continue;
        }

        validMove = 1;
        movePieces(fromX, fromY, toX, toY, board);
        char promPiece;
        promPiece = promotion(fromX, fromY, toX, toY, board);
        if(promPiece != 'x'){
            if (turnCounter % 2 == 0){    
                switch (promPiece)
                {
                case 'q':
                    board[toX][toY] = L'♛';
                    break;
                case 'r':
                    board[toX][toY] = L'♜';
                    break;
                case 'k':
                    board[toX][toY] = L'♞';
                    break;
                default:
                    board[toX][toY] = L'♝';
                    break;
                }
            }
            else{
                switch (promPiece)
                {
                case 'q':
                    board[toX][toY] = L'♕';
                    break;
                case 'r':
                    board[toX][toY] = L'♖';
                    break;
                case 'k':
                    board[toX][toY] = L'♘';
                    break;
                default:
                    board[toX][toY] = L'♗';
                    break;
                }
            }
        }

        turnCounter ++;



        for(int i = 0; i<8;i++){
            for(int j = 0; j<8;j++){
                chessboard[i][j] = board[i][j];
            }
        }

    }
    
    return 1;
}



int checkIfMoveOnBoard(char mF1, char mFA, char mT1, char mTA){
    char tilesA[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    char tiles1[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};
    int realMoveFromA = 0, realMoveFrom1 = 0, realMoveToA = 0, realMoveTo1 = 0;

    for (int i = 0; i < 8; i++){

        if (mFA == tilesA[i]){realMoveFromA = 1;}
        if (mF1 == tiles1[i]){realMoveFrom1 = 1;}
        if (mTA == tilesA[i]){realMoveToA = 1;}
        if (mT1 == tiles1[i]){realMoveTo1 = 1;}
        
    }
    if (realMoveFrom1 && realMoveFromA && realMoveTo1 && realMoveToA){
            return 1;
        }
    else{
            return 0;
        }
}

int pieceChosen(char fromX, char fromY, wchar_t board[8][8]){
    if (board[fromX][fromY] == '-'){
        return 0;
    }
    else{
        return 1;
    }
}

void convertToCo(char originalMoveList[4],int newMoveList[4]){
    char rowIF, columnIF, rowOF, columnOF, rowIT, columnIT, rowOT, columnOT;

    for (int i = 0; i < 4; i++){
        if (originalMoveList[i] == 'A'){originalMoveList[i] = 1;}
        else if (originalMoveList[i] == 'B'){originalMoveList[i] = 2;}
        else if (originalMoveList[i] == 'C'){originalMoveList[i] = 3;}
        else if (originalMoveList[i] == 'D'){originalMoveList[i] = 4;}
        else if (originalMoveList[i] == 'E'){originalMoveList[i] = 5;}
        else if (originalMoveList[i] == 'F'){originalMoveList[i] = 6;}
        else if (originalMoveList[i] == 'G'){originalMoveList[i] = 7;}
        else if (originalMoveList[i] == 'H'){originalMoveList[i] = 8;}
    }

    
    for (int i = 0; i < 4; i++){
        if (isdigit(originalMoveList[i])){
            originalMoveList[i] = originalMoveList[i] - '0';
        }
        originalMoveList[i]--;
    }
    

    rowIF = originalMoveList[0];
    columnIF = originalMoveList[1];
    rowIT = originalMoveList[2];
    columnIT = originalMoveList[3];



    rowOF =  7 - rowIF;
    columnOF = columnIF;
    rowOT = 7 - rowIT;
    columnOT = columnIT;

    newMoveList[0] = rowOF;
    newMoveList[1] = columnOF;
    newMoveList[2] = rowOT;
    newMoveList[3] = columnOT;

    return;
    
}

int takingOwnPiece(int toX, int toY, wchar_t board[8][8]){
    //White move
    if (turnCounter % 2==0){
        wchar_t whitePieces[6] = {L'♟', L'♜', L'♞', L'♝', L'♛', L'♚'};
        for (int i=0; i < 6; i++){
            if (board[toX][toY] == whitePieces[i]){
                return 0;
            }
        }
    }
    else{
        wchar_t blackPieces[6] = {L'♕', L'♔', L'♗', L'♘', L'♖', L'♙'};
        for (int i=0; i < 6; i++){
            if (board[toX][toY] == blackPieces[i]){
                return 0;
            }
        }
    }
    return 1;
}

int differentSquaresChosen(int fromX, int fromY, int toX, int toY){
    if (fromX == toX && fromY == toY){
        return 0;
    }
    else{
        return 1;
    }
}

int ownsPieceMoving(int fromX, int fromY, wchar_t board[8][8]){
    //White move
    if (turnCounter % 2==0){
        wchar_t blackPieces[6] = {L'♕', L'♔', L'♗', L'♘', L'♖', L'♙'};
        for (int i=0; i < 6; i++){
            if (board[fromX][fromY] == blackPieces[i]){
                return 0;
            }
        }
    }
    else{
        wchar_t whitePieces[6] = {L'♟', L'♜', L'♞', L'♝', L'♛', L'♚'};        
        for (int i=0; i < 6; i++){
            if (board[fromX][fromY] == whitePieces[i]){
                return 0;
            }
        }
    }
    return 1;
}

char whichPiece(int x, int y, wchar_t board[8][8]){
    wchar_t piece = board[x][y];

    if (piece == L'♟' || piece == L'♙'){return 'p';}
    else if (piece == L'♜' || piece == L'♖'){return 'r';}
    else if (piece == L'♞' || piece == L'♘'){return 'n';}
    else if (piece == L'♛' || piece == L'♕'){return 'q';}
    else if (piece == L'♗' || piece == L'♝'){return 'b';}
    else{return 'k';}
}

int ifMoveDoable(char piece, int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    int illegalMove = 0;
    switch (piece){
        case 'p':
            illegalMove = pawnMove(fromX, fromY, toX, toY, board);
            break;
        case 'q':
            illegalMove = queenMove(fromX, fromY, toX, toY, board);
            break;
        case 'n':
            illegalMove = knightMove(fromX, fromY, toX, toY, board);
            break;
        case 'b':
            illegalMove = bishopMove(fromX, fromY, toX, toY, board);
            break;
        case 'r':
            illegalMove = rookMove(fromX, fromY, toX, toY, board);
            break;
        default:
            illegalMove = kingMove(fromX, fromY, toX, toY, board);
            break;
    }
    if (illegalMove){
        return 0;
    }
    else{
        return 1;
    }
}

//Working (- en pessant)
int pawnMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    int doubleJump = 1;

    // White's move
    if(turnCounter % 2 == 0){
        if(fromX != 6){ doubleJump = 0; }

        // Moving straight
        if(fromY == toY){
            if(fromX - 1 == toX){          
                if(board[toX][toY] != '-') return 1;
                return 0;
            }
            else if(doubleJump && fromX - 2 == toX){  // Double jump
                if(board[fromX - 1][toY] != '-' || board[toX][toY] != '-') return 1;
                enPassantWhiteX = toX;
                enPassantWhiteY = toY;
                return 0;
            }
            else{
                return 1;
            }
        }
        else{
            // Diagonal
            if((fromX - 1 == toX) && (fromY - 1 == toY || fromY + 1 == toY)){
                if(board[toX][toY] != '-') return 0; // Can capture
                else{
                    if(toX +1 == enPassantBlackX){
                        board[enPassantBlackX][enPassantBlackY] = L'-';
                        return 0;
                    }
                    else{
                        return 1; // Cannot move diagonally to empty square
                    }
                    
                } 
            }
            else{
                return 1;
            }
        }
    }
    // Black's move
    else{
        if(fromX != 1){ doubleJump = 0; }

        // Moving straight
        if(fromY == toY){
            if(fromX + 1 == toX){    
                if(board[toX][toY] != '-') return 1;
                return 0;
            }
            else if(doubleJump && fromX + 2 == toX){  // Double jump
                if(board[fromX + 1][toY] != '-' || board[toX][toY] != '-') return 1;
                enPassantBlackX = toX;
                enPassantBlackY = toY;
                return 0;
            }
            else{
                return 1;
            }
        }
        else{
            // Diagonal 
            if((fromX + 1 == toX) && (fromY - 1 == toY || fromY + 1 == toY)){
                if(board[toX][toY] != '-') return 0; // Can capture
                else{
                    if(toX -1 == enPassantWhiteX){
                        board[enPassantWhiteX][enPassantWhiteY] = L'-';
                        return 0;
                    }
                    else{
                        return 1; // Cannot move diagonally to empty square
                    }
                } 
            }
            else{
                return 1;
            }
        }
    }


    return 1;
}

//Working but all code copied from bishop and rook so MESSY 
int queenMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    //Queen must either move diagonally or be in the same row or column
    if (((abs(fromX - toX)) != (abs(fromY - toY))) && (fromX != toX && fromY != toY)){
        return 1;
    }
    int xMoves = abs(fromX - toX);
    int yMoves = abs(fromY - toY);
    int movingLeft = 0;
    int movingUp = 0;

    if (fromY > toY){movingLeft = 1;}
    if (fromX == toX){
        //she is moving horiz (on board)
        for(int i = 1; i < yMoves; i++){
            if(movingLeft){
                if (board[fromX][fromY -i] != '-'){
                    return 1;
                }
            }
            else{
                if (board[fromX][fromY +i] != '-'){
                    return 1;
                }
            }
        }
    }
    if (toX > fromX){movingUp = 1;}
    else if (fromY == toY){
        //she is moving vert(on board)
        for(int i = 1; i < xMoves; i++){
            if(movingUp){
                if (board[fromX + i][fromY] != '-'){
                    return 1;
                }
            }
            else{
                if (board[fromX -i][fromY] != '-'){
                    return 1;
                }
            }
        }
    }
    else{
        //she must be moving diag
        //all this imported from bishop (should work)
        int dir;
        int distMoved = abs(fromX - toX);
        if((fromX > toX) && (fromY > toY)){
        //bottom left
        dir = 3;
    }
    else if((fromX < toX) && (fromY > toY)){
        //bottom right
        dir = 4;
    }
    else if((fromX > toX) && (fromY < toY)){
        //top left
        dir = 2;
    }
    else{
        //top right
        dir = 1;
    }

    //Correct Loop
    for(int i = 1; i < distMoved; i++){
        switch (dir)
        {
        //tr
        case 1:
            if(board[fromX+i][fromY+i] != '-'){return 1;}
            break;
        //tl
        case 2:
            if(board[fromX-i][fromY+i] != '-'){return 1;}
            break;
        //bl
        case 3:
            if(board[fromX-i][fromY-i] != '-'){return 1;}
            break; 
        //br    
        default:
            if(board[fromX+i][fromY-i] != '-'){return 1;}
            break;
        }
    }
    }
    return 0;
}

//Working
int knightMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    //Have to test the 8 possible moves for a knight
    if (((fromX -2 == toX)&&(fromY -1 == toY)) || ((fromX -2 == toX)&&(fromY +1 == toY)) || ((fromX -1 == toX)&&(fromY -2 == toY)) || ((fromX -1 == toX)&&(fromY +2 == toY)) || ((fromX +1 == toX)&&(fromY -2 == toY)) || ((fromX +1 == toX)&&(fromY +2 == toY) || (fromX +2 == toX)&&(fromY -1 == toY)) || ((fromX +2 == toX)&&(fromY +1 == toY))){
    return 0;
    }
    else{
        return 1;
    }

}

//Working
int bishopMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    int dir;
    if(fromX == toX || fromY == toY){
        return 1;
    }
    int distMoved = abs(fromX - toX);
    //Ensuring it moves diagonally
    if ((abs(fromX - toX)) != (abs(fromY - toY))){
        return 1;
    }

    //Finding direction
    if((fromX > toX) && (fromY > toY)){
        //bottom left
        dir = 3;
    }
    else if((fromX < toX) && (fromY > toY)){
        //bottom right
        dir = 4;
    }
    else if((fromX > toX) && (fromY < toY)){
        //top left
        dir = 2;
    }
    else{
        //top right
        dir = 1;
    }

    //Correct Loop
    for(int i = 1; i < distMoved; i++){
        switch (dir)
        {
        //tr
        case 1:
            if(board[fromX+i][fromY+i] != '-'){return 1;}
            break;
        //tl
        case 2:
            if(board[fromX-i][fromY+i] != '-'){return 1;}
            break;
        //bl
        case 3:
            if(board[fromX-i][fromY-i] != '-'){return 1;}
            break; 
        //br    
        default:
            if(board[fromX+i][fromY-i] != '-'){return 1;}
            break;
        }
    }

    
    return 0;
}

//Working
int rookMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    if(fromX != toX && fromY != toY){
        return 1;
    }

    int xMoves = abs(fromX - toX);
    int yMoves = abs(fromY - toY);
    int movingLeft = 0;
    int movingUp = 0;

    //Side to side
    if (fromY > toY){movingLeft = 1;}
    if(fromX == toX){  
        for(int i = 1; i < yMoves; i++){
            if(movingLeft){
                if (board[fromX][fromY -i] != '-'){
                    return 1;
                }
            }
            else{
                if (board[fromX][fromY +i] != '-'){
                    return 1;
                }
            }
        }
    }
    //Up and Down
    if (toX > fromX){movingUp = 1;}
    else if(fromY == toY){
        for(int i = 1; i < xMoves; i++){
            if(movingUp){
                if (board[fromX + i][fromY] != '-'){
                    return 1;
                }
            }
            else{
                if (board[fromX -i][fromY] != '-'){
                    return 1;
                }
            }
        }
    }

    //noting if a rook move so king cant castle with it
    if(fromX == 7 && fromY == 0){
        WLrook = 0;
    }
    else if(fromX == 7 && fromY == 7){
        WRrook = 0;
    }
    else if(fromX == 0 && fromY == 0){
        BLrook = 0;
    }
    else if(fromX == 0 && fromY == 7){
        BRrook = 0;
    }


    return 0;
}

//Working (??)
int kingMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    if((abs(fromX - toX) > 1) || (abs(fromY - toY) >1)){
        
        if(turnCounter % 2 == 0){

            if(!whiteCanCastle){return 1;}

            //white castlable squares
            if(toX != 7){return 1;}
            if(toY != 2 && toY != 6){return 1;}
        }
        else if(turnCounter % 2 != 0){

            if(!blackCanCastle){return 1;}

            //black castleable squares
            if(toX != 0){return 1;}
            if(toY != 2 && toY != 6){return 1;}
        }

        //In each, use the function on the squares to see if they are in check
        
        //Must be castleing
        if (toX == 7 && toY == 6){
            //white short castle
            if (board[7][6] != L'-' || board[7][5] != L'-'){
                return 1;
            }
            if (!WRrook){
                return 1;
            }
            if(checkForCheck(7, 6, board) || checkForCheck(7, 5, board)){
                return 1;
            }
        }
        else if (toX == 7 && toY == 2){
            //white long castle
            if (board[7][1] != L'-' || board[7][2] != L'-' || board[7][3] != L'-'){
                return 1;
            }
            if (!WLrook){
                return 1;
            }
            if(checkForCheck(7, 1, board) || checkForCheck(7, 2, board) || checkForCheck(7, 3, board)){
                return 1;
            }
        }
        else if (toX == 0 && toY == 6){
            //black short castle
            if (board[0][6] != L'-' || board[0][5] != L'-'){
                return 1;
            }
            if (!BRrook){
                return 1;
            }
            if(checkForCheck(0, 6, board) || checkForCheck(0, 5, board)){
                return 1;
            }
        }
        else if (toX == 0 && toY == 2){
            //black long castle
            if (board[0][1] != L'-' || board[0][2] != L'-' || board[0][3] != L'-'){
                return 1;
            }
            if (!BLrook){
                return 1;
            }
            if(checkForCheck(0, 1, board) || checkForCheck(0, 2, board) || checkForCheck(0, 3, board)){
                return 1;
            }
        }
        else{
            wprintf(L"BIG ERROR, THIS SHOULD NOT BE POSSIBLE\n");
        }
        
        
        if(turnCounter % 2 == 0){
            if(whiteCanCastle){
                movePiecesToCastle(fromX, fromY, toX, toY, board);
                whiteCanCastle = 0;
            }
        }
        else{
            if(blackCanCastle){
                movePiecesToCastle(fromX, fromY, toX, toY, board);
                blackCanCastle = 0;
            }
        }
    }

    
    kingMoving = 1;
    return 0;
}

int checkForCheck(int x, int y, wchar_t board[8][8]){

    //1 is check
    //0 is not in check

    //Divide into section per piece
    //Ensure not to check outside scope otherwise unexpected behaviour will happen

    /* IDEAS FOR STAYING IN SCOPE: (do thise for knight, idk about rest) have 2 lists, a for loop iterates through them and they conatin the changes to be made to x or y
    if the number produced is not between 0 and 8 for any of them then continue it until completion */

    //Knight (WORKS)
    int xPossibilities[8] = {-1, -2, -2, -1, 1, 2, 2, 1};
    int yPossibilities[8] = {-2, -1, 1, 2, 2, 1, -1, -2};

    for(int i = 0; i<8; i++){
        int testX = x + xPossibilities[i];
        int testY = y + yPossibilities[i];

        if(testX < 0 || testY < 0 || testX > 7 || testY > 7){
            continue;
        }

        if(turnCounter % 2 == 0){
            if(board[testX][testY] == L'♘'){
                return 1;
            }
        }
        else{
            if(board[testX][testY] == L'♞'){
                return 1;
            }
        }
    }


    //Rook and queen WORKING

    //UP
    for (int i = 1; i < 8; i++){
        int testX = x - i;
        if(testX < 0 || testX > 7){
            break;
        }
        
        if (board[testX][y] == L'-'){
            continue;
        } 
        else if ((turnCounter % 2 == 0) && (board[testX][y] == L'♖' || board[testX][y] == L'♕')){
            return 1;
        }
        else if ((turnCounter % 2 != 0) && (board[testX][y] == L'♜' || board[testX][y] == L'♛')){
            return 1;
        }
        else{
            break;
        }
    }
    //DOWN
    for (int i = 1; i < 8; i++){
        int testX = x + i;
        if(testX < 0 || testX > 7){
            break;
        }
        
        if (board[testX][y] == L'-'){
            continue;
        } 
        else if ((turnCounter % 2 == 0) && (board[testX][y] == L'♖' || board[testX][y] == L'♕')){
            return 1;
        }
        else if ((turnCounter % 2 != 0) && (board[testX][y] == L'♜' || board[testX][y] == L'♛')){
            return 1;
        }
        else{
            break;
        }
    }
    //LEFT
    for (int i = 1; i < 8; i++){
        int testY = y - i;
        if(testY < 0 || testY > 7){
            break;
        }
        
        if (board[x][testY] == L'-'){
            continue;
        } 
        else if ((turnCounter % 2 == 0) && (board[x][testY] == L'♖' || board[x][testY] == L'♕')){
            return 1;
        }
        else if ((turnCounter % 2 != 0) && (board[x][testY] == L'♜' || board[x][testY] == L'♛')){
            return 1;
        }
        else{
            break;
        }
    }
    //RIGHT
    for (int i = 1; i < 8; i++){
        int testY = y + i;
        if(testY < 0 || testY > 7){
            break;
        }
        
        if (board[x][testY] == L'-'){
            continue;
        } 
        else if ((turnCounter % 2 == 0) && (board[x][testY] == L'♖' || board[x][testY] == L'♕')){
            return 1;
        }
        else if ((turnCounter % 2 != 0) && (board[x][testY] == L'♜' || board[x][testY] == L'♛')){
            return 1;
        }
        else{
            break;
        }
    }

    //Bishop and Queen

    //TOP RIGHT
    for (int i = 1; i < 8; i++){
        int testX = x - i;
        int testY = y + i;
        if(testY < 0 || testY > 7 || testX < 0 || testX > 7){
            break;
        }
        
        if (board[testX][testY] == L'-'){
            continue;
        } 
        else if ((turnCounter % 2 == 0) && (board[testX][testY] == L'♗' || board[testX][testY] == L'♕')){
            return 1;
        }
        else if ((turnCounter % 2 != 0) && (board[testX][testY] == L'♝' || board[testX][testY] == L'♛')){
            return 1;
        }
        else{
            break;
        }
    }
    //TOP LEFT
    for (int i = 1; i < 8; i++){
        int testX = x - i;
        int testY = y - i;
        if(testY < 0 || testY > 7 || testX < 0 || testX > 7){
            break;
        }
        
        if (board[testX][testY] == L'-'){
            continue;
        } 
        else if ((turnCounter % 2 == 0) && (board[testX][testY] == L'♗' || board[testX][testY] == L'♕')){
            return 1;
        }
        else if ((turnCounter % 2 != 0) && (board[testX][testY] == L'♝' || board[testX][testY] == L'♛')){
            return 1;
        }
        else{
            break;
        }
    }
    //BOTTOM RIGHT
    for (int i = 1; i < 8; i++){
        int testX = x + i;
        int testY = y + i;
        if(testY < 0 || testY > 7 || testX < 0 || testX > 7){
            break;
        }
        
        if (board[testX][testY] == L'-'){
            continue;
        } 
        else if ((turnCounter % 2 == 0) && (board[testX][testY] == L'♗' || board[testX][testY] == L'♕')){
            return 1;
        }
        else if ((turnCounter % 2 != 0) && (board[testX][testY] == L'♝' || board[testX][testY] == L'♛')){
            return 1;
        }
        else{
            break;
        }
    }
    //BOTTOM LEFT
    for (int i = 1; i < 8; i++){
        int testX = x + i;
        int testY = y - i;
        if(testY < 0 || testY > 7 || testX < 0 || testX > 7){
            break;
        }
        
        if (board[testX][testY] == L'-'){
            continue;
        } 
        else if ((turnCounter % 2 == 0) && (board[testX][testY] == L'♗' || board[testX][testY] == L'♕')){
            return 1;
        }
        else if ((turnCounter % 2 != 0) && (board[testX][testY] == L'♝' || board[testX][testY] == L'♛')){
            return 1;
        }
        else{
            break;
        }
    }

    //Pawns and Kings

    //KING
    int xPossibilitiesK[8] = {0, -1, -1, -1, 0, 1, 1, 1};
    int yPossibilitiesK[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

    for(int i = 0; i<8; i++){
        int testX = x + xPossibilitiesK[i];
        int testY = y + yPossibilitiesK[i];

        if(testX < 0 || testY < 0 || testX > 7 || testY > 7){
            continue;
        }

        if(turnCounter % 2 == 0){
            if(board[testX][testY] == L'♔'){
                return 1;
            }
        }
        else{
            if(board[testX][testY] == L'♚'){
                return 1;
            }
        }
    }
    //PAWN
    if(turnCounter % 2 == 0){
       int xPossibilitiesP[2] = {-1, -1};
       int yPossibilitiesP[2] = {-1, 1};
       
       for(int i = 0; i<2; i++){
        int testX = x + xPossibilitiesP[i];
        int testY = y + yPossibilitiesP[i];

        if(testX < 0 || testY < 0 || testX > 7 || testY > 7){
            continue;
        }

        if(board[testX][testY] == L'♙'){
            return 1;
        }
        }
    }
    else{
       int xPossibilitiesP[2] = {1, 1};
       int yPossibilitiesP[2] = {-1, 1};
       
       for(int i = 0; i<2; i++){
        int testX = x + xPossibilitiesP[i];
        int testY = y + yPossibilitiesP[i];

        if(testX < 0 || testY < 0 || testX > 7 || testY > 7){
            continue;
        }

        if(board[testX][testY] == L'♟'){
            return 1;
        }
        }
    }
    return 0;
}

void movePiecesToCastle(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    if (toX == 7 && toY == 6){
        //white short castle
        board[7][6] = L'♚';
        board[7][4] = L'-';
        board[7][5] = L'♜';
        board[7][7] = L'-';
        whiteCastledThisTurn = 1;
    }
    else if (toX == 7 && toY == 2){
        //white long castle
        board[7][2] = L'♚';
        board[7][4] = L'-';
        board[7][3] = L'♜';
        board[7][0] = L'-';
        whiteCastledThisTurn = 1;
    }
    else if (toX == 0 && toY == 6){
        //black short castle
        board[0][6] = L'♔';
        board[0][4] = L'-';
        board[0][5] = L'♖';
        board[0][7] = L'-';
        blackCastledThisTurn = 1;
    }
    else if (toX == 0 && toY == 2){
        //black long castle
        board[0][2] = L'♔';
        board[0][4] = L'-';
        board[0][3] = L'♖';
        board[0][0] = L'-';
        blackCastledThisTurn = 1;
    }
}

void movePieces(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    if ((blackCastledThisTurn || whiteCastledThisTurn) && !testing){
        blackCastledThisTurn = 0;
        whiteCastledThisTurn = 0;
    }
    else{
        wchar_t pieceToMove = board[fromX][fromY];
        board[toX][toY] = pieceToMove;
        board[fromX][fromY] = L'-';
    }
    return;
}

int checkAfterMove(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    wchar_t sbBoard[8][8];
    int kingX;
    int kingY;
    for (int i = 0; i < 8; i++){

        for (int j = 0; j < 8; j++){
            sbBoard[i][j] = board [i][j];
            if (turnCounter % 2 == 0){
                if (board[i][j] == L'♚'){
                    kingX = i;
                    kingY = j;
                }
            }
            else{
                if (board[i][j] == L'♔'){
                    kingX = i;
                    kingY = j;
                }
            }
        }
    }
    if(kingMoving){
            kingX = toX;
            kingY = toY;
        }
    testing = 1;
    movePieces(fromX, fromY, toX, toY, sbBoard);
    testing = 0;
    kingMoving = 0;
    if (checkForCheck(kingX, kingY, sbBoard)){
        return 1;
    }
    else{
        return 0;
    }
}

char promotion(int fromX, int fromY, int toX, int toY, wchar_t board[8][8]){
    char choice = 'x';
    if (turnCounter % 2 == 0){
        if ((board[toX][toY] == L'♟') && (toX == 0)){
            while(choice != 'q' && choice != 'r' && choice != 'b' && choice != 'k'){
                wprintf(L"White, Choose a piece to promote to (Q, R, B, K): ");
                scanf(" %c", &choice);

                choice = tolower(choice);
            }
        }
    }
    else{
       if ((board[toX][toY] == L'♙') && (toX == 7)){
            while(choice != 'q' && choice != 'r' && choice != 'b' && choice != 'k'){
                wprintf(L"Black, Choose a piece to promote to (Q, R, B, K): ");
                scanf(" %c", &choice);

                choice = tolower(choice);
            }
        } 
    }
    return choice;
}

void resign(){
    if (turnCounter % 2 == 0){
        wprintf(L"Black Wins!\n");
    }
    else{
        wprintf(L"White Wins\n");
    }
    exit(0);
}

void draw(){
    char response;
    while (response != 'y' && response != 'n'){
        if (turnCounter % 2 == 0){
            wprintf(L"White has offered a draw. Black, do you accept? (Y/N): ");
            scanf(" %c", &response);
        }
        else{
            wprintf(L"Black has offered a draw. White, do you accept? (Y/N): ");
            scanf(" %c", &response);
        }
        response = tolower(response);
    }
    if (response == 'y'){
        wprintf(L"Game ends in a draw.\n");
        exit(0);
    }
    else{
        wprintf(L"Draw not accepted\n");
        return;
    }

}