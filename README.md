# Infinity-Connect - PowerN

## Install
* Download files
* `make`  
* `./pow`  

## User Guide

### 1. RULES
* Decide who plays first. Players will alternate turns after playing a checker. *NOTE: The player starting the game will play second in the next game.* 
* On a player turn, drop one of your checkers down **ANY** of the slots in the top of the grid. 
* Player alternates until one player gets **FIVE** checkers of his or her colour in a row. The five in a row can be **horizontal**, **vertical**, or **diagonal** (see [attachments](#attachments)). 
### 2. HOW TO PLAY

When your turn arrives, you have several options, you need to enter the terminal: 

- `p[column]` to **PLAY** a checker in the column. 
- `f[column]<:[row]>` to **FOCUS** your view on a part of the grid (the column and row correspond to the checker at the bottom middle). 

*NOTE: row is set to 0 by default.* 

- `s` to **SAVE** the game. Enter a **free** file name. 
- `q` to **QUIT** the game. 

*NOTE: [parameter] means required parameter and <element> means optional element.*  

### 3. MENUS

To navigate through the menus, simply enter the number of your choice. 

When you want to load a game from the main menu, you will need to give the **exact name** (including the extension) of the corresponding file. If you wish to load a game that is not from our game program, please **respect the format** in the [attachments.](#attachments) 

In the “others” submenu, you can access the “settings” submenu. Here you can see the current settings (“show settings”), load a settings file (“load settings”), or create a default settings file that will be loaded (“reset settings”). Please **respect the format** in the [attachments.](#attachments) 

*Note: File manipulation works as it does for the game saves.* 

### [+] Attachments <a name="attachments"></a>

#### Winning Rows
![image](https://github.com/voXrey/infinity-connect/assets/72698969/5f66ac68-61da-4c50-aedb-8c02b696b045)  
*Figure 1- Horizontal Row*  

![image](https://github.com/voXrey/infinity-connect/assets/72698969/edb25384-ec6a-4aae-a52e-058e9591691c)  
*Figure 2- Diagonal 1 Row*  

![image](https://github.com/voXrey/infinity-connect/assets/72698969/364d90fb-12d0-4314-8c8e-03e35f65be29)  
*Figure 3 - Diagonal 2 Row*  

![image](https://github.com/voXrey/infinity-connect/assets/72698969/86414eec-a70a-46ac-a8c1-24c663a8b398)  
*Figure 4 - Vertical Row*  

#### Save File Format
`FreeNameFile:`
> Player 1’s pseudo  
> Player 2’s pseudo  
> First Column Played by Player 1  
> First Column Played by Player 2  
> Second Column Played by Player 1  
> ... 

#### Settings File Format

`FreeNameFile:`
> POWER==5  
> WIDTH==15  
> HEIGHT==20  
> P1\_TOKEN==🔵  
> P2\_TOKEN==🔴  
> VOID\_TOKEN\_STR==    
> VOID\_TOKEN\_SIZE==2  
> VOID\_TOKEN\_STR\_LAST\_LINE\_UNIT==\_

*Note: It’s important not to change settings order. Note: By default “settings.txt” is loaded.*
