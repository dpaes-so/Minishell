# Minishell
The goal of this project is to create a minimalist shell.

-> Executte project with this flags: valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp -s ./minishell
fot readline leaks supp


Studying ast tree, and bash manual [];

Learning to code with binary trees from C13[✔];

Mess around with bash commands [];


stuff exec needs to do:

1.use readline in my here doc
   
2.start making the built ins

3.make a function that makes pipes, in a loop it only recevies a string with the commands and flags together(need to make pipex work with true path add a strtrim or someshit to make it work problem for later)

4.stil dont know how ima do the redirections, problem for my future self



stuff parser needs to do:

1.make lexer

2.make parser

3.help exec with redirect logic

4.eat a snickers
