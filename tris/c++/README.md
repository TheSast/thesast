# Author of "C++ Tris"

Name: TheSast

E-mail: no

# Paramaters
## -h
 Enables the gameboard history.

## -b<u>WIDTH</u>[<u>*</u>|<u>x</u>|<u>:</u><u>WIDTH</u>]
 Sets the size of the board.
 ### WIDTH of the board
 range `3` - `∞`  
 default `3`
 ### HEIGTH of the board
 range `3` - `∞`  
 default is `WIDTH` if unused, else `3`

## -c[<u>STRENGTH</u>] and -co
 Enables the computer opponent, they will play in the stead of Player 2.
 ### -co
 Enables a second computer opponent, they will play in the stead of Player 1.
 ### -cd
 Adds delay after the computer opponent executes a move.
 ### STRENGTH of the computer opponent 
 range `0` - `5`  
 default is `5` if `STRENGTH` exceeds `5`, else `2`

## -t<u>LENGTH</u>
 Sets how many marks a player has to place in a horizontal, vertical, or diagonal row to win the game.
 ### LENGTH is that number of marks
 range `3` - `WIDTH` or `HEIGHT`, whichever is lower  
 default `3`

## -s<u>SYMBOL1</u>[<u>SYMBOL2</u>]
 Sets the character that represents the players on the game board.
 ### SYMBOL1 is the character representing Player 1 
 default is `X`
 ### SYMBOL2 is the character representing Player 2
 default is `X` if `SYMBOL1` equals `O`, else `O`.  