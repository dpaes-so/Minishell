# Minishell
The goal of this project is to create a minimalist shell.

->valgrind --leak-check=full --trace-children=yes --track-fds=yes --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp -s ./minishell

for readline leaks supp


Studying ast tree, and bash manual [✔];

Learning to code with binary trees from C13[✔];

Mess around with bash commands [✔];


stuff exec needs to do:

1.use readline in my here doc
   
2.start making the built ins

3.make a function that makes pipes, in a loop it only recevies a string with the commands and flags together(need to make pipex work with true path add a strtrim or someshit to make it work problem for later(not hard just an access before the path if not acccescible executael dup coamnds)) and also correct exit cods also with acces easy

4.stil dont know how ima do the redir, problem for my future self

5. im here now. daniel was late today,gotta check that eit stuff cuz the  beta male wanted me to do it

fix export sem = ater a real export 
<< lim | echo no new line

linhas no export.c
heredoc.c


stuff parser needs to do:
echo ola > txt ola > outro | echo banana > banana
1.make lexer
   • tokenize the input to then check syntax errors and such
2.make parser
   • every tree node has struct cmd, cmd will only be used if node isnt pipe, in char *cmd just holds first string, **redir will hold all redir in a array
3.help exec with redirect logic

4.eat a snickers

FUCK THIS
