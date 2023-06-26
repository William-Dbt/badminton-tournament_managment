# 🏸 Personnal Project | Badminton Tournament Management

## Objectives

This program is used to manage a badminton tournament. It automatize the choice of the matches that players have to do.  
For the moment the program take only one mode, the ALL vs ALL mode, all players have to play against the other ones.  
A history is managed automatically by the program, once the FINISH command is called, the program will save the logs of the tournament in a file that can be used to start it (it's the first question asked when the program is launched).  

## Usage

`make` to compile the program.  
`clean` to clean objects files.  
`fclean` to clean objects files and program file.  

**Execute**: `./tournament`  

The program is interactive, it will first ask you the list of players and the number of courts to start the matches.  
Once the tournament is started, you can do some actions on it as:  
- **MATCH**, it will start a match or finish one, the program ask for name of a player in active match, if no one is started it will try to start one.  
- **INFOS**, it shows the matches in progress and the players waiting list.  
- **JOUEUR**, it allows you to add or remove a player in the middle of the tournament.  
- **HISTORIQUE**, it asks for the name of a player to show its history (played matches, points ...).  
- **FIN**, use it to finish the tournament.  

## Notes

The program print messages in french because it will be used by french ones.  
In the future multiple modes will be managed.  
