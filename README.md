# MINISHELL

Minishell est un projet long a realiser, surtout si on veut le faire BIEN. C'est a dire qu'on cherche a se rapprocher le plus possible du comportement de bash.
C'est le premier projet qui comporte autant de cas particuliers a gerer, c'est important d'avoir un certain souci du detail.
N'essayez pas de tout faire a la va-vite. Prenez votre temps, testez les commandes avec bash et comprenez le raisonnement afin de reproduire le meme comportement.

Apres avoir creer votre prompt avec la fonction readline(), Minishell se divise en 3 grosse parties :
Le lexer, le parser, et enfin l'executer.

## LEXER

Le but du lexer est de transformer notre input en liste chainee, chaque node contenant la chaine de caractere et son token associe.

Par exemple :
```ls | cat``` 
devient

<img width="269" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/3d05fbea-36a1-4997-80d9-3c53f16f57c4">

(Attention, ```ls |       cat``` ou ```ls|cat``` ou encore ```"ls"|   'cat'``` donnent le meme resultat.)

Autre exemple, un peu plus complexe :

```(< Makefile cat && echo a) | wc -l``` devient

<img width="1065" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/04e1d356-1637-4c7a-b358-c3e43b1619d6">

Comme le cas precedent, je peux l'ecrire de plusieurs facons... voici un exemple parmi tant d'autres:

```(<Makefile    'cat'&& "echo"      a   )|wc     '-l'```

Notre liste de tokens :
- WORD (un mot, qui sera un nom de commande, une option, un argument, un path...)
- PIPE
- AND
- OR
- R_REDIR ( > )
- RR_REDIR ( >> )
- L_REDIR ( < )
- LL_REDIR ( << )
- PAR_LEFT
- PAR_RIGHT


## PARSER

Nous avons cree un AST (Abstract Syntax Tree), qui se comporte comme une liste chainee mais avec 2 pointeurs (right et left) vers les nodes suivantes, contrairement a une liste chainee classique qui n'en a qu'une.
Cela a pour but d'organiser nos tokens par ordres de priorite, afin de faciliter l'executer.

L'ordre de priorite a niveau EGAL (c'est a dire en prenant en compte les parentheses) est le suivant:
- AND / OR (en partant de la fin)
- PIPE (en partant de la fin)
- L_REDIR / LL_REDIR / R_REDIR / RR_REDIR (en partant du debut)
- WORD (en partant du debut)

Dans l'exemple precedent, notre AST ressemblera a ca :

```(< Makefile cat && echo a) | wc -l```

<img width="570" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/f992de3b-8440-458b-a087-c38b8a9a8462">

A noter que sans les parentheses, notre input donnerait un AST ainsi qu'un resultat completement different :

```< Makefile cat && echo a | wc -l```

<img width="666" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/35d59795-b302-4e6a-a822-06bf7ac5678f">

```cat <Makefile | grep -v a | >> outfile wc > out -l```

<img width="968" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/ea556588-a4f1-4c04-ac8a-26d3a058ce4e">


## EXECUTER

L'AST etant cree, il ne reste plus qu'a executer le tout de maniere recursif avec quelques regles:

- Au lancement, je declare dans ma data, data->in = STDIN_FILENO, et data->out = STDOUT_FILENO.
- Si token = PIPE, je pipe(fd) et je cree 2 childs (donc 2 forks):
  . child1 sur node->left avec data->out = fd[1]
  . child2 sur node->right avec data->in = fd[0].
  . Le parent attend ses deux enfants.
- Si token = AND, je continue sur node->left, et lorsque toute l'expression sera finie, je check l'exit_status.
  . Si exit_status = 0 (donc la commande a reussi), alors je continue sur node->right;
  . Sinon, je ne fais rien et je remonte naturellement a la node precedente.
- Si token = OR, je continue sur node->left, et lorsque toute l'expression sera finie, je check l'exit_status.
  . Si exit_status != 0 (donc la commande a echoue), alors je continue sur node->right;
  . Sinon, je ne fais rien et je remonte naturellement a la node precedente.
- Sinon, je suis forcement dans l'execution d'une commande:
  . update data->in et data->out en fonction des token REDIR presents dans l'expression.
  . Si builtin -> pas de fork;
  .	Si la node precedente est PIPE ->pas besoin de fork;
  . Sinon fork et execve la commande.



## COMMENT GERER LES PIPES

```ls | grep -v a | wc -l | cat```

(J'ai regroupe les nodes sur l'image pour une meilleure clarte)

<img width="880" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/950648ee-4f27-428c-a234-6ab085f56898">

On commence par PIPE 1, data->in = 0 (STDIN_FILENO), et data->out = 1 (STDOUT_FILENO), par defaut en haut de la node.  
Je pipe fd[2] : fd[0] = 3 et fd[1] = 4;  
Je fork 2 fois et cree 2 childs a qui j'envoie les fd correspondants :  
	- Child right: data->in = fd[0] (donc 3), et data->out = data->out (donc 1);  
 	- Child left:  data->in = data->in (donc 0) , et data->out = fd[1] (donc 4);  
  
Je pars a gauche et arrive sur le PIPE 2, je pipe fd[2] : fd[0] = 5 et fd[1] = 6;  
Je fork 2 fois et cree 2 childs:  
	- Child right: data->in = fd[0] (donc 5), et data->out = data->out (donc 4);  
 	- Child left:  data->in = data->in (donc 0), et data->out = fd[1] (donc 6);  
  
Je pars a gauche et arrive sur le PIPE 3, je pipe fd[2] : fd[0] = 7 et fd[1] = 8;  
Je fork 2 fois et cree 2 childs:  
	- Child right: data->in = fd[0] (donc 7), et data->out = data->out (donc 6);  
 	- Child left:  data->in = data->in (donc 0), et data->out = fd[1] (donc 8);  
  
Et voila, chaque expression peut maintenant lire et ecrire dans leur fd par defaut.  
Evidemment, si des redirect sont dans l'expression, il faut mettre a jour le data->in et data->out;  
ATTENTION ! Ne pas oublier de close les fds inutilises dans les childs et dans les parents.  

```(ls | grep -v a && echo hello) | cat```

<img width="912" alt="image" src="https://github.com/Rantanzyon/minishell_GL/assets/144052557/6ea4a6b3-e431-45cb-9a4e-0bd09257c96d">

Ici, le token AND ne cree pas de nouveau pipe, il envoie juste le meme data->in et data->out aux nodes gauche puis droite, qui vont lire et ecrire au meme endroit par defaut.

## EXEMPLE DE CODE
Grossierement, voici comment est construit l'execution en recursif.
Evidemment, je montre juste l'idee globale, tout n'est pas gere dans ce morceau de code (la gestion des signaux, la gestion des closes, finir une node en la laissant remonter elle meme ou exit si on se trouve dans un child, etc...).  

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



