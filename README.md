## Table des Matières
- [Lexer](#lexer)
- [Parser](#parser)
- [Executer](#executer)


## LEXER

Le but du lexer est de transformer notre input en liste chainée, chaque node contenant la chaîne de caractères et son token associé.

Par exemple :
```ls | cat``` 
devient


<img width="331" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/b2c9c12b-0e3b-4151-a7f1-1c5407a99ccf">


(On remarque que ```ls |       cat```,  ```ls|cat``` ou encore ```"ls"|   'cat'``` donnent le même résultat.)

Autre exemple, avec plus de contenu:

```< Makefile cat && echo hello | wc```
<img width="930" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/69d2ea9e-445a-4519-8c3a-a8c4a77b8d01">


Comme le cas précédent, je peux l'écrire de plusieurs façons... voici un exemple parmi tant d'autres:  
```<Makefile    'cat'  &&echo   'h'"e"'ll'o   |wc```

Notre liste de tokens :
- WORD: (un mot, qui sera un nom de commande, une option, un argument, un path...)
- PIPE:		|
- AND:		&&
- OR:		||
- R_REDIR:	>
- RR_REDIR:	>>
- L_REDIR:	<
- LL_REDIR:	<<
- PAR_LEFT:	(
- PAR_RIGHT:	)


## PARSER

Création un AST (Abstract Syntax Tree), qui se comporte comme une liste chaînée mais avec 2 pointeurs (right et left) vers les nodes suivantes, contrairement a une liste chaînée classique qui n'en a qu'une.
Cela a pour but d'organiser nos tokens par ordres de priorité, afin de faciliter l'exécution.

L'ordre de priorité HORS PARENTHÈSES est le suivant:
- AND / OR (en partant de la fin)
- PIPE (en partant de la fin)
- L_REDIR / LL_REDIR / R_REDIR / RR_REDIR (en partant du début)
- WORD (en partant du début)

Dans l'exemple précédent, notre AST ressemblera a ca :  


```< Makefile cat && echo a | wc -l```

<img width="666" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/35d59795-b302-4e6a-a822-06bf7ac5678f">

À noter qu'avec les parenthèses, notre input donnerait un AST ainsi qu'un résultat complètement différent:  
```(< Makefile cat && echo a) | wc -l```

<img width="570" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/f992de3b-8440-458b-a087-c38b8a9a8462">


```cat <Makefile | grep -v a | >> outfile wc > out -l```

<img width="968" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/ea556588-a4f1-4c04-ac8a-26d3a058ce4e">


## EXECUTER

L'AST étant créé, il ne reste plus qu'à exécuter le tout de manière récursive avec quelques règles:

- Au lancement, je déclare dans ma data, data->in = STDIN_FILENO, et data->out = STDOUT_FILENO.
- Si token = PIPE, je pipe(fd) et je crée 2 childs (donc 2 forks):
  . child1 sur node->left avec data->out = fd[1]
  . child2 sur node->right avec data->in = fd[0].
  . Le parent attend ses deux enfants.
- Si token = AND, je continue sur node->left, et lorsque toute l'expression sera finie, je check l'exit_status.
  . Si exit_status = 0 (donc la commande a réussi), alors je continue sur node->right;
  . Sinon, je ne fais rien et je remonte naturellement à la node précédente.
- Si token = OR, je continue sur node->left, et lorsque toute l'expression sera finie, je check l'exit_status.
  . Si exit_status != 0 (donc la commande a échoué), alors je continue sur node->right;
  . Sinon, je ne fais rien et je remonte naturellement à la node précédente.
- Sinon, je suis forcément dans l'exécution d'une commande:
  . update data->in et data->out en fonction des token REDIR présents dans l'expression.
  . Si builtin -> pas de fork;
  .	Si la node précédente est PIPE ->pas besoin de fork;
  . Sinon fork et execve la commande.  




## COMMENT GÉRER LES PIPES

```ls | grep -v a | wc -l | cat```

(J'ai regroupé les nodes sur l'image pour une meilleure clarté)

<img width="880" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/950648ee-4f27-428c-a234-6ab085f56898">

On commence par PIPE 1, data->in = 0 (STDIN_FILENO), et data->out = 1 (STDOUT_FILENO), par défaut en haut de la node.  
Je pipe fd[2] : fd[0] = 3 et fd[1] = 4;  
Je fork 2 fois et crée 2 childs à qui j'envoie les fd correspondants :  
	- Child right: data->in = fd[0] (donc 3), et data->out = data->out (donc 1);  
 	- Child left:  data->in = data->in (donc 0) , et data->out = fd[1] (donc 4);  
  
Je pars à gauche et arrive sur le PIPE 2, je pipe fd[2] : fd[0] = 5 et fd[1] = 6;  
Je fork 2 fois et crée 2 childs:  
	- Child right: data->in = fd[0] (donc 5), et data->out = data->out (donc 4);  
 	- Child left:  data->in = data->in (donc 0), et data->out = fd[1] (donc 6);  
  
Je pars à gauche et arrive sur le PIPE 3, je pipe fd[2] : fd[0] = 7 et fd[1] = 8;  
Je fork 2 fois et crée 2 childs:  
	- Child right: data->in = fd[0] (donc 7), et data->out = data->out (donc 6);  
 	- Child left:  data->in = data->in (donc 0), et data->out = fd[1] (donc 8);  
  
Et voilà, chaque expression peut maintenant lire et écrire dans leur fd par défaut.  
Evidemment, si des redirect sont dans l'expression, il faut mettre à jour le data->in et data->out;  



```c
((A && B) | C | (D || E | F)) && G
```
<img width="1173" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/cd8d86f1-2f0a-4135-9cc5-e4a9b88027b7">

## EXEMPLE DE CODE
Grossièrement, voici comment est construit l'exécution en récursif.
Evidemment, je montre juste l'idée globale, tout n'est pas géré dans ce morceau de code (la gestion des signaux, la gestion des closes, finir une node en la laissant remonter elle même ou exit si on se trouve dans un child, etc...).  

Mon premier appel sera de la forme

```c
executer(node, STDIN_FILENO, STDOUT_FILENO);
```


```c
void	executer(t_ast *node, int in, int out)
{
	if (node->token == AND)
		exec_and(node, in, out);
	else if (node->token == OR)
		exec_or(node, in, out);
	else if (node->token == PIPE)
		exec_pipe(node, in, out);
	else
		exec_expression(node, in, out);
}

void	exec_and(t_ast *node, int in, int out)
{
	executer(node->left, in, out);
	if (data->exit_status == 0)
		executer(node->right, in, out);
}

void	exec_or(t_ast *node, int in, int out)
{
	executer(node->left, in, out);
	if (data->exit_status != 0)
		executer(node->right, in, out);
}

void	exec_pipe(t_ast *node, int in, int out)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;

	pipe(fd);
	pid1 = fork();
	pid2 = fork();
	if (pid1 == 0)
		executer(node->left, in, fd[1]);
	if (pid2 == 0)
		executer(node->right, fd[0], out);
	waitpid(pid1, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		data->exit_status = WEXITSTATUS(status);
}

void	exec_expression(t_ast *node, int in, int out);
{
	update_redirect();
	execve_cmd();
}
```



